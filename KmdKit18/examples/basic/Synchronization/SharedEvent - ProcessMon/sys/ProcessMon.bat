;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  Process Monitor - An example how to notify the user mode about some sort of “event” has happened.
;  (Another common way to provide kernel-to-user notification technique is Pending Irp)
;
;   This method is applicable only for highest-level or monolithic driver
;   because the event handle driver recieves from user mode is only valid
;   in the process context where the handle is created.
;
;  Written by Four-F (four-f@mail.ru)
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

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

include ..\common.inc
include ProcPath.asm

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 R E A D O N L Y    D A T A                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

CCOUNTED_UNICODE_STRING	"\\Device\\ProcessMon", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\ProcessMon", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_pkEventObject			PKEVENT			?
g_dwImageFileNameOffset	DWORD			?
g_fbNotifyRoutineSet	BOOL			?

; We do not syncronize access to this global variable
; because system itself syncronize its procrss database
; while creation/termination the process. So only one
; thread can touch it at a time.

g_ProcessData			PROCESS_DATA	<>

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   DispatchCreateClose                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchCreateClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	mov ecx, pIrp
	mov (_IRP PTR [ecx]).IoStatus.Status, STATUS_SUCCESS
	and (_IRP PTR [ecx]).IoStatus.Information, 0

	fastcall IofCompleteRequest, ecx, IO_NO_INCREMENT

	mov eax, STATUS_SUCCESS
	ret

DispatchCreateClose endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     ProcessNotifyRoutine                                          
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ProcessNotifyRoutine proc dwParentId:DWORD, dwProcessId:DWORD, bCreate:BOOL	; BOOLEAN

local peProcess:PVOID				; PEPROCESS
local fbDereference:BOOL
local us:UNICODE_STRING
local as:ANSI_STRING

	push eax						; reserve DWORD on stack
	invoke PsLookupProcessByProcessId, dwProcessId, esp
	pop peProcess					; -> EPROCESS
	.if eax == STATUS_SUCCESS
		mov fbDereference, TRUE		; PsLookupProcessByProcessId references process object
	.else
		; PsLookupProcessByProcessId fails (on w2k only) with STATUS_INVALID_PARAMETER
		; if called in the very same process context.
		; So if we are here it maight mean (on w2k) we are in process context being terminated.
		invoke IoGetCurrentProcess
		mov peProcess, eax
		and fbDereference, FALSE	; IoGetCurrentProcess doesn't references process object
	.endif

	mov eax, dwProcessId
	mov g_ProcessData.dwProcessId, eax

	mov eax, bCreate
	mov g_ProcessData.bCreate, eax

	invoke memset, addr g_ProcessData.szProcessName, 0, IMAGE_FILE_PATH_LEN

	invoke GetImageFilePath, peProcess, addr us
	.if eax == STATUS_SUCCESS

		lea eax, g_ProcessData.szProcessName
		mov as.Buffer,			eax
		mov as.MaximumLength,	IMAGE_FILE_PATH_LEN
		and as._Length,			0

		invoke RtlUnicodeStringToAnsiString, addr as, addr us, FALSE

		invoke ExFreePool, us.Buffer		; Free memory allocated by GetImageFilePath
	.else

		; If we fail to get process's image file path
		; just use only process name from EPROCESS.

		mov eax, g_dwImageFileNameOffset
		.if eax != 0
			add eax, peProcess
			invoke memcpy, addr g_ProcessData.szProcessName, eax, 16
		.endif

	.endif

	.if fbDereference
		fastcall ObfDereferenceObject, peProcess
	.endif

	; Notify user-mode client.

	invoke KeSetEvent, g_pkEventObject, 0, FALSE

	ret

ProcessNotifyRoutine endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     DispatchControl                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchControl proc uses esi edi pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

local liDelayTime:LARGE_INTEGER

	mov esi, pIrp
	assume esi:ptr _IRP

	; Initialize to failure.

	mov [esi].IoStatus.Status, STATUS_UNSUCCESSFUL
	and [esi].IoStatus.Information, 0

	IoGetCurrentIrpStackLocation esi
	mov edi, eax
	assume edi:ptr IO_STACK_LOCATION

	.if [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_SET_NOTIFY
		.if [edi].Parameters.DeviceIoControl.InputBufferLength >= sizeof HANDLE

			.if g_fbNotifyRoutineSet == FALSE		; For sure

				mov edx, [esi].AssociatedIrp.SystemBuffer
				mov edx, [edx]			; user-mode hEvent

				mov ecx, ExEventObjectType
				mov ecx, [ecx]
				mov ecx, [ecx]			; PTR OBJECT_TYPE

				invoke ObReferenceObjectByHandle, edx, EVENT_MODIFY_STATE, ecx, \
										UserMode, addr g_pkEventObject, NULL
				.if eax == STATUS_SUCCESS

					; If passed event handle is valid add a driver-supplied callback routine
					; to a list of routines to be called whenever a process is created or deleted.
				
					invoke PsSetCreateProcessNotifyRoutine, ProcessNotifyRoutine, FALSE
					mov [esi].IoStatus.Status, eax

					.if eax == STATUS_SUCCESS

						mov g_fbNotifyRoutineSet, TRUE

						invoke DbgPrint, \
								$CTA0("ProcessMon: Notification was set\n")
	
						; Make driver nonunloadable

						mov eax, pDeviceObject
						mov eax, (DEVICE_OBJECT PTR [eax]).DriverObject
						and (DRIVER_OBJECT PTR [eax]).DriverUnload, NULL

					.else
						invoke DbgPrint, \
						$CTA0("ProcessMon: Couldn't set notification\n")
					.endif

				.else
					mov [esi].IoStatus.Status, eax
					invoke DbgPrint, \
					$CTA0("ProcessMon: Couldn't reference user event object. Status: %08X\n"), \
					eax
				.endif
			.endif
		.else
			mov [esi].IoStatus.Status, STATUS_BUFFER_TOO_SMALL
		.endif

	.elseif [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_REMOVE_NOTIFY

		; Remove a driver-supplied callback routine from a list of routines
		; to be called whenever a process is created or deleted.

		.if g_fbNotifyRoutineSet == TRUE

			invoke PsSetCreateProcessNotifyRoutine, ProcessNotifyRoutine, TRUE
			mov [esi].IoStatus.Status, eax

			.if eax == STATUS_SUCCESS

				and g_fbNotifyRoutineSet, FALSE

				invoke DbgPrint, $CTA0("ProcessMon: Notification was removed\n")
					
				; Just for sure. It's theoreticaly possible our ProcessNotifyRoutine is now being executed.
				; So we wait for some small amount of time (~50 ms).

				or liDelayTime.HighPart, -1
				mov liDelayTime.LowPart, -1000000
	
				invoke KeDelayExecutionThread, KernelMode, FALSE, addr liDelayTime

				; Make driver unloadable

				mov eax, pDeviceObject
				mov eax, (DEVICE_OBJECT PTR [eax]).DriverObject
				mov (DRIVER_OBJECT PTR [eax]).DriverUnload, offset DriverUnload

				.if g_pkEventObject != NULL
					invoke ObDereferenceObject, g_pkEventObject
					and g_pkEventObject, NULL
				.endif
			.else
				invoke DbgPrint, \
				$CTA0("ProcessMon: Couldn't remove notification\n")
			.endif
			
		.endif

	.elseif [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_GET_PROCESS_DATA
		.if [edi].Parameters.DeviceIoControl.OutputBufferLength >= sizeof PROCESS_DATA

			mov eax, [esi].AssociatedIrp.SystemBuffer
			invoke memcpy, eax, offset g_ProcessData, sizeof g_ProcessData
	
			mov [esi].IoStatus.Status, STATUS_SUCCESS
			mov [esi].IoStatus.Information, sizeof g_ProcessData

		.else
			mov [esi].IoStatus.Status, STATUS_BUFFER_TOO_SMALL
		.endif

	.else
		mov [esi].IoStatus.Status, STATUS_INVALID_DEVICE_REQUEST
	.endif

	; After IoCompleteRequest returns, the IRP pointer
	; is no longer valid and cannot safely be dereferenced.

	push [esi].IoStatus.Status
	
	assume edi:nothing
	assume esi:nothing

	fastcall IofCompleteRequest, esi, IO_NO_INCREMENT

	pop eax			; [esi].IoStatus.Status
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
;                                    GetImageFileNameOffset                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetImageFileNameOffset proc uses esi ebx

; Finds EPROCESS.ImageFileName field offset

; W2K		EPROCESS.ImageFileName = 01FCh
; WXP		EPROCESS.ImageFileName = 0174h
; WNET		EPROCESS.ImageFileName = 0154h

; Instead of hardcoding above offsets we just scan
; the EPROCESS structure of System process one page down.
; It's well-known trick.

	invoke IoGetCurrentProcess
	mov esi, eax

	xor ebx, ebx
	.while ebx < 1000h			; one page more than enough.
		; Case insensitive compare.
		lea eax, [esi+ebx]
		invoke _strnicmp, eax, $CTA0("system"), 6
		.break .if eax == 0
		inc ebx
	.endw

	.if eax == 0
		; Found.
		mov eax, ebx
	.else
		; Not found.
		xor eax, eax
	.endif

	ret

GetImageFileNameOffset endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local status:NTSTATUS
local pDeviceObject:PDEVICE_OBJECT

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, \
				FILE_DEVICE_UNKNOWN, 0, TRUE, addr pDeviceObject
	.if eax == STATUS_SUCCESS
		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usDeviceName
		.if eax == STATUS_SUCCESS
			mov eax, pDriverObject
			assume eax:ptr DRIVER_OBJECT
			mov [eax].MajorFunction[IRP_MJ_CREATE*(sizeof PVOID)],			offset DispatchCreateClose
			mov [eax].MajorFunction[IRP_MJ_CLOSE*(sizeof PVOID)],			offset DispatchCreateClose
			mov [eax].MajorFunction[IRP_MJ_DEVICE_CONTROL*(sizeof PVOID)],	offset DispatchControl
			mov [eax].DriverUnload,											offset DriverUnload
			assume eax:nothing

			and g_fbNotifyRoutineSet, FALSE
			invoke memset, addr g_ProcessData, 0, sizeof g_ProcessData
		
			invoke GetImageFileNameOffset
			mov g_dwImageFileNameOffset, eax			; it can be not found and equal to 0, btw

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

set drv=ProcessMon

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj
move %drv%.sys ..

echo.
pause
