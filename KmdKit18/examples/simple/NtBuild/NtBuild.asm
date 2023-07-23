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

include \masm32\include\winioctl.inc

include common.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_hSCManager		HANDLE	?
g_hService			HANDLE	?
g_acDriverPath		CHAR	MAX_PATH dup(?)
g_ss				SERVICE_STATUS <>

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

DisplayNtBuild proc uses ebx

local abyBuffer[DATA_SIZE]:BYTE
local acBuffer[32]:CHAR
local dwNumberOfBytesRead:DWORD
local hDevice:HANDLE

	mov ebx, INVALID_HANDLE_VALUE			; assume driver is not installed

	; Driver will receive IRP_MJ_CREATE
	invoke CreateFile, $CTA0("\\\\.\\NtBuild"), GENERIC_READ, FILE_SHARE_READ, NULL, \
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	.if eax != INVALID_HANDLE_VALUE
		mov hDevice, eax
		xor ebx, ebx						; Driver installed

		; Driver will receive IRP_MJ_READ
		invoke ReadFile, hDevice, addr abyBuffer, DATA_SIZE, addr dwNumberOfBytesRead, NULL

		.if ( eax != 0 ) && ( dwNumberOfBytesRead == DATA_SIZE )
			mov ecx, dword ptr abyBuffer
			mov edx, ecx
			shr ecx, 28
			and edx, 3FFFh
			.if cl == 0Fh
				mov ecx, $CTA0("Free")
			.elseif cl == 0Ch
				mov ecx, $CTA0("Checked")
			.else
				mov ecx, $CTA0("??")
			.endif
			invoke wsprintf, addr acBuffer, $CTA0("%s build %u"), ecx, edx
			invoke MessageBox, NULL, addr acBuffer, $CTA0("Windows Build Info"), MB_OK + MB_ICONINFORMATION
		.else
			invoke MessageBox, NULL, $CTA0("Can't send control code to driver."), NULL, MB_OK + MB_ICONSTOP
		.endif
		; Driver will receive IRP_MJ_CLOSE
		invoke CloseHandle, hDevice
	.endif

	mov eax, ebx
	ret

DisplayNtBuild endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start:

	invoke DisplayNtBuild

	.if eax == INVALID_HANDLE_VALUE
		; Driver is not installed. Let's do it.

		; Open a handle to the SC Manager database
		invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
		.if eax != NULL
			mov g_hSCManager, eax

			push eax
			invoke GetFullPathName, $CTA0("NtBuild.sys"), sizeof g_acDriverPath, addr g_acDriverPath, esp
	    	pop eax

			; Install service
			invoke CreateService, g_hSCManager, $CTA0("NtBuild"), $CTA0("OS Build Fetcher"), \
				SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
				SERVICE_ERROR_IGNORE, addr g_acDriverPath, NULL, NULL, NULL, NULL, NULL
			.if eax != NULL
				mov g_hService, eax

				; Driver's DriverEntry procedure will be called
				invoke StartService, g_hService, 0, NULL
				.if eax != 0
					invoke DisplayNtBuild
					.if eax == INVALID_HANDLE_VALUE
						invoke MessageBox, NULL, $CTA0("Device is not present."), NULL, MB_OK + MB_ICONSTOP
					.endif
					; DriverUnload proc in our driver will be called
					invoke ControlService, g_hService, SERVICE_CONTROL_STOP, addr g_ss
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
	.endif

	invoke ExitProcess, 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start