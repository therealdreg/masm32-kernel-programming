
; KmdManager - utility for simplify kmd un/loading and sending control codes
; Written by Four-F (four-f@mail.ru)

.386
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\windows.inc

include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include \masm32\include\advapi32.inc
include \masm32\include\comdlg32.inc
include \masm32\include\shell32.inc

includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\comdlg32.lib
includelib \masm32\lib\shell32.lib

include Macros.mac
include \masm32\Macros\Strings.mac
include \masm32\cocomac\cocomac.mac
include \masm32\cocomac\ListView.mac
include \masm32\cocomac\Header.mac
include htodw.asm
include memory.asm
include string.asm
include MaskedEdit.asm
include theme.asm

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                         F U N C T I O N S   P R O T O T Y P E S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proto :HWND, :UINT, :WPARAM, :LPARAM

IDD_DIALOG				equ		1000
IDB_BROWSE				equ		1001
IDB_REGISTER			equ		1002
IDB_RUN					equ		1003
IDB_IOCONTROL			equ		1004
IDB_UNREGISTER			equ		1005
IDB_STOP				equ		1006

IDB_OPTIONS				equ		1007
IDB_ABOUT				equ		1008
IDB_EXIT				equ		1009

IDCHK_REGTORUNLINK		equ		1010
IDCHK_UNREGTOSTOPLINK	equ		1011
IDCHK_IOCONTROLLINK		equ		1012

IDE_PATH				equ		1020
IDE_CONTROL_CODE		equ		1021

IDC_REPORT_LIST			equ		1030

IDI_ICON				equ		2000

IDM_CLEAR_LOG			equ		5000

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                      U S E R   D E F I N E D   S T R U C T U R E S                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
g_szFilterString		db "Kernel-Mode Drivers", 0, "*.sys", 0
						db "All Files", 0, "*.*", 0, 0

g_szOpenDriverTitle		db "Choose Driver", 0

g_szSuccess				db "Success", 0
g_szFail				db "Fail", 0
g_szCriticalError		db "Critical Error", 0
g_szOpenSCManagerError	db "Can't get Service Control Manager handle.", 0
g_szEnterFullDriverPath	db "Enter full driver path.", 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              I N I T I A L I Z E D  D A T A                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_hInstance				HINSTANCE	?
;g_pfnPrevStaticProc		LPVOID		?
g_hwndEditDriverPath	HWND		?
g_hwndEditControlCode	HWND		?
g_hwndReportListView	HWND		?

g_hwndButtonRegister	HWND		?
g_hwndButtonRun			HWND		?
g_hwndButtonControl		HWND		?
g_hwndButtonStop		HWND		?
g_hwndButtonUnregister	HWND		?
g_hListViewPopupMenu	HMENU		?
g_hwndCheckRegToRun		HWND		?
g_hwndCheckUnregToStop	HWND		?
g_hwndCheckLinkAll		HWND		?

g_pfnListViewProcPrev	LPVOID		?

g_acErrorDescription	CHAR	256	dup(?)


g_dwDlgMinHeight		DWORD		?
g_dwDlgMaxHeight		DWORD		?

g_dwDlgWidth			DWORD		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        LastError                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

LastError proc; pacBuffer:LPVOID

    pushfd
    pushad
    
    invoke GetLastError
    push eax

	invoke RtlZeroMemory, offset g_acErrorDescription, sizeof g_acErrorDescription
	pop eax
    mov ecx, SUBLANG_DEFAULT
    shl ecx, 10
    add ecx, LANG_NEUTRAL               ; MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) User default language
    
    invoke FormatMessage, FORMAT_MESSAGE_FROM_SYSTEM + FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL, \
    						eax, ecx, offset g_acErrorDescription, 128, NULL

    .if eax == 0
		invoke lstrcpy, offset g_acErrorDescription, $CTA0("Error number not found.")
    .endif

    popad
    popfd
    
    ret

LastError endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      ReportStatus                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ReportStatus proc uses esi pszDriverName:LPSTR, pszOperation:LPSTR, pszStatus:LPSTR, pszLastError:LPSTR

LOCAL lvi:LV_ITEM

	mov lvi.imask, LVIF_TEXT 
	m2m lvi.pszText,pszDriverName
	and lvi.iSubItem, 0
	ListView_GetItemCount g_hwndReportListView
	mov esi, eax
	mov lvi.iItem, eax

	ListView_InsertItem g_hwndReportListView, addr lvi	

	ListView_SetItemText g_hwndReportListView, esi, 1, pszOperation
	ListView_SetItemText g_hwndReportListView, esi, 2, pszStatus
	ListView_SetItemText g_hwndReportListView, esi, 3, pszLastError

	; Make it fully visible
	ListView_EnsureVisible g_hwndReportListView, esi, FALSE

	ret

ReportStatus endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     RegisterDriver                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RegisterDriver proc uses esi edi ebx pszDriverName:LPSTR, pszDriverPath:LPSTR

	xor ebx, ebx		; assume error
	mov edi, offset g_szFail

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_CREATE_SERVICE
	.if eax != NULL
		mov esi, eax

		; Register driver - fill registry directory
		invoke CreateService, esi, pszDriverName, pszDriverName, \
					0, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, \
					pszDriverPath, NULL, NULL, NULL, NULL, NULL

		invoke LastError

		.if eax != NULL
			invoke CloseServiceHandle, eax
			inc ebx					; success
			mov edi, offset g_szSuccess
		.endif
		invoke CloseServiceHandle, esi
	.else
		invoke MessageBox, NULL, addr g_szOpenSCManagerError, addr g_szCriticalError, MB_OK + MB_ICONSTOP
	.endif

	invoke ReportStatus, pszDriverName, $CTA0("Register"), edi, offset g_acErrorDescription

	return ebx

RegisterDriver endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    UnregisterDriver                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

UnregisterDriver proc uses esi edi ebx pszDriverName:LPSTR

	xor ebx, ebx		; assume error
	mov edi, offset g_szFail

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_CONNECT
	.if eax != NULL
		mov esi, eax
		
		; Unregister driver - remove registry directory
		invoke OpenService, esi, pszDriverName, DELETE

		invoke LastError

		.if eax != NULL
			push eax
			invoke DeleteService, eax

			invoke LastError

			.if eax != 0
				inc ebx					; success
				mov edi, offset g_szSuccess
			.endif
			call CloseServiceHandle
		.endif

		invoke CloseServiceHandle, esi
	.else
		invoke MessageBox, NULL, addr g_szOpenSCManagerError, addr g_szCriticalError, MB_OK + MB_ICONSTOP
	.endif

	invoke ReportStatus, pszDriverName, $CTA0("Unregister"), edi, offset g_acErrorDescription

	return ebx

UnregisterDriver endp


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       RunDriver                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RunDriver proc uses esi edi ebx pszDriverName:LPSTR

	xor ebx, ebx		; assume error
	mov edi, offset g_szFail

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_CONNECT
	.if eax != NULL
		mov esi, eax
		
		; Unregister driver - remove registry directory
		invoke OpenService, esi, pszDriverName, SERVICE_START

		invoke LastError

		.if eax != NULL
			push eax
			invoke StartService, eax, 0, NULL
			
			invoke LastError
			
			.if eax != 0
				inc ebx					; success
				mov edi, offset g_szSuccess
			.endif

			call CloseServiceHandle
			mov edi, offset g_szSuccess
		.endif
		invoke CloseServiceHandle, esi
	.else
		invoke MessageBox, NULL, addr g_szOpenSCManagerError, addr g_szCriticalError, MB_OK + MB_ICONSTOP
	.endif

	invoke ReportStatus, pszDriverName, $CTA0("Start"), edi, offset g_acErrorDescription

	return ebx

RunDriver endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       StopDriver                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

StopDriver proc uses esi ebx pszDriverName:LPSTR

LOCAL sest:SERVICE_STATUS

	xor ebx, ebx		; assume error
	mov edi, offset g_szFail

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_CONNECT
	.if eax != NULL
		mov esi, eax
		
		; Unregister driver - remove registry directory
		invoke OpenService, esi, pszDriverName, SERVICE_STOP

		invoke LastError

		.if eax != NULL
			push eax
			mov ecx, eax
			invoke ControlService, ecx, SERVICE_CONTROL_STOP, addr sest
			
			invoke LastError

			.if eax != 0
				inc ebx					; success
				mov edi, offset g_szSuccess
			.endif
			call CloseServiceHandle
		.endif

		invoke CloseServiceHandle, esi
	.else
		invoke MessageBox, NULL, addr g_szOpenSCManagerError, addr g_szCriticalError, MB_OK + MB_ICONSTOP
	.endif

	invoke ReportStatus, pszDriverName, $CTA0("Stop"), edi, offset g_acErrorDescription

	return ebx

StopDriver endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      ControlDevice                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ControlDriver proc uses esi edi ebx pszDriverName:LPSTR, dwCode:DWORD

LOCAL acBuffer[MAX_PATH]:CHAR
LOCAL dwBytesReturned:DWORD

	xor ebx, ebx		; assume error
	mov edi, offset g_szFail

	invoke GetVersion
	.if al >= 5
		mov eax, $CTA0("\\\\.\\Global\\%s")
	.else
		mov eax, $CTA0("\\\\.\\%s")
	.endif
	invoke wsprintf, addr acBuffer, eax, pszDriverName

	invoke CreateFile, addr acBuffer, GENERIC_READ + GENERIC_WRITE, 0, \
				NULL, OPEN_EXISTING, 0, NULL

	invoke LastError

	.if eax != INVALID_HANDLE_VALUE
		mov esi, eax
		invoke DeviceIoControl, esi, dwCode, NULL, 0, NULL, 0, addr dwBytesReturned, NULL

		invoke LastError

		.if eax != 0
			inc ebx					; success
			mov edi, offset g_szSuccess
		.endif
		invoke CloseHandle, esi
	.else
		invoke MessageBox, NULL, $CTA0("Can't get Driver handle."), addr g_szCriticalError, MB_OK + MB_ICONSTOP
	.endif

	invoke ReportStatus, pszDriverName, $CTA0("Control"), edi, offset g_acErrorDescription

	return ebx

ControlDriver endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  GetDriverNameFromPath                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetDriverNameFromPath proc uses esi edi ebx pDriverPath:LPSTR, pBuffer:LPVOID

	xor ebx, ebx	; assume error

	mov edi, pDriverPath
	mov esi, edi
	invoke lstrlen, edi
	add esi, eax
	sub esi, 4			; ".sys"
	invoke lstrcmpi, $CTA0(".sys"), esi
	.if eax == 0
		xor ecx, ecx
		dec esi

	    .while esi > edi
		    mov al, [esi]
	    	.break .if al == '\'
		    inc ecx
		    dec esi
		.endw

		.if esi != edi
			inc esi
			mov edi, pBuffer
			rep movsb
		    mov byte ptr [edi], 0
		    inc ebx				; success
		.else
			invoke MessageBox, NULL, $CTA0("Can't extract Driver Name.\nYou have to specify full path."), \
									NULL, MB_OK + MB_ICONSTOP
		.endif

	.else
		invoke MessageBox, NULL, $CTA0("Can't recognize Driver Name.\nThe file extension must be '.sys'."), \
									NULL, MB_OK + MB_ICONSTOP
	.endif

	return ebx

GetDriverNameFromPath endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  InsertReportListColumns                                          
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

InsertReportListColumns proc hwndListView:HWND
	
LOCAL lvc:LV_COLUMN
LOCAL lvi:LV_ITEM
	
	ListView_SetExtendedListViewStyle hwndListView, LVS_EX_GRIDLINES + LVS_EX_FULLROWSELECT

	mov lvc.imask, LVCF_TEXT + LVCF_WIDTH + LVCF_FMT
	mov lvc.fmt, LVCFMT_LEFT
	mov lvc.pszText, $TA0("Driver")
	mov lvc.lx, 60
	ListView_InsertColumn hwndListView, 0, addr lvc

	mov lvc.pszText, $TA0("Operation")
	ListView_InsertColumn hwndListView, 1, addr lvc

	mov lvc.pszText, $TA0("Status")
	ListView_InsertColumn hwndListView, 2, addr lvc

	mov lvc.lx, 400
	mov lvc.pszText, $TA0("Last Error")
	ListView_InsertColumn hwndListView, 3, addr lvc

	ret

InsertReportListColumns endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      MakeFilePathOnly                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MakeFilePathOnly proc lpPathName:LPVOID

	invoke fstrlen, lpPathName

    mov ecx, lpPathName
    lea ecx, [ecx][eax-6]

@@:
    mov al, [ecx]
    dec ecx
    cmp al, '\'
    jne @B

    mov byte ptr [ecx+2], 0

    ret

MakeFilePathOnly endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ListViewProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

	mov eax, uMsg
	.if eax == WM_CONTEXTMENU

		; Don't pop up menu if list is empty
		ListView_GetItemCount g_hwndReportListView
		.if eax != 0
			mov eax, lParam
			mov ecx, eax
			and eax, 0FFFFh
			shr ecx, 16
			invoke TrackPopupMenu, g_hListViewPopupMenu, TPM_LEFTALIGN, eax, ecx, NULL, hWnd, NULL
		.endif

	.elseif eax == WM_COMMAND

		mov eax, $LOWORD(wParam)
		.if eax == IDM_CLEAR_LOG	
			ListView_DeleteAllItems g_hwndReportListView
		.endif

	.endif

	invoke CallWindowProc, g_pfnListViewProcPrev, hWnd, uMsg, wParam, lParam

	ret

ListViewProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Dlg_OnInitDialog                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnInitDialog proc hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local rect:RECT

	; Set Dialog Icon
	invoke LoadIcon, g_hInstance, IDI_ICON
	invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

	mov g_hwndEditDriverPath, $invoke(GetDlgItem, hDlg, IDE_PATH)
	mov g_hwndEditControlCode, $invoke(GetDlgItem, hDlg, IDE_CONTROL_CODE)
	; Thnx to James Brown for idea
	invoke MaskEditControl, g_hwndEditControlCode, $CTA0("0123456789abcdefABCDEF"), TRUE
	invoke SendMessage, g_hwndEditControlCode, EM_LIMITTEXT, 8, 0

	mov g_hwndButtonRegister,	$invoke(GetDlgItem, hDlg, IDB_REGISTER)
	mov g_hwndButtonRun,		$invoke(GetDlgItem, hDlg, IDB_RUN)
	mov g_hwndButtonControl,	$invoke(GetDlgItem, hDlg, IDB_IOCONTROL)
	mov g_hwndButtonStop,		$invoke(GetDlgItem, hDlg, IDB_STOP)
	mov g_hwndButtonUnregister, $invoke(GetDlgItem, hDlg, IDB_UNREGISTER)

	mov g_hwndCheckRegToRun,	$invoke(GetDlgItem, hDlg, IDCHK_REGTORUNLINK)
	mov g_hwndCheckUnregToStop, $invoke(GetDlgItem, hDlg, IDCHK_UNREGTOSTOPLINK)
	mov g_hwndCheckLinkAll,		$invoke(GetDlgItem, hDlg, IDCHK_IOCONTROLLINK)

	mov g_hwndReportListView, $invoke(GetDlgItem, hDlg, IDC_REPORT_LIST)
	invoke InsertReportListColumns, g_hwndReportListView

	mov g_pfnListViewProcPrev, $invoke(SetWindowLong, g_hwndReportListView, GWL_WNDPROC, offset ListViewProc)	
	; Create popup menu
	invoke CreatePopupMenu
	mov g_hListViewPopupMenu,eax
	invoke AppendMenu, g_hListViewPopupMenu, MF_STRING, IDM_CLEAR_LOG, $CTA0("Clear log")
	
	invoke GetDlgItem, hDlg, IDB_OPTIONS
	invoke EnableWindow, eax, FALSE

	; for tracking size

	invoke GetWindowRect, hDlg, addr rect

	lea edx, rect
	mov eax, (RECT PTR [edx]).right
	sub eax, (RECT PTR [edx]).left

	mov g_dwDlgWidth, eax

	mov eax, (RECT PTR [edx]).bottom
	sub eax, (RECT PTR [edx]).top

	mov g_dwDlgMinHeight, eax
	add eax, 195
	mov g_dwDlgMaxHeight, eax
	
	; If we XP themed, remove WS_EX_STATICEDGE. Looks better.

	invoke AdjustGuiIfThemed, hDlg
		
	ret

Dlg_OnInitDialog endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    Dlg_OnCommand                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnCommand proc hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

LOCAL ofn:OPENFILENAME
LOCAL acBufferPath[MAX_PATH]:CHAR
LOCAL acBufferName[MAX_PATH]:CHAR
LOCAL acIoCode[16]:CHAR
LOCAL dwIoCode:DWORD

	mov eax, $LOWORD(wParam)
	.if eax == IDCANCEL
		; ask only if user hit Esc key
		invoke MessageBox, hDlg, $CTA0("Sure want to exit?"), \
							$CTA0("Exit Confirmation"), MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON1
		.if eax == IDYES
			invoke EndDialog, hDlg, 0
		.endif

	.elseif eax == IDB_EXIT
		invoke EndDialog, hDlg, 0

	.elseif eax == IDB_BROWSE

		; Get driver's path to load
		invoke RtlZeroMemory, addr ofn, sizeof ofn
		mov ofn.lStructSize, sizeof ofn
		m2m ofn.hwndOwner, hDlg
		m2m ofn.hInstance, g_hInstance
 		mov ofn.lpstrFilter, offset g_szFilterString
		invoke RtlZeroMemory, addr acBufferPath, sizeof acBufferPath
 		lea eax, acBufferPath
		mov ofn.lpstrFile, eax
		mov ofn.nMaxFile, sizeof acBufferPath

		invoke GetCurrentDirectory, sizeof acBufferName, addr acBufferName
		lea eax, acBufferName
		mov ofn.lpstrInitialDir, eax
		mov ofn.lpstrTitle, offset g_szOpenDriverTitle
		mov ofn.Flags, OFN_FILEMUSTEXIST + OFN_PATHMUSTEXIST + OFN_LONGNAMES + OFN_EXPLORER
		invoke GetOpenFileName, addr ofn
		.if eax != 0
			; Show it in edit control
			invoke SetWindowText, g_hwndEditDriverPath, ofn.lpstrFile
		.endif

	.elseif eax == IDB_REGISTER

		invoke RtlZeroMemory, addr acBufferPath, sizeof acBufferPath
		invoke GetWindowText, g_hwndEditDriverPath, addr acBufferPath, sizeof acBufferPath
		
		.if eax != 0
			invoke GetDriverNameFromPath, addr acBufferPath, addr acBufferName
			.if eax == TRUE
				invoke RegisterDriver, addr acBufferName, addr acBufferPath

				invoke SendMessage, g_hwndCheckRegToRun, BM_GETCHECK, 0, 0
				.if eax == BST_CHECKED
				;invoke IsWindowEnabled, g_hwndButtonRun
				;.if eax == 0
					; linked
					invoke RunDriver, addr acBufferName				
				.endif
			.endif
		.else
			invoke MessageBox, hDlg, addr g_szEnterFullDriverPath, NULL, MB_OK + MB_ICONSTOP
		.endif

	.elseif eax == IDB_UNREGISTER

		invoke RtlZeroMemory, addr acBufferPath, sizeof acBufferPath
		invoke GetWindowText, g_hwndEditDriverPath, addr acBufferPath, sizeof acBufferPath
		
		.if eax != 0
			invoke GetDriverNameFromPath, addr acBufferPath, addr acBufferName
			.if eax == TRUE
				invoke SendMessage, g_hwndCheckUnregToStop, BM_GETCHECK, 0, 0
				.if eax == BST_CHECKED
				;invoke IsWindowEnabled, g_hwndButtonStop
				;.if eax == 0
					; linked
					invoke StopDriver, addr acBufferName				
				.endif
				invoke UnregisterDriver, addr acBufferName
			.endif
		.else
			invoke MessageBox, hDlg, addr g_szEnterFullDriverPath, NULL, MB_OK + MB_ICONSTOP
		.endif

	.elseif eax == IDB_RUN

		invoke RtlZeroMemory, addr acBufferPath, sizeof acBufferPath
		invoke GetWindowText, g_hwndEditDriverPath, addr acBufferPath, sizeof acBufferPath
		
		.if eax != 0
			invoke GetDriverNameFromPath, addr acBufferPath, addr acBufferName
			.if eax == TRUE
				invoke RunDriver, addr acBufferName
			.endif
		.else
			invoke MessageBox, hDlg, addr g_szEnterFullDriverPath, NULL, MB_OK + MB_ICONSTOP
		.endif

	.elseif eax == IDB_STOP

		invoke RtlZeroMemory, addr acBufferPath, sizeof acBufferPath
		invoke GetWindowText, g_hwndEditDriverPath, addr acBufferPath, sizeof acBufferPath
		
		.if eax != 0
			invoke GetDriverNameFromPath, addr acBufferPath, addr acBufferName
			.if eax == TRUE
				invoke StopDriver, addr acBufferName
			.endif
		.else
			invoke MessageBox, hDlg, addr g_szEnterFullDriverPath, NULL, MB_OK + MB_ICONSTOP
		.endif

	.elseif eax == IDB_IOCONTROL

		invoke RtlZeroMemory, addr acBufferPath, sizeof acBufferPath
		invoke GetWindowText, g_hwndEditDriverPath, addr acBufferPath, sizeof acBufferPath
		
		.if eax != 0
			invoke GetDriverNameFromPath, addr acBufferPath, addr acBufferName
			.if eax == TRUE
				invoke GetWindowText, g_hwndEditControlCode, addr acIoCode, sizeof acIoCode
				.if eax != 0
					invoke htodw, addr acIoCode
					mov dwIoCode, eax

					invoke SendMessage, g_hwndCheckLinkAll, BM_GETCHECK, 0, 0
					.if eax == BST_CHECKED
						; linked
						.while TRUE
							; he-he
							invoke RegisterDriver, addr acBufferName, addr acBufferPath
							.break .if eax == FALSE
							invoke RunDriver, addr acBufferName
							.break .if eax == FALSE
							invoke ControlDriver, addr acBufferName, dwIoCode
							.break .if eax == FALSE
							invoke StopDriver, addr acBufferName
							.break .if eax == FALSE
							invoke UnregisterDriver, addr acBufferName
							.break
						.endw
					.else
						invoke ControlDriver, addr acBufferName, dwIoCode
					.endif

				.else
					invoke MessageBox, hDlg, $CTA0("Enter control code first."), NULL, MB_OK + MB_ICONSTOP				
				.endif
			.endif
		.else
			invoke MessageBox, hDlg, addr g_szEnterFullDriverPath, NULL, MB_OK + MB_ICONSTOP
		.endif

	.elseif eax == IDB_ABOUT
		invoke MessageBox, hDlg, $CTA0("Kernel-Mode Driver Manager\n\tv 1.3\n\nWritten by Four-F\nfour-f@mail.ru"), \
								$CTA0("About"), MB_OK + MB_ICONINFORMATION

	.elseif eax == IDCHK_REGTORUNLINK
		invoke SendDlgItemMessage, hDlg, IDCHK_REGTORUNLINK, BM_GETCHECK, 0, 0
		.if eax == BST_CHECKED
			invoke EnableWindow, g_hwndButtonRun, FALSE
			invoke SetWindowText, g_hwndButtonRegister, $CTA0("&Reg'n'Run")
		.else
			invoke EnableWindow, g_hwndButtonRun, TRUE
			invoke SetWindowText, g_hwndButtonRegister, $CTA0("Re&gister")
		.endif	

	.elseif eax == IDCHK_UNREGTOSTOPLINK
		invoke SendDlgItemMessage, hDlg, IDCHK_UNREGTOSTOPLINK, BM_GETCHECK, 0, 0
		.if eax == BST_CHECKED
			invoke EnableWindow, g_hwndButtonStop, FALSE
			invoke SetWindowText, g_hwndButtonUnregister, $CTA0("&Stop'n'Unreg")
		.else
			invoke EnableWindow, g_hwndButtonStop, TRUE
			invoke SetWindowText, g_hwndButtonUnregister, $CTA0("&Unregister")
		.endif	

	.elseif eax == IDCHK_IOCONTROLLINK
		invoke SendDlgItemMessage, hDlg, IDCHK_IOCONTROLLINK, BM_GETCHECK, 0, 0
		.if eax == BST_CHECKED
			invoke EnableWindow, g_hwndButtonRegister, FALSE
			invoke EnableWindow, g_hwndCheckRegToRun, FALSE
			invoke EnableWindow, g_hwndButtonRun, FALSE
			invoke EnableWindow, g_hwndButtonStop, FALSE
			invoke EnableWindow, g_hwndCheckUnregToStop, FALSE
			invoke EnableWindow, g_hwndButtonUnregister, FALSE
			invoke SetWindowText, g_hwndButtonControl, $CTA0("All A&ctions")

		.else
			invoke EnableWindow, g_hwndButtonRegister, TRUE
			invoke EnableWindow, g_hwndCheckRegToRun, TRUE

			invoke SendMessage, g_hwndCheckRegToRun, BM_GETCHECK, 0, 0
			.if eax == BST_UNCHECKED
				invoke EnableWindow, g_hwndButtonRun, TRUE
			.endif	

			invoke SendMessage, g_hwndCheckUnregToStop, BM_GETCHECK, 0, 0
			.if eax == BST_UNCHECKED
				invoke EnableWindow, g_hwndButtonStop, TRUE
			.endif

			invoke EnableWindow, g_hwndCheckUnregToStop, TRUE
			invoke EnableWindow, g_hwndButtonUnregister, TRUE

			invoke SetWindowText, g_hwndButtonControl, $CTA0("I/O &Control")
		.endif

	.endif

	ret

Dlg_OnCommand endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       Dlg_OnSize                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnSize proc uses esi esi ebx hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local rcDlg:RECT
local rcList:RECT
local sz:_SIZE
local pos:POINT

	invoke GetWindowRect, hDlg, addr rcDlg
	invoke GetWindowRect, g_hwndReportListView, addr rcList

	mov eax, rcList.bottom
	sub eax, rcList.top

	mov ecx, rcDlg.bottom
	sub ecx, rcList.bottom
	sub ecx, 6
	add eax, ecx
	mov sz.y, eax

	mov eax, rcList.right
	sub eax, rcList.left
	mov sz.x, eax

	mrm pos.x, rcList.left
	mrm pos.y, rcList.top

	invoke ScreenToClient, hDlg, addr pos

	invoke MoveWindow, g_hwndReportListView, pos.x, pos.y, sz.x, sz.y, TRUE

	ret

Dlg_OnSize endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  Dlg_OnGetMinMaxInfo                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnGetMinMaxInfo proc uses esi esi ebx hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

	mov ecx, lParam
	assume ecx:ptr MINMAXINFO
	mov eax, g_dwDlgWidth
	mov [ecx].ptMinTrackSize.x, eax
	mov [ecx].ptMaxTrackSize.x, eax

	mrm [ecx].ptMinTrackSize.y, g_dwDlgMinHeight
	mrm [ecx].ptMaxTrackSize.y, g_dwDlgMaxHeight
	assume ecx:nothing

	ret

Dlg_OnGetMinMaxInfo endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    Dlg_OnDropFiles                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Dlg_OnDropFiles proc uses esi esi ebx hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local acDroppedFilePath[MAX_PATH]:CHAR
local acDriverName[MAX_MODULE_NAME32]:CHAR

	invoke DragQueryFile, wParam, 0, addr acDroppedFilePath, sizeof acDroppedFilePath

	; Show it in edit control

	invoke SetWindowText, g_hwndEditDriverPath, addr acDroppedFilePath

	invoke MakeFilePathOnly, addr acDroppedFilePath
	invoke SetCurrentDirectory, addr acDroppedFilePath

	ret

Dlg_OnDropFiles endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 D I A L O G      P R O C E D U R E                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

	mov eax, uMsg

	.if eax == WM_COMMAND
		invoke Dlg_OnCommand, hDlg, uMsg, wParam, lParam

	.elseif eax == WM_INITDIALOG
		invoke Dlg_OnInitDialog, hDlg, uMsg, wParam, lParam

	.elseif eax == WM_GETMINMAXINFO
		invoke Dlg_OnGetMinMaxInfo, hDlg, uMsg, wParam, lParam

	.elseif eax == WM_SIZE
		invoke Dlg_OnSize, hDlg, uMsg, wParam, lParam

	.elseif eax == WM_DROPFILES
		invoke Dlg_OnDropFiles, hDlg, uMsg, wParam, lParam

	.elseif eax == WM_CLOSE
		invoke EndDialog, hDlg, 0

	.else
 
		return FALSE
      
	.endif
   
	return TRUE
    
DlgProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start:

	mov g_hInstance, $invoke(GetModuleHandle, NULL)
	invoke DialogBoxParam, g_hInstance, IDD_DIALOG, NULL, addr DlgProc, 0
	invoke ExitProcess, eax

	ret

end start
