;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  giveio - Kernel Mode Driver
;
;  Demonstrate direct port I/O access from a user mode.
;   Based on c-souce by Dale Roberts
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
;                           U S E R   D E F I N E D   E Q U A T E S                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IOPM_SIZE equ 2000h				; sizeof I/O permission map

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local status:NTSTATUS
local oa:OBJECT_ATTRIBUTES
local hKey:HANDLE
local kvpi:KEY_VALUE_PARTIAL_INFORMATION
local pIopm:PVOID
local pProcess:PVOID

	invoke DbgPrint, $CTA0("giveio: Entering DriverEntry\n")
		
	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	lea ecx, oa
	InitializeObjectAttributes ecx, pusRegistryPath, 0, NULL, NULL

	invoke ZwOpenKey, addr hKey, KEY_READ, ecx
	.if eax == STATUS_SUCCESS

		push eax
		invoke ZwQueryValueKey, hKey, $CCOUNTED_UNICODE_STRING("ProcessId", 4), \
								KeyValuePartialInformation, addr kvpi, sizeof kvpi, esp
		pop ecx

		.if ( eax != STATUS_OBJECT_NAME_NOT_FOUND ) && ( ecx != 0 )

			invoke DbgPrint, $CTA0("giveio: Process ID: %X\n"), \
								dword ptr (KEY_VALUE_PARTIAL_INFORMATION PTR [kvpi]).Data

			; Allocate a buffer for the IOPM (I/O permission map).
			; Holds 8K * 8 bits -> 64K bits of the IOPM, which maps the
			;  entire 64K I/O space of the x86 processor.
			;  Any 0 bits will give access to the corresponding port for user mode processes.
			;  Any 1 bits will disallow I/O access to the corresponding port.

			invoke MmAllocateNonCachedMemory, IOPM_SIZE
			.if eax != NULL
				mov pIopm, eax

				lea ecx, kvpi
				invoke PsLookupProcessByProcessId, \
						dword ptr (KEY_VALUE_PARTIAL_INFORMATION PTR [ecx]).Data, addr pProcess
				.if eax == STATUS_SUCCESS

					invoke DbgPrint, $CTA0("giveio: PTR KPROCESS: %08X\n"), pProcess

					invoke Ke386QueryIoAccessMap, 0, pIopm
					.if al != 0

						; We need only 70h & 71h I/O port access.
						; So, we clear corresponding bits in IOPM.

						; I/O access for 70h port
						mov ecx, pIopm
						add ecx, 70h / 8
						mov eax, [ecx]
						btr eax, 70h MOD 8
						mov [ecx], eax

						; I/O access for 71h port
						mov ecx, pIopm
						add ecx, 71h / 8
						mov eax, [ecx]
						btr eax, 71h MOD 8
						mov [ecx], eax

						; Set modified IOPM

						invoke Ke386SetIoAccessMap, 1, pIopm
						.if al != 0

							; If second parameter to Ke386IoSetAccessProcess is 1, the process is given I/O access.
							; If it is 0, access is removed.

							invoke Ke386IoSetAccessProcess, pProcess, 1
							.if al != 0
								invoke DbgPrint, $CTA0("giveio: I/O permission is successfully given\n")
							.else
								invoke DbgPrint, $CTA0("giveio: I/O permission is failed\n")
								mov status, STATUS_IO_PRIVILEGE_FAILED
							.endif
						.else
							mov status, STATUS_IO_PRIVILEGE_FAILED
						.endif
					.else
						mov status, STATUS_IO_PRIVILEGE_FAILED
					.endif
					invoke ObDereferenceObject, pProcess
				.else
					mov status, STATUS_OBJECT_TYPE_MISMATCH
				.endif
				invoke MmFreeNonCachedMemory, pIopm, IOPM_SIZE
			.else
				invoke DbgPrint, $CTA0("giveio: Call to MmAllocateNonCachedMemory failed\n")
				mov status, STATUS_INSUFFICIENT_RESOURCES
			.endif
		.endif
		invoke ZwClose, hKey
	.endif

	invoke DbgPrint, $CTA0("giveio: Leaving DriverEntry\n")

	mov eax, status
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=giveio

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
