
malloc proto :DWORD
free proto :DWORD

.code

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

free proc lpMem:DWORD
; frees memory block allocated from current process's heap
; HeapFree(GetProcessHead(), 0, lpMem)

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke GetProcessHeap
	invoke HeapFree, eax, 0, [esp+4]
	ret (sizeof DWORD)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

free endp