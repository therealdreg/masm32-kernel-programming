;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  Process Monitor control programm.
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
include \masm32\include\advapi32.inc
include \masm32\include\comctl32.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\comctl32.lib

include \masm32\include\winioctl.inc

include cocomac\cocomac.mac
include cocomac\ListView.mac
include \masm32\Macros\Strings.mac

include ..\common.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IDD_MAIN		equ	1000
IDC_LISTVIEW	equ 1001
IDI_ICON		equ 1002

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_hInstance		HINSTANCE	?
g_hwndDlg		HWND		?
g_hwndListView	HWND		?

g_hSCManager	HANDLE		?
g_hService		HANDLE		?
g_hEvent		HANDLE		?

g_hDevice		HANDLE		?

g_fbExitNow		BOOL		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        C O D E                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                             MyUnhandledExceptionFilter                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MyUnhandledExceptionFilter proc lpExceptionInfo:PTR EXCEPTION_POINTERS

; Just cleanup every possible thing.

local dwBytesReturned:DWORD
local _ss:SERVICE_STATUS

	; If something went wrong let the driver know it should undo the things.

	invoke DeviceIoControl, g_hDevice, IOCTL_REMOVE_NOTIFY, \
					NULL, 0, NULL, 0, addr dwBytesReturned, NULL

	mov g_fbExitNow, TRUE		; If exception has occured not in loop thread it should exit now.
	invoke SetEvent, g_hEvent
					
	invoke Sleep, 100

	invoke CloseHandle, g_hEvent
	invoke CloseHandle, g_hDevice

	invoke ControlService, g_hService, SERVICE_CONTROL_STOP, addr _ss

	invoke DeleteService, g_hService

	invoke CloseServiceHandle, g_hService
	invoke CloseServiceHandle, g_hSCManager

	mov eax, EXCEPTION_EXECUTE_HANDLER
	ret

MyUnhandledExceptionFilter endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     ListViewInsertColumn                                          
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ListViewInsertColumn proc

local lvc:LV_COLUMN

	mov lvc.imask, LVCF_TEXT + LVCF_WIDTH 
	mov lvc.pszText, $CTA0("Process")
	mov lvc.lx, 354
	ListView_InsertColumn g_hwndListView, 0, addr lvc

	mov lvc.pszText, $CTA0("PID")
	or lvc.imask, LVCF_FMT
	mov lvc.fmt, LVCFMT_RIGHT
	mov lvc.lx, 40
	ListView_InsertColumn g_hwndListView, 1, addr lvc

	mov lvc.fmt, LVCFMT_LEFT
	mov lvc.lx, 80
	mov lvc.pszText, $CTA0("State")
	ListView_InsertColumn g_hwndListView, 2, addr lvc

	ret

ListViewInsertColumn endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        FillProcessInfo                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FillProcessInfo proc uses esi pProcessData:PTR PROCESS_DATA

local lvi:LV_ITEM
local buffer[1024]:CHAR

	mov esi, pProcessData
	assume esi:ptr PROCESS_DATA

	mov lvi.imask, LVIF_TEXT

	ListView_GetItemCount g_hwndListView
	mov lvi.iItem, eax

	; The path can be it the short form. Convert it to long.
	; If no long path is found or path is in long form, GetLongPathName
	; simply returns the specified path.

	invoke GetLongPathName, addr [esi].szProcessName, addr buffer, sizeof buffer
	.if ( eax == 0 ) || ( eax >= sizeof buffer )

		; 1024 bytes was not enough. Just display whatever we've got from the driver.
		; I want to keep the things simple. But you'd better to allocate more memory
		; and call GetLongPathName again and again until the buffer size will
		; satisfy the need.
		
		lea ecx, [esi].szProcessName

	.else

		lea ecx, buffer

	.endif

	and lvi.iSubItem, 0
	mov lvi.pszText, ecx
	ListView_InsertItem g_hwndListView, addr lvi

	inc lvi.iSubItem
	invoke wsprintf, addr buffer, $CTA0("%X"), [esi].dwProcessId
	lea eax, buffer
	mov lvi.pszText, eax
	ListView_SetItem g_hwndListView, addr lvi

	inc lvi.iSubItem
	.if [esi].bCreate
		mov lvi.pszText, $CTA0("Created")
	.else
		mov lvi.pszText, $CTA0("Destroyed")
	.endif
	ListView_SetItem g_hwndListView, addr lvi

	assume esi:nothing

	; Scroll down if needed
	ListView_GetItemCount g_hwndListView
	dec eax				; Make index zero-based
	ListView_EnsureVisible g_hwndListView, eax, FALSE

	ret

FillProcessInfo endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     WaitForProcessData                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

WaitForProcessData proc hEvent:HANDLE

local ProcessData:PROCESS_DATA
local dwBytesReturned:DWORD

	invoke GetCurrentThread
	invoke SetThreadPriority, eax, THREAD_PRIORITY_HIGHEST	

	.while TRUE
		invoke WaitForSingleObject, hEvent, INFINITE
		.if eax != WAIT_FAILED

			.break .if g_fbExitNow == TRUE

			invoke DeviceIoControl, g_hDevice, IOCTL_GET_PROCESS_DATA, NULL, 0, \
						addr ProcessData, sizeof ProcessData, addr dwBytesReturned, NULL

			.if eax != 0
				invoke FillProcessInfo, addr ProcessData
			.endif

		.else
			invoke MessageBox, g_hwndDlg, \
				$CTA0("Wait for event failed. Thread now exits. Restart application."), \
				NULL, MB_ICONERROR
			.break
		.endif
	.endw

	invoke ExitThread, 0
	ret							; Never executed.

WaitForProcessData endp
	
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               D I A L O G     P R O C E D U R E                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local rect:RECT

	mov eax, uMsg
	.if eax == WM_INITDIALOG

		push hDlg
		pop g_hwndDlg

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

		invoke GetDlgItem, hDlg, IDC_LISTVIEW
		mov g_hwndListView, eax
		invoke SetFocus, g_hwndListView

		invoke GetClientRect, hDlg, addr rect
		invoke MoveWindow, g_hwndListView, rect.left, rect.top, rect.right, rect.bottom, FALSE

		ListView_SetExtendedListViewStyle g_hwndListView, LVS_EX_GRIDLINES + LVS_EX_FULLROWSELECT

		invoke ListViewInsertColumn

	.elseif eax == WM_SIZE

		mov eax, lParam
		mov ecx, eax
		and eax, 0FFFFh
		shr ecx, 16
		invoke MoveWindow, g_hwndListView, 0, 0, eax, ecx, TRUE

	.elseif eax == WM_COMMAND

		mov eax, wParam
		and eax, 0FFFFh
		.if eax == IDCANCEL
			invoke MessageBox, hDlg, $CTA0("Sure want to exit?"), \
					$CTA0("Exit Confirmation"), MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON1
			.if eax == IDYES
				invoke EndDialog, hDlg, 0
			.endif
		.endif

	.elseif uMsg == WM_GETMINMAXINFO

		mov ecx, lParam
		mov (MINMAXINFO PTR [ecx]).ptMinTrackSize.x, 380
		mov (MINMAXINFO PTR [ecx]).ptMinTrackSize.y, 150

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

local acModulePath[MAX_PATH]:CHAR
local _ss:SERVICE_STATUS
local dwBytesReturned:DWORD

	CTA  "This program was tested on Windows 2000+sp2/sp3/sp4,\n", szExecutionConfirmation
	CTA  "Windows XP no sp, Windows Server 2003 Std and\n"
	CTA  "seems to be workable. But it uses undocumented\n"
	CTA  "tricks in kernel mode and may crash your system\:\n"
	CTA  "\n"
	CTA0 "Are your shure you want to run it?\n"

	invoke MessageBox, NULL, addr szExecutionConfirmation, \
		$CTA0("Execution Confirmation"), MB_YESNO + MB_ICONQUESTION + MB_DEFBUTTON2
	.if eax == IDNO
		invoke ExitProcess, 0
	.endif

	; The very first thing we have to do is to install exception handler
	
	invoke SetUnhandledExceptionFilter, MyUnhandledExceptionFilter

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
	.if eax != NULL
		mov g_hSCManager, eax

		push eax
		invoke GetFullPathName, $CTA0("ProcessMon.sys"), sizeof acModulePath, addr acModulePath, esp
    	pop eax

		invoke CreateService, g_hSCManager, $CTA0("ProcessMon"), \
			$CTA0("Process creation/destruction monitor"), \
			SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
			SERVICE_ERROR_IGNORE, addr acModulePath, NULL, NULL, NULL, NULL, NULL

		.if eax != NULL
			mov g_hService, eax

			invoke StartService, g_hService, 0, NULL
			.if eax != 0

				invoke CreateFile, $CTA0("\\\\.\\ProcessMon"), \
						GENERIC_READ + GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL

				.if eax != INVALID_HANDLE_VALUE
					mov g_hDevice, eax

					; No need it to be registered anymore

					invoke DeleteService, g_hService
			
					; Create unnamed auto-reset event to be signalled when there is data to read.

					invoke CreateEvent, NULL, FALSE, FALSE, NULL
					mov g_hEvent, eax

					and g_fbExitNow, FALSE

					; Create thread to wait event signalled.

					push eax								; place for dwThreadID
					invoke CreateThread, NULL, 0, offset WaitForProcessData, g_hEvent, 0, esp
					pop ecx									; throw dwThreadID away
					.if eax != NULL
					
						invoke CloseHandle, eax								

						;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

						invoke DeviceIoControl, g_hDevice, IOCTL_SET_NOTIFY, \
								addr g_hEvent, sizeof g_hEvent, NULL, 0, addr dwBytesReturned, NULL

						.if eax != 0

							invoke GetModuleHandle, NULL
							mov g_hInstance, eax
							invoke DialogBoxParam, g_hInstance, IDD_MAIN, NULL, addr DlgProc, 0

							invoke DeviceIoControl, g_hDevice, IOCTL_REMOVE_NOTIFY, \
										NULL, 0, NULL, 0, addr dwBytesReturned, NULL
						.else
							invoke MessageBox, NULL, \
									$CTA0("Can't set notify."), NULL, MB_ICONSTOP
						.endif

						;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

						mov g_fbExitNow, TRUE
						invoke SetEvent, g_hEvent		; Signal event to make loop thread exit.
					
						invoke Sleep, 100
					
					.else
						invoke MessageBox, NULL, $CTA0("Can't create thread."), NULL, MB_ICONSTOP						
					.endif

					invoke CloseHandle, g_hEvent
					invoke CloseHandle, g_hDevice
				.else
					invoke MessageBox, NULL, $CTA0("Can't open device."), NULL, MB_ICONSTOP
				.endif
				invoke ControlService, g_hService, SERVICE_CONTROL_STOP, addr _ss
			.else
				invoke MessageBox, NULL, $CTA0("Can't start driver."), NULL, MB_ICONSTOP
			.endif

			invoke DeleteService, g_hService
			invoke CloseServiceHandle, g_hService

		.else
			invoke MessageBox, NULL, $CTA0("Can't register driver."), NULL, MB_ICONSTOP
		.endif
		invoke CloseServiceHandle, g_hSCManager
	.else
		invoke MessageBox, NULL, \
			$CTA0("Can't connect to SCM."), NULL, MB_ICONSTOP
	.endif

	invoke ExitProcess, 0
	invoke InitCommonControls
	ret

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start

:make

set exe=ProcessMon

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
if exist ..\%exe%.exe del ..\%exe%.exe

\masm32\bin\ml /nologo /c /coff %exe%.bat
\masm32\bin\link /nologo /subsystem:windows %exe%.obj rsrc.obj

del %exe%.obj
move %exe%.exe ..
if exist %exe%.exe del %exe%.exe

echo.
pause
