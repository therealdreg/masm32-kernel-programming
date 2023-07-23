;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; HiddenDriver - Kernel Mode Driver
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

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
CCOUNTED_UNICODE_STRING	"\\Device\\HiddenDriver", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\HiddenDriver", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                            N O N D I S C A R D A B L E   C O D E                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   DispatchCreateClose                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchCreateClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	invoke DbgPrint, $CTA0("HiddenDriver: Creating or Closing...\n")

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
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

	invoke DbgPrint, $CTA0("HiddenDriver: Unloading...\n")

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

	invoke DbgPrint, $CTA0("\nHiddenDriver: Loading...\n")

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, addr pDeviceObject
	.if eax == STATUS_SUCCESS
		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usDeviceName
		.if eax == STATUS_SUCCESS
			mov eax, pDriverObject
			assume eax:ptr DRIVER_OBJECT
			mov [eax].MajorFunction[IRP_MJ_CREATE*(sizeof PVOID)],			offset DispatchCreateClose
			mov [eax].MajorFunction[IRP_MJ_CLOSE*(sizeof PVOID)],			offset DispatchCreateClose
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

set drv=HiddenDriver

rem Driver's client must be recompiled
if exist ..\%drv%.exe del ..\%drv%.exe
if exist ..\%drv%.sys del ..\%drv%.sys

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj
move %drv%.sys ..

echo.
pause
