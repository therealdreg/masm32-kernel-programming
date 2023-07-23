
.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        CheckMask                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CheckMask proc uses esi edi lpabyMask:LPVOID, lpstrClipboardText:LPSTR

	xor eax, eax
	inc eax					; eax = TRUE		assume check passed

	mov esi, lpabyMask
	mov edi, lpstrClipboardText
	xor ecx, ecx
	.while byte ptr [edi] != 0				; scan string
		mov cl, byte ptr [edi]
		.if ( BYTE PTR [esi][ecx] != TRUE)
			xor eax, eax					; check failed
			.break
		.endif
		inc edi								; next char
	.endw

	ret

CheckMask endp
						
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       MaskedEditProc                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MaskedEditProc proc uses edi ebx hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local hClipboardData:HANDLE
local fDoPaste:BOOL

	invoke GetWindowLong, hWnd, GWL_USERDATA
	mov edi, eax
	mov ebx, [edi][256]

	; WM_NCDESTROY is the LAST message that a window will receive - 
	; therefore we must finally remove the old wndproc here
	.if uMsg == WM_NCDESTROY
		invoke free, edi

	.elseif uMsg == WM_PASTE

		and fDoPaste, FALSE				; assume bad clipboard data

		invoke IsClipboardFormatAvailable, CF_TEXT
		.if eax != 0

			invoke OpenClipboard, NULL
			.if eax != 0

				invoke GetClipboardData, CF_TEXT
				.if eax != NULL 	

					mov hClipboardData, eax
					invoke GlobalLock, hClipboardData
					.if eax != NULL

						invoke CheckMask, edi, eax
						.if eax == TRUE
							inc fDoPaste				; TRUE
						.endif
 
						invoke GlobalUnlock, hClipboardData
					.endif
				.endif

				invoke CloseClipboard
			.endif
		.endif

		.if fDoPaste == FALSE
			jmp @F										; don't paste bad data
		.endif

	.elseif uMsg == WM_CHAR

		invoke GetAsyncKeyState, VK_CONTROL		; allow clipboard works
		.if !( eax && 80000000h )
			mov eax, wParam
			and eax, 0FFh

			.if ( BYTE PTR [edi][eax] != TRUE) && !( eax == VK_BACK )
				xor eax, eax
				ret
			.endif
		.endif

	.endif

	invoke CallWindowProc, ebx, hWnd, uMsg, wParam, lParam
@@:
	ret

MaskedEditProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   MaskEditControl                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MaskEditControl proc uses esi edi ebx hwndEdit:HWND, pszMask:LPVOID, fOptions:BOOL

	; don't make a new mask if there is already one available
	
	invoke GetWindowLong, hwndEdit, GWL_WNDPROC
	mov ebx, eax

	.if ebx != MaskedEditProc
		invoke malloc, (256 + sizeof LPVOID)
		mov edi, eax
	.else
		invoke GetWindowLong, hwndEdit, GWL_USERDATA
		mov edi, eax
	.endif

	; build the mask lookup table. The method varies depending
	; on whether we want to allow or disallow the specified szMask characters
	mov esi, pszMask
	.if fOptions == TRUE
		invoke fZeroMemory, edi, 256
		xor ecx, ecx
		xor eax, eax
		.while BYTE PTR [esi][ecx] != 0
			mov al, [esi][ecx]
			mov BYTE PTR [edi][eax], TRUE
			inc ecx
		.endw
	.else
		invoke fFillMemory, edi, 256, TRUE
		xor ecx, ecx
		xor eax, eax
		.while BYTE PTR [esi][ecx] != 0
			mov al, [esi][ecx]
			mov BYTE PTR [edi][eax], FALSE
			inc ecx
		.endw	
	.endif

	; don't update the user data if it is already in place
	.if ebx != MaskedEditProc
		invoke SetWindowLong, hwndEdit, GWL_WNDPROC, addr MaskedEditProc
		mov [edi][256], eax
		invoke SetWindowLong, hwndEdit, GWL_USERDATA, edi
	.endif

	xor eax, eax
	inc eax
	ret						; return TRUE

MaskEditControl endp
