;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; NtBuild - Kernel Mode Driver
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

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

include ..\common.inc

include seh0.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
CCOUNTED_UNICODE_STRING	"\\Device\\NtBuild", g_usDeviceName, 4
;CCOUNTED_UNICODE_STRING	"\\??\\NtBuild", g_usSymbolicLinkName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\NtBuild", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                            N O N D I S C A R D A B L E   C O D E                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   DispatchCreateClose                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchCreateClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; CreateFile was called, to get driver handle
	; CloseHandle was called, to close driver handle
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

DispatchRead proc uses esi edi pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; ReadFile was called
	; We are in user process context here
	; And can touch user addresses directly

local status:NTSTATUS
local dwBytesReturned:DWORD

	and dwBytesReturned, 0

	mov esi, pIrp
	assume esi:ptr _IRP

	IoGetCurrentIrpStackLocation esi
	mov edi, eax
	assume edi:ptr IO_STACK_LOCATION

	.if [edi].Parameters.Read._Length >= DATA_SIZE

		mov eax, [esi].UserBuffer

		_try

;		invoke ProbeForWrite, [esi].UserBuffer, DATA_SIZE, 1

		mov eax, MmUserProbeAddress		; 7fff0000h
		mov eax, [eax]
		mov eax, [eax]

		.if [esi].UserBuffer < eax
			mov eax, NtBuildNumber
			mov eax, [eax]
			push [eax]
			mov eax, [esi].UserBuffer
			pop [eax]

			mov dwBytesReturned, DATA_SIZE		
			mov status, STATUS_SUCCESS
		.else
			mov status, STATUS_INVALID_PARAMETER
		.endif

		_finally

	.else
		mov status, STATUS_BUFFER_TOO_SMALL
	.endif

	assume edi:nothing

	push status
	pop [esi].IoStatus.Status

	push dwBytesReturned
	pop [esi].IoStatus.Information

	assume esi:nothing

	fastcall IofCompleteRequest, esi, IO_NO_INCREMENT

	mov eax, status
	ret

DispatchRead endp

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
			mov [eax].MajorFunction[IRP_MJ_READ*(sizeof PVOID)],			offset DispatchRead
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

set drv=NtBuild

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
if exist ..\%exe%.exe del ..\%exe%.exe

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj rsrc.obj

del %drv%.obj
move %drv%.sys ..

echo.
pause
