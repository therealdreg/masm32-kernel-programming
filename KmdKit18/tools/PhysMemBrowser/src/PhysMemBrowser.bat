;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  Physical Memory Browser - Let you browse physical memory
;
;     Based on Mark Russinovich's Physmem code ( http://www.sysinternals.com )
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

include \masm32\include\w2k\native.inc

include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\comctl32.inc
include \masm32\include\gdi32.inc
include \masm32\include\advapi32.inc

include \masm32\include\w2k\ntdll.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\comctl32.lib
includelib \masm32\lib\gdi32.lib
includelib \masm32\lib\advapi32.lib

includelib \masm32\lib\w2k\ntdll.lib

include \masm32\Macros\Strings.mac
;include ReportLastError.asm
include memory.asm
include string.asm
include MaskedEdit.asm
include htodw.asm
include theme.asm

include seh3.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 S T R U C T U R E S                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                         F U N C T I O N S   P R O T O T Y P E S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc	proto :HWND, :UINT, :WPARAM, :LPARAM

GetNtdllEntries proto
externdef GetNtdllEntries:proc

OpenPhysicalMemory proto
externdef OpenPhysicalMemory:proc

MapPhysicalMemory proto :HANDLE, :PDWORD, :PDWORD, :PDWORD
externdef MapPhysicalMemory:proc

UnmapPhysicalMemory proto :DWORD
externdef UnmapPhysicalMemory:proc

NtStatusToDosError proto :DWORD
externdef NtStatusToDosError:proc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                         F U N C T I O N S   P R O T O T Y P E S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include protos.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        M A C R O S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

$invoke MACRO vars:VARARG
     invoke vars
     EXITM <eax>
ENDM

mrm MACRO Des:REQ, Sor:REQ
	mov eax, Sor
	mov Des, eax
ENDM

$LOWORD MACRO dwVar:REQ
	IFDIFI <dwVar>, <eax>	;; don't move eax onto itself
		mov eax, dwVar
	ENDIF
	and eax, 0FFFFh
	EXITM <eax>
ENDM

$HIWORD MACRO dwVar:REQ
	IFDIFI <dwVar>, <eax>	;; don't move eax onto itself
		mov eax, dwVar
	ENDIF
	shr eax, 16
	EXITM <eax>
ENDM

date MACRO
local pos, month

	;; Day
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 4) OR (pos EQ 5)
			db "&chr"
		ENDIF
		pos = pos + 1
	ENDM

	;; Month
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 1)
			month TEXTEQU @SubStr(%@Date, 1 , 2)
			IF month EQ 01
				db " Jan "	
			ELSEIF month EQ 02
				db " Feb "	
			ELSEIF month EQ 03
				db " Mar "	
			ELSEIF month EQ 04
				db " Apr "	
			ELSEIF month EQ 05
				db " May "	
			ELSEIF month EQ 06
				db " Jun "	
			ELSEIF month EQ 07
				db " Jul "	
			ELSEIF month EQ 08
				db " Aug "	
			ELSEIF month EQ 09
				db " Sep "	
			ELSEIF month EQ 10
				db " Oct "	
			ELSEIF month EQ 11
				db " Nov "	
			ELSEIF month EQ 12
				db " Dec "	
			ENDIF
		ENDIF
		pos = pos + 1
	ENDM

	;; Year
	db "20"
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 7) OR (pos EQ 8)
			db "&chr"
		ENDIF
		pos = pos + 1
	ENDM

ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Fix helper macro                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Fix MACRO txt:=<Fix this later!!!!>
	local pos, spos

	pos = 0
	spos = 0

	% FORC chr, @FileCur		;; Don't display full path. Easier to read.
		pos = pos + 1
		IF "&chr" EQ 5Ch		;; "/"
			spos = pos
		ENDIF
	ENDM

	% ECHO @CatStr(<Fix: >, @SubStr(%@FileCur, spos+1,), <(%@Line) - txt>)
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IDD_MAIN					equ	1000
IDE_ADDRESS					equ 1001
IDCB_SIZE					equ 1002
IDB_DUMP					equ 1003
IDE_DUMP					equ 1004
IDB_CLEAR					equ 1005

IDR_BYTE					equ 1006
IDR_WORD					equ 1007
IDR_DWORD					equ 1008

IDC_TOTAL_PHYS_PAGES		equ 1009
IDC_LOWEST_PHYS_ADDRESS		equ 1010
IDC_HIGHEST_PHYS_ADDRESS	equ 1011

IDC_LINE					equ 1020

;IDM_CLEAR					equ 2001
;IDM_COPY_CLIPBOARD			equ 2002
IDM_ABOUT					equ	2000

IDI_ICON					equ 3000

STATUS_SUCCESS				equ 0

TEXT_BUFFER_SIZE			equ 30000

TOP_INDENT					equ 62

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  R E A D O N L Y  D A T A                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

szAbout						db "About...", 0
szWrittenBy					db "Physical Memory Browser v1.2", 0Ah, 0Dh
							db "Built on "
							date
							db 0Ah, 0Dh, 0Ah, 0Dh
							db "Written by Four-F <four-f@mail.ru>", 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_hInstance					HINSTANCE	?
g_hDlg						HWND		?
g_hwndEditAddress			HWND		?
g_hwndComboSize				HWND		?
g_hwndEditDump				HWND		?
g_hwndStatusBar				HWND		?

;g_hPopupMenu				HMENU		?

g_hPhysMem					HANDLE		?

g_hFontOld					HFONT		?
g_hFontNew					HFONT		?

g_pTextBuffer				LPSTR		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     ErrorToStatusBar                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ErrorToStatusBar proc pszError:LPSTR, status:DWORD

; pError:
;	Pointer to message
;	NULL	- Grab error description from system
;	-1		- Clear Status Bar

local dwLanguageId:DWORD
local acErrorDescription[256]:CHAR
local acBuffer[1024]:CHAR

    pushfd
    pushad

	.if pszError == -1
		; Clear status bar
		invoke SendMessage, g_hwndStatusBar, SB_SETTEXT, 0, NULL
	.else

		.if pszError != NULL
			invoke lstrcpy, addr acBuffer, pszError
		.endif

		.if status != 0
			invoke NtStatusToDosError, status
    		mov ecx, eax

		   	invoke FormatMessage, FORMAT_MESSAGE_FROM_SYSTEM, NULL,\
   						 ecx, SUBLANG_DEFAULT SHL 10 + LANG_NEUTRAL, addr acErrorDescription, sizeof acErrorDescription, NULL

	    	.if eax != 0
				invoke lstrcat, addr acBuffer, addr acErrorDescription
		    .else
				invoke lstrcat, addr acBuffer, $CTA0("Error number not found.")
	    	.endif
		.endif

		invoke SendMessage, g_hwndStatusBar, SB_SETTEXT, 0, addr acBuffer

	.endif

    popad
    popfd
    
    ret

ErrorToStatusBar endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        FillComboBox                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FillComboBox proc uses esi edi ebx

.data
aszSizes	dd $CTA0("16")
			dd $CTA0("32")
			dd $CTA0("64")
			dd $CTA0("128")
			dd $CTA0("256")
			dd $CTA0("512")
			dd $CTA0("1024")
			dd $CTA0("2048")
			dd $CTA0("4096")
cbSizes		equ $-aszSizes
.code

	invoke SendMessage, g_hwndComboSize, CB_RESETCONTENT, 0, 0

	mov esi, cbSizes
	shr esi, 2				; / sizeof DWORD = number of strings

	lea edi, aszSizes

	xor ebx, ebx
	.while ebx < esi

		invoke SendMessage, g_hwndComboSize, CB_ADDSTRING, 0, [edi]
		mov ecx, ebx
		mov edx, 10h
		shl edx, cl
		invoke SendMessage, g_hwndComboSize, CB_SETITEMDATA, eax, edx

		add edi, sizeof DWORD	; next string pointer
		inc ebx
	.endw

	; set size of 64 bytes by default
	invoke SendMessage, g_hwndComboSize, CB_SETCURSEL , 2, 0

	ret

FillComboBox endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         PrintHexDump                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintHexDump proc uses esi edi ebx pVirtAddress:LPVOID, dwPhysAddress:DWORD, dwSize:DWORD

local acBuffer[256]:CHAR
local dwPhysAddressCurrent:DWORD
local dwFmt:DWORD

.data
szFmt1	db "%08X:  %02X %02X %02X %02X %02X %02X %02X %02X-%02X %02X %02X %02X %02X %02X %02X %02X  ", 0
szFmt2	db "%08X:  %04X  %04X  %04X  %04X  %04X  %04X  %04X  %04X   ", 0
szFmt4	db "%08X:  %08X    %08X    %08X    %08X     ", 0

.code

	_try

	mov edi, g_pTextBuffer
	invoke fZeroMemory, edi, TEXT_BUFFER_SIZE
	mov esi, pVirtAddress
	push dwPhysAddress
	pop dwPhysAddressCurrent
	mov ebx, dwSize
	.if ( esi != NULL ) && ( ebx != 0 )

		; wich format: byte, word or dword?
		invoke IsDlgButtonChecked, g_hDlg, IDR_BYTE
		.if eax == BST_CHECKED
			mov dwFmt, IDR_BYTE
		.endif
		invoke IsDlgButtonChecked, g_hDlg, IDR_WORD
		.if eax == BST_CHECKED
			mov dwFmt, IDR_WORD
		.endif
		invoke IsDlgButtonChecked, g_hDlg, IDR_DWORD
		.if eax == BST_CHECKED
			mov dwFmt, IDR_DWORD
		.endif

		shr ebx, 4				; / 16 - number of 16-byte lines to print

		.while ebx
			mov ecx, 16
			xor eax, eax
			.while ecx
				.if dwFmt == IDR_WORD
					dec ecx
					dec ecx
					mov ax, [esi][ecx]
				.elseif dwFmt == IDR_DWORD
					sub ecx, 4
					mov eax, [esi][ecx]
				.else
					dec ecx
					mov al, [esi][ecx]
				.endif
				push eax
			.endw

			push dwPhysAddressCurrent

			.if dwFmt == IDR_WORD
				push offset szFmt2
			.elseif dwFmt == IDR_DWORD
				push offset szFmt4
			.else
				push offset szFmt1
			.endif

			push edi				; current pointer to text buffer
			call wsprintf
			.if dwFmt == IDR_WORD
				add esp, 02Ch
			.elseif dwFmt == IDR_DWORD
				add esp, 01Ch
			.else
				add esp, 04Ch
			.endif

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

			add esi, 16							; next 16 bytes
			add dwPhysAddressCurrent, 16		; next 16 bytes
			dec ebx								; next line
		.endw

		invoke fstrcpy, edi, $CTA0("----------------------------------------------------------------------------\n", szBreakLine)
		add edi, sizeof szBreakLine - 1		; shift current pointer to next free place

		; New line
		mov al, 0Dh
		stosb
		mov al, 0Ah
		stosb

		; Buffer is ready to be printed, but is it enough place in the edit control?
		.while TRUE
			invoke SendMessage, g_hwndEditDump, EM_GETLIMITTEXT, 0, 0
			push eax
			invoke SendMessage, g_hwndEditDump, WM_GETTEXTLENGTH, 0, 0
			add eax, edi
			sub eax, g_pTextBuffer		; eax = sizeof(text in edit control) + sizeof(text in buffer)
			pop ecx						; edit control text limit
			sub ecx, eax
			.if SIGN?

				push edi
				xor edi, edi				; number of chars to remove
				xor ebx, ebx
				.while ebx < 100			; remove first 100 lines
					; we have to do some clean up

					; Get first line text
					mov word ptr acBuffer, sizeof acBuffer
					invoke SendMessage, g_hwndEditDump, EM_GETLINE, ebx, addr acBuffer
					inc eax			; cr
					inc eax			; lf
					add edi, eax

					inc ebx
				.endw

				invoke SendMessage, g_hwndEditDump, EM_GETHANDLE, 0, 0
				invoke SendMessage, g_hwndEditDump, EM_SETSEL, 0, edi
				mov byte ptr acBuffer, 0
				invoke SendMessage, g_hwndEditDump, EM_REPLACESEL, FALSE, addr acBuffer

				invoke SendMessage, g_hwndEditDump, WM_GETTEXTLENGTH, 0, 0
				invoke SendMessage, g_hwndEditDump, EM_SETSEL, eax, eax

				pop edi

			.else
				.break					; now we have enough free place in the edit control
			.endif

		.endw

		invoke SendMessage, g_hwndEditDump, WM_GETTEXTLENGTH, 0, 0
		invoke SendMessage, g_hwndEditDump, EM_SETSEL, eax, eax
		invoke SendMessage, g_hwndEditDump, EM_REPLACESEL, FALSE, g_pTextBuffer

	.endif

	_finally

	ret

PrintHexDump endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         DumpMemory                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DumpMemory proc

local dwBaseAddress:DWORD
local acAddress[16]:CHAR

local dwAddress:DWORD
local dwRoundedAddress:DWORD
local dwSize:DWORD
local dwMappedSize:DWORD

local acBuffer[512]:CHAR

	invoke ErrorToStatusBar, -1, 0

	invoke GetWindowText, g_hwndEditAddress, addr acAddress, sizeof acAddress
	.if eax != 0
		invoke htodw, addr acAddress
		mov dwAddress, eax
		mov dwRoundedAddress, eax	; after MapPhysicalMemory is rounded down to the next allocation granularity size boundary

		invoke SendMessage, g_hwndComboSize, CB_GETCURSEL, 0, 0
		invoke SendMessage, g_hwndComboSize, CB_GETITEMDATA, eax, 0

		; if we cross page boundary ask to map one page more
		mov dwSize, eax
		mov ecx, dwAddress
		and ecx, 0FFFh
		add ecx, eax
		mov dwMappedSize, ecx		; will receive the actual size, in bytes, of the view.

		invoke MapPhysicalMemory, g_hPhysMem, addr dwRoundedAddress, addr dwMappedSize, addr dwBaseAddress
		.if eax == STATUS_SUCCESS

			mov eax, dwAddress
			sub eax, dwRoundedAddress		; bias
			mov ecx, dwBaseAddress
			add ecx, eax
			invoke PrintHexDump, ecx, dwAddress, dwSize

			; Unmap the view
			invoke UnmapPhysicalMemory, dwBaseAddress
			.if eax != STATUS_SUCCESS
				invoke wsprintf, addr acBuffer, $CTA0("Couldn't unmap view of %08X: "), dwAddress
				invoke ErrorToStatusBar, addr acBuffer, eax
			.endif
		.else
			invoke wsprintf, addr acBuffer, $CTA0("Couldn't map view of %08X: "), dwAddress
			invoke ErrorToStatusBar, addr acBuffer, eax
		.endif

	.endif

	ret

DumpMemory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   MeasurePhysicalMemory                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MeasurePhysicalMemory proc

local sbi:SYSTEM_BASIC_INFORMATION
local buffer[256]:CHAR

		invoke ZwQuerySystemInformation, SystemBasicInformation, addr sbi, sizeof sbi, NULL
		.if eax == STATUS_SUCCESS

			mov eax, sbi.NumberOfPhysicalPages
			mov ecx, sbi.PhysicalPageSize
			xor edx, edx
			mul ecx
			invoke wsprintf, addr buffer, $CTA0("Total physical memory: %08Xh"), eax
			invoke SetDlgItemText, g_hDlg, IDC_TOTAL_PHYS_PAGES, addr buffer

			mov eax, sbi.LowestPhysicalPage
			dec eax
			mov ecx, sbi.PhysicalPageSize
			xor edx, edx
			mul ecx
			invoke wsprintf, addr buffer, $CTA0("Lowest phys addr: %08Xh"), eax
			invoke SetDlgItemText, g_hDlg, IDC_LOWEST_PHYS_ADDRESS, addr buffer

			mov eax, sbi.HighestPhysicalPage
			inc eax
			mov ecx, sbi.PhysicalPageSize
			xor edx, edx
			mul ecx
			dec eax
			invoke wsprintf, addr buffer, $CTA0("Highest phys addr: %08Xh"), eax
			invoke SetDlgItemText, g_hDlg, IDC_HIGHEST_PHYS_ADDRESS, addr buffer

		.endif

comment ^
	PhysicalPageSize
	NumberOfPhysicalPages
	LowestPhysicalPage
	HighestPhysicalPage
^
	ret

MeasurePhysicalMemory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               D I A L O G     P R O C E D U R E                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local rect:RECT

local lf:LOGFONT
;LOCAL ps:PAINTSTRUCT
;LOCAL bm:BITMAP
;LOCAL p:POINT

	mov eax, uMsg
	.if eax == WM_COMMAND
		mov eax, $LOWORD(wParam)
		.if eax == IDCANCEL
			invoke EndDialog, hDlg, 0

		.elseif eax == IDCB_SIZE
			mov eax, $HIWORD(wParam)
;			.if eax == CBN_DROPDOWN
;				invoke FillComboBox

			.if eax == CBN_SELENDOK
;				invoke SetFocus, g_hwndListView
            .endif

		.elseif eax == IDB_CLEAR
			invoke ErrorToStatusBar, -1, 0
			invoke SendMessage, g_hwndEditDump, WM_SETTEXT, 0, 0

		.elseif eax == IDB_DUMP
			invoke DumpMemory

;		.elseif eax == IDM_COPY_CLIPBOARD
;			invoke CopyToClipboard
		.endif

	.elseif eax == WM_SIZE

		mov esi, $HIWORD(lParam)
		invoke MoveWindow, g_hwndStatusBar, 0, esi, $LOWORD(lParam), esi, TRUE

		invoke GetClientRect, g_hwndStatusBar, addr rect

		sub esi, TOP_INDENT
		sub esi, rect.bottom
		invoke MoveWindow, g_hwndEditDump, 0, TOP_INDENT, $LOWORD(lParam), esi, TRUE


;		invoke GetWindowRect, g_hwndStatusBar, addr rect
;		invoke ScreenToClient, hDlg, addr rect
		invoke GetDlgItem, hDlg, IDC_LINE
		mov ecx, lParam
		and ecx, 0FFFFh			; width of dialog client area
		sub ecx, 6
		invoke MoveWindow, eax, 3, 23, ecx, 2, TRUE


	.elseif eax == WM_INITDIALOG

		; Initialize global variables
		mrm g_hDlg, hDlg

		invoke SetWindowText, hDlg, $CTA0("Physical Memory Browser")

		; Set Dialog Icon
		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax


		; If we XP themed, remove WS_EX_STATICEDGE. Looks better.
		
		invoke AdjustGuiIfThemed, hDlg


		mov g_hwndEditAddress, $invoke(GetDlgItem, hDlg, IDE_ADDRESS)

		; Thnx to James Brown for idea
		invoke MaskEditControl, g_hwndEditAddress, $CTA0("0123456789abcdefABCDEF"), TRUE
		invoke SendMessage, g_hwndEditAddress, EM_LIMITTEXT, 8, 0
		invoke SendMessage, g_hwndEditAddress, WM_SETTEXT, 0, $CTA0("0")
	
comment ^
		; Create popup menu
		mov g_hPopupMenu, $invoke(CreatePopupMenu)

		invoke AppendMenu, g_hPopupMenu, MF_STRING, IDM_CLEAR, $CTA0("Clear")
		invoke AppendMenu, g_hPopupMenu, MF_SEPARATOR, 0, NULL
		invoke AppendMenu, g_hPopupMenu, MF_STRING, IDM_COPY_CLIPBOARD, $CTA0("Copy To Clipboard")
^
		mov g_hwndComboSize, $invoke(GetDlgItem, hDlg, IDCB_SIZE)
		invoke SetFocus, g_hwndComboSize

		invoke FillComboBox

		mov g_hwndEditDump, $invoke(GetDlgItem, hDlg, IDE_DUMP)
		invoke SendMessage, g_hwndEditDump, EM_SETLIMITTEXT, 65535, 0

		mov	g_hFontOld, $invoke(SendMessage, g_hwndEditDump, WM_GETFONT, 0, 0)
		invoke GetObject, g_hFontOld, sizeof LOGFONT, addr lf

		lea ecx, lf.lfFaceName
		invoke lstrcpy, ecx, $CTA0("Courier New")
		invoke CreateFontIndirect, addr lf		
		mov	g_hFontNew, eax

		invoke SendMessage, g_hwndEditDump, WM_SETFONT, g_hFontNew, FALSE

		; Create status bar
		mov g_hwndStatusBar, $invoke(CreateStatusWindow, WS_CHILD + WS_VISIBLE + SBS_SIZEGRIP, NULL, hDlg, 200)

		invoke CheckRadioButton, hDlg, IDR_BYTE, IDR_DWORD, IDR_BYTE

		; Add about menu
		push ebx
		invoke GetSystemMenu, hDlg, FALSE
		mov ebx, eax
		invoke InsertMenu, ebx, -1, MF_BYPOSITION + MF_SEPARATOR, 0, 0
		invoke InsertMenu, ebx, -1, MF_BYPOSITION + MF_STRING, IDM_ABOUT, offset szAbout
		pop ebx


		; Tell the user how much physical memory he/she has
		invoke MeasurePhysicalMemory


comment ^
	.elseif eax == WM_NOTIFY

		mov edi, lParam
		assume edi:ptr NMHDR
		mov eax, [edi].hwndFrom
		.if eax == g_hwndListView
			; Notify message from List
			.if [edi].code == LVN_COLUMNCLICK			

				assume edi:ptr NM_LISTVIEW
				mov eax, g_uPrevClickedColumn
				.if [edi].iSubItem != eax
					; Remove bitmap from prev header column
					invoke ImageToHeaderItem, g_hwndHeader, g_uPrevClickedColumn, NULL
					mov g_uSortOrder, SORT_NOT_YET
					mrm g_uPrevClickedColumn, [edi].iSubItem
				.endif

			.endif
			assume edi:nothing
		.endif
^
comment ^
	.elseif eax == WM_CONTEXTMENU

		mov eax, $LOWORD(lParam)
		mov ecx, $HIWORD(lParam)
		invoke TrackPopupMenu, g_hPopupMenu, TPM_LEFTALIGN, eax, ecx, NULL, hDlg, NULL
^
	.elseif uMsg == WM_GETMINMAXINFO

		mov ecx, lParam
		mov (MINMAXINFO PTR [ecx]).ptMinTrackSize.x, 380
		mov (MINMAXINFO PTR [ecx]).ptMinTrackSize.y, 150

	.elseif uMsg == WM_DESTROY

;		invoke DestroyMenu, g_hPopupMenu
		invoke SendMessage, g_hwndEditDump, WM_SETFONT, g_hFontOld, FALSE
		invoke DeleteObject, g_hFontNew

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
;                                           start                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start:
			
	; Open physical memory device
	invoke OpenPhysicalMemory
	.if eax != NULL
		mov g_hPhysMem, eax

		invoke malloc, TEXT_BUFFER_SIZE
		.if eax != NULL
			mov g_pTextBuffer, eax


	
			mov g_hInstance, $invoke(GetModuleHandle, NULL)
			invoke DialogBoxParam, g_hInstance, IDD_MAIN, NULL, addr DlgProc, 0

		.else
			invoke MessageBox, NULL, $CTA0("Couldn't allocate memory buffer."), NULL, MB_OK + MB_ICONERROR					
		.endif

		; Close physical memory device
		invoke CloseHandle, g_hPhysMem

	.else
		invoke MessageBox, NULL, $CTA0("Couldn't open PhysicalMemory device."), NULL, MB_OK + MB_ICONERROR		
	.endif

	invoke ExitProcess, 0

end start

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:make

set exe=PhysMemBrowser
set mod=PhysMemWorks

if exist %exe%.exe del %exe%.exe
if exist %exe%.obj del %exe%.obj
if exist %mod%.obj del %mod%.obj

\masm32\bin\ml /nologo /c /coff %mod%.asm

if errorlevel 0 goto makerc
	echo.
	pause
	exit

:makerc
if exist rsrc.obj goto final
	\masm32\bin\rc /v rsrc.rc
	\masm32\bin\cvtres /machine:ix86 rsrc.res
	if errorlevel 0 goto final
		echo.
		pause
		exit

:final
if exist rsrc.res del rsrc.res
\masm32\bin\ml /nologo /c /coff %exe%.bat
\masm32\bin\link /nologo /subsystem:windows %exe%.obj %mod%.obj rsrc.obj

del %mod%.obj
del %exe%.obj

echo.
pause
