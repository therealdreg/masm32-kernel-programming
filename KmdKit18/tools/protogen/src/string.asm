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
;                                            xstrcpy                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fstrcpy proc pBuffer:LPSTR, pString:LPSTR

option PROLOGUE:NONE
option EPILOGUE:NONE

Fix Find more fast version

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
;                                          InString                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

InString proc startpos:DWORD,lpSource:DWORD,lpPattern:DWORD

  ; ------------------------------------------------------------------
  ; InString searches for a substring in a larger string and if it is
  ; found, it returns its position in eax. 
  ;
  ; It uses a one (1) based character index (1st character is 1,
  ; 2nd is 2 etc...) for both the "StartPos" parameter and the returned
  ; character position.
  ;
  ; Return Values.
  ; If the function succeeds, it returns the 1 based index of the start
  ; of the substring.
  ;  0 = no match found
  ; -1 = substring same length or longer than main string
  ; -2 = "StartPos" parameter out of range (less than 1 or longer than
  ; main string)
  ; ------------------------------------------------------------------

    LOCAL sLen:DWORD
    LOCAL pLen:DWORD

    push ebx
    push esi
    push edi

    invoke fstrlen,lpSource
    mov sLen, eax           ; source length
    invoke fstrlen,lpPattern
    mov pLen, eax           ; pattern length

    cmp startpos, 1
    jge @F
    mov eax, -2
    jmp isOut               ; exit if startpos not 1 or greater
  @@:

    dec startpos            ; correct from 1 to 0 based index

    cmp  eax, sLen
    jl @F
    mov eax, -1
    jmp isOut               ; exit if pattern longer than source
  @@:

    sub sLen, eax           ; don't read past string end
    inc sLen

    mov ecx, sLen
    cmp ecx, startpos
    jg @F
    mov eax, -2
    jmp isOut               ; exit if startpos is past end
  @@:

  ; ----------------
  ; setup loop code
  ; ----------------
    mov esi, lpSource
    mov edi, lpPattern
    mov al, [edi]           ; get 1st char in pattern

    add esi, ecx            ; add source length
    neg ecx                 ; invert sign
    add ecx, startpos       ; add starting offset

    jmp Scan_Loop

    align 16

  ; @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

  Pre_Scan:
    inc ecx                 ; start on next byte

  Scan_Loop:
    cmp al, [esi+ecx]       ; scan for 1st byte of pattern
    je Pre_Match            ; test if it matches
    inc ecx
    js Scan_Loop            ; exit on sign inversion

 ;     cmp ecx, 0           ; works but 1 instruction longer
 ;     jl Scan_Loop

    jmp No_Match

  Pre_Match:
    lea ebx, [esi+ecx]      ; put current scan address in EBX
    mov edx, pLen           ; put pattern length into EDX

  Test_Match:
    mov ah, [ebx+edx-1]     ; load last byte of pattern length in main string
    cmp ah, [edi+edx-1]     ; compare it with last byte in pattern
    jne Pre_Scan            ; jump back on mismatch
    dec edx
    jnz Test_Match          ; 0 = match, fall through on match

  ; @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

  Match:
    add ecx, sLen
    mov eax, ecx
    inc eax
    jmp isOut
    
  No_Match:
    xor eax, eax

  isOut:
    pop edi
    pop esi
    pop ebx

    ret

InString endp

