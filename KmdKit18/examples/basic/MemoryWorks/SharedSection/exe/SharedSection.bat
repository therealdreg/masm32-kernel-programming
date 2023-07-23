;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  SharedSection
;
;  Client of SharedSection.sys driver
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

include \masm32\include\w2k\native.inc
include \masm32\include\w2k\ntstatus.inc
include \masm32\include\winioctl.inc

include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\advapi32.inc
include \masm32\include\w2k\ntdll.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\w2k\ntdll.lib

include \masm32\Macros\Strings.mac

include ..\common.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    CallDriver                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CallDriver proc

local fOk:BOOL

local hSCManager:HANDLE
local hService:HANDLE
local acModulePath[MAX_PATH]:CHAR
local _ss:SERVICE_STATUS
local hDevice:HANDLE

local abyOutBuffer[4]:BYTE
local dwBytesReturned:DWORD

	and fOk, FALSE				; assume error

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
	.if eax != NULL
		mov hSCManager, eax

		push eax
		invoke GetFullPathName, $CTA0("SharedSection.sys"), sizeof acModulePath, addr acModulePath, esp
    	pop eax

		invoke CreateService, hSCManager, $CTA0("SharedSection"), $CTA0("One way to share section"), \
			SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
			SERVICE_ERROR_IGNORE, addr acModulePath, NULL, NULL, NULL, NULL, NULL

		.if eax != NULL
			mov hService, eax

			invoke StartService, hService, 0, NULL
			.if eax != 0

				invoke CreateFile, $CTA0("\\\\.\\SharedSection"), 0, \
										0, NULL, OPEN_EXISTING, 0, NULL

				.if eax != INVALID_HANDLE_VALUE
					mov hDevice, eax

					;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

					invoke DeviceIoControl, hDevice, IOCTL_SHARE_MY_SECTION, NULL, 0, NULL, 0, \
												addr dwBytesReturned, NULL
					.if eax != 0
						inc fOk					; set success
					.else
						invoke MessageBox, NULL, $CTA0("Can't send control code to device."), NULL, \
													MB_OK + MB_ICONSTOP
					.endif

					;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

					invoke CloseHandle, hDevice
				.else
					invoke MessageBox, NULL, $CTA0("Device is not present."), NULL, MB_ICONSTOP
				.endif
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
		invoke MessageBox, NULL, $CTA0("Can't connect to Service Control Manager."), \
								NULL, MB_OK + MB_ICONSTOP
	.endif

	mov eax, fOk
	ret

CallDriver endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         start                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc

local hSection:HANDLE
local liSectionSize:_LARGE_INTEGER
local oa:OBJECT_ATTRIBUTES
local pSectionBaseAddress:PVOID
local liViewSize:_LARGE_INTEGER

	and liSectionSize.HighPart, 0
	mov liSectionSize.LowPart, SECTION_SIZE

	lea ecx, oa
	InitializeObjectAttributes ecx, offset g_usSectionName, OBJ_CASE_INSENSITIVE, NULL, NULL

	invoke ZwCreateSection, addr hSection, SECTION_MAP_WRITE + SECTION_MAP_READ, addr oa, \
							addr liSectionSize, PAGE_READWRITE, SEC_COMMIT, NULL
	.if eax == STATUS_SUCCESS

		and pSectionBaseAddress, NULL	; The system itself should choose the address
		and liViewSize.HighPart, 0		; Map whole section
		and liViewSize.LowPart, 0
		; NtCurrentProcess equ -1
		invoke ZwMapViewOfSection, hSection, NtCurrentProcess, addr pSectionBaseAddress, 0, SECTION_SIZE, \
									NULL, addr liViewSize, ViewShare, 0, PAGE_READWRITE
		.if eax == STATUS_SUCCESS

			; The reversed string you will able to read if everithing goes fine
			CTA ".revird ecived a dna sessecorp resu neewteb yromem ", g_szStrToReverse
			CTA "erahs ot euqinhcet emas eht esu nac uoy ,revewoH "
			CTA ".sessecorp resu gnoma yromem gnirahs rof desu euqinhcet "
			CTA0 "nommoc a si elif gnigap eht yb dekcab elif deppam-yromem A"

			invoke strcpy, pSectionBaseAddress, addr g_szStrToReverse

			invoke CallDriver
			.if eax == TRUE
				invoke MessageBox, NULL, pSectionBaseAddress, \
								$CTA0("HOWTO: Share Memory Between User Mode and Kernel Mode"), \
								MB_OK + MB_ICONINFORMATION
			.endif

			invoke ZwUnmapViewOfSection, NtCurrentProcess, pSectionBaseAddress
		.else
			invoke MessageBox, NULL, $CTA0("Can't map section."), NULL, MB_OK + MB_ICONSTOP
		.endif

		invoke ZwClose, hSection
	.else
		invoke MessageBox, NULL, $CTA0("Can't create section."), NULL, MB_OK + MB_ICONSTOP
	.endif

	invoke ExitProcess, 0
	ret

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start

:make

set exe=SharedSection

if exist ..\%exe%.exe del ..\%exe%.exe

\masm32\bin\ml /nologo /c /coff %exe%.bat
\masm32\bin\link /nologo /subsystem:windows %exe%.obj

del %exe%.obj
move %exe%.exe ..
if exist %exe%.exe del %exe%.exe

echo.
pause
