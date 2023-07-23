;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  SystemModules - How to allocate/free from/to system pool.
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
include \masm32\include\w2k\native.inc
include \masm32\include\w2k\ntoskrnl.inc

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              D I S C A R D A B L E   C O D E                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code INIT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc uses esi edi ebx pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local cb:DWORD
local p:PVOID
local dwNumModules:DWORD
local pMessage:LPSTR
local buffer[256+40]:CHAR

	invoke DbgPrint, $CTA0("\nSystemModules: Entering DriverEntry\n")

	and cb, 0
	; How much space we need? Use p as fake memory.
	invoke ZwQuerySystemInformation, SystemModuleInformation, addr p, 0, addr cb
	.if cb != 0
		invoke ExAllocatePool, PagedPool, cb
		.if eax != NULL
			mov p, eax
			invoke DbgPrint, $CTA0("SystemModules: %u bytes of paged memory allocted at address %08X\n"), cb, p
			; Now we have memory buffer with appropriate size. Call ZwQuerySystemInformation again.
			invoke ZwQuerySystemInformation, SystemModuleInformation, p, cb, addr cb
			.if eax == STATUS_SUCCESS
				mov esi, p

				; First DWORD is a number of SYSTEM_MODULE_INFORMATION an the array pointed by esi+4
				push dword ptr [esi]
				pop dwNumModules

				; Allocate memory enough for module name and some additional info
				mov cb, (sizeof SYSTEM_MODULE_INFORMATION.ImageName + 100)*2	; 256 + 40 for module should be enough
				invoke ExAllocatePool, PagedPool, cb
				.if eax != NULL
					mov pMessage, eax
					invoke DbgPrint, $CTA0("SystemModules: %u bytes of paged memory allocted at address %08X\n"), \
													cb, pMessage
					; zero memory buffer
					invoke memset, pMessage, 0, cb

					add esi, sizeof DWORD
					; Now esi -> first SYSTEM_MODULE_INFORMATION in the array
					assume esi:ptr SYSTEM_MODULE_INFORMATION
					xor ebx, ebx
					; Find "ntoskrnl" module. It should be here
					.while ebx < dwNumModules
						lea edi, [esi].ImageName
						movzx ecx, [esi].ModuleNameOffset
						add edi, ecx

						; Compare case insensitive

						; If you have multiprocessor system use "ntkrnlmp.exe".
						; If your system has PAE - "ntkrnlpa.exe"
						; Multiprocessor + PAE - "ntkrpamp.exe"

                        invoke _strnicmp, edi, $CTA0("ntoskrnl.exe", szNtoskrnl, 4), sizeof szNtoskrnl - 1
                        push eax
                        invoke _strnicmp, edi, $CTA0("ntice.sys", szNtIce, 4), sizeof szNtIce - 1

						pop ecx
						and eax, ecx
						.if ZERO?
							; Found either ntoskrnl or ntice
							invoke _snprintf, addr buffer, sizeof buffer, \
									$CTA0("SystemModules: Found %s base: %08X size: %08X\n", 4), edi, [esi].Base, [esi]._Size
							invoke strcat, pMessage, addr buffer
						.endif

						add esi, sizeof SYSTEM_MODULE_INFORMATION
						inc ebx
					.endw
					assume esi:nothing

					mov eax, pMessage
					.if byte ptr [eax] != 0
						invoke DbgPrint, pMessage
					.else
						invoke DbgPrint, $CTA0("SystemModules: Found neither ntoskrnl nor ntice.\n")
					.endif

					invoke ExFreePool, pMessage
					invoke DbgPrint, $CTA0("SystemModules: Memory at address %08X released\n"), pMessage
				.endif
			.endif
			invoke ExFreePool, p
			invoke DbgPrint, $CTA0("SystemModules: Memory at address %08X released\n"), p
		.endif
	.endif

	invoke DbgPrint, $CTA0("SystemModules: Leaving DriverEntry\n")

	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=SystemModules

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj

echo.
pause
