;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  SymLinks - Let you browse symbolic links
;
;  Written by Four-F (four-f@mail.ru)
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.486
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\windows.inc
include \masm32\include\w2k\ntstatus.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include \masm32\include\comctl32.inc
include \masm32\include\gdi32.inc
include \masm32\include\w2k\ntdll.inc

includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\comctl32.lib
includelib \masm32\lib\gdi32.lib
includelib \masm32\lib\w2k\ntdll.lib

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include Macros.mac
include \masm32\Macros\Strings.mac
include ListView.mac
include memory.asm
include theme.asm

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       E Q U A T E S                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IDD_MAIN_DIALOG		equ 100
IDC_LISTVIEW		equ 101

IDI_MAIN_ICON		equ 200

MAX_TEXT_LENGTH		equ	128

IDM_ABOUT			equ	2000

IDI_UP_ARROW		equ 2001
IDI_DOWN_ARROW		equ 2002

CX_HEADERBITMAP		equ 9
CY_HEADERBITMAP		equ 5

SORT_NOT_YET		equ 0
SORT_ASCENDING		equ 1
SORT_DESCENDING		equ 2

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  R E A D O N L Y  D A T A                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
g_szListViewClassName		db "SysListView32", 0

szAbout						db "About...", 0
szWrittenBy					db "Symbolic Links Viewer v1.2", 0Ah, 0Dh
							db "Built on "
							date
							db 0Ah, 0Dh, 0Ah, 0Dh
							db "Written by Four-F <four-f@mail.ru>", 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_hInstance				HINSTANCE	?
g_hDlg					HWND		?		; global handle of the main dialog
g_hMainIcon				HICON		?
g_hwndListView			HWND		?
g_hwndHeader			HWND		?

g_uPrevClickedColumn	UINT		?
g_uSortOrder			UINT		?

g_hbmpHeaderArrowUp		HBITMAP		?
g_hbmpHeaderArrowDown	HBITMAP		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        CompareFunc                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CompareFunc proc uses edi lParam1:DWORD, lParam2:DWORD, uClickedColumn:UINT

local buffer[256]:CHAR
local buffer1[256]:CHAR
local lvi:LV_ITEM
	
	mov lvi.imask,LVIF_TEXT
	lea eax,buffer
	mov lvi.pszText, eax
	mov lvi.cchTextMax, sizeof buffer

	push uClickedColumn
	pop lvi.iSubItem

	invoke SendMessage, g_hwndListView, LVM_GETITEMTEXT, lParam1, addr lvi
	invoke lstrcpy, addr buffer1, addr buffer
	invoke SendMessage, g_hwndListView, LVM_GETITEMTEXT, lParam2, addr lvi

	.if g_uSortOrder == SORT_ASCENDING
		invoke lstrcmpi, addr buffer1, addr buffer		
	.else
		invoke lstrcmpi, addr buffer, addr buffer1
	.endif

	ret

CompareFunc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      UpdatelParam                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

UpdatelParam proc uses edi

LOCAL lvi:LV_ITEM
    
	invoke SendMessage, g_hwndListView, LVM_GETITEMCOUNT, 0, 0
	mov edi, eax
	mov lvi.imask, LVIF_PARAM

	xor eax, eax
	mov lvi.iSubItem, eax
	mov lvi.iItem, eax
	.while edi > 0
		push lvi.iItem
		pop lvi.lParam
		invoke SendMessage, g_hwndListView, LVM_SETITEM, 0, addr lvi
		inc lvi.iItem
		dec edi
	.endw

	ret

UpdatelParam endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    LoadHeaderBitmap                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

LoadHeaderBitmap proc uses esi edi ecx edx ebx

	mov g_hbmpHeaderArrowDown, $invoke(LoadImage, g_hInstance, IDI_DOWN_ARROW, IMAGE_BITMAP, CX_HEADERBITMAP, CY_HEADERBITMAP, LR_LOADMAP3DCOLORS)
	mov g_hbmpHeaderArrowUp, $invoke(LoadImage, g_hInstance, IDI_UP_ARROW, IMAGE_BITMAP, CX_HEADERBITMAP, CY_HEADERBITMAP, LR_LOADMAP3DCOLORS)

    ret

LoadHeaderBitmap endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    DeleteHeaderBitmap                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DeleteHeaderBitmap proc 

	invoke DeleteObject, g_hbmpHeaderArrowDown
	invoke DeleteObject, g_hbmpHeaderArrowUp

    ret

DeleteHeaderBitmap endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    ImageToHeaderItem                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ImageToHeaderItem proc hwndHeader:HWND, uColumn:UINT, hbmp:HBITMAP

; hbmp == NULL: Remove bitmap

LOCAL hdi:HD_ITEM

	mov hdi.imask, HDI_FORMAT
	invoke SendMessage, hwndHeader, HDM_GETITEM, uColumn, addr hdi
	.if hbmp != NULL
		mov hdi.imask, HDI_FORMAT + HDI_BITMAP
		or hdi.fmt, HDF_BITMAP + HDF_BITMAP_ON_RIGHT
		m2m hdi.hbm, hbmp
	.else
		mov hdi.imask, HDI_FORMAT
		and hdi.fmt, NOT HDF_BITMAP
	.endif
    invoke SendMessage, hwndHeader, HDM_SETITEM, uColumn, addr hdi

	ret

ImageToHeaderItem endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   InsertListViewColumn                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

InsertListViewColumn proc uses esi edi

local lvc:LV_COLUMN

	lea esi, lvc
	assume esi:PTR LV_COLUMN
	mov edi, g_hwndListView

    mov [esi].imask, LVCF_FMT + LVCF_TEXT + LVCF_WIDTH

    mov [esi].fmt, LVCFMT_LEFT
    mov [esi].pszText, $CTA0("Name")
    mov [esi].lx, 300
	ListView_InsertColumn edi, 0, esi

    mov [esi].pszText, $CTA0("Link Target")
	ListView_InsertColumn edi, 1, esi

	assume esi:nothing

    ret
    		
InsertListViewColumn endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       FillListView                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FillListView proc uses esi edi ebx

local lvi:LV_ITEM 
local buffer[1024]:CHAR
local lvc:LVCOLUMN
;local pMemory:LPVOID

	invoke SendMessage, g_hwndListView, WM_SETREDRAW, FALSE, 0

	ListView_DeleteAllItems g_hwndListView

	xor edi, edi
	mov ebx, 1000h										; start with one page
	.while TRUE

		invoke malloc, ebx
		.break .if eax == 0

		mov edi, eax
		invoke QueryDosDevice, NULL, edi, ebx
		.break .if eax != 0

		invoke GetLastError
		push eax
		invoke free, edi
		xor edi, edi
		pop eax

		.break .if eax != ERROR_INSUFFICIENT_BUFFER	; something strange
		shl ebx, 1									; ask twice more memory
		.break .if ebx > 1000h * 1000				; 1000 pages should be enough anyway

	.endw

	.if edi != NULL

		lea esi, lvi
		assume esi:ptr LV_ITEM
		mov [esi].imask, LVIF_TEXT
		and lvi.iItem, 0

		mov ebx, edi
		.if byte ptr [ebx] != 0
			.while TRUE
				invoke QueryDosDevice, ebx, addr buffer, sizeof buffer
				.if eax != 0

					and lvi.iSubItem, 0
					mov [esi].pszText, ebx
					ListView_InsertItem g_hwndListView, esi
						
					inc lvi.iSubItem
					lea eax, buffer
					mov [esi].pszText, eax
					ListView_SetItem g_hwndListView, esi

				.endif
				invoke lstrlen, ebx
				mov  cl, [ebx+eax+1]
				.break .if cl == 0
				lea  ebx, [ebx+eax+1]
				inc lvi.iItem
			.endw
		.endif

		invoke free, edi
		assume esi:nothing

	.endif

	invoke SendMessage, g_hwndListView, WM_SETREDRAW, TRUE, 0

	ret

FillListView endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Dlg_OnNotify                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnNotify proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

option PROLOGUE:NONE
option EPILOGUE:NONE

	mov eax, lParam
	mov eax, (NMHDR ptr [eax]).hwndFrom
	.if eax == g_hwndListView
		; Notify message from Process List
		mov edi, lParam

		assume edi:ptr NMHDR
		.if [edi].code == LVN_COLUMNCLICK			
			assume edi:ptr NM_LISTVIEW
			mov eax, g_uPrevClickedColumn
			.if [edi].iSubItem != eax
				; Remove bitmap from prev header column
				invoke ImageToHeaderItem, g_hwndHeader, g_uPrevClickedColumn, NULL
				mov g_uSortOrder, SORT_NOT_YET
				m2m g_uPrevClickedColumn, [edi].iSubItem
			.endif					
					
			.if g_uSortOrder == SORT_NOT_YET || g_uSortOrder == SORT_DESCENDING
				mov g_uSortOrder, SORT_ASCENDING
				invoke ImageToHeaderItem, g_hwndHeader, [edi].iSubItem, g_hbmpHeaderArrowDown
			.else
				mov g_uSortOrder, SORT_DESCENDING
				invoke ImageToHeaderItem, g_hwndHeader, [edi].iSubItem, g_hbmpHeaderArrowUp
				.endif					
			invoke SendMessage, g_hwndListView, LVM_SORTITEMSEX, [edi].iSubItem, addr CompareFunc
			invoke UpdatelParam
		.endif
	.endif

	pop eax
	jmp eax								; jmp LeaveDlgProc1

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

Dlg_OnNotify endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Dlg_OnCommand                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnCommand proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

option PROLOGUE:NONE
option EPILOGUE:NONE

	mov eax, $LOWORD(wParam)
	.if eax == IDCANCEL
		invoke EndDialog, hDlg, 0
	.endif

	pop eax
	jmp eax								; jmp LeaveDlgProc1

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

Dlg_OnCommand endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Dlg_OnSize                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnSize proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

option PROLOGUE:NONE
option EPILOGUE:NONE

	; accommodate ListView size to main dialog

	mov eax, lParam
	and eax, 0FFFFh									; width of main dlg client area

	mov ecx, lParam
	shr ecx, 16										; height of main dlg client area
	invoke MoveWindow, g_hwndListView, 0, 0, eax, ecx, TRUE

	pop eax
	jmp eax								; jmp LeaveDlgProc1

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

Dlg_OnSize endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Dlg_OnDestroy                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnDestroy proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke DestroyIcon, g_hMainIcon
	invoke DeleteHeaderBitmap

	pop eax
	jmp eax								; jmp LeaveDlgProc1

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

Dlg_OnDestroy endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Dlg_OnInitDialog                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnInitDialog proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

option PROLOGUE:NONE
option EPILOGUE:NONE

	m2m g_hDlg, hDlg

	mov g_uPrevClickedColumn, -1
	mov g_uSortOrder, SORT_NOT_YET

	; Set Dialog Icon
	mov g_hMainIcon, $invoke(LoadIcon, g_hInstance, IDI_MAIN_ICON)
	invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, g_hMainIcon

	; If we XP themed, remove WS_EX_STATICEDGE. Looks better.
		
	invoke AdjustGuiIfThemed, hDlg
		
	mov g_hwndListView, $invoke(GetDlgItem, hDlg, IDC_LISTVIEW)

	ListView_SetExtendedListViewStyle g_hwndListView, LVS_EX_GRIDLINES + LVS_EX_FULLROWSELECT

	mov g_hwndHeader, $invoke(SendMessage, g_hwndListView, LVM_GETHEADER, 0, 0)

	; Change List View Header Stiles
	invoke GetWindowLong, g_hwndHeader, GWL_STYLE
	or eax, HDS_HOTTRACK
	invoke SetWindowLong, g_hwndHeader, GWL_STYLE, eax
		
    invoke LoadHeaderBitmap

	; Add about menu
	invoke GetSystemMenu, hDlg, FALSE
	mov esi, eax
	invoke InsertMenu, esi, -1, MF_BYPOSITION + MF_SEPARATOR, 0, 0
	invoke InsertMenu, esi, -1, MF_BYPOSITION + MF_STRING, IDM_ABOUT, offset szAbout

	invoke InsertListViewColumn
	invoke FillListView

	pop eax
	jmp eax								; jmp LeaveDlgProc1

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

Dlg_OnInitDialog endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Dlg_OnClose                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnClose proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke EndDialog, hDlg, 0

	pop eax
	jmp eax								; jmp LeaveDlgProc1

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

Dlg_OnClose endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     Dlg_OnSysCommand                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnSysCommand proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

option PROLOGUE:NONE
option EPILOGUE:NONE

	.if wParam == IDM_ABOUT
		invoke MessageBox, hDlg, addr szWrittenBy, addr szAbout, MB_OK + MB_ICONINFORMATION
	.endif

	pop eax
	jmp LeaveDlgProc0

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

Dlg_OnSysCommand endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   Dlg_OnSysColorChange                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnSysColorChange proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke DeleteHeaderBitmap
	invoke LoadHeaderBitmap

	pop eax
	jmp LeaveDlgProc0

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

Dlg_OnSysColorChange endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        Dlg_DlgProc                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

DLG_PROC_LOCAL

	push LeaveDlgProc1
	mov eax, uMsg

	IF_MSG	WM_NOTIFY,			Dlg_OnNotify
	IF_MSG	WM_COMMAND,			Dlg_OnCommand
	IF_MSG	WM_SIZE,			Dlg_OnSize
	IF_MSG	WM_CLOSE,			Dlg_OnClose
	IF_MSG	WM_DESTROY,			Dlg_OnDestroy
	IF_MSG	WM_INITDIALOG,		Dlg_OnInitDialog
	IF_MSG	WM_SYSCOMMAND,		Dlg_OnSysCommand
	IF_MSG	WM_SYSCOLORCHANGE,	Dlg_OnSysColorChange



	pop eax						; remove LeaveDlgProc1 from stack
	; default 
	LeaveDlgProc0::
	xor eax, eax
	ret							; return FALSE

	LeaveDlgProc1::
	xor eax, eax
	inc eax
	ret							; return TRUE
    
DlgProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start:

    mov g_hInstance, $invoke(GetModuleHandle, NULL)
    invoke DialogBoxParam, g_hInstance, IDD_MAIN_DIALOG, NULL, addr DlgProc, 0

	invoke ExitProcess, eax
    invoke InitCommonControls

end start
