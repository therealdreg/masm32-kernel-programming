;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  DateTime.asm
;
;  Service Control Program for giveio.sys driver
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
;                           U S E R   D E F I N E D   M A C R O S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CMOS MACRO by:REQ
	mov al, by
	out 70h, al
	in al, 71h

	mov ah, al
	shr al, 4
	add al, '0'

	and ah, 0Fh
	add ah, '0'
	stosw
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        DateTime                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DateTime proc uses edi

local acDate[16]:CHAR
local acTime[16]:CHAR
local acOut[64]:CHAR

	; See Ralf Brown's Interrupt List for details

	; RTC - STATUS REGISTER B
	mov al, 0Bh
	out 70h, al
	in al, 71h


	push eax			; save old format
	and al, 11111011y	; Bit 2: Data Mode - 0: BCD, 1: Binary
	or al, 010y			; Bit 1: 24/12 hour selection - 1 enables 24 hour mode
	out 71h, al


	; *** Lets' fetch current date ***
	lea edi, acDate

	; RTC - DATE OF MONTH
	CMOS 07h
	mov al, '.'
	stosb

	; RTC - MONTH
	CMOS 08h
	mov al, '.'
	stosb

	; IBM - CENTURY BYTE (BCD value for the century - currently 19h)
	CMOS 32h
	; RTC - YEAR
	CMOS 09h

	xor eax, eax	; terminate with zero
	stosb


	; *** Lets' fetch current time ***
	lea edi, acTime

	; RTC - HOURS
	CMOS 04h
	mov al, ':'
	stosb

	; RTC - MINUTES
	CMOS 02h
	mov al, ':'
	stosb

	; RTC - SECONDS
	CMOS 0h

	xor eax, eax	; terminate with zero
	stosb


	; restore old format
	mov al, 0Bh
	out 70h, al
	pop eax
	out 71h, al

	invoke wsprintf, addr acOut, $CTA0("Date:\t%s\nTime:\t%s"), addr acDate, addr acTime
	invoke MessageBox, NULL, addr acOut, $CTA0("Current Date and Time"), MB_OK

	ret

DateTime endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         start                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc

local fOK:BOOL
local hSCManager:HANDLE
local hService:HANDLE
local acDriverPath[MAX_PATH]:CHAR

local hKey:HANDLE
local dwProcessId:DWORD

	and fOK, 0		; assume an error

	; Open a handle to the SC Manager database
	invoke OpenSCManager, NULL, NULL, SC_MANAGER_CREATE_SERVICE
	.if eax != NULL
		mov hSCManager, eax

		push eax
		invoke GetFullPathName, $CTA0("giveio.sys"), sizeof acDriverPath, addr acDriverPath, esp
    	pop eax

		; Register driver in SCM active database
		invoke CreateService, hSCManager, $CTA0("giveio"), $CTA0("Current Date and Time fetcher."), \
				SERVICE_START + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
				SERVICE_ERROR_IGNORE, addr acDriverPath, NULL, NULL, NULL, NULL, NULL
		.if eax != NULL
			mov hService, eax

			invoke RegOpenKeyEx, HKEY_LOCAL_MACHINE, \
									$CTA0("SYSTEM\\CurrentControlSet\\Services\\giveio"), \
									0, KEY_CREATE_SUB_KEY + KEY_SET_VALUE, addr hKey

			.if eax == ERROR_SUCCESS
				invoke GetCurrentProcessId
				mov dwProcessId, eax
				invoke RegSetValueEx, hKey, $CTA0("ProcessId", szProcessId), NULL, REG_DWORD, \
										addr dwProcessId, sizeof DWORD
				.if eax == ERROR_SUCCESS				
					invoke StartService, hService, 0, NULL
					inc fOK				; set flag
					invoke RegDeleteValue, hKey, addr szProcessId
				.else
					invoke MessageBox, NULL, $CTA0("Can't add Process ID into registry."), \
										NULL, MB_ICONSTOP
				.endif
				
				invoke RegCloseKey, hKey

			.else
				invoke MessageBox, NULL, $CTA0("Can't open registry."), NULL, MB_ICONSTOP
			.endif

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

	.if fOK
		; Display current date and time to user
		invoke DateTime
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start