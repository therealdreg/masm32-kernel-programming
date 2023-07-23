; Four-F, 2002
; four-f@mail.ru

fCopyMemory proto :LPVOID, :LPVOID, :UINT
fZeroMemory proto :LPVOID, :UINT
fFillMemory proto :LPVOID, :UINT, :BYTE
new proto :DWORD
delete proto :LPVOID

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       fCopyMemory                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fCopyMemory proc pDest:LPVOID, pSour:LPVOID, cbLen:UINT

option PROLOGUE:NONE
option EPILOGUE:NONE

	push esi
	push edi								; p01:1, p3:1, p4:1

	mov eax, esp							; p01:1
	cld										; p01:4
	mov ecx, [eax + (sizeof DWORD)*5]		; p2:1
	mov esi, [eax + (sizeof DWORD)*4]		; p2:1
	push ecx
	shr ecx, 2								; p0:1
	mov edi, [eax + (sizeof DWORD)*3]		; p2:1
	rep movsd
	pop ecx
	and ecx, 011y							; p01:1
	rep movsb

	pop edi									; p01:1, p2:1
	pop esi

	ret (sizeof DWORD)*3

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

fCopyMemory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        fZeroMemory                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fZeroMemory proc pDest:LPVOID, cbLen:UINT

option PROLOGUE:NONE
option EPILOGUE:NONE

	xor eax, eax								; p01:1
	push edi									; p01:1, p3:1, p4:1
	mov ecx, [esp + sizeof DWORD * 3]			; p2:1
	mov edx, ecx								; p01:1
	cld											; p01:4
	mov edi, [esp + sizeof DWORD * 2]			; p2:1
	shr ecx, 2									; p0:1
	jz @F
	rep stosd
@@:
	and edx, 011y								; p01:1
	mov ecx, edx								; p01:1
	rep stosb
	pop edi										; p01:1, p2:1

	ret (sizeof DWORD)*2

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

fZeroMemory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        fFillMemory                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fFillMemory proc pDest:LPVOID, cbLen:UINT, Fill:BYTE

option PROLOGUE:NONE
option EPILOGUE:NONE

	push edi									; p01:1, p3:1, p4:1
	mov ecx, [esp + sizeof DWORD * 4]			; p2:1
	
	mov ch, cl
	shrd eax, ecx, 16
	mov ax, cx

	mov ecx, [esp + sizeof DWORD * 3]			; p2:1
	mov edx, ecx								; p01:1
	cld											; p01:4
	mov edi, [esp + sizeof DWORD * 2]			; p2:1
	shr ecx, 2									; p0:1
	jz @F
	rep stosd
@@:
	and edx, 011y								; p01:1
	mov ecx, edx								; p01:1
	rep stosb
	pop edi										; p01:1, p2:1

	ret (sizeof DWORD)*3

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

fFillMemory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                            malloc                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

malloc proc dwBytes:DWORD
; allocates dwBytes from current process's heap
; and returns pointer to allocated memory block.
; HeapAlloc(GetProcessHeap(), 0, dwBytes)

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke GetProcessHeap
	invoke HeapAlloc, eax, 0, [esp+4]
	ret (sizeof DWORD)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

malloc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                           free                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

free proc lpMem:LPVOID
; frees memory block allocated from current process's heap
; HeapFree(GetProcessHead(), 0, lpMem)

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke GetProcessHeap
	invoke HeapFree, eax, 0, [esp+4]
	ret (sizeof LPVOID)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

free endp