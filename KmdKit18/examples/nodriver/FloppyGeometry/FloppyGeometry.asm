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
include \masm32\include\w2k\mountmgr.inc
include \masm32\include\w2k\ntdddisk.inc

include \masm32\Macros\Strings.mac
include memory.asm

IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER equ CTL_CODE(IOCTL_STORAGE_BASE, 0304h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_DISK_GET_LENGTH_INFO            equ CTL_CODE(IOCTL_DISK_BASE, 0017h, METHOD_BUFFERED, FILE_READ_ACCESS)

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    P R O T O T Y P E S                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

wsprintfW proto C :DWORD, :VARARG
pwsprintfW typedef proto C :DWORD, :VARARG
					
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       E Q U A T E S                                               
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
local cb:DWORD
local dwNumberOfSupportedMediaTypes:DWORD
local pdg:PTR DISK_GEOMETRY

local buffer[2048]:WCHAR
local buffer2[128]:WCHAR
    
	invoke CreateFileW, $CTW0("\\\\.\\A:"), GENERIC_READ, \
			FILE_SHARE_READ + FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL

	.if eax != INVALID_HANDLE_VALUE
		mov hDevice, eax

		mov cb, (sizeof DISK_GEOMETRY) * 2			; start with two
		.while cb < (sizeof DISK_GEOMETRY * 20)
			invoke malloc, cb
			mov pdg, eax
			.if eax != NULL
				invoke DeviceIoControl, hDevice, IOCTL_STORAGE_GET_MEDIA_TYPES, \
								NULL, 0, pdg, cb, addr dwBytesReturned, NULL
				.break .if ( eax != 0 )
				invoke free, pdg
				and pdg, NULL
			.endif
			shl cb, 1								; * 2
		.endw

		.if pdg != NULL

			mov eax, dwBytesReturned
			mov ecx, sizeof DISK_GEOMETRY
			xor edx, edx
			div ecx
			mov dwNumberOfSupportedMediaTypes, eax


			invoke memset, addr buffer, 0, sizeof buffer	; zero for string operations

			invoke DeviceIoControl, hDevice, IOCTL_MOUNTDEV_QUERY_DEVICE_NAME, NULL, 0, \
						addr buffer2, sizeof buffer2, addr dwBytesReturned, NULL
			.if ( eax != 0 )
				invoke lstrcatW, addr buffer, $CTW0("Device name: ")
				lea edx, buffer2
				assume edx:ptr MOUNTDEV_NAME
				movzx ecx, [edx].NameLength
				lea eax, [edx]._Name
				add eax, ecx
				; The name may by not zero terminated
				and word ptr [eax], 0
				invoke lstrcatW, addr buffer, addr [edx]._Name
				assume edx:ptr nothing
				invoke lstrcatW, addr buffer, $CTW0("\n\n")
			.endif

			mov esi, pdg
			assume esi:ptr DISK_GEOMETRY
			.while dwNumberOfSupportedMediaTypes
				.if [esi].MediaType == F3_720_512
					invoke lstrcatW, addr buffer, $CTW0("3.5\=,  720KB,  512 bytes/sector\n")
				.elseif [esi].MediaType == F3_1Pt44_512
					invoke lstrcatW, addr buffer, $CTW0("3.5\=,  1.44MB, 512 bytes/sector\n")
				.elseif [esi].MediaType == F3_2Pt88_512
					invoke lstrcatW, addr buffer, $CTW0("3.5\=,  2.88MB, 512 bytes/sector\n")
				.endif

				; CTW0 macro has limitation of 47 wide character. So we split it.
				CTW "Cylinders:\t\t%d\n", szFormat
				CTW "Tracks Per Cylinder:\t%d\n"
				CTW "Sectors Per Track:\t\t%d\n"
				CTW0 "Bytes Per Sector:\t\t%d\n\n"

				.const
				; $CTA0("wsprintfW") doesn't work because of masm limitation
				sz_wsprintfW	db "wsprintfW", 0
				.code
				invoke GetModuleHandleW, $CTW0("user32.dll")
				invoke GetProcAddress, eax, addr sz_wsprintfW
				.if eax != NULL
					mov ecx, eax

					invoke pwsprintfW ptr ecx, addr buffer2, addr szFormat, \
					[esi].Cylinders.LowPart, [esi].TracksPerCylinder, [esi].SectorsPerTrack, [esi].BytesPerSector

					invoke lstrcatW, addr buffer, addr buffer2

				.endif

				add esi, sizeof DISK_GEOMETRY			; next
				dec dwNumberOfSupportedMediaTypes
			.endw
			assume esi:nothing

			invoke MessageBoxW, NULL, addr buffer, $CTW0("Floppy Drive Geometry"), MB_ICONINFORMATION

			invoke free, pdg			

		.else
			invoke MessageBoxW, NULL, $CTW0("Could't get floppy drive geometry."), NULL, MB_ICONSTOP
		.endif
		invoke CloseHandle, hDevice
	.else
		invoke MessageBoxW, NULL, $CTW0("Couldn't open device."), NULL, MB_ICONSTOP
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start
