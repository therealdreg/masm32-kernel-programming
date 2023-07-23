;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  SEH - Structured Exception Handling. An example how to handle exceptions.
;
;  WARNING: This is raw-SEH. So, do not use it to wrap ProbeForRead, ProbeForWrite etc...
;
; If you don't know what SEH is, find and read the article
; "Win32 Exception handling for assembler programmers" by Jeremy Gordon.
; It's about exceptions handling in user mode but the principles absolutely the same.
; But remember...
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                  YOU CAN'T HANDLE ALL EXCEPTIONS WITH SEH IN KERNEL MODE !!!                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; For example, the attempt to divide by zero will result in BSOD even with installed SEH-handler.
; Even worse, we must not reference an invalid kernel-mode memory :-(
;
; According to Gary Nebbett's research the following exceptions can be trapped
; with the SEH when they occur at IRQL less than or equal to DISPATCH_LEVEL:
; - Any exception thrown by ExRaiseStatus and related functions
; - Reference to invalid user-mode memory
; - Breakpoint exception
; - Integer overflow
; - Invalid opcode
;
; So, we just have to write our code so as not to generate not listed above exceptions.
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

include seh0.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     S T R U C T U R E S                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SEH STRUCT 
	SafeEip			dd	?	; The offset where it's safe to continue execution
	PrevEsp			dd	?	; The previous value of esp 
	PrevEbp			dd	?	; The previous value of ebp 
SEH ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

seh	SEH	<>

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       BuggyReader                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BuggyReader proc

	xor eax, eax
	mov eax, [eax]				; !!! Without SEH this causes BSOD !!!

	ret

BuggyReader endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       BuggyWriter                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BuggyWriter proc

	mov eax, MmUserProbeAddress
	mov eax, [eax]
	mov eax, [eax]
	
	mov byte ptr [eax], 0		; !!! Without SEH this causes BSOD !!!

	ret

BuggyWriter endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      ExceptionHandler                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ExceptionHandler proc C uses esi pExcept:DWORD, pFrame:DWORD, pContext:DWORD, pDispatch:DWORD

	mov esi, pExcept

	invoke DbgPrint, $CTA0("\nSEH: An exception %08X has occured\n"), \
						[esi][EXCEPTION_RECORD.ExceptionCode]

	.if [esi][EXCEPTION_RECORD.ExceptionCode] == 0C0000005h			; EXCEPTION_ACCESS_VIOLATION

		; if EXCEPTION_ACCESS_VIOLATION we have some additional info

		invoke DbgPrint, $CTA0("     Access violation at address: %08X\n"), \
						[esi][EXCEPTION_RECORD.ExceptionAddress]

		.if [esi][EXCEPTION_RECORD.ExceptionInformation][0]			; Read or write ?

			invoke DbgPrint, $CTA0("     The code tried to write to address %08X\n\n"), \
						[esi][EXCEPTION_RECORD.ExceptionInformation][4]
		.else
			invoke DbgPrint, $CTA0("     The code tried to read from address %08X\n\n"), \
						[esi][EXCEPTION_RECORD.ExceptionInformation][4]
		.endif
	.endif

	lea eax, seh
    push (SEH PTR [eax]).SafeEip
    push (SEH PTR [eax]).PrevEsp
    push (SEH PTR [eax]).PrevEbp

	mov eax, pContext
    pop (CONTEXT PTR [eax]).regEbp
    pop (CONTEXT PTR [eax]).regEsp
    pop (CONTEXT PTR [eax]).regEip

    xor eax, eax			; return ExceptionContinueExecution
    ret 

ExceptionHandler endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

	invoke DbgPrint, $CTA0("\nSEH: Entering DriverEntry\n")

	;::::::::::::::::::::::::::::::::
	; Manually set/remove seh-frame :
	;::::::::::::::::::::::::::::::::

	assume fs:nothing
	push offset ExceptionHandler
	push fs:[0]
	mov fs:[0], esp
	assume fs:error

	mov seh.SafeEip, offset SafePlace
	mov seh.PrevEbp, ebp
	mov seh.PrevEsp, esp

	invoke BuggyReader

SafePlace:
	; Remove seh-frame  
	assume fs:nothing
	pop fs:[0]
	add esp, sizeof DWORD
	assume fs:error


	;:::::::::::::::::::::::::::::::::::::::::::::::
	; SEH works using macro. It's a bit easier ;-) :
	;:::::::::::::::::::::::::::::::::::::::::::::::

	_try

	invoke BuggyWriter

	_finally


	invoke DbgPrint, $CTA0("\nSEH: Leaving DriverEntry\n")

	; Remove driver from the memory.
	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=seh

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
