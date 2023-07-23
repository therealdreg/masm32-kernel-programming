;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  IsSafeBootMode - The way you know whether the system is running in safe mode or not.
;
;   Influenced by the "How to Determine if System Running in Safe Mode" article on osronline.com
;   I've been asked about it numerous times. So I've decided to write this crappy code.
;   Sould work under Windows 2000, XP and Server 2003.
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

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    E Q U A T I O N S                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

NORMALBOOT			equ 0	; The system is not in safe mode
SAFEBOOT_MINIMAL	equ 1
SAFEBOOT_NETWORK	equ 2
SAFEBOOT_DSREPAIR	equ 3	; (for Windows Domain Controllers Only)

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

	mov eax, InitSafeBootMode
	mov eax, [eax]
	mov eax, [eax]

	.if eax == NORMALBOOT
		invoke DbgPrint, $CTA0("Normal boot\n")
	.elseif eax == SAFEBOOT_MINIMAL
		invoke DbgPrint, $CTA0("Minimal safe boot\n")
	.elseif eax == SAFEBOOT_NETWORK
		invoke DbgPrint, $CTA0("Network safe boot\n")
	.elseif eax == SAFEBOOT_DSREPAIR
		invoke DbgPrint, $CTA0("Repair safe boot\n")
	.else
		invoke DbgPrint, $CTA0("Invalid safeboot option: %d\n"), eax
	.endif

	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=IsSafeBootMode

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
