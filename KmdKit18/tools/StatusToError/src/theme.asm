
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               AdjustGuiIfThemedEnumChildProc                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

AdjustGuiIfThemedEnumChildProc proc hwnd:HWND, lParam:LPARAM

; lParam = TRUE if theme active or FALSE otherwise

local acClassName[64]:CHAR
local bReadOnlyEdit:BOOL

	; The things are not so easy as clearing WS_EX_STATICEDGE.
	; "edit" windows with ES_READONLY style looks better with
	; WS_EX_STATICEDGE (at least for me).  And normal "edit"
	; windows looks better without WS_EX_STATICEDGE.  So we
	; have to do more work.
	
	and bReadOnlyEdit, FALSE
					
	invoke GetClassName, hwnd, addr acClassName, sizeof acClassName
	.if eax != 0
		invoke lstrcmpi, addr acClassName, $CTA0("edit")
		.if eax == 0

			invoke GetWindowLong, hwnd, GWL_STYLE
			.if eax != 0
			.if eax & ES_READONLY
				mov bReadOnlyEdit, TRUE
			.endif
			.endif
			
		.endif
	.endif	

	; Remove WS_EX_STATICEDGE if needed

	.if lParam == TRUE && bReadOnlyEdit == FALSE
		invoke GetWindowLong, hwnd, GWL_EXSTYLE
		.if eax != 0
			and eax, not WS_EX_STATICEDGE	
			invoke SetWindowLong, hwnd, GWL_EXSTYLE, eax
		.endif
	.endif
comment ^
	; Set WS_EX_STATICEDGE if needed
	
 	.if lParam == FALSE && bStaticEdged == FALSE
		invoke GetWindowLong, hwnd, GWL_EXSTYLE
		.if eax != 0
			and eax, not WS_EX_STATICEDGE	
			invoke SetWindowLong, hwnd, GWL_EXSTYLE, eax
		.endif
	.endif
^
	; We have to enumerate all childs. So always return TRUE

	mov eax, TRUE
	ret

AdjustGuiIfThemedEnumChildProc endp
   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       AdjustGuiIfThemed                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

AdjustGuiIfThemed proc hwndMain:HWND

local bThemed:BOOL

	invoke GetModuleHandle, $CTA0("UxTheme.dll")
	.if eax != NULL
		invoke GetProcAddress, eax, $CTA0("IsAppThemed")
		.if eax != NULL
			call eax
			.if eax
				; We are themed.
				mov bThemed, TRUE
			.else
				; We are back to classic.
				and bThemed, FALSE
			.endif
				
			; Enum all child windows
			; and remove or set WS_EX_STATICEDGE style
			invoke EnumChildWindows, hwndMain, \
							AdjustGuiIfThemedEnumChildProc, bThemed
		.endif
	.endif

	ret

AdjustGuiIfThemed endp
