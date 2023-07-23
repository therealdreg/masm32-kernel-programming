;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  skeleton.asm
;
;  Service Control Program for WhichIrqlAndContext driver
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

include common.inc

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

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       start                                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc uses esi edi

local hSCManager:HANDLE
local hService:HANDLE
local acModulePath[MAX_PATH]:CHAR
local _ss:SERVICE_STATUS
local hDevice:HANDLE

local achBuffer[128]:CHAR
local dwBytesReturned:DWORD

	; Open a handle to the SC Manager database
	invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
	.if eax != NULL
		mov hSCManager, eax

		;invoke GetCurrentDirectory, sizeof g_acBuffer, addr g_acBuffer
		push eax
		invoke GetFullPathName, $CTA0("WhichIrqlAndContext.sys"), sizeof acModulePath, addr acModulePath, esp
    	pop eax

		; Install service
		invoke CreateService, hSCManager, $CTA0("WhichIrqlAndContext"), $CTA0("WhichIrqlAndContext Driver"), \
			SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
			SERVICE_ERROR_IGNORE, addr acModulePath, NULL, NULL, NULL, NULL, NULL

		.if eax != NULL
			mov hService, eax

			; Driver's DriverEntry procedure will be called
			invoke StartService, hService, 0, NULL
			.if eax != 0

				; Driver will receive I/O request packet (IRP) of type IRP_MJ_CREATE
				invoke CreateFile, $CTA0("\\\\.\\WhichIrqlAndContext"), GENERIC_READ + GENERIC_WRITE, \
								0, NULL, OPEN_EXISTING, 0, NULL

				.if eax != INVALID_HANDLE_VALUE
					mov hDevice, eax

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

					; Driver will receive IRP of type IRP_MJ_DEVICE_CONTROL
					invoke DeviceIoControl, hDevice, IOCTL_DUMMY, NULL, 0, \
										NULL, 0, addr dwBytesReturned, NULL

					.if ( eax != 0 )

						invoke GetCurrentThreadId
						push eax
						invoke GetCurrentProcessId
						push eax
						push $CTA0("PID=%08X, TID=%08X")
						lea eax, achBuffer
						push eax
						call wsprintf
						add esp, 4 * sizeof DWORD

						invoke MessageBox, NULL, addr achBuffer, $CTA0("Driver started"), MB_OK + MB_ICONINFORMATION
					.else
						invoke MessageBox, NULL, $CTA0("Can't send control code to device."), NULL, MB_OK + MB_ICONSTOP
					.endif

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

					; Driver will receive IRP of type IRP_MJ_CLOSE
					invoke CloseHandle, hDevice
				.else
					invoke MessageBox, NULL, $CTA0("Device is not present."), NULL, MB_ICONSTOP
				.endif
				; DriverUnload proc in our driver will be called
				invoke ControlService, hService, SERVICE_CONTROL_STOP, addr _ss
			.else
				invoke MessageBox, NULL, $CTA0("Can't start driver."), NULL, MB_OK + MB_ICONSTOP
			.endif
			invoke DeleteService, hService
			invoke CloseServiceHandle, hService
		.else
			invoke MessageBox, NULL, $CTA0("Can't register driver."), NULL, MB_OK + MB_ICONSTOP
		.endif
		invoke CloseServiceHandle, hSCManager
	.else
		invoke MessageBox, NULL, $CTA0("Can't connect to Service Control Manager."), NULL, MB_OK + MB_ICONSTOP
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start
