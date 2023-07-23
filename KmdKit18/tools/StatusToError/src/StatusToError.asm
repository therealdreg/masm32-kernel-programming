;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  Nt Status To Win32 Error - Let you convert STATUS_XXX to Win32 Error
;
;  Written by Four-F (four-f@mail.ru)
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.386
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\windows.inc

include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\comctl32.inc
include \masm32\include\gdi32.inc
include \masm32\include\shell32.inc
include \masm32\include\w2k\ntdll.inc

include \masm32\include\w2k\ntstatus.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\comctl32.lib
includelib \masm32\lib\gdi32.lib
includelib \masm32\lib\shell32.lib
includelib \masm32\lib\w2k\ntdll.lib

include \masm32\Macros\Strings.mac
include Macros.mac
include memory.asm
include theme.asm

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        E Q U A T E S                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IDD_DIALOG				equ 1000

IDC_ERROR_DESCRIPTION	equ 1001
IDC_NT_STATUS			equ 1002
IDC_DOS_ERROR_ID		equ 1003
IDC_ALWAYS_ON_TOP		equ 1004

IDM_ABOUT				equ	2000

IDI_ICON				equ 3000

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              I N I T I A L I Z E D  D A T A                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
szAbout				db "About...", 0
szWrittenBy			db "Nt Status To Win32 Error v1.3", 0Ah, 0Dh
					db "Built on "
					date
					db 0Ah, 0Dh, 0Ah, 0Dh
					db "Written by Four-F <four-f@mail.ru>", 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_hInstance					HINSTANCE	?
g_hwndSpin					HWND		?
g_hwndEditDosErrorId		HWND		?
g_hwndEditNtStatus			HWND		?
g_hwndEditErrorDescription	HWND		?
g_lpBuffer					LPDWORD		?
g_hDll						HANDLE		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                             htodw                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

htodw proc uses esi pString:LPSTR

option PROLOGUE:none
option EPILOGUE:none

	push esi
	push edx

	mov esi, [esp + 0Ch]
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
;                                  MaskedEditProc                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MaskedEditProc proc uses edi ebx hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

	mov edi, $invoke(GetWindowLong, hWnd, GWL_USERDATA)
	mov ebx, [edi][256]

	; WM_NCDESTROY is the LAST message that a window will receive - 
	; therefore we must finally remove the old wndproc here
	.if uMsg == WM_NCDESTROY
		invoke free, edi

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
	ret

MaskedEditProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   MaskEditControl                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MaskEditControl proc uses esi edi ebx hwndEdit:HWND, pszMask:LPVOID, fOptions:BOOL

	; don't make a new mask if there is already one available
	mov ebx, $invoke(GetWindowLong, hwndEdit, GWL_WNDPROC)

	.if ebx != MaskedEditProc
		mov edi, $invoke(malloc, (256 + sizeof LPVOID))
	.else
		mov edi, $invoke(GetWindowLong, hwndEdit, GWL_USERDATA)
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
		mov [edi][256], $invoke(SetWindowLong, hwndEdit, GWL_WNDPROC, addr MaskedEditProc)
		invoke SetWindowLong, hwndEdit, GWL_USERDATA, edi
	.endif

	ret

MaskEditControl endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        DlgProc                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local dwNtStatus:DWORD
local dwErrorId:DWORD
local buffer[16]:CHAR

local lf:LOGFONT
local p:POINT

	mov eax, uMsg
	.if eax == WM_INITDIALOG

		; If we XP themed, remove WS_EX_STATICEDGE. Looks better.
		
		invoke AdjustGuiIfThemed, hDlg

		invoke GetDlgItem, hDlg, IDC_NT_STATUS
		mov g_hwndEditNtStatus, eax

		; Tnx James Brown
		invoke MaskEditControl, g_hwndEditNtStatus, $CTA0("0123456789abcdefABCDEF"), TRUE

		invoke SendMessage, g_hwndEditNtStatus, EM_SETLIMITTEXT, 8, 0

		; STATUS_INSUFFICIENT_RESOURCES
		invoke SendMessage, g_hwndEditNtStatus, WM_SETTEXT, 0, $CTA0("C000009A")

		invoke GetDlgItem, hDlg, IDC_DOS_ERROR_ID
		mov g_hwndEditDosErrorId, eax

;		invoke GetDlgItem, hDlg, IDC_ERROR_DESCRIPTION
;		mov g_hwndEditErrorDescription, eax
	     
		invoke CreateUpDownControl, WS_CHILD + WS_BORDER + WS_VISIBLE + UDS_ALIGNRIGHT, \
							0, 0, 0, 0, hDlg, 0, g_hInstance, g_hwndEditNtStatus, 1, 0, 0
		mov g_hwndSpin, eax

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

		; Add "About..." to sys menu
		invoke GetSystemMenu, hDlg, FALSE
		push eax
		invoke InsertMenu, eax, -1, MF_BYPOSITION + MF_SEPARATOR, 0, 0
		pop eax
		invoke InsertMenu, eax, -1, MF_BYPOSITION + MF_STRING, IDM_ABOUT, offset szAbout

	.elseif eax == WM_CLOSE
		invoke EndDialog, hDlg, 0

	.elseif eax == WM_NOTIFY
		mov eax, lParam
		mov ecx, (NMHDR ptr [eax]).hwndFrom
		.if ( ecx == g_hwndSpin ) && ( [NMHDR ptr [eax]].code == UDN_DELTAPOS )

			invoke SendMessage, g_hwndEditNtStatus, WM_GETTEXT, sizeof buffer, addr buffer
			invoke htodw, addr buffer
			mov ecx, lParam
			mov ecx, (NM_UPDOWN ptr [ecx]).iDelta
			.if ecx == 1
				inc eax
			.elseif ecx == -1
				dec eax
			.endif

			invoke wsprintf, addr buffer, $CTA0("%08X", szHexFmt), eax
			invoke SendMessage, g_hwndEditNtStatus, WM_SETTEXT, 0, addr buffer

		.endif

	.elseif eax == WM_COMMAND

		mov eax, $LOWORD(wParam)

		.if eax == IDCANCEL
			invoke EndDialog, hDlg, 0
		.elseif eax == IDC_ALWAYS_ON_TOP

			invoke IsDlgButtonChecked, hDlg, IDC_ALWAYS_ON_TOP

			.if eax==BST_CHECKED
				mov eax, HWND_TOPMOST
			.else
				mov eax, HWND_NOTOPMOST
			.endif
			invoke SetWindowPos, hDlg, eax, 0, 0, 0, 0, SWP_NOMOVE + SWP_NOSIZE
        
		.else
			mov eax, $HIWORD(wParam)

			.if eax == EN_CHANGE
				mov eax, lParam
				.if eax == g_hwndEditNtStatus

;					invoke SendMessage, g_hwndEditNtStatus, WM_GETTEXT, sizeof buffer, addr buffer
					invoke GetDlgItemText, hDlg, IDC_NT_STATUS, addr buffer, sizeof buffer

					invoke htodw, addr buffer
					mov dwNtStatus, eax

					invoke RtlNtStatusToDosError, dwNtStatus
					mov dwErrorId, eax

					invoke FormatMessage, FORMAT_MESSAGE_FROM_SYSTEM + FORMAT_MESSAGE_ALLOCATE_BUFFER, \
							NULL, dwErrorId, SUBLANG_DEFAULT SHL 10 + LANG_NEUTRAL, addr g_lpBuffer, 0, NULL

					.if eax != 0
						invoke LocalLock, g_lpBuffer
						invoke SetDlgItemText, hDlg, IDC_ERROR_DESCRIPTION, g_lpBuffer
;						invoke SendMessage, g_hwndEditErrorDescription, WM_SETTEXT, 0, g_lpBuffer
						invoke LocalFree, g_lpBuffer

						invoke wsprintf, addr buffer, addr szHexFmt, dwErrorId
;						invoke SendMessage, g_hwndEditDosErrorId, WM_SETTEXT, 0, addr buffer
						invoke SetDlgItemText, hDlg, IDC_DOS_ERROR_ID, addr buffer

					.else
						invoke SetDlgItemText, hDlg, IDC_DOS_ERROR_ID, $CTA0()
;						invoke SendMessage, g_hwndEditDosErrorId, WM_SETTEXT, 0, $CTA0()

						invoke SetDlgItemText, hDlg, IDC_ERROR_DESCRIPTION, $CTA0("Sorry. Error number not found.")
;						invoke SendMessage, g_hwndEditErrorDescription, WM_SETTEXT, 0, $CTA0("Sorry. Error number not found.")
					.endif

				.endif
			.endif
		.endif

	.elseif eax == WM_SYSCOMMAND
		.if wParam == IDM_ABOUT
			invoke MessageBox, hDlg, addr szWrittenBy, addr szAbout, MB_OK + MB_ICONINFORMATION
		.endif
		xor eax, eax
		ret

	.else
    
		xor eax, eax
		ret
      
	.endif
    
	xor eax, eax
	inc eax
	ret
    
DlgProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         start                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc

	invoke GetModuleHandle, NULL
	mov    g_hInstance, eax

	invoke DialogBoxParam, g_hInstance, IDD_DIALOG, NULL, addr DlgProc, NULL

	invoke ExitProcess, 0
    invoke InitCommonControls

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start