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
include \masm32\include\comctl32.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\comctl32.lib

include \masm32\include\w2k\ntddkbd.inc

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; ntddk.inc can't be included because of windows.inc
FILE_DEVICE_KEYBOARD	equ 0Bh
FILE_ANY_ACCESS			equ 0
METHOD_BUFFERED         equ 0

IDD_MAIN				equ	1000
IDC_DELAY				equ 1001
IDC_RATE				equ 1002
IDC_APPLY				equ 1003

IDI_ICON				equ 2000

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_hInstance		HINSTANCE	?
;g_hDlg			HWND		?
g_hDevice		HANDLE		?

g_hwndTbDelay	HWND		?
g_hwndTbRate	HWND		?
g_hwndBtnApply	HWND		?

g_dwUnitId		DWORD		?

g_dwMinRate		DWORD		?
g_dwMaxRate		DWORD		?
g_dwCurRate		DWORD		?

g_dwMinDelay	DWORD		?
g_dwMaxDelay	DWORD		?
g_dwCurDelay	DWORD		?

g_dwTbDelayPos	DWORD		?
g_dwTbRatePos	DWORD		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        GetRateAndDelay                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetRateAndDelay proc uses ebx

local ka:KEYBOARD_ATTRIBUTES
local ktp:KEYBOARD_TYPEMATIC_PARAMETERS
local dwBytesReturned:DWORD

	and ebx, FALSE			; assume error

	; Firstly we have to know minimum and maximum allowable values of keyboard typematic rate and delay.
	invoke DeviceIoControl, g_hDevice, IOCTL_KEYBOARD_QUERY_ATTRIBUTES, NULL, 0, \
						addr ka, sizeof ka, addr dwBytesReturned, NULL
	.if ( eax != 0 ) && ( dwBytesReturned != 0 )

		; Minimum allowable values of keyboard typematic rate and delay.

		movzx eax, ka.KeyRepeatMinimum.Rate
		mov g_dwMinRate, eax
		movzx eax, ka.KeyRepeatMinimum.Delay
		mov g_dwMinDelay, eax

		; Maximum allowable values of keyboard typematic rate and delay.

		movzx eax, ka.KeyRepeatMaximum.Rate
		mov g_dwMaxRate, eax
		movzx eax, ka.KeyRepeatMaximum.Delay
		mov g_dwMaxDelay, eax

		; Secondly we need to know current values of keyboard typematic rate and delay.
		invoke DeviceIoControl, g_hDevice, IOCTL_KEYBOARD_QUERY_TYPEMATIC, NULL, 0, \
							addr ktp, sizeof ktp, addr dwBytesReturned, NULL
		.if ( eax != 0 ) && ( dwBytesReturned != 0 )

			; Unit identifier.  Specifies the device unit for which this request is intended.
			; Should be a value of zero for default unit ID.
			movzx eax, ktp.UnitId
			mov g_dwUnitId, eax

			; Typematic rate, in repeats per second.
			movzx eax, ktp.Rate
			mov g_dwCurRate, eax

			; Typematic delay, in milliseconds.
			movzx eax, ktp.Delay
			mov g_dwCurDelay, eax

			mov ebx, TRUE			; indicate success

		.endif
	.endif

	mov eax, ebx
	ret

GetRateAndDelay endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        SetRateAndDelay                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SetRateAndDelay proc uses ebx

local ktp:KEYBOARD_TYPEMATIC_PARAMETERS
local dwBytesReturned:DWORD

	and ebx, FALSE			; assume error

	mov eax, g_dwUnitId
	mov ktp.UnitId, ax

	; Typematic rate, in repeats per second.

	invoke SendMessage, g_hwndTbRate, TBM_GETPOS, 0, 0

	mov ecx, g_dwMaxRate
	sub ecx, g_dwMinRate
	shr ecx, 4					; / 16
	xor edx, edx
	mul ecx
	add eax, g_dwMinRate
	mov ktp.Rate, ax

	; Typematic delay, in milliseconds.

	invoke SendMessage, g_hwndTbDelay, TBM_GETPOS, 0, 0

	mov ecx, g_dwMaxDelay
	sub ecx, g_dwMinDelay
	shr ecx, 3					; / 8
	xor edx, edx
	mul ecx
	add eax, g_dwMinDelay
	mov ktp.Delay, ax

	invoke DeviceIoControl, g_hDevice, IOCTL_KEYBOARD_SET_TYPEMATIC, addr ktp, sizeof ktp, \
						NULL, 0, addr dwBytesReturned, NULL
	.if ( eax != 0 )
		mov ebx, TRUE			; indicate success
	.endif

	mov eax, ebx
	ret

SetRateAndDelay endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               D I A L O G     P R O C E D U R E                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc uses ebx hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

	mov eax, uMsg
	.if eax == WM_INITDIALOG

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

		invoke GetDlgItem, hDlg, IDC_DELAY
		mov g_hwndTbDelay, eax
		invoke GetDlgItem, hDlg, IDC_RATE
		mov g_hwndTbRate, eax
		invoke GetDlgItem, hDlg, IDC_APPLY
		mov g_hwndBtnApply, eax
		invoke EnableWindow, eax, FALSE

		; Range 0-7
		invoke SendMessage, g_hwndTbDelay, TBM_SETRANGE, TRUE, (7 SHL 16) OR 0
		invoke SendMessage, g_hwndTbDelay, TBM_SETPAGESIZE, 0, 2

		mov ecx, g_dwMaxDelay
		sub ecx, g_dwMinDelay
		shr ecx, 3					; / 8

		mov eax, g_dwCurDelay
		sub eax, g_dwMinDelay
		.if eax != 0				; Is it possible MaxDelay = MinDelay ? I don't know.
			xor edx, edx
			div ecx
		.endif
		.if eax > 7
			mov eax, 7
		.endif
		mov g_dwTbDelayPos, eax
		invoke SendMessage, g_hwndTbDelay, TBM_SETPOS, TRUE, eax
 

		; Range 0-15
		invoke SendMessage, g_hwndTbRate, TBM_SETRANGE, TRUE, (15 SHL 16) OR 0
		invoke SendMessage, g_hwndTbRate, TBM_SETPAGESIZE, 0, 4

		mov ecx, g_dwMaxRate
		sub ecx, g_dwMinRate
		shr ecx, 4					; / 16

		mov eax, g_dwCurRate
		sub eax, g_dwMinRate
		.if eax != 0				; Is it possible MaxRate = MinRate ? I don't know.
			xor edx, edx
			div ecx
		.endif
		.if eax > 15
			mov eax, 15		
		.endif
		mov g_dwTbRatePos, eax
		invoke SendMessage, g_hwndTbRate, TBM_SETPOS, TRUE, eax


	.elseif eax == WM_COMMAND

		mov ebx, wParam
		and ebx, 0FFFFh
		.if ( ebx == IDC_APPLY ) || ( ebx == IDOK )
			invoke SetRateAndDelay
			.if eax == TRUE
				invoke EnableWindow, g_hwndBtnApply, FALSE

				invoke SendMessage, g_hwndTbDelay, TBM_GETPOS, 0, 0
				mov g_dwTbDelayPos, eax

				invoke SendMessage, g_hwndTbRate, TBM_GETPOS, 0, 0
				mov g_dwTbRatePos, eax
			.else
				invoke MessageBox, NULL, $CTA0("Couldn't set keyboard typematic parameters"), \
							NULL, MB_ICONEXCLAMATION
			.endif
			.if ebx == IDOK
				invoke EndDialog, hDlg, 0
			.endif
		.elseif ebx == IDCANCEL
			invoke EndDialog, hDlg, 0
		.endif

	.elseif eax == WM_HSCROLL
		mov eax, lParam
		.if ( eax == g_hwndTbDelay ) || ( eax == g_hwndTbRate )
			invoke SendMessage, g_hwndTbDelay, TBM_GETPOS, 0, 0
			push eax
			invoke SendMessage, g_hwndTbRate, TBM_GETPOS, 0, 0
			pop ecx
			.if ( ecx == g_dwTbDelayPos ) && ( eax == g_dwTbRatePos )
				invoke EnableWindow, g_hwndBtnApply, FALSE
			.else
				invoke EnableWindow, g_hwndBtnApply, TRUE
			.endif
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
	invoke DefineDosDevice, DDD_RAW_TARGET_PATH, $CTA0("KbdTypematic"), $CTA0("\\Device\\KeyboardClass0")
	.if eax != 0

		invoke CreateFile, $CTA0("\\\\.\\KbdTypematic"), 0, 0, NULL, OPEN_EXISTING, 0, NULL
		.if eax != INVALID_HANDLE_VALUE
			mov g_hDevice, eax

			invoke GetRateAndDelay
			.if eax == TRUE
				invoke DialogBoxParam, g_hInstance, IDD_MAIN, NULL, addr DlgProc, 0
			.else
				invoke MessageBox, NULL, $CTA0("Couldn't query keyboard attributes"), NULL, MB_ICONEXCLAMATION
			.endif

			invoke CloseHandle, g_hDevice                 
		.else
			invoke MessageBox, NULL, $CTA0("Couldn't open keyboard device"), NULL, MB_ICONEXCLAMATION
		.endif
		invoke DefineDosDevice, DDD_REMOVE_DEFINITION, $CTA0("KbdTypematic"), NULL
	.else
		invoke MessageBox, NULL, $CTA0("Couldn't define link to keyboard device"), NULL, MB_ICONEXCLAMATION
	.endif

	invoke ExitProcess, 0
	invoke InitCommonControls

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start
