;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  GetKernelBase - Kernel Mode Driver
;    Finds the base of ntoskrnl.exe
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
;                                        C O D E                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

GetKernelBase proc uses esi edi ebx dwSomewhereInKernel:DWORD

	xor edi, edi			; assume error

	mov eax, MmSystemRangeStart
	mov eax, [eax]
	mov eax, [eax]

	.if dwSomewhereInKernel >= eax

		mov esi, dwSomewhereInKernel
		and esi, not (PAGE_SIZE-1)			; start down-search from here
		mov ebx, esi
		sub ebx, eax						; - MmSystemRangeStart
		shr ebx, PAGE_SHIFT					; Number of pages to search

		.while ebx
			invoke MmIsAddressValid, esi
			.break .if al == FALSE				; bad
			mov eax, [esi]
			.if eax == 00905A4Dh			; MZ signature
				mov edi, esi
				.break
			.endif
			sub esi, PAGE_SIZE				; next page down
			dec ebx							; next page
		.endw

	.endif

	mov eax, edi
	ret

GetKernelBase endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

	lea ecx, [ebp][4]
	push ecx
	invoke MmIsAddressValid, ecx
	pop ecx
	.if al
		mov ecx, [ecx]					; Get return address from stack
		invoke GetKernelBase, ecx
		.if eax != 0
			invoke DbgPrint, $CTA0("GetKernelBase: ntoskrnl.exe base = %08X\n"), eax
		.else
			invoke DbgPrint, $CTA0("GetKernelBase: Couldn't find ntoskrnl.exe base\n")
		.endif
	.endif

	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=GetKernelBase

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
