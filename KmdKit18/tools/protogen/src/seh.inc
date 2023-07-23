; SEH.inc (MASM)

SEH STRUCT 
	SafeOffset		dd	?	; The offset where it's safe to continue execution 
	PrevEsp			dd	?	; the old value in esp 
	PrevEbp			dd	?	; The old value in ebp 
SEH ENDS

SEH_SafePlaceCounter	= 0

externdef g_hConsoleOutput:HANDLE

PrintConsole proto :LPSTR, :DWORD
externdef PrintConsole:proc

;g_cbInclude

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_try MACRO SafePlace, xHandler:=<DefaultExceptionHandler>
	local sp_lbl

	PUSHCONTEXT ASSUMES    
	assume fs:nothing
	
	IFNDEF seh
		.data
		seh	SEH <>
		.code
	ENDIF
   
	push offset xHandler
	push fs:[0]						; address of next ERR structure
	mov fs:[0], esp					; give FS:[0] the ERR address just made

	IFB <SafePlace>
		sp_lbl TEXTEQU @CatStr(<SEH_SafePlace>, %(SEH_SafePlaceCounter))
		IFDEF DEBUG
		% echo sp_lbl
		ENDIF
		mov seh.SafeOffset, offset sp_lbl
	ELSE
		mov seh.SafeOffset, offset SafePlace	
	ENDIF
	mov seh.PrevEbp, ebp
	mov seh.PrevEsp, esp

	POPCONTEXT ASSUMES

ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_finally MACRO
	local sp_lbl

	sp_lbl TEXTEQU @CatStr(<SEH_SafePlace>, %(SEH_SafePlaceCounter))
	SEH_SafePlaceCounter = SEH_SafePlaceCounter + 1
	sp_lbl:

	PUSHCONTEXT ASSUMES    
	assume fs:nothing
	pop fs:[0]						; restore next ERR structure to FS:[0]
	add esp, sizeof DWORD			; throw away rest of ERR structure
	POPCONTEXT ASSUMES
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_throw MACRO ExceptionCode:REQ
;	IFB <ExceptionCode>
;		ECHO _throw macro ERROR! Specify Exception Code.
;		.ERR; <_throw macro ERROR! Specify Exception Code.>
;	ENDIF
	xor eax, eax
	invoke	RaiseException, ExceptionCode, eax, eax, eax
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DefaultExceptionHandler proc C uses esi pExcept:DWORD, pFrame:DWORD, pContext:DWORD, pDispatch:DWORD

;local dwNumberOfBytesWritten:DWORD
local acMessage[128]:CHAR

	mov esi, pExcept

	.if [esi][EXCEPTION_RECORD.ExceptionCode] == EXCEPTION_ACCESS_VIOLATION
		; which address ?
		mov eax, [esi][EXCEPTION_RECORD.ExceptionInformation][4]

		mov ecx, g_pInclude
		add ecx, g_cbInclude

		mov edx, g_pSymEntries
		add edx, g_cbSymEntries

		.if ( ( eax >= g_pInclude ) && ( eax < ecx ) ) || ( ( eax >= g_pSymEntries ) && ( eax < edx ) )
			.if [esi][EXCEPTION_RECORD.ExceptionInformation][0]			; Read or write ?
				; parser needs more memory to write data to
				; let's commit one page more

				invoke VirtualAlloc, [esi][EXCEPTION_RECORD.ExceptionInformation][4], \ 
										1000h , MEM_COMMIT, PAGE_READWRITE
				.if eax == NULL
					; if could not be committed prompt ot user
;					invoke WriteFile, g_hConsoleOutput, \
;								$CTA0("\nException handling: Insufficient Memory Available\:\n\n", szInsufficientMemory), \
;								sizeof szInsufficientMemory, addr dwNumberOfBytesWritten, NULL
					invoke PrintConsole, $CTA0("\nException handling: Insufficient Memory Available\:\n\n"), 0

				.else
					; exception successefuly handled. let's continue
					; reload context & continue execution
					IFDEF DEBUG
					invoke wsprintf, addr acMessage, \
							$CTA0("\nException handling: Memory (one page) allocated at address %08X\n\n"), eax
;					mov ecx, eax
;					invoke WriteFile, g_hConsoleOutput, addr acMessage, ecx, addr dwNumberOfBytesWritten, NULL
					invoke PrintConsole, addr acMessage, 0
					ENDIF
				    xor eax, eax		;mov eax, ExceptionContinueExecution
				    ret
				.endif

			.endif
		.else
			Fix Remove this
			mov eax, g_pInclude
			add eax, g_cbInclude
		.endif
	.endif

	invoke PrintConsole, $CTA0("\nAn exception has occured\:\n"), FOREGROUND_RED + FOREGROUND_INTENSITY

	CTA  "  Exception Code:    %08X\n", g_szExceptionInfo
	CTA0 "  Exception Address: %08X\n\n"
	CTA0 "  Attempt to %s data at address %08X\n\n", g_szAccessViolation

	invoke wsprintf, addr acMessage, addr g_szExceptionInfo, [esi][EXCEPTION_RECORD.ExceptionCode], [esi][EXCEPTION_RECORD.ExceptionAddress]
;	mov ecx, eax
;	invoke WriteFile, g_hConsoleOutput, addr acMessage, ecx, addr dwNumberOfBytesWritten, NULL
	invoke PrintConsole, addr acMessage, 0

	.if [esi][EXCEPTION_RECORD.ExceptionCode] == EXCEPTION_ACCESS_VIOLATION
		; There is some extra info
		.if [esi][EXCEPTION_RECORD.ExceptionInformation][0]			; Read or write ?
			mov eax, $CTA0("write")
		.else
			mov eax, $CTA0("read")
		.endif
		invoke wsprintf, addr acMessage, addr g_szAccessViolation, eax, [esi][EXCEPTION_RECORD.ExceptionInformation][4]
;		mov ecx, eax
;		invoke WriteFile, g_hConsoleOutput, addr acMessage, ecx, addr dwNumberOfBytesWritten, NULL
		invoke PrintConsole, addr acMessage, 0
	.endif

	IFNDEF seh
		.data
		seh	SEH <>
		.code
	ENDIF

	lea eax, seh
    push (SEH PTR [eax]).SafeOffset
    push (SEH PTR [eax]).PrevEsp
    push (SEH PTR [eax]).PrevEbp

	mov eax, pContext
    pop (CONTEXT PTR [eax]).regEbp
    pop (CONTEXT PTR [eax]).regEsp
    pop (CONTEXT PTR [eax]).regEip

	; reload context & continue execution
    xor eax, eax						;mov eax, ExceptionContinueExecution

    ret 

DefaultExceptionHandler endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
