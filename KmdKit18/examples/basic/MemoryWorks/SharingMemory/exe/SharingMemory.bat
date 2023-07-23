;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  SharingMemory
;
;  Client of SharingMemory.sys driver
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

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib

include \masm32\include\winioctl.inc

include \masm32\Macros\Strings.mac

include ..\common.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IDD_MAIN			equ	1000
IDC_TIME			equ 1001
IDI_ICON			equ 1002

TIMER_ID			equ		100

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              I N I T I A L I Z E D  D A T A                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_hDevice			HANDLE		?
g_hInstance			HINSTANCE	?
g_hDlg				HWND		?
g_pSharedMemory		LPVOID		?

g_hSCManager		HANDLE		?
g_hService			HANDLE		?


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                             MyUnhandledExceptionFilter                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MyUnhandledExceptionFilter proc lpExceptionInfo:PTR EXCEPTION_POINTERS

; Just cleanup every possible thing
	
local _ss:SERVICE_STATUS

	invoke KillTimer, g_hDlg, TIMER_ID

	; The most important thing here is CloseHandle

	; If something went wrong we must close device handle
	; to let the driver know it should unmap memory.
	; The driver should do it before application exits
	; otherwise the system may crash!
	; So, in driver we unmap memory by processing IRP_MJ_CLEANUP not IRP_MJ_CLOSE
	; because of IRP_MJ_CLEANUP is processed before CloseHandle exits.
	
	invoke CloseHandle, g_hDevice
	invoke ControlService, g_hService, SERVICE_CONTROL_STOP, addr _ss
	invoke DeleteService, g_hService
	invoke CloseServiceHandle, g_hService
	invoke CloseServiceHandle, g_hSCManager

	mov eax, EXCEPTION_EXECUTE_HANDLER
	ret

MyUnhandledExceptionFilter endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                              UpdateTime                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

UpdateTime proc

local stime:SYSTEMTIME
local buffer[64]:CHAR

	.if g_pSharedMemory != NULL		; It can't be zero but who cares...
		invoke FileTimeToSystemTime, g_pSharedMemory, addr stime
		movzx eax, stime.wHour
		movzx ecx, stime.wMinute
		movzx edx, stime.wSecond

		invoke wsprintf, addr buffer, $CTA0("%02d:%02d:%02d"), eax, ecx, edx

		invoke SetDlgItemText, g_hDlg, IDC_TIME, addr buffer
	.endif

	ret

UpdateTime endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               D I A L O G     P R O C E D U R E                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

	mov eax, uMsg
	.if eax == WM_TIMER

		invoke UpdateTime

	.elseif eax == WM_INITDIALOG

		push hDlg
		pop g_hDlg

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

		invoke SetWindowText, hDlg, $CTA0("Kernel Timer")

		invoke UpdateTime

		invoke SetTimer, hDlg, TIMER_ID, 1000, NULL

	.elseif eax == WM_COMMAND

		mov eax, wParam
		.if ax == IDCANCEL
			invoke EndDialog, hDlg, 0
		.endif

	.elseif eax == WM_DESTROY

		invoke KillTimer, hDlg, TIMER_ID

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

start proc uses esi edi

local acModulePath[MAX_PATH]:CHAR
local _ss:SERVICE_STATUS
local dwBytesReturned:DWORD

	; explicity set for sure

	and g_pSharedMemory, NULL

	; The very first thing we have to do is to install exception handler

	invoke SetUnhandledExceptionFilter, MyUnhandledExceptionFilter

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
	.if eax != NULL
		mov g_hSCManager, eax

		push eax
		invoke GetFullPathName, $CTA0("SharingMemory.sys"), sizeof acModulePath, addr acModulePath, esp
    	pop eax

		invoke CreateService, g_hSCManager, $CTA0("SharingMemory"), $CTA0("Another way how to share memory"), \
			SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
			SERVICE_ERROR_IGNORE, addr acModulePath, NULL, NULL, NULL, NULL, NULL

		.if eax != NULL
			mov g_hService, eax

			invoke StartService, g_hService, 0, NULL
			.if eax != 0

				invoke CreateFile, $CTA0("\\\\.\\SharingMemory"), GENERIC_READ, \
								0, NULL, OPEN_EXISTING, 0, NULL

				.if eax != INVALID_HANDLE_VALUE
					mov g_hDevice, eax

					;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

					invoke DeviceIoControl, g_hDevice, IOCTL_GIVE_ME_YOUR_MEMORY, NULL, 0, \
								addr g_pSharedMemory, sizeof g_pSharedMemory, \
								addr dwBytesReturned, NULL

					.if ( eax != 0 ) && ( dwBytesReturned == sizeof g_pSharedMemory )

						; Here g_pSharedMemory contains the pointer
						; to mapped by the driver memory buffer

						invoke GetModuleHandle, NULL
						mov g_hInstance, eax
						invoke DialogBoxParam, g_hInstance, IDD_MAIN, NULL, addr DlgProc, 0

					.else
						invoke MessageBox, NULL, $CTA0("Can't send control code to device."), \
													NULL, MB_OK + MB_ICONSTOP
					.endif

					;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

					invoke CloseHandle, g_hDevice
				.else
					invoke MessageBox, NULL, $CTA0("Device is not present."), NULL, MB_ICONSTOP
				.endif
				invoke ControlService, g_hService, SERVICE_CONTROL_STOP, addr _ss
			.else
				invoke MessageBox, NULL, $CTA0("Can't start driver."), NULL, MB_OK + MB_ICONSTOP
			.endif
			invoke DeleteService, g_hService
			invoke CloseServiceHandle, g_hService
		.else
			invoke MessageBox, NULL, $CTA0("Can't register driver."), NULL, MB_OK + MB_ICONSTOP
		.endif
		invoke CloseServiceHandle, g_hSCManager
	.else
		invoke MessageBox, NULL, $CTA0("Can't connect to Service Control Manager."), NULL, MB_OK + MB_ICONSTOP
	.endif

	; Restore default handler

	invoke SetUnhandledExceptionFilter, NULL

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start

:make

set exe=SharingMemory

if exist ..\%exe%.exe del ..\%exe%.exe

if exist rsrc.obj goto final
	\masm32\bin\rc /v rsrc.rc
	\masm32\bin\cvtres /machine:ix86 rsrc.res
	if errorlevel 0 goto final
		pause
		exit

:final
if exist rsrc.res del rsrc.res

\masm32\bin\ml /nologo /c /coff %exe%.bat
\masm32\bin\link /nologo /subsystem:windows %exe%.obj rsrc.obj

del %exe%.obj
move %exe%.exe ..
if exist %exe%.exe del %exe%.exe

echo.
pause
