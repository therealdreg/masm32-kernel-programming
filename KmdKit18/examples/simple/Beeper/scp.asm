;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  scp.asm
;
;  Service Control Program for beeper.sys driver
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

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

start proc

local hSCManager:HANDLE
local hService:HANDLE
local acDriverPath[MAX_PATH]:CHAR

	; Open a handle to the SC Manager database
	invoke OpenSCManager, NULL, NULL, SC_MANAGER_CREATE_SERVICE
	.if eax != NULL
		mov hSCManager, eax

		push eax
		invoke GetFullPathName, $CTA0("beeper.sys"), sizeof acDriverPath, addr acDriverPath, esp
    	pop eax

		; Register driver in SCM active database
		invoke CreateService, hSCManager, $CTA0("beeper"), $CTA0("Nice Melody Beeper"), \
				SERVICE_START + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
				SERVICE_ERROR_IGNORE, addr acDriverPath, NULL, NULL, NULL, NULL, NULL
		.if eax != NULL
			mov hService, eax
			invoke StartService, hService, 0, NULL
			; Here driver beeper.sys plays its nice melody
			; and reports error to be removed from memory
			; Remove driver from SCM database
			invoke DeleteService, hService
			invoke CloseServiceHandle, hService
		.else
			invoke MessageBox, NULL, $CTA0("Can't register driver."), NULL, MB_ICONSTOP
		.endif
		invoke CloseServiceHandle, hSCManager
	.else
		invoke MessageBox, NULL, $CTA0("Can't connect to Service Control Manager."), \
							NULL, MB_ICONSTOP
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start