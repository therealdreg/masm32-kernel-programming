; Written by Four-F
; seh3.inc (MASM)

	IFNDEF RaiseException
		include \masm32\include\kernel32.inc
		includelib \masm32\lib\kernel32.lib
	ENDIF

	IFNDEF MessageBox
		include \masm32\include\user32.inc
		includelib \masm32\lib\user32.lib
	ENDIF

	IFNDEF EXCEPTION_STACK_OVERFLOW
		EXCEPTION_STACK_OVERFLOW equ STATUS_STACK_OVERFLOW
	ENDIF

SEH_SafePlaceCounter	= 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_try MACRO SafePlace, xHandler:=<DefaultExceptionHandler>
	local sp_lbl

	PUSHCONTEXT ASSUMES    
	assume fs:nothing

	push offset xHandler
	push fs:[0]						; address of next ERR structure
	mov fs:[0], esp					; give FS:[0] the ERR address just made

	POPCONTEXT ASSUMES

	push esp
	push ebp
	IFB <SafePlace>
		sp_lbl TEXTEQU @CatStr(<SEH_SafePlace>, %(SEH_SafePlaceCounter))
		% ECHO sp_lbl
		push offset sp_lbl	
	ELSE
		push offset SafePlace	
	ENDIF
	push esi
	push edi
	push ebx

ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_finally MACRO
	local sp_lbl

	sp_lbl TEXTEQU @CatStr(<SEH_SafePlace>, %(SEH_SafePlaceCounter))
	SEH_SafePlaceCounter = SEH_SafePlaceCounter + 1
	sp_lbl:
comment ^
	PUSHCONTEXT ASSUMES    
	assume fs:nothing

	mov eax, [esp][-0Ch]			; as we use stack to hold ebp, esp and SafePlace
									; esp now points to wrong place
	mov fs:[0], eax					; restore next ERR structure to FS:[0]

	add esp, (sizeof DWORD) * 2		; throw away ERR structure
	POPCONTEXT ASSUMES
^

	PUSHCONTEXT ASSUMES    
	add esp, 0Ch*2
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

DefaultExceptionHandler proc C pExcept:DWORD, pFrame:DWORD, pContext:DWORD, pDispatch:DWORD

local acMessage[MAX_PATH*2]:CHAR

	.data
	g_szExceptCaption	db "An exception has occured!", 0
	g_szException		db	"Exception Code:", 9, "%08X", 13, 10, "Exception Address:", 9, "%08X", 0

	.code
	mov eax, pExcept
	push [eax][EXCEPTION_RECORD.ExceptionAddress]
	push [eax][EXCEPTION_RECORD.ExceptionCode]
	push offset g_szException
	lea eax, acMessage
	push eax
	call wsprintf
	add esp, 10h

	invoke	MessageBox, NULL, addr acMessage, addr g_szExceptCaption, MB_OK + MB_ICONHAND + MB_TASKMODAL

	mov eax, pFrame
	sub eax, 0Ch*2
	mov ecx, esp
	mov esp, eax
	mov eax, pContext

	pop (CONTEXT PTR [eax]).regEbx
	pop (CONTEXT PTR [eax]).regEdi
	pop (CONTEXT PTR [eax]).regEsi

	pop (CONTEXT PTR [eax]).regEip
	pop (CONTEXT PTR [eax]).regEbp
	pop edx
	sub edx, 0Ch*2
	mov (CONTEXT PTR [eax]).regEsp, edx
	mov esp, ecx

	; reload context & continue execution
    xor eax, eax						;mov eax, ExceptionContinueExecution

    ret 

DefaultExceptionHandler endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
