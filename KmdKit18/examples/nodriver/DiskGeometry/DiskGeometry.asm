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

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib

include \masm32\include\w2k\ntddk.inc
include \masm32\include\w2k\devioctl.inc
include \masm32\include\w2k\ntddstor.inc
include \masm32\include\w2k\ntdddisk.inc

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

start proc uses esi edi

local hDevice:HANDLE
local dwBytesReturned:DWORD
local sdn:STORAGE_DEVICE_NUMBER
local dg:DISK_GEOMETRY
local acBuffer[128]:CHAR

local dcn:DISK_CONTROLLER_NUMBER 
local buffer[128]:BYTE

	invoke CreateFile, $CTA0("\\\\.\\PhysicalDrive0"), GENERIC_READ, \
				FILE_SHARE_READ + FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL

	.if eax != INVALID_HANDLE_VALUE
		mov hDevice, eax

		invoke DeviceIoControl, hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, \
								addr sdn, sizeof sdn, addr dwBytesReturned, NULL
		.if ( eax != 0 ) && ( sdn.DeviceType == FILE_DEVICE_DISK )

			; Let's fetch information about the physical disk's geometry
			
			invoke DeviceIoControl, hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, \
									addr dg, sizeof dg, addr dwBytesReturned, NULL

			.if ( eax != 0 ) && ( dwBytesReturned >= (sizeof DISK_GEOMETRY) )

				mov eax, dg.MediaType
  
				CTA "Number of cylinders:\t%u\n", szGeometry, 4
				CTA "Tracks per cylinder:\t\t%u\n"
				CTA "Sectors per track:\t\t%u\n"
				CTA0 "Bytes per sector:\t\t%u\n"

				invoke wsprintf, addr acBuffer, addr szGeometry, \
						dword ptr dg.Cylinders, dg.TracksPerCylinder, dg.SectorsPerTrack, dg.BytesPerSector
			
				invoke MessageBox, NULL, addr acBuffer, $CTA0("PhysicalDrive0 geometry"), MB_ICONINFORMATION 
			.else
				invoke MessageBox, NULL, $CTA0("Could't get information about the PhysicalDrive0 geometry."), NULL, MB_ICONSTOP
			.endif

		.else
			invoke MessageBox, NULL, $CTA0("Could't get device type."), NULL, MB_ICONSTOP
		.endif
		invoke CloseHandle, hDevice
	.else
		invoke MessageBox, NULL, $CTA0("Couldn't open device."), NULL, MB_ICONSTOP
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start
