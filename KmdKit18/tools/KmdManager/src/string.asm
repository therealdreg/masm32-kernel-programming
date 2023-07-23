.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                            fstrlen                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fstrlen proc pString:LPSTR

option PROLOGUE:NONE
option EPILOGUE:NONE

	mov ecx, [esp + sizeof DWORD]

	xor	eax, eax
@@:	mov	dl, [ecx+eax]
	inc	eax
	or	dl, dl
	jnz	@B
	dec	eax

	ret sizeof DWORD

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

fstrlen endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                            fstrcpy                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fstrcpy proc pBuffer:LPSTR, pString:LPSTR

option PROLOGUE:NONE
option EPILOGUE:NONE

	push esi
	push edi

	xor eax, eax

	mov esi, [esp + sizeof DWORD * 4]		; pString
	or esi, esi
	jz @F

	mov edi, [esp + sizeof DWORD * 3]		; pBuffer
	or edi, edi
	jz @F

	dec eax									; eax = -1

fstrcpy_loop:
	inc eax
	mov dl, byte ptr [esi + eax]
	or dl, dl
	mov byte ptr [edi + eax], dl
	jnz fstrcpy_loop

@@:
	pop edi
	pop esi

	ret sizeof DWORD * 2

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

fstrcpy endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                            fstrcat                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fstrcat proc pString1:LPSTR, pString2:LPSTR

; If the function succeeds, the return value is a pointer to the buffer.
; If the function fails, the return value is NULL.

option PROLOGUE:NONE
option EPILOGUE:NONE

	push esi
	push edi

	xor eax, eax

	mov edi, [esp + sizeof DWORD * 3]		; pString1
	or edi, edi
	jz @F

	mov esi, [esp + sizeof DWORD * 4]		; pString2
	or esi, esi
	jz @F

	invoke fstrlen, edi
	add edi, eax
	invoke fstrcpy, edi, esi

@@:
	pop edi
	pop esi

	ret sizeof DWORD * 2

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

fstrcat endp
