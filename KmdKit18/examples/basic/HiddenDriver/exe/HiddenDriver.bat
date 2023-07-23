;@echo off
;goto make

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

includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\advapi32.lib

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       M A C R O S                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

$invoke MACRO vars:VARARG
     invoke vars
     EXITM <eax>
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        E Q U A T E S                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IDR_DRIVER_SYS			equ 1000

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_hInstance			HINSTANCE	?
g_szDriverFilePath	db MAX_PATH dup(?)

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    FlushDriverToDisk                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FlushDriverToDisk proc

local acBuffer[MAX_PATH]:CHAR
local hResInfo:HRSRC
local hFile:HANDLE
local cb:UINT
local pRes:LPVOID
local dwNumberOfBytesWritten:DWORD
local fOk:BOOL

	and fOk, FALSE					; assume error

	invoke FindResource, NULL, IDR_DRIVER_SYS, $CTA0("sys")
	.if eax != NULL
		mov hResInfo, eax

		invoke LoadResource, NULL, hResInfo
		.if eax != NULL
			mov pRes, eax

			invoke SizeofResource, NULL, hResInfo
			mov cb, eax

			invoke LockResource, hResInfo

			invoke GetSystemDirectory, addr acBuffer, sizeof acBuffer
			invoke wsprintf, addr g_szDriverFilePath, $CTA0("%s\\Drivers\\%s"), addr acBuffer, $CTA0("HiddenDriver.sys")


			invoke CreateFile, addr g_szDriverFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
			.if eax != INVALID_HANDLE_VALUE
				mov hFile, eax
	
				invoke WriteFile, hFile, pRes, cb, addr dwNumberOfBytesWritten, NULL
				.if eax != 0
					inc fOk				; set TRUE
				.else
					invoke MessageBox, NULL, $CTA0("Could not write to file."), NULL, MB_OK + MB_ICONSTOP
				.endif
				invoke CloseHandle, hFile
			.else
				invoke MessageBox, NULL, $CTA0("Could not create driver file."), NULL, MB_OK + MB_ICONSTOP
			.endif
		.else
			invoke MessageBox, NULL, $CTA0("Could not load driver from the resources."), NULL, MB_OK + MB_ICONSTOP
		.endif
	.else
		invoke MessageBox, NULL, $CTA0("Could not locate driver in the resources."), NULL, MB_OK + MB_ICONSTOP
	.endif

	mov eax, fOk
	ret

FlushDriverToDisk endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        WipeDriverFromDisk                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

WipeDriverFromDisk proc
	invoke DeleteFile, addr g_szDriverFilePath
	ret
WipeDriverFromDisk endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      RegisterAndStartDriver                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RegisterAndStartDriver proc

local hSCManager:HANDLE
local hService:HANDLE
local fOk:BOOL

	and fOk, FALSE					; assume error

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
	.if eax != NULL
		mov hSCManager, eax

		invoke CreateService, hSCManager, $CTA0("HiddenDriver"), $CTA0("Hidden Driver"), \
			SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
			SERVICE_ERROR_IGNORE, addr g_szDriverFilePath, NULL, NULL, NULL, NULL, NULL

		.if eax != NULL
			mov hService, eax

			invoke StartService, hService, 0, NULL
			.if eax != 0
				inc fOk
			.else
				invoke MessageBox, NULL, $CTA0("Could not start driver."), NULL, MB_ICONSTOP
			.endif
			
			; mark it for deletion.
			
			invoke DeleteService, hService
			invoke CloseServiceHandle, hService
		.else
			invoke MessageBox, NULL, $CTA0("Could not register driver."), NULL, MB_ICONSTOP
		.endif
		invoke CloseServiceHandle, hSCManager
	.else
		invoke MessageBox, NULL, $CTA0("Could not connect to Service Control Manager."), NULL, MB_ICONSTOP
	.endif

	mov eax, fOk
	ret

RegisterAndStartDriver endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     StopAndUnregisterDriver                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

StopAndUnregisterDriver proc

local hSCManager:HANDLE
local hService:HANDLE
local _ss:SERVICE_STATUS

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
	.if eax != NULL
		mov hSCManager, eax

		invoke OpenService, hSCManager, $CTA0("HiddenDriver"), SERVICE_STOP	

		.if eax != NULL
			mov hService, eax

			invoke ControlService, hService, SERVICE_CONTROL_STOP, addr _ss
			.if eax == 0
				invoke MessageBox, NULL, $CTA0("Could not stop driver."), NULL, MB_ICONSTOP			
			.endif
			
			invoke CloseServiceHandle, hService
		.else
			invoke MessageBox, NULL, $CTA0("Could not open driver service."), NULL, MB_ICONSTOP
		.endif
		invoke CloseServiceHandle, hSCManager
	.else
		invoke MessageBox, NULL, $CTA0("Could not connect to Service Control Manager."), NULL, MB_ICONSTOP
	.endif

	ret

StopAndUnregisterDriver endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc

local hDevice:HANDLE

	mov g_hInstance, $invoke(GetModuleHandle, NULL)

	; Try to open device first. May be it loaded

	invoke CreateFile, $CTA0("\\\\.\\HiddenDriver"), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL

	mov hDevice, eax
	.if eax == INVALID_HANDLE_VALUE
	
		; driver is not loaded yet
	
		invoke FlushDriverToDisk
		.if eax == TRUE
			invoke RegisterAndStartDriver
			push eax

			; Anyway we don't need it anymore

			invoke WipeDriverFromDisk

			pop eax
			.if eax

				invoke CreateFile, $CTA0("\\\\.\\HiddenDriver"), GENERIC_READ, 0, NULL, \
											OPEN_EXISTING, 0, NULL
				.if eax != INVALID_HANDLE_VALUE
				
					; OK. Devise is here.
					
					mov hDevice, eax

					invoke MessageBox, NULL, $CTA0("Mmm... Where is this f.... driver's file?"), \
												$CTA0("Hidden Driver"), MB_ICONQUESTION	

					invoke CloseHandle, hDevice
					mov hDevice, INVALID_HANDLE_VALUE
				.else
					invoke MessageBox, NULL, $CTA0("Could not get device handle."), NULL, MB_ICONSTOP
				.endif

				invoke StopAndUnregisterDriver

			.endif
		.endif
	.endif

	.if hDevice != INVALID_HANDLE_VALUE
		invoke CloseHandle, hDevice
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:make

set exe=HiddenDriver

if exist ..\%exe%.exe del ..\%exe%.exe

if exist rsrc.obj del rsrc.obj
	\masm32\bin\rc /v rsrc.rc
	\masm32\bin\cvtres /machine:ix86 rsrc.res
	if errorlevel 0 goto final
		pause
		exit

:final
if exist rsrc.res del rsrc.res

\masm32\bin\ml /nologo /c /coff %exe%.bat
\masm32\bin\link /nologo /subsystem:windows %exe%.obj rsrc.obj

rem Driver was packed into exe. Delete it.
if exist ..\%exe%.sys del ..\%exe%.sys

del %exe%.obj
move %exe%.exe ..

echo.
pause
