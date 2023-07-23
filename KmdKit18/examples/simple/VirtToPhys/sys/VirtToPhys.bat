;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; VirtToPhys - Kernel Mode Driver
;
; Translates virtual addres to physical address
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
include \masm32\include\w2k\w2kundoc.inc

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

include ..\common.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
CCOUNTED_UNICODE_STRING	"\\Device\\devVirtToPhys", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\??\\slVirtToPhys", g_usSymbolicLinkName, 4

; May be you have to use this line instead of above one
; if your Windows NT version is <= 4.0
; It will work also under 2K & XP
;CCOUNTED_UNICODE_STRING	"\\DosDevices\\slVirtToPhys", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                            N O N D I S C A R D A B L E   C O D E                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    GetPhysicalAddress                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetPhysicalAddress proc dwAddress:DWORD

; Converts virtual address in dwAddress to corresponding physical address

	mov eax, dwAddress
	mov ecx, eax

	shr eax, 22         									; (Address >> 22) => Page Directory Index, PDI
	shl eax, 2												; * sizeof PDE = PDE offset

	mov eax, [0C0300000h][eax]								; [Page Directory Base + PDE offset]

	.if ( eax & (mask pde4kValid) )							; .if ( eax & 01y )
		; PDE is valid
		.if !( eax & (mask pde4kLargePage) )				; .if ( eax & 010000000y )
			; small page (4kB)
			mov eax, ecx
			; (Address >> 12) * sizeof PTE => PTE offset
			shr eax, 10
			and eax, 1111111111111111111100y
			add eax, 0C0000000h								; add Page Table Array Base
			mov eax, [eax]									; fetch PTE

			.if eax & (mask pteValid)						; .if ( eax & 01y )
				; PTE is valid
				and eax, mask ptePageFrameNumber			; mask PFN   (and eax, 11111111111111111111000000000000y)

				; We actually don't need these two lines
				; because of module base is always page aligned
				and ecx, 00000000000000000000111111111111y	; Byte Index
				add eax, ecx								; add byte offset to physical address
			.else
				xor eax, eax								; error
			.endif
		.else
			; large page (4mB)
			and eax, mask pde4mPageFrameNumber				; mask PFN   (and eax, 11111111110000000000000000000000y)
			and ecx, 00000000001111111111111111111111y		; Byte Index
			add eax, ecx									; add byte offset to physical address
		.endif
	.else
		xor eax, eax										; error
	.endif

	ret

GetPhysicalAddress endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   DispatchCreateClose                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchCreateClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; CreateFile was called, to get device handle
	; CloseHandle was called, to close device handle
	; In both cases we are in user process context here

	mov eax, pIrp
	assume eax:ptr _IRP
	mov [eax].IoStatus.Status, STATUS_SUCCESS
	and [eax].IoStatus.Information, 0
	assume eax:nothing

	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT

	mov eax, STATUS_SUCCESS
	ret

DispatchCreateClose endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     DispatchControl                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchControl proc uses esi edi ebx pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; DeviceIoControl was called
	; We are in user process context here

local status:NTSTATUS
local dwBytesReturned:DWORD

	and dwBytesReturned, 0

	mov esi, pIrp
	assume esi:ptr _IRP

	IoGetCurrentIrpStackLocation esi
	mov edi, eax
	assume edi:ptr IO_STACK_LOCATION

	.if [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_GET_PHYS_ADDRESS

		mov status, STATUS_BUFFER_TOO_SMALL
		.if ( [edi].Parameters.DeviceIoControl.OutputBufferLength >= DATA_SIZE )
		.if ( [edi].Parameters.DeviceIoControl.InputBufferLength >= DATA_SIZE )

			mov edi, [esi].AssociatedIrp.SystemBuffer
			assume edi:ptr DWORD

			xor ebx, ebx
			.while ebx < NUM_DATA_ENTRY

				; Change proc name to MmGetPhysicalAddress
				; if you want to ask kernel to do all job for you

				invoke GetPhysicalAddress, [edi][ebx*(sizeof DWORD)]

				mov [edi][ebx*(sizeof DWORD)], eax
				inc ebx
			.endw

			mov dwBytesReturned, DATA_SIZE
			mov status, STATUS_SUCCESS

		.endif
		.endif

	.else
		mov status, STATUS_INVALID_DEVICE_REQUEST
	.endif

	assume edi:nothing

	push status
	pop [esi].IoStatus.Status

	push dwBytesReturned
	pop [esi].IoStatus.Information

	assume esi:nothing

	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT

	mov eax, status
	ret

DispatchControl endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

	; ControlService,,SERVICE_CONTROL_STOP was called
	; We are in System process (pid = 8) context here

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

	; StartService was called
	; We are in System process (pid = 8) context here

local status:NTSTATUS
local pDeviceObject:PDEVICE_OBJECT

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, addr pDeviceObject
	.if eax == STATUS_SUCCESS
		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usDeviceName
		.if eax == STATUS_SUCCESS
			mov eax, pDriverObject
			assume eax:ptr DRIVER_OBJECT
			mov [eax].DriverUnload,											offset DriverUnload
			mov [eax].MajorFunction[IRP_MJ_CREATE*(sizeof PVOID)],			offset DispatchCreateClose
			mov [eax].MajorFunction[IRP_MJ_CLOSE*(sizeof PVOID)],			offset DispatchCreateClose
			mov [eax].MajorFunction[IRP_MJ_DEVICE_CONTROL*(sizeof PVOID)],	offset DispatchControl
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

set drv=VirtToPhys

:makerc
if exist rsrc.obj goto final
	\masm32\bin\rc /v rsrc.rc
	\masm32\bin\cvtres /machine:ix86 rsrc.res
	if errorlevel 0 goto final
		echo.
		pause
		exit

:final

if exist rsrc.res del rsrc.res

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj rsrc.obj

del %drv%.obj
move %drv%.sys ..

echo.
pause