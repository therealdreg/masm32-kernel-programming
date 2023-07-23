;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  WhichIrqlAndContext - Let you know the IRQL and Process/Thread context
;                        at which the main driver's routines are running.
;
;  Written by Four-F (four-f@mail.ru)
;
;  WARNING: Tested W2000 only!
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

include common.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 R E A D O N L Y    D A T A                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

CCOUNTED_UNICODE_STRING	"\\Device\\WhichIrqlAndContext", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\WhichIrqlAndContext", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       PrintReport                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintReport proc pszRoutineName:PVOID

	invoke PsGetCurrentThreadId
	push eax
	invoke PsGetCurrentProcessId
	push eax
	invoke KeGetCurrentIrql
	push eax
	push pszRoutineName
	push $CTA0("WhichIrqlAndContext: %s IRQL=%u, PID=%08X, TID=%08X\n")
	call DbgPrint
	add esp, 5 * sizeof DWORD

	ret

PrintReport endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   DispatchCreate                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchCreate proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; CreateFile was called, to get driver handle
	; We are in user process context here

	invoke PrintReport, $CTA0("DispatchCreate ")

	mov eax, pIrp
	assume eax:ptr _IRP
	mov [eax].IoStatus.Status, STATUS_SUCCESS
	and [eax].IoStatus.Information, 0
	assume eax:nothing

	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT

	mov eax, STATUS_SUCCESS
	ret

DispatchCreate endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   DispatchClose                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; CloseHandle was called, to close driver handle
	; We are in user process context here

	invoke PrintReport, $CTA0("DispatchClose  ")

	mov eax, pIrp
	assume eax:ptr _IRP
	mov [eax].IoStatus.Status, STATUS_SUCCESS
	and [eax].IoStatus.Information, 0
	assume eax:nothing

	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT

	mov eax, STATUS_SUCCESS
	ret

DispatchClose endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     DispatchControl                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchControl proc uses esi edi pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; DeviceIoControl was called
	; We are in user process context here

local status:NTSTATUS

	mov esi, pIrp
	assume esi:ptr _IRP

	IoGetCurrentIrpStackLocation esi
	mov edi, eax
	assume edi:ptr IO_STACK_LOCATION

	.if [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_DUMMY
		invoke PrintReport, $CTA0("DispatchControl")
		mov status, STATUS_SUCCESS
	.else
		mov status, STATUS_INVALID_DEVICE_REQUEST
	.endif

	assume edi:nothing

	push status
	pop [esi].IoStatus.Status

	and [esi].IoStatus.Information, 0

	assume esi:nothing

	fastcall IofCompleteRequest, esi, IO_NO_INCREMENT

	mov eax, status
	ret

DispatchControl endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

	; ControlService,,SERVICE_CONTROL_STOP was called
	; We are in System process (pid = 4 or 8) context here

	invoke PrintReport, $CTA0("DriverUnload   ")

	invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName

	mov eax, pDriverObject
	invoke IoDeleteDevice, (DRIVER_OBJECT PTR [eax]).DeviceObject

	ret

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

	; StartService was called
	; We are in System process (pid = 4 or 8) context here

	invoke DbgPrint, $CTA0("\n")
	invoke PrintReport, $CTA0("DriverEntry    ")

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, addr pDeviceObject
	.if eax == STATUS_SUCCESS
		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usDeviceName
		.if eax == STATUS_SUCCESS
			mov eax, pDriverObject
			assume eax:ptr DRIVER_OBJECT
			mov [eax].MajorFunction[IRP_MJ_CREATE*(sizeof PVOID)],			offset DispatchCreate
			mov [eax].MajorFunction[IRP_MJ_CLOSE*(sizeof PVOID)],			offset DispatchClose
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

set drv=WhichIrqlAndContext

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj

echo.
pause
