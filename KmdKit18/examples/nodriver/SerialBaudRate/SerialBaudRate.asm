;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
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

include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\w2k\ntdll.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\w2k\ntdll.lib

include \masm32\include\w2k\ntddk.inc
include \masm32\include\w2k\ntstatus.inc

include \masm32\include\w2k\ntddser.inc

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; windows.inc can't be included because of ntddk.inc

OPEN_EXISTING			equ 3
MB_ICONHAND				equ 10h
MB_ICONSTOP				equ MB_ICONHAND
MB_ICONINFORMATION		equ 40h
INVALID_HANDLE_VALUE	equ -1

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       start                                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc

local status:NTSTATUS
local oa:OBJECT_ATTRIBUTES
local us:UNICODE_STRING
local iosb:IO_STATUS_BLOCK

local hDevice:HANDLE
local sbr:SERIAL_BAUD_RATE
local dwBytesReturned:DWORD

local buffer[128]:BYTE

	CCOUNTED_UNICODE_STRING	"\\Device\\Serial0", g_usSerialDeviceName, 4

	InitializeObjectAttributes addr oa, addr g_usSerialDeviceName, OBJ_CASE_INSENSITIVE, NULL, NULL

	invoke ZwOpenFile, addr hDevice, FILE_READ_ACCESS, addr oa, addr iosb, \
				FILE_SHARE_READ + FILE_SHARE_WRITE + FILE_SHARE_DELETE, 0
	.if eax == STATUS_SUCCESS

		invoke DeviceIoControl, hDevice, IOCTL_SERIAL_GET_BAUD_RATE, NULL, 0, \
							addr sbr, sizeof sbr, addr dwBytesReturned, NULL
		.if eax != 0
			invoke wsprintf, addr buffer, $CTA0("Current Baud Rate: %d"), sbr.BaudRate
			invoke MessageBox, NULL, addr buffer, $CTA0("\\Device\\Serial0 Info"), MB_ICONINFORMATION
		.endif

		invoke ZwClose, hDevice

	.else
		invoke MessageBox, NULL, $CTA0("Couldn't open serial device."), NULL, MB_ICONSTOP
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start
