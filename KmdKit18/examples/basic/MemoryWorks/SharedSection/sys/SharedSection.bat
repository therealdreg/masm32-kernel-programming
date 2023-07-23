;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  SharedSection - How to share section between kernel-mode driver and its user-mode client
;
; This method is applicable only for highest-level or monolithic driver
; because the section object is always mapped in the user address space of a process
; So, the address is valid only if it is accessed in the context of the process.
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
include \masm32\include\w2k\native.inc

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

include ..\common.inc
include seh0.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 R E A D O N L Y    D A T A                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

CCOUNTED_UNICODE_STRING	"\\Device\\SharedSection", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\SharedSection", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

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

local hSection:HANDLE
local oa:OBJECT_ATTRIBUTES
local pSectionBaseAddress:PVOID
local liViewSize:LARGE_INTEGER

	invoke DbgPrint, $CTA0("\nSharedSection: Entering DispatchControl\n")

	mov esi, pIrp
	assume esi:ptr _IRP

	; Assume unsuccess
	mov [esi].IoStatus.Status, STATUS_UNSUCCESSFUL
	; We copy nothing
	and [esi].IoStatus.Information, 0

	IoGetCurrentIrpStackLocation esi
	mov edi, eax
	assume edi:ptr IO_STACK_LOCATION

	.if [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_SHARE_MY_SECTION

		invoke DbgPrint, $CTA0("SharedSection: Opening section object\n")

		lea ecx, oa
		InitializeObjectAttributes ecx, offset g_usSectionName, OBJ_CASE_INSENSITIVE, NULL, NULL
		invoke ZwOpenSection, addr hSection, SECTION_MAP_WRITE + SECTION_MAP_READ, addr oa
		.if eax == STATUS_SUCCESS

			invoke DbgPrint, $CTA0("SharedSection: Section object opened\n")

			and pSectionBaseAddress, NULL			; The system itself should choose the address
			and liViewSize.HighPart, 0
			and liViewSize.LowPart, 0
			; NtCurrentProcess equ -1
			invoke ZwMapViewOfSection, hSection, NtCurrentProcess, addr pSectionBaseAddress, 0, SECTION_SIZE, \
									NULL, addr liViewSize, ViewShare, 0, PAGE_READWRITE
			.if eax == STATUS_SUCCESS

				invoke DbgPrint, $CTA0("SharedSection: Section mapped at address %08X\n"), pSectionBaseAddress

				_try

				invoke _strrev, pSectionBaseAddress
				mov [esi].IoStatus.Status, STATUS_SUCCESS

				invoke DbgPrint, $CTA0("SharedSection: String reversed\n")

				_finally

				invoke ZwUnmapViewOfSection, NtCurrentProcess, pSectionBaseAddress

				invoke DbgPrint, $CTA0("SharedSection: Section at address %08X unmapped \n"), pSectionBaseAddress

			.else
				invoke DbgPrint, $CTA0("SharedSection: Couldn't map view of section. Status: %08X\n"), eax
			.endif
			invoke ZwClose, hSection
			invoke DbgPrint, $CTA0("SharedSection: Section object handle closed\n")
		.else
			invoke DbgPrint, $CTA0("SharedSection: Couldn't open section. Status: %08X\n"), eax
		.endif

	.else
		mov [esi].IoStatus.Status, STATUS_INVALID_DEVICE_REQUEST
	.endif

	; We MUST NOT touch IRP after IoCompleteRequest has returned. It might be freed.

	push [esi].IoStatus.Status

	assume edi:nothing
	assume esi:nothing
	
	fastcall IofCompleteRequest, esi, IO_NO_INCREMENT

	invoke DbgPrint, $CTA0("SharedSection: Leaving DispatchControl\n")

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

	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, FILE_DEVICE_UNKNOWN, 0, TRUE, addr pDeviceObject
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

set drv=SharedSection

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj
move %drv%.sys ..

echo.
pause
