;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  FindShadowTable - How to find ServiceDescriptorTableShadow
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
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                            GetServiceDescriptorTableShadowAddress                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetServiceDescriptorTableShadowAddress proc uses esi edi ebx

local dwThreadId:DWORD

	xor ebx, ebx				; = NULL. Assume ServiceDescriptorTableShadow will be not found

	mov eax, KeServiceDescriptorTable
	mov esi, [eax]

	; Find KTHREAD.ServiceTable field
	; For non-GUI threads this field == KeServiceDescriptorTable
	; and it points to ServiceDescriptorTable
	; For GUI threads
	; ServiceDescriptorTableShadow

	invoke KeGetCurrentThread
	mov edi, 200h-4
	.while edi
		.break .if dword ptr [eax][edi] == esi
		dec edi
	.endw

	.if edi != 0
		; edi = offset to ServiceTable field in KTHREAD structure
		mov dwThreadId, 080h
		.while dwThreadId < 400h
			push eax					; reserve DWORD on stack
			invoke PsLookupThreadByThreadId, dwThreadId, esp
			pop ecx						; -> ETHREAD/KTHREAD
			.if eax == STATUS_SUCCESS
				push dword ptr [ecx][edi]
				fastcall ObfDereferenceObject, ecx
				pop eax
				.if eax != esi
					mov edx, MmSystemRangeStart
					mov edx, [edx]
					mov edx, [edx]
					.if eax > edx		; some stupid error checking
						mov ebx, eax
						invoke DbgPrint, $CTA0("FindShadowTable: Found in thread with ID: %X\n"), dwThreadId
						.break
					.endif
				.endif
			.endif
			add dwThreadId, 4
		.endw
	.endif

	mov eax, ebx
	ret

GetServiceDescriptorTableShadowAddress endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

	invoke DbgPrint, $CTA0("\nFindShadowTable: Entering DriverEntry\n")

	mov eax, KeServiceDescriptorTable
	mov eax, [eax]
	invoke DbgPrint, $CTA0("FindShadowTable: ServiceDescriptorTable at address: %08X\n"), eax

	invoke GetServiceDescriptorTableShadowAddress
	.if eax != NULL
		invoke DbgPrint, $CTA0("FindShadowTable: ServiceDescriptorTableShadow found at address: %08X\n"), eax
	.endif

	invoke DbgPrint, $CTA0("FindShadowTable: Leaving DriverEntry\n")

	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=FindShadowTable

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
