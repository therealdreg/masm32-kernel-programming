;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; SharingMemory - How to share memory between kernel-mode driver and its user-mode client
;
; This method is applicable only for highest-level or monolithic driver
; because of while processing IRP such driver's type is in the context
; of the requested user process which address space driver maps the memory buffer into.
;
; Written by Four-F (four-f@mail.ru)
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.386
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\w2k\ntstatus.inc
include \masm32\include\w2k\ntddk.inc

include \masm32\include\w2k\ntoskrnl.inc
include \masm32\include\w2k\hal.inc

includelib \masm32\lib\w2k\ntoskrnl.lib
includelib \masm32\lib\w2k\hal.lib

include \masm32\Macros\Strings.mac

include ..\common.inc
include seh0.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
CCOUNTED_UNICODE_STRING	"\\Device\\SharingMemory", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\SharingMemory", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_pSharedMemory		PVOID	?
g_pMdl				PVOID	?
g_pUserAddress		PVOID	?

g_fTimerStarted		BOOL	?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                            N O N D I S C A R D A B L E   C O D E                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        UpdateTime                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

UpdateTime proc

; This routine is called from TimerRoutine at IRQL DISPATCH_LEVEL !

; The routine itself and all memory it touches must be in nonpaged memory.
; The memory pointed by g_pSharedMemory and the driver's code (except INIT or PAGED sections)
; is in nonpaged memory. KeQuerySystemTime and ExSystemTimeToLocalTime can be called at any IRQL.
; So, no problem here.

local SysTime:LARGE_INTEGER

	invoke KeQuerySystemTime, addr SysTime
	invoke ExSystemTimeToLocalTime, addr SysTime, g_pSharedMemory

	ret

UpdateTime endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       TimerRoutine                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TimerRoutine proc pDeviceObject:PDEVICE_OBJECT, pContext:PVOID

; This routine is called at IRQL DISPATCH_LEVEL !

	invoke UpdateTime

	ret

TimerRoutine endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          Cleanup                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Cleanup proc pDeviceObject:PDEVICE_OBJECT

	.if g_fTimerStarted
		invoke IoStopTimer, pDeviceObject
		invoke DbgPrint, $CTA0("SharingMemory: Timer stopped\n")
	.endif

	.if ( g_pUserAddress != NULL ) && ( g_pMdl != NULL )

		; If the call to MmMapLockedPages or MmMapLockedPagesSpecifyCache specified user mode,
		; the caller must be in the context of the original process before calling MmUnmapLockedPages.
		; Cleanup routine is called either from DispatchCleanup or DispatchControl.
		; So we always in appropriate process context.

		invoke MmUnmapLockedPages, g_pUserAddress, g_pMdl
		invoke DbgPrint, $CTA0("SharingMemory: Memory at address %08X unmapped\n"), g_pUserAddress
		and g_pUserAddress, NULL
	.endif

	.if g_pMdl != NULL
		invoke IoFreeMdl, g_pMdl
		invoke DbgPrint, $CTA0("SharingMemory: MDL at address %08X freed\n"), g_pMdl
		and g_pMdl, NULL
	.endif

	.if g_pSharedMemory != NULL
		invoke ExFreePool, g_pSharedMemory
		invoke DbgPrint, $CTA0("SharingMemory: Memory at address %08X released\n"), g_pSharedMemory
		and g_pSharedMemory, NULL
	.endif

	ret

Cleanup endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     DispatchCleanup                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchCleanup proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

; We MUST unmap the memory mapped into the user process before it exits
; It's better to do it as early as possible. 
; The driver recieves IRP_MJ_CLEANUP while user mode app just calls CloseHandle.

	invoke DbgPrint, $CTA0("\nSharingMemory: Entering DispatchCleanup\n")

	invoke Cleanup, pDeviceObject

	mov eax, pIrp
	mov (_IRP PTR [eax]).IoStatus.Status, STATUS_SUCCESS
	and (_IRP PTR [eax]).IoStatus.Information, 0

	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT

	invoke DbgPrint, $CTA0("SharingMemory: Leaving DispatchCleanup\n")

	mov eax, STATUS_SUCCESS
	ret

DispatchCleanup endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   DispatchCreateClose                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchCreateClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	mov eax, pIrp
	mov (_IRP PTR [eax]).IoStatus.Status, STATUS_SUCCESS
	and (_IRP PTR [eax]).IoStatus.Information, 0

	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT

	mov eax, STATUS_SUCCESS
	ret

DispatchCreateClose endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     DispatchControl                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchControl proc uses esi edi pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

local dwContext:DWORD

	invoke DbgPrint, $CTA0("\nSharingMemory: Entering DispatchControl\n")

	mov esi, pIrp
	assume esi:ptr _IRP

	mov [esi].IoStatus.Status, STATUS_UNSUCCESSFUL
	and [esi].IoStatus.Information, 0

	IoGetCurrentIrpStackLocation esi
	mov edi, eax
	assume edi:ptr IO_STACK_LOCATION

	.if [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_GIVE_ME_YOUR_MEMORY
		.if [edi].Parameters.DeviceIoControl.OutputBufferLength >= sizeof PVOID

			invoke ExAllocatePool, NonPagedPool, PAGE_SIZE
			.if eax != NULL
				mov g_pSharedMemory, eax

				invoke DbgPrint, \
				$CTA0("SharingMemory: %X bytes of nonpaged memory allocated at address %08X\n"), \
				PAGE_SIZE, g_pSharedMemory

				; The memory g_pSharedMemory points to contains garbage
				; because of the memory allocated in kernel doesn't zeroed out
				; So, if you want to do some string operations in such buffer
				; it may be better to fill it with the zeroes before.
				; In this example it's not required

				invoke IoAllocateMdl, g_pSharedMemory, PAGE_SIZE, FALSE, FALSE, NULL
				.if eax != NULL
					mov g_pMdl, eax

					invoke DbgPrint, \
							$CTA0("SharingMemory: MDL allocated at address %08X\n"), g_pMdl

					invoke MmBuildMdlForNonPagedPool, g_pMdl

					; If AccessMode is UserMode and the specified pages cannot be mapped,
					; the routine raises an exception. Callers that specify UserMode
					; must wrap the call to MmMapLockedPagesSpecifyCache in a try/except block. 

					_try

					; Under NT4 use MmMapLockedPages instead of MmMapLockedPagesSpecifyCache
					; invoke MmMapLockedPages, g_pMdl, UserMode

					invoke MmMapLockedPagesSpecifyCache, g_pMdl, UserMode, MmCached, \
										NULL, FALSE, NormalPagePriority
					.if eax != NULL

						mov g_pUserAddress, eax

						invoke DbgPrint, \
						$CTA0("SharingMemory: Memory mapped into user space at address %08X\n"), \
						g_pUserAddress

						mov eax, [esi].AssociatedIrp.SystemBuffer
						push g_pUserAddress
						pop dword ptr [eax]

						invoke UpdateTime

						invoke IoInitializeTimer, pDeviceObject, TimerRoutine, addr dwContext
						.if eax == STATUS_SUCCESS
							; Our TimerRoutine routine will be called once per second.
							invoke IoStartTimer, pDeviceObject
							inc g_fTimerStarted

							invoke DbgPrint, $CTA0("SharingMemory: Timer started\n")

							mov [esi].IoStatus.Information, sizeof PVOID
							mov [esi].IoStatus.Status, STATUS_SUCCESS

						.endif
					.endif

					_finally

				.endif
			.endif

		.else
			mov [esi].IoStatus.Status, STATUS_BUFFER_TOO_SMALL
		.endif
	.else
		mov [esi].IoStatus.Status, STATUS_INVALID_DEVICE_REQUEST
	.endif

	assume edi:nothing

	; If something went wrong do cleanup
	.if [esi].IoStatus.Status != STATUS_SUCCESS

		invoke DbgPrint, $CTA0("SharingMemory: Something went wrong\:\n")

		invoke Cleanup, pDeviceObject

	.endif

	; We MUST NOT touch IRP after IoCompleteRequest has returned. It might be freed.

	push [esi].IoStatus.Status

	assume esi:nothing
	
	fastcall IofCompleteRequest, esi, IO_NO_INCREMENT

	invoke DbgPrint, $CTA0("SharingMemory: Leaving DispatchControl\n")

	pop eax
	ret

DispatchControl endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

	invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName

	mov eax, pDriverObject
	invoke IoDeleteDevice, (DRIVER_OBJECT PTR [eax]).DeviceObject

	ret

DriverUnload endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              D I S C A R D A B L E   C O D E                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code INIT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local status:NTSTATUS
local pDeviceObject:PDEVICE_OBJECT

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	; Explicity initialize global variables
	and g_pSharedMemory, NULL
	and g_pMdl, NULL
	and g_pUserAddress, NULL
	and g_fTimerStarted, FALSE

	; Create exclusive device
	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, FILE_DEVICE_UNKNOWN, 0, TRUE, addr pDeviceObject
	.if eax == STATUS_SUCCESS
		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usDeviceName
		.if eax == STATUS_SUCCESS
			mov eax, pDriverObject
			assume eax:ptr DRIVER_OBJECT
			mov [eax].MajorFunction[IRP_MJ_CREATE*(sizeof PVOID)],			offset DispatchCreateClose
			mov [eax].MajorFunction[IRP_MJ_CLEANUP*(sizeof PVOID)],			offset DispatchCleanup
			mov [eax].MajorFunction[IRP_MJ_CLOSE*(sizeof PVOID)],			offset DispatchCreateClose
			mov [eax].MajorFunction[IRP_MJ_DEVICE_CONTROL*(sizeof PVOID)],	offset DispatchControl
			mov [eax].DriverUnload,											offset DriverUnload
			assume eax:nothing
			mov status, STATUS_SUCCESS
		.else
			invoke IoDeleteDevice, pDeviceObject
		.endif
	.endif

	mov eax, status
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=SharingMemory

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj
move %drv%.sys ..

echo.
pause
