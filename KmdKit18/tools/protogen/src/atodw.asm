.code

atodw proc lpAscii:DWORD
; bitRAKE
	mov	ecx, lpAscii
	xor	eax, eax
; use this line if not assume one digit and move INC ECX
;	mov	edx, '0'
	movzx	edx, BYTE PTR [ecx]
@@:
	lea	eax, [eax*4+eax]
	inc	ecx
	lea	eax, [eax*2+edx-'0']
	movzx	edx, BYTE PTR [ecx]
	cmp	edx, '9'+1
	jnb	@F
	cmp	edx, '0'
	jnb	@B
@@:
	ret
atodw endp