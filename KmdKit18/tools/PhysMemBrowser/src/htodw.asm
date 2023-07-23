.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

htodw proc uses edx esi pString:LPSTR

option PROLOGUE:none
option EPILOGUE:none

; pHexString equ [esp + 0Ch]

	push esi
	push edx

	mov esi, [esp + 0Ch] ; pHexString
	xor edx, edx

ALIGN 4

htodw_loop:     
	mov al, [esi] 
	inc esi
	sub   al, "0"
	js   htodw_endloop
	shl   edx, 4
	cmp   al, 9
	jbe   @F
	sub   al, "a" - "0" - 10
	jns   @F
	add al, 20h

@@:
	xor dl, al
	jmp htodw_loop

htodw_endloop:
	mov eax, edx

	pop edx
	pop esi

	ret sizeof DWORD

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

htodw endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::