;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
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

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib

include \masm32\include\w2k\ntddkbd.inc

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; ntddk.inc can't be included because of windows.inc
FILE_DEVICE_KEYBOARD	equ 0bh
FILE_ANY_ACCESS			equ 0
METHOD_BUFFERED         equ 0

IDD_MAIN			equ	1000

IDC_LIGHT			equ 1001

IDI_ICON			equ 2000

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_hInstance		HINSTANCE	?
g_hDlg			HWND		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          Do                                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Do proc uses esi ebx

local hDevice:HANDLE
local kip:KEYBOARD_INDICATOR_PARAMETERS
local dwBytesReturned:DWORD
local LedFlags:DWORD

	invoke DefineDosDevice, DDD_RAW_TARGET_PATH, $CTA0("KbdGarland"), $CTA0("\\Device\\KeyboardClass0")
	.if eax != 0

		invoke CreateFile, $CTA0("\\\\.\\KbdGarland"), 0, 0, NULL, OPEN_EXISTING, 0, NULL
		.if eax != INVALID_HANDLE_VALUE

			mov hDevice, eax

			invoke DeviceIoControl, hDevice, IOCTL_KEYBOARD_QUERY_INDICATORS, NULL, 0, \
								addr kip, sizeof kip, addr dwBytesReturned, NULL
			
			.if ( eax != 0 ) && ( dwBytesReturned != 0 )

				lea esi, kip
				assume esi:ptr KEYBOARD_INDICATOR_PARAMETERS

				movzx eax, [esi].LedFlags
				mov LedFlags, eax					; save
				
				mov ebx, 5
				.while ebx

					mov [esi].LedFlags, KEYBOARD_NUM_LOCK_ON
					invoke DeviceIoControl, hDevice, IOCTL_KEYBOARD_SET_INDICATORS, addr kip, sizeof kip, \
								NULL, 0, addr dwBytesReturned, NULL
					invoke Sleep, 100

					mov [esi].LedFlags, KEYBOARD_CAPS_LOCK_ON
					invoke DeviceIoControl, hDevice, IOCTL_KEYBOARD_SET_INDICATORS, addr kip, sizeof kip, \
								NULL, 0, addr dwBytesReturned, NULL
					invoke Sleep, 100

					mov [esi].LedFlags, KEYBOARD_SCROLL_LOCK_ON
					invoke DeviceIoControl, hDevice, IOCTL_KEYBOARD_SET_INDICATORS, addr kip, sizeof kip, \
								NULL, 0, addr dwBytesReturned, NULL
					invoke Sleep, 100

					dec ebx
				.endw

				mov eax, LedFlags
				mov [esi].LedFlags, ax				; restore
				invoke DeviceIoControl, hDevice, IOCTL_KEYBOARD_SET_INDICATORS, addr kip, sizeof kip, \
								NULL, 0, addr dwBytesReturned, NULL

				assume esi:nothing

			.endif
			invoke CloseHandle, hDevice                 
		.else
			invoke MessageBox, g_hDlg, $CTA0("Couldn't open keyboard device"), NULL, MB_ICONEXCLAMATION
		.endif
		invoke DefineDosDevice, DDD_REMOVE_DEFINITION, $CTA0("KbdGarland"), NULL
	.else
		invoke MessageBox, g_hDlg, $CTA0("Couldn't define link to keyboard device"), NULL, MB_ICONEXCLAMATION
	.endif

	ret

Do endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               D I A L O G     P R O C E D U R E                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

	mov eax, uMsg
	.if eax == WM_INITDIALOG

		push hDlg
		pop g_hDlg

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

	.elseif eax == WM_COMMAND

		mov eax, wParam
		and eax, 0FFFFh
		.if eax == IDCANCEL
			invoke EndDialog, hDlg, 0
		.elseif eax == IDC_LIGHT
			invoke Do
		.endif

	.elseif eax == WM_CLOSE
		invoke EndDialog, hDlg, 0

	.else

		xor eax, eax
		ret
	
	.endif

	xor eax, eax
	inc eax
	ret
    
DlgProc endp


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       start                                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start:

	invoke GetModuleHandle, NULL
	mov g_hInstance, eax
	invoke DialogBoxParam, g_hInstance, IDD_MAIN, NULL, addr DlgProc, 0
	invoke ExitProcess, 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start
