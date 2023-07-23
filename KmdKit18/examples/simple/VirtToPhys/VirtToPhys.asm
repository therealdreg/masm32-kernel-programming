;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  VirtToPhys.asm
;
;  VirtToPhys.sys driver's client
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
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      BigNumToString                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BigNumToString proc uNum:UINT, pacBuf:LPSTR

; This function accepts a number and converts it to a
; string, inserting  separators where appropriate.

local acNum[32]:CHAR
local nf:NUMBERFMT

	invoke wsprintf, addr acNum, $CTA0("%u"), uNum

	and nf.NumDigits, 0
	and nf.LeadingZero, FALSE
	mov nf.Grouping, 3
	mov nf.lpDecimalSep, $CTA0(".")
	mov nf.lpThousandSep, $CTA0(" ")
	and nf.NegativeOrder, 0
	invoke GetNumberFormat, LOCALE_USER_DEFAULT, 0, addr acNum, addr nf, pacBuf, 32

	ret

BigNumToString endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       start                                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc uses esi edi

local hSCManager:HANDLE
local hService:HANDLE
local acModulePath[MAX_PATH]:CHAR
local _ss:SERVICE_STATUS
local hDevice:HANDLE

local adwInBuffer[NUM_DATA_ENTRY]:DWORD
local adwOutBuffer[NUM_DATA_ENTRY]:DWORD
local dwBytesReturned:DWORD

local acBuffer[256+64]:CHAR
local acThis[64]:CHAR
local acKernel[64]:CHAR
local acUser[64]:CHAR
local acAdvapi[64]:CHAR

local acNumber[32]:CHAR

	; Open a handle to the SC Manager database
	invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
	.if eax != NULL
		mov hSCManager, eax

		push eax
		invoke GetFullPathName, $CTA0("VirtToPhys.sys"), sizeof acModulePath, addr acModulePath, esp
    	pop eax

		; Install service
		invoke CreateService, hSCManager, $CTA0("VirtToPhys"), $CTA0("Virtual To Physical Address Converter"), \
			SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
			SERVICE_ERROR_IGNORE, addr acModulePath, NULL, NULL, NULL, NULL, NULL

		.if eax != NULL
			mov hService, eax

			; Driver's DriverEntry procedure will be called
			invoke StartService, hService, 0, NULL
			.if eax != 0


				; Driver will receive I/O request packet (IRP) of type IRP_MJ_CREATE
				invoke CreateFile, $CTA0("\\\\.\\slVirtToPhys"), GENERIC_READ + GENERIC_WRITE, \
								0, NULL, OPEN_EXISTING, 0, NULL

				.if eax != INVALID_HANDLE_VALUE
					mov hDevice, eax

					lea esi, adwInBuffer
					assume esi:ptr DWORD
					invoke GetModuleHandle, NULL
					mov [esi][0*(sizeof DWORD)], eax
					invoke GetModuleHandle, $CTA0("kernel32.dll", szKernel32)
					mov [esi][1*(sizeof DWORD)], eax
					invoke GetModuleHandle, $CTA0("user32.dll", szUser32)
					mov [esi][2*(sizeof DWORD)], eax
					invoke GetModuleHandle, $CTA0("advapi32.dll", szAdvapi32)
					mov [esi][3*(sizeof DWORD)], eax

					lea edi, adwOutBuffer
					assume edi:ptr DWORD

					; Driver will receive IRP of type IRP_MJ_DEVICE_CONTROL
					invoke DeviceIoControl, hDevice, IOCTL_GET_PHYS_ADDRESS, esi, sizeof adwInBuffer, \
										edi, sizeof adwOutBuffer, addr dwBytesReturned, NULL

					.if ( eax != 0 ) && ( dwBytesReturned != 0 )

						invoke GetModuleFileName, [esi][0*(sizeof DWORD)], addr acModulePath, sizeof acModulePath

						lea ecx, acModulePath[eax-5]
					    .repeat
							dec ecx
							mov al, [ecx]
					    .until al == '\'
						inc ecx
						push ecx

						CTA0 "%s \t%08Xh\t%08Xh   ( %s )\n", szFmtMod

						invoke BigNumToString, [edi][0*(sizeof DWORD)], addr acNumber
						pop ecx
						invoke wsprintf, addr acThis,	addr szFmtMod, ecx,				[esi][0*(sizeof DWORD)], [edi][0*(sizeof DWORD)], addr acNumber

						invoke BigNumToString, [edi][1*(sizeof DWORD)], addr acNumber
						invoke wsprintf, addr acKernel,	addr szFmtMod, addr szKernel32,	[esi][1*(sizeof DWORD)], [edi][1*(sizeof DWORD)], addr acNumber

						invoke BigNumToString, [edi][2*(sizeof DWORD)], addr acNumber
						invoke wsprintf, addr acUser,	addr szFmtMod, addr szUser32,	[esi][2*(sizeof DWORD)], [edi][2*(sizeof DWORD)], addr acNumber

						invoke BigNumToString, [edi][3*(sizeof DWORD)], addr acNumber
						invoke wsprintf, addr acAdvapi,	addr szFmtMod, addr szAdvapi32,	[esi][3*(sizeof DWORD)], [edi][3*(sizeof DWORD)], addr acNumber

						invoke wsprintf, addr acBuffer, $CTA0("Module:\t\tVirtual:\t\tPhysical:\n\n%s\n%s%s%s"), \
											addr acThis, addr acKernel, addr acUser, addr acAdvapi

						assume esi:nothing
						assume edi:nothing
						invoke MessageBox, NULL, addr acBuffer, $CTA0("Modules Base Address"), MB_OK + MB_ICONINFORMATION
					.else
						invoke MessageBox, NULL, $CTA0("Can't send control code to device."), NULL, MB_OK + MB_ICONSTOP
					.endif
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