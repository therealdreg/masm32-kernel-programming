;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  seh0.inc
;
;  Structured Exception Handling helper macros
;
;  Can not be nested!
;
;  Written by Four-F (four-f@mail.ru)
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_SEH STRUCT 
	SafeEip		dd	?	; The offset where it's safe to continue execution 
	PrevEsp			dd	?	; The previous value of esp 
	PrevEbp			dd	?	; The previous value of ebp 
_SEH ENDS

SEH_SafePlaceCounter	= 0

SEH_INSTALLED			= 0


SEH_CurrentSegmentName TEXTEQU @CurSeg
SEH_CurrentSegmentNameLenght TEXTEQU %@SizeStr(%@CurSeg)
.data?
	_seh	_SEH <>
@CurSeg ENDS
IF SEH_CurrentSegmentNameLenght NE 0
	SEH_CurrentSegmentName SEGMENT
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_try MACRO SafePlace, xHandler:=<DefaultExceptionHandler>
	local sp_lbl

	IF SEH_INSTALLED GT 0
		echo ERROR!: Nested SEH-frames are not supported
		.ERR
	ELSE
		SEH_INSTALLED = SEH_INSTALLED + 1
	ENDIF

	PUSHCONTEXT ASSUMES    
	assume fs:nothing

	push offset xHandler
	push fs:[0]						; address of next ERR structure
	mov fs:[0], esp					; give FS:[0] the ERR address just made
	POPCONTEXT ASSUMES

	IFB <SafePlace>
		sp_lbl TEXTEQU @CatStr(<SEH_SafePlace>, %(SEH_SafePlaceCounter))
		mov _seh.SafeEip, offset sp_lbl
	ELSE
		mov _seh.SafeEip, offset SafePlace	
	ENDIF
	mov _seh.PrevEbp, ebp
	mov _seh.PrevEsp, esp

ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_finally MACRO
	local sp_lbl

	IF SEH_INSTALLED EQ 0
		echo ERROR!: _finally without _try
		.ERR
	ELSE
		SEH_INSTALLED = SEH_INSTALLED - 1
	ENDIF

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

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DefaultExceptionHandler proc C pExcept:DWORD, pFrame:DWORD, pContext:DWORD, pDispatch:DWORD

	mov eax, pExcept
	invoke DbgPrint, $CTA0("Exception: %08X at address: %08X\n"), \
						[eax][EXCEPTION_RECORD.ExceptionCode], \
						[eax][EXCEPTION_RECORD.ExceptionAddress]

	lea eax, _seh
    push (_SEH PTR [eax]).SafeEip
    push (_SEH PTR [eax]).PrevEsp
    push (_SEH PTR [eax]).PrevEbp

	mov eax, pContext
    pop (CONTEXT PTR [eax]).regEbp
    pop (CONTEXT PTR [eax]).regEsp
    pop (CONTEXT PTR [eax]).regEip

	; reload context & continue execution
    xor eax, eax			; return ExceptionContinueExecution
    ret 

DefaultExceptionHandler endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
