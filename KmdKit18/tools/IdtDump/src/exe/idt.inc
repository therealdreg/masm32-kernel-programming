
; See "Intel Architecture Software Developer’s Manual Volume 3: System Programming" for details

; IDT descriptor entry

KIDTENTRY STRUCT			; sizeof = 08h
	_Offset			WORD	?	; 0000h
	Selector		WORD	?	; 0002h
	Access			WORD	?	; 0004h
	ExtendedOffset	WORD	?	; 0006h
KIDTENTRY ENDS

