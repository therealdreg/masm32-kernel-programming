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
include \masm32\include\gdi32.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\gdi32.lib

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IDD_MAIN			equ	1000

IDC_DUMP			equ 1001

IDI_ICON			equ 2000

MBR_SIZE			equ 512
TEXT_BUFFER_SIZE	equ 1000h*2

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

MBR					BYTE MBR_SIZE dup(?)
g_hInstance			HINSTANCE	?
g_hDlg				HWND		?

g_hwndEditDump		HWND		?

g_hFontOld			HFONT		?
g_hFontNew			HFONT		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         PrintHexDump                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintHexDump proc uses esi edi ebx pData:LPVOID, dwOffset:DWORD, dwSize:DWORD

local acBuffer[256]:CHAR
local dwCurrentOffset:DWORD
local g_pTextBuffer:LPVOID

.data
szFmt	db "%08X:  %02X %02X %02X %02X %02X %02X %02X %02X-%02X %02X %02X %02X %02X %02X %02X %02X  ", 0

.code

	invoke GetProcessHeap
	invoke HeapAlloc, eax, HEAP_NO_SERIALIZE + HEAP_ZERO_MEMORY, TEXT_BUFFER_SIZE
	.if eax != NULL

		mov g_pTextBuffer, eax
		mov edi, eax

		invoke RtlZeroMemory, edi, TEXT_BUFFER_SIZE
		mov esi, pData
		push dwOffset
		pop dwCurrentOffset
		mov ebx, dwSize
		.if ( esi != NULL ) && ( ebx != 0 )

			shr ebx, 4				; / 16 - number of 16-byte lines to print

			.while ebx
				mov ecx, 16
				xor eax, eax
				.while ecx
					dec ecx
					mov al, [esi][ecx]
					push eax
				.endw
				push dwCurrentOffset
				push offset szFmt
				push edi				; current pointer to text buffer
				call wsprintf
				add esp, 04Ch

				add edi, eax			; shift current pointer to next free place

				xor ecx, ecx
				.while ecx < 16
					mov al, [esi][ecx]
					.if al < ' '
						mov al, '.'
					.endif
					stosb
					inc ecx
				.endw

				; New line
				mov al, 0Dh
				stosb
				mov al, 0Ah
				stosb

				add esi, 16					; next 16 bytes
				add dwCurrentOffset, 16		; next 16 bytes
				dec ebx						; next line
			.endw

			invoke SendMessage, g_hwndEditDump, WM_GETTEXTLENGTH, 0, 0
			invoke SendMessage, g_hwndEditDump, EM_SETSEL, eax, eax
			invoke SendMessage, g_hwndEditDump, EM_REPLACESEL, FALSE, g_pTextBuffer

		.endif

		invoke GetProcessHeap
		invoke HeapFree, eax, HEAP_NO_SERIALIZE, g_pTextBuffer
	.endif

	ret

PrintHexDump endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               D I A L O G     P R O C E D U R E                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local lf:LOGFONT
local rect:RECT

	mov eax, uMsg
	.if eax == WM_INITDIALOG

		push hDlg
		pop g_hDlg

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

		invoke GetDlgItem, hDlg, IDC_DUMP
		mov g_hwndEditDump, eax
		invoke SendMessage, g_hwndEditDump, EM_SETLIMITTEXT, 65535, 0

		invoke SendMessage, g_hwndEditDump, WM_GETFONT, 0, 0
		mov	g_hFontOld, eax
		invoke GetObject, g_hFontOld, sizeof LOGFONT, addr lf

		lea ecx, lf.lfFaceName
		invoke lstrcpy, ecx, $CTA0("Courier New")
		invoke CreateFontIndirect, addr lf		
		mov	g_hFontNew, eax

		invoke SendMessage, g_hwndEditDump, WM_SETFONT, g_hFontNew, FALSE

		invoke PrintHexDump, addr MBR, 0, MBR_SIZE

		; The following four lines is needed because of while processing
		; the WM_INITDIALOG message of a dialog box, sending an EM_SETSEL
		; fails to remove the highlight from (unselect) the edit control text.
		
		; Read Microsoft Knowledge Base Article - 96674
		; "Unselecting Edit Control Text at Dialog Box Initialization"
		; for more info
  
 		invoke SetFocus, g_hwndEditDump
		invoke PostMessage, g_hwndEditDump, EM_SETSEL, -1, -1
		xor eax, eax
		ret

	.elseif eax == WM_SIZE

		mov eax, lParam
		mov ecx, eax
		and eax, 0FFFFh
		shr ecx, 16
		invoke MoveWindow, g_hwndEditDump, 0, 0, eax, ecx, TRUE

	.elseif eax == WM_COMMAND

		mov eax, wParam
		and eax, 0FFFFh
		.if eax == IDCANCEL
			invoke EndDialog, hDlg, 0
		.endif

	.elseif eax == WM_CLOSE
		invoke EndDialog, hDlg, 0

	.elseif uMsg == WM_DESTROY

		invoke SendMessage, g_hwndEditDump, WM_SETFONT, g_hFontOld, FALSE
		invoke DeleteObject, g_hFontNew

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

start proc uses esi ebx

local hDevice:HANDLE
local dwNumberOfBytesRead:DWORD

	invoke CreateFile, $CTA0("\\\\.\\PhysicalDrive0"), GENERIC_READ, \
						FILE_SHARE_READ + FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL
	.if eax != INVALID_HANDLE_VALUE

		mov hDevice, eax

		invoke ReadFile, hDevice, addr MBR, MBR_SIZE, addr dwNumberOfBytesRead, NULL
		.if eax != 0

			invoke GetModuleHandle, NULL
			mov g_hInstance, eax
			invoke DialogBoxParam, g_hInstance, IDD_MAIN, NULL, addr DlgProc, 0

		.else
			invoke MessageBox, g_hDlg, $CTA0("Couldn't read MBR"), NULL, MB_ICONEXCLAMATION
		.endif

		invoke CloseHandle, hDevice                 
	.else
		invoke MessageBox, g_hDlg, $CTA0("Couldn't open PhysicalDrive0 device"), NULL, MB_ICONEXCLAMATION
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start
