;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
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
include \masm32\include\w2k\guiddef.inc
include \masm32\include\w2k\ntoskrnl.inc

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

include wdmsec.inc
includelib wdmsec.lib

IoCreateDeviceSecure equ WdmlibIoCreateDeviceSecure
BUFFER_LENGTH equ 256
	
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

; This and other GUIDs are defined in devguid.inc

DEFINE_GUID GUID_DEVCLASS_UNKNOWN, 04d36e97eh, 0e325h, 011ceh, 0bfh, 0c1h, 008h, 000h, 02bh, 0e1h, 003h, 018h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_dwSuffix		DWORD	?


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                            N O N D I S C A R D A B L E   C O D E                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      DispatchCreate                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchCreate proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	invoke DbgPrint, $CTA0("SecureDevices: Create request\n")

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
;                                      DispatchClose                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DispatchClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	invoke DbgPrint, $CTA0("SecureDevices: Close request\n")
	
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
;                                       DeleteAllDevices                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DeleteAllDevices proc pDriverObject:PDRIVER_OBJECT

local pNextDeviceObject:PDEVICE_OBJECT

	and pNextDeviceObject, NULL

	mov eax, pDriverObject
	mov eax, (DRIVER_OBJECT PTR [eax]).DeviceObject

	.while eax != NULL
		mov ecx, (DEVICE_OBJECT PTR [eax]).NextDevice
		mov pNextDeviceObject, ecx
		invoke IoDeleteDevice, eax
		mov eax, pNextDeviceObject
	.endw

	ret

DeleteAllDevices endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    DeleteAllSymbolicLinks                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DeleteAllSymbolicLinks proc

local usSuffix:UNICODE_STRING
local usSymbolicLinkName:UNICODE_STRING
local buffer[8]:BYTE

	invoke ExAllocatePool, PagedPool, BUFFER_LENGTH
	.if eax != NULL

		mov usSymbolicLinkName.Buffer, eax

		.while TRUE

			and usSymbolicLinkName._Length, 0
			mov usSymbolicLinkName.MaximumLength, BUFFER_LENGTH

			invoke RtlAppendUnicodeToString, addr usSymbolicLinkName, $CTW0("\\DosDevices\\SecureDevice")
			.if eax == STATUS_SUCCESS
		
				and usSuffix._Length, 0
				mov usSuffix.MaximumLength, sizeof buffer
				lea eax, buffer
				mov usSuffix.Buffer, eax

				invoke RtlIntegerToUnicodeString, g_dwSuffix, 10, addr usSuffix
				.if eax == STATUS_SUCCESS
		
					invoke RtlAppendUnicodeStringToString, addr usSymbolicLinkName, addr usSuffix
					.if eax == STATUS_SUCCESS

						invoke IoDeleteSymbolicLink, addr usSymbolicLinkName
						
					.endif
				.endif
			.endif

			.break .if g_dwSuffix == 0
			
			dec g_dwSuffix
		.endw

		invoke ExFreePool, usSymbolicLinkName.Buffer
	.endif

	ret

DeleteAllSymbolicLinks endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

	invoke DeleteAllSymbolicLinks
	invoke DeleteAllDevices, pDriverObject

	ret

DriverUnload endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              D I S C A R D A B L E   C O D E                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code INIT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     CreateSecureDevice                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CreateSecureDevice proc pDriverObject:PDRIVER_OBJECT, pusSDDL:PUNICODE_STRING

local pDeviceObject:PDEVICE_OBJECT
local usSuffix:UNICODE_STRING
local usDeviceName:UNICODE_STRING
local usSymbolicLinkName:UNICODE_STRING
local buffer[8]:BYTE
local fDeleteDevice:BOOL

	mov fDeleteDevice, TRUE
	and pDeviceObject, NULL

	invoke ExAllocatePool, PagedPool, BUFFER_LENGTH
	.if eax != NULL

		and usDeviceName._Length, 0
		mov usDeviceName.MaximumLength, BUFFER_LENGTH
		mov usDeviceName.Buffer, eax

		invoke RtlAppendUnicodeToString, addr usDeviceName, $CTW0("\\Device\\SecureDevice")
		.if eax == STATUS_SUCCESS

			and usSuffix._Length, 0
			mov usSuffix.MaximumLength, sizeof buffer
			lea eax, buffer
			mov usSuffix.Buffer, eax

			invoke RtlIntegerToUnicodeString, g_dwSuffix, 10, addr usSuffix
			.if eax == STATUS_SUCCESS
		
				invoke RtlAppendUnicodeStringToString, addr usDeviceName, addr usSuffix
				.if eax == STATUS_SUCCESS
		
					invoke IoCreateDeviceSecure, pDriverObject, 0, addr usDeviceName, FILE_DEVICE_UNKNOWN, \
										0, FALSE, pusSDDL, addr GUID_DEVCLASS_UNKNOWN, addr pDeviceObject
					.if eax == STATUS_SUCCESS

						invoke ExAllocatePool, PagedPool, BUFFER_LENGTH
						.if eax != NULL

							and usSymbolicLinkName._Length, 0
							mov usSymbolicLinkName.MaximumLength, BUFFER_LENGTH
							mov usSymbolicLinkName.Buffer, eax
		
							invoke RtlAppendUnicodeToString, addr usSymbolicLinkName, $CTW0("\\DosDevices\\SecureDevice")
							.if eax == STATUS_SUCCESS
						
								invoke RtlAppendUnicodeStringToString, addr usSymbolicLinkName, addr usSuffix
								.if eax == STATUS_SUCCESS

									invoke IoCreateSymbolicLink, addr usSymbolicLinkName, addr usDeviceName
									.if eax == STATUS_SUCCESS
										inc g_dwSuffix
										mov fDeleteDevice, FALSE
									.endif

								.endif
							.endif
				
							invoke ExFreePool, usSymbolicLinkName.Buffer

						.endif
					.endif
					
					.if fDeleteDevice == TRUE
						invoke IoDeleteDevice, pDeviceObject
						and pDeviceObject, NULL
					.endif

				.endif
			.endif
		.endif

		invoke ExFreePool, usDeviceName.Buffer

	.endif

	mov eax, pDeviceObject
	ret

CreateSecureDevice endp
		
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local status:NTSTATUS
local pDeviceObject:PDEVICE_OBJECT
local usSDDL:UNICODE_STRING

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	and g_dwSuffix, 0

	; We will create a secure device objects to apply the specified
	; security settings.  Sometimes we need to secure our devices
	; because they are created by a legacy driver and there is no INF
	; involved in installing the driver.  For PNP drivers, security
	; descriptor is typically specified for the FDO in the INF file.

	; First use predefined SDDLs.  They all are defined in wdmsec.lib.

	; User mode code (including processes running as system) cannot
	; open this device. 

	lea eax, SDDL_DEVOBJ_KERNEL_ONLY
	invoke CreateSecureDevice, pDriverObject, eax
	.if eax != NULL
		mov status, STATUS_SUCCESS
	.endif

	; This SDDL allows the kernel, system, and administrator complete control
	; over the device.  No other users may access the device.

	lea eax, SDDL_DEVOBJ_SYS_ALL_ADM_ALL
	invoke CreateSecureDevice, pDriverObject, eax
	.if eax != NULL
		mov status, STATUS_SUCCESS
	.endif

	; This SDDL allows the kernel and system complete control over the device.
	; By default the administrator can access the entire device, but cannot
	; change the ACL (the administrator must take control of the device first.)

	lea eax, SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_R_RES_R
	invoke CreateSecureDevice, pDriverObject, eax
	.if eax != NULL
		mov status, STATUS_SUCCESS
	.endif

	; Our custom SDDL.
	
	; SDDL strings for device objects are of the form "D:P" followed by one or more
	; expressions of the form "(A;;Access;;;SID)".  The SID value specifies a security
	; identifier that determines to whom the Access value applies (for example, a user
	; or group).  The Access value specifies the access rights allowed for the SID.
	;
	; Refer "Security Descriptor String Format" section in the platform SDK documentation
	; and "SDDL for Device Objects" in the DDK to understand the format of the sddl string.

	invoke RtlInitUnicodeString, addr usSDDL, $CTW0("D:P(A;;GA;;;SY)(A;;GR;;;BA)")
	invoke CreateSecureDevice, pDriverObject, addr usSDDL
	.if eax != NULL
		mov status, STATUS_SUCCESS
	.endif

	.if status == STATUS_SUCCESS
	
		mov eax, pDriverObject
		assume eax:ptr DRIVER_OBJECT
		mov [eax].MajorFunction[IRP_MJ_CREATE*(sizeof PVOID)],		offset DispatchCreate
		mov [eax].MajorFunction[IRP_MJ_CLOSE*(sizeof PVOID)],		offset DispatchClose
		mov [eax].DriverUnload,										offset DriverUnload
		assume eax:nothing
	.endif
			
	mov eax, status
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=SecureDevices

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj

echo.
pause
