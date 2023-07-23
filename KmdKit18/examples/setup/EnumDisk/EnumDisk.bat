;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  EnumDisk - Enumerates all available disk devices and gets the device property.
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

include \masm32\include\w2k\ntddk.inc
include \masm32\include\w2k\ntddstor.inc
include \masm32\include\w2k\guiddef.inc

includelib setupapi.lib

include \masm32\Macros\Strings.mac

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
MB_OK					equ 0h
MB_ICONHAND				equ 10h
MB_ICONERROR			equ MB_ICONHAND
MB_ICONSTOP				equ MB_ICONHAND
MB_ICONINFORMATION		equ 40h
INVALID_HANDLE_VALUE	equ -1

COORD STRUCT
  x  WORD      ?
  y  WORD      ?
COORD ENDS

SMALL_RECT STRUCT
  Left      WORD      ?
  Top       WORD      ?
  Right     WORD      ?
  Bottom    WORD      ?
SMALL_RECT ENDS

CONSOLE_SCREEN_BUFFER_INFO STRUCT
  dwSize                COORD <>
  dwCursorPosition      COORD <>
  wAttributes           WORD      ?
  srWindow              SMALL_RECT <>
  dwMaximumWindowSize   COORD <>
CONSOLE_SCREEN_BUFFER_INFO ENDS

STD_INPUT_HANDLE                     equ -10
STD_OUTPUT_HANDLE                    equ -11
STD_ERROR_HANDLE                     equ -12

FOREGROUND_BLUE                      equ 1h
FOREGROUND_GREEN                     equ 2h
FOREGROUND_RED                       equ 4h
FOREGROUND_INTENSITY                 equ 8h

ERROR_INVALID_DATA                   equ 13
ERROR_INSUFFICIENT_BUFFER            equ 122
ERROR_NO_MORE_ITEMS                  equ 259

; Definitiond from \setupapi.h

HDEVINFO	typedef	DWORD

SP_DEVINFO_DATA STRUCT
	cbSize		DWORD	?
	ClassGuid	GUID	<>
	DevInst		DWORD	?	; DEVINST handle
	Reserved	DWORD	?	; ULONG_PTR
SP_DEVINFO_DATA ENDS
PSP_DEVINFO_DATA typedef ptr SP_DEVINFO_DATA

SP_DEVICE_INTERFACE_DATA STRUCT
	cbSize				DWORD	?
	InterfaceClassGuid	GUID	<>
	Flags				DWORD	?
	Reserved			DWORD	?	; ULONG_PTR
SP_DEVICE_INTERFACE_DATA ENDS
PSP_DEVICE_INTERFACE_DATA typedef ptr SP_DEVICE_INTERFACE_DATA

SP_DEVICE_INTERFACE_DETAIL_DATA_A  STRUCT	; sizeof = 5
	cbSize		DWORD	?
	DevicePath	db 1 dup(?)		; CHAR [ANYSIZE_ARRAY]
SP_DEVICE_INTERFACE_DETAIL_DATA_A ENDS
PSP_DEVICE_INTERFACE_DETAIL_DATA_A typedef ptr SP_DEVICE_INTERFACE_DETAIL_DATA_A

SP_DEVICE_INTERFACE_DETAIL_DATA_W  STRUCT	; sizeof = 5
	cbSize		DWORD	?
	DevicePath	WORD 1 dup(?)	; WCHAR [ANYSIZE_ARRAY]
SP_DEVICE_INTERFACE_DETAIL_DATA_W ENDS
PSP_DEVICE_INTERFACE_DETAIL_DATA_W typedef ptr SP_DEVICE_INTERFACE_DETAIL_DATA_W
	
IFDEF UNICODE
	SP_DEVICE_INTERFACE_DETAIL_DATA		equ <SP_DEVICE_INTERFACE_DETAIL_DATA_W>
	PSP_DEVICE_INTERFACE_DETAIL_DATA	equ	<PSP_DEVICE_INTERFACE_DETAIL_DATA_W>
ELSE
	SP_DEVICE_INTERFACE_DETAIL_DATA		equ <SP_DEVICE_INTERFACE_DETAIL_DATA_A>
	PSP_DEVICE_INTERFACE_DETAIL_DATA	equ	<PSP_DEVICE_INTERFACE_DETAIL_DATA_A>
ENDIF

SPDRP_HARDWAREID		equ 00000001h	; HardwareID (R/W)

DIGCF_DEFAULT           equ 00000001  ; only valid with DIGCF_DEVICEINTERFACE
DIGCF_PRESENT           equ 00000002
DIGCF_ALLCLASSES        equ 00000004
DIGCF_PROFILE           equ 00000008
DIGCF_DEVICEINTERFACE   equ 00000010h

SetupDiDestroyDeviceInfoList proto :DWORD
SetupDiEnumDeviceInfo proto :DWORD,:DWORD,:DWORD
SetupDiEnumDeviceInterfaces proto :DWORD,:DWORD,:DWORD,:DWORD,:DWORD

SetupDiGetDeviceRegistryPropertyA proto :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
SetupDiGetDeviceRegistryPropertyW proto :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
IFDEF UNICODE
	SetupDiGetDeviceRegistryProperty equ <SetupDiGetDeviceRegistryPropertyW>
ELSE
	SetupDiGetDeviceRegistryProperty equ <SetupDiGetDeviceRegistryPropertyA>
ENDIF

SetupDiGetDeviceInterfaceDetailA proto :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
SetupDiGetDeviceInterfaceDetailW proto :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
IFDEF UNICODE
	SetupDiGetDeviceInterfaceDetail equ <SetupDiGetDeviceInterfaceDetailW>
ELSE
	SetupDiGetDeviceInterfaceDetail	equ <SetupDiGetDeviceInterfaceDetailA>
ENDIF

SetupDiGetClassDevsA proto :DWORD,:DWORD,:DWORD,:DWORD
SetupDiGetClassDevsW proto :DWORD,:DWORD,:DWORD,:DWORD
IFDEF UNICODE
	SetupDiGetClassDevs equ <SetupDiGetClassDevsW>
ELSE
	SetupDiGetClassDevs equ <SetupDiGetClassDevsA>
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              I N I T I A L I Z E D  D A T A                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

POINTERS SEGMENT READONLY PUBLIC USE32 'CONST'

; Bus Type

g_apszBusType label LPSTR
LPSTR	$CTA0("UNKNOWN")
LPSTR	$CTA0("SCSI")
LPSTR	$CTA0("ATAPI")
LPSTR	$CTA0("ATA")
LPSTR	$CTA0("IEEE 1394")
LPSTR	$CTA0("SSA")
LPSTR	$CTA0("FIBRE")
LPSTR	$CTA0("USB")
LPSTR	$CTA0("RAID")
g_cbBusType	equ $-g_apszBusType

; SCSI Device Type

g_apszDeviceType label LPSTR
LPSTR	$CTA0("Direct Access Device")
LPSTR	$CTA0("Tape Device")
LPSTR	$CTA0("Printer Device")
LPSTR	$CTA0("Processor Device")
LPSTR	$CTA0("WORM Device")
LPSTR	$CTA0("CDROM Device")
LPSTR	$CTA0("Scanner Device")
LPSTR	$CTA0("Optical Disk")
LPSTR	$CTA0("Media Changer")
LPSTR	$CTA0("Comm. Device")
LPSTR	$CTA0("ASCIT8")
LPSTR	$CTA0("ASCIT8")
LPSTR	$CTA0("Array Device")
LPSTR	$CTA0("Enclosure Device")
LPSTR	$CTA0("RBC Device")
LPSTR	$CTA0("Unknown Device")
g_cbDeviceType	equ $-g_apszDeviceType

POINTERS ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_hConsoleInput		HANDLE ?
g_hConsoleOutput	HANDLE	?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   R E A D O N L Y    D A T A                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

DEFINE_GUID GUID_DEVCLASS_DISKDRIVE, 04d36e967h, 0e325h, 011ceh, 0bfh, 0c1h, 008h, 000h, 02bh, 0e1h, 003h, 018h
;GUID_DEVCLASS_DISKDRIVE GUID {04d36e967h, 0e325h, 011ceh, {0bfh, 0c1h, 008h, 000h, 02bh, 0e1h, 003h, 018h}}

DEFINE_GUID GUID_DEVINTERFACE_DISK, 053f56307h, 0b6bfh, 011d0h, 094h, 0f2h, 000h, 0a0h, 0c9h, 01eh, 0fbh, 08bh
;GUID_DEVINTERFACE_DISK	GUID {053f56307h, 0b6bfh, 011d0h, {094h, 0f2h, 000h, 0a0h, 0c9h, 01eh, 0fbh, 08bh}}

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                            malloc                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

malloc proc dwBytes:DWORD
; allocates dwBytes from current process's heap
; and returns pointer to allocated memory block.
; HeapAlloc(GetProcessHeap(), 0, dwBytes)

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke GetProcessHeap
	invoke HeapAlloc, eax, 0, [esp+4]
	ret (sizeof DWORD)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

malloc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                           free                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

free proc lpMem:PVOID
; frees memory block allocated from current process's heap
; HeapFree(GetProcessHead(), 0, lpMem)

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke GetProcessHeap
	invoke HeapFree, eax, 0, [esp+4]
	ret (sizeof PVOID)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

free endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       PrintConsole                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintConsole proc psz:LPSTR, dwAttributes:DWORD

local csbi:CONSOLE_SCREEN_BUFFER_INFO
local dwNumberOfBytesWritten:DWORD

	.if dwAttributes != 0
		invoke GetConsoleScreenBufferInfo, g_hConsoleOutput, addr csbi
		invoke SetConsoleTextAttribute, g_hConsoleOutput, dwAttributes
	.endif

	.if psz != NULL
		invoke lstrlen, psz
		.if eax
			mov ecx, eax
			invoke WriteFile, g_hConsoleOutput, psz, ecx, addr dwNumberOfBytesWritten, NULL
		.endif
	.endif

	.if dwAttributes != 0
		movzx eax, csbi.wAttributes
		invoke SetConsoleTextAttribute, g_hConsoleOutput, eax
	.endif

	ret

PrintConsole endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    GetRegistryProperty                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetRegistryProperty proc hDevInfo:HDEVINFO, dwIndex:DWORD

comment ^
Routine Description:
    This routine enumerates the disk devices using the Setup class interface
    GUID GUID_DEVCLASS_DISKDRIVE. Gets the Device ID from the Registry 
    property.

Arguments:
    hDevInfo	- Handles to the device information list
    dwIndex		- Device member 

Return Value:
	TRUE or FALSE. This decides whether to continue or not
^

local DevInfoData:SP_DEVINFO_DATA
local cb:DWORD
local dwDataType:DWORD
local pstr:LPSTR
local buffer[128]:CHAR

	mov DevInfoData.cbSize, sizeof SP_DEVINFO_DATA

	invoke SetupDiEnumDeviceInfo, hDevInfo, dwIndex, addr DevInfoData

	.if eax == FALSE

		invoke GetLastError

		.if eax == ERROR_NO_MORE_ITEMS
			invoke PrintConsole, $CTA0("No more devices.\n\n"), 0
		.else
			invoke wsprintf, addr buffer, $CTA0("SetupDiEnumDeviceInfo failed with error: %d\n"), eax
			invoke PrintConsole, addr buffer, 0
		.endif
		
		jmp ExitWithFalse
  
	.endif
        
	; We won't know the size of the HardwareID buffer until we call
	; this function. So call it with a null to begin with, and then 
	; use the required buffer size to Alloc the necessary space.
	; Keep calling we have success or an unknown failure.

	and cb, 0

	invoke SetupDiGetDeviceRegistryProperty, hDevInfo, addr DevInfoData, \
						SPDRP_HARDWAREID, addr dwDataType, pstr, cb, addr cb

	.if eax == FALSE

		invoke GetLastError

		.if eax != ERROR_INSUFFICIENT_BUFFER

			.if eax == ERROR_INVALID_DATA
 
				; May be a Legacy Device with no HardwareID. Continue.

				jmp ExitWithTrue

			.else
				invoke wsprintf, addr buffer, $CTA0("SetupDiGetDeviceInterfaceDetail failed with error: %d\n"), eax
				invoke PrintConsole, addr buffer, 0

				jmp ExitWithFalse
			.endif
		.endif
	.endif

	; We need to change the buffer size.

	invoke malloc, cb
	.if eax != NULL

		mov pstr, eax

		invoke SetupDiGetDeviceRegistryProperty, hDevInfo, addr DevInfoData, \
						SPDRP_HARDWAREID, addr dwDataType, pstr, cb, addr cb

		.if eax == FALSE
		
			invoke GetLastError
			
			.if eax == ERROR_INVALID_DATA

				; May be a Legacy Device with no HardwareID. Continue.

				xor eax, eax
				inc eax

			.else
				invoke wsprintf, addr buffer, $CTA0("SetupDiGetDeviceInterfaceDetail failed with error: %d\n"), eax
				invoke PrintConsole, addr buffer, 0

				xor eax, eax
			.endif
			
		.endif

		.if eax
			invoke wsprintf, addr buffer, $CTA0("\n\nDevice ID: %s\n"), pstr
			invoke PrintConsole, addr buffer, 0
		.endif
    
		invoke free, pstr

	.endif

ExitWithTrue:
	xor eax, eax
	inc eax
	ret					; return TRUE

ExitWithFalse:
	xor eax, eax
	ret					; return FALSE
	
GetRegistryProperty endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     GetDeviceProperty                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetDeviceProperty proc uses esi edi ebx hIntDevInfo:HDEVINFO, dwIndex:DWORD

comment ^
Routine Description:
    This routine enumerates the disk devices using the Device interface
    GUID DiskClassGuid. Gets the Adapter & Device property from the port
    driver. Then sends IOCTL through SPTI to get the device Inquiry data.

Arguments:
    hIntDevInfo	- Handles to the interface device information list
    dwIndex		- Device member 

Return Value:
	TRUE or FALSE. This decides whether to continue or not
^

local InterfaceData:SP_DEVICE_INTERFACE_DATA
local pInterfaceDetailData:PSP_DEVICE_INTERFACE_DETAIL_DATA
local StoragePropertyQuery:STORAGE_PROPERTY_QUERY
local adpDesc:PSTORAGE_ADAPTER_DESCRIPTOR
local devDesc:PSTORAGE_DEVICE_DESCRIPTOR
local hDevice:HANDLE
local cbReturned:DWORD
local cbInterfaceDetail:DWORD
local cb:DWORD
local buffer[128]:CHAR
local DataBuf[1024]:BYTE

	mov InterfaceData.cbSize, sizeof SP_DEVICE_INTERFACE_DATA

	invoke SetupDiEnumDeviceInterfaces, hIntDevInfo, 0, \
					addr GUID_DEVINTERFACE_DISK, dwIndex, addr InterfaceData

	.if eax == FALSE

		invoke GetLastError

		.if eax == ERROR_NO_MORE_ITEMS
			invoke PrintConsole, $CTA0("No more interfaces.\n\n"), 0
		.else
			invoke wsprintf, addr buffer, \
					$CTA0("SetupDiEnumDeviceInterfaces failed with error: %d\n"), eax
			invoke PrintConsole, addr buffer, 0
		.endif

		jmp ExitWithFalse
  
	.endif

	; Find out required buffer size, so pass NULL 

	and cb, 0

	invoke SetupDiGetDeviceInterfaceDetail, hIntDevInfo, \
							addr InterfaceData, NULL, 0, addr cb, NULL

	; This call returns ERROR_INSUFFICIENT_BUFFER with cb 
	; set to the required buffer size. Ignore the above error and
	; pass a bigger buffer to get the detail data

	.if eax == FALSE

		invoke GetLastError

		.if eax != ERROR_INSUFFICIENT_BUFFER

			invoke wsprintf, addr buffer, $CTA0("SetupDiGetDeviceInterfaceDetail failed with error: %d\n"), eax
			invoke PrintConsole, addr buffer, 0

			jmp ExitWithFalse

		.endif
	.endif
	
	; Allocate memory to get the interface detail data
	; This contains the devicepath we need to open the device

	mov eax, cb
	mov cbInterfaceDetail, eax

	invoke malloc, cbInterfaceDetail
	.if eax == NULL

		invoke PrintConsole, $CTA0("Unable to allocate memory to get the interface detail data.\n"), 0

		jmp ExitWithFalse

	.endif

	mov esi, eax
	assume esi:ptr SP_DEVICE_INTERFACE_DETAIL_DATA

	mov [esi].cbSize, sizeof SP_DEVICE_INTERFACE_DETAIL_DATA

	invoke SetupDiGetDeviceInterfaceDetail, hIntDevInfo, \
					addr InterfaceData, esi, cbInterfaceDetail, addr cb, NULL

	.if eax == FALSE

		invoke GetLastError

		invoke wsprintf, addr buffer, \
					$CTA0("SetupDiGetDeviceInterfaceDetail failed with error: %d\n"), eax
		invoke PrintConsole, addr buffer, 0

		invoke free, esi

		jmp ExitWithFalse
  
	.endif

	; Now we have the device path. Open the device interface
	; to send Pass Through command

	invoke wsprintf, addr buffer, $CTA0("Interface: %s\n"), addr [esi].DevicePath
	invoke PrintConsole, addr buffer, 0

	invoke CreateFile, addr [esi].DevicePath, GENERIC_READ + GENERIC_WRITE, \
						FILE_SHARE_READ + FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL
	mov hDevice, eax

	; We have the handle to talk to the device. 
	; So we can release the InterfaceDetailData buffer

	invoke free, esi
	assume esi:nothing

	.if hDevice == INVALID_HANDLE_VALUE

		invoke GetLastError

		invoke wsprintf, addr buffer, $CTA0("CreateFile failed with error: %d\n"), eax
		invoke PrintConsole, addr buffer, 0

		jmp ExitWithTrue
  
	.endif

	mov StoragePropertyQuery.PropertyId, StorageAdapterProperty
	mov StoragePropertyQuery.QueryType, PropertyStandardQuery

	invoke DeviceIoControl, hDevice, IOCTL_STORAGE_QUERY_PROPERTY, \
						addr StoragePropertyQuery, sizeof STORAGE_PROPERTY_QUERY, \
						addr DataBuf, sizeof DataBuf, addr cbReturned, NULL                    

	.if eax == 0

		invoke GetLastError

		invoke wsprintf, addr buffer, $CTA0("DeviceIoControl failed with error: %d\n"), eax
		invoke PrintConsole, addr buffer, 0

	.else

		lea esi, DataBuf
		assume esi:ptr STORAGE_ADAPTER_DESCRIPTOR

		invoke PrintConsole, $CTA0("\nAdapter Properties\n"), 0
		invoke PrintConsole, $CTA0("------------------\n"), 0

		movzx eax, [esi].BusType
		shl eax, 2					; * sizeof LPSTR
		invoke wsprintf, addr buffer, $CTA0("Bus Type       : %s\n"), g_apszBusType[eax]
		invoke PrintConsole, addr buffer, 0

		invoke wsprintf, addr buffer, $CTA0("Max. Tr. Length: 0x%x\n"), [esi].MaximumTransferLength
		invoke PrintConsole, addr buffer, 0

		invoke wsprintf, addr buffer, $CTA0("Max. Phy. Pages: 0x%x\n"), [esi].MaximumPhysicalPages
		invoke PrintConsole, addr buffer, 0
		
		invoke wsprintf, addr buffer, $CTA0("Alignment Mask : 0x%x\n"), [esi].AlignmentMask
		invoke PrintConsole, addr buffer, 0

		assume esi:nothing

		mov StoragePropertyQuery.PropertyId, StorageDeviceProperty
		mov StoragePropertyQuery.QueryType, PropertyStandardQuery

		invoke DeviceIoControl, hDevice, IOCTL_STORAGE_QUERY_PROPERTY, addr StoragePropertyQuery, \
							sizeof STORAGE_PROPERTY_QUERY, addr DataBuf, sizeof DataBuf, addr cbReturned, NULL                    

		.if eax == 0

			invoke GetLastError

			invoke wsprintf, addr buffer, $CTA0("DeviceIoControl failed with error: %d\n"), eax
			invoke PrintConsole, addr buffer, 0

		.else

			invoke PrintConsole, $CTA0("\nDevice Properties\n"), 0
			invoke PrintConsole, $CTA0("-----------------\n"), 0

			lea esi, DataBuf
			assume esi:ptr STORAGE_DEVICE_DESCRIPTOR
		
			; Our device table can handle only 16 devices.

			mov eax, g_cbDeviceType
			shr eax, 2						; / sizeof LPSTR
			.if [esi].DeviceType > al
                mov ecx, 0Fh
			.else
				movzx ecx, [esi].DeviceType
			.endif

			invoke wsprintf, addr buffer, $CTA0("Device Type     : %s (0x%X)\n"), g_apszDeviceType[ecx*sizeof LPSTR], ecx
			invoke PrintConsole, addr buffer, 0

			movzx ecx, [esi].DeviceTypeModifier
			.if [esi].DeviceTypeModifier
				invoke wsprintf, addr buffer, $CTA0("Device Modifier : 0x%x\n"), ecx
				invoke PrintConsole, addr buffer, 0
			.endif

			.if [esi].RemovableMedia
				mov ecx, $CTA0("Yes")
			.else
				mov ecx, $CTA0("No")
			.endif

			invoke wsprintf, addr buffer, $CTA0("Removable Media : %s\n"), ecx
			invoke PrintConsole, addr buffer, 0

			mov ebx, [esi].VendorIdOffset
			mov al, byte ptr [esi+ebx]
			.if ebx  &&  al != NULL
				invoke PrintConsole, $CTA0("Vendor ID       : "), 0

				.while ebx < cbReturned

					.break .if byte ptr [esi+ebx] == NULL

					movzx ecx, byte ptr [esi+ebx]
					invoke wsprintf, addr buffer, $CTA0("%c"), ecx
					invoke PrintConsole, addr buffer, 0
			
					inc ebx
				.endw

				invoke PrintConsole, $CTA0("\n"), 0
			.endif			

			mov ebx, [esi].ProductIdOffset
			mov al, byte ptr [esi+ebx]
			.if ebx &&  al != NULL

				invoke PrintConsole, $CTA0("Product ID      : "), 0

				.while ebx < cbReturned

					.break .if byte ptr [esi+ebx] == NULL

					movzx ecx, byte ptr [esi+ebx]
					invoke wsprintf, addr buffer, $CTA0("%c"), ecx
					invoke PrintConsole, addr buffer, 0
			
					inc ebx
				.endw

				invoke PrintConsole, $CTA0("\n"), 0
			.endif
			
			mov ebx, [esi].ProductRevisionOffset
			mov al, byte ptr [esi+ebx]
			.if ebx  &&  al != NULL

				invoke PrintConsole, $CTA0("Product Revision: "), 0

				.while ebx < cbReturned

					.break .if byte ptr [esi+ebx] == NULL

					movzx ecx, byte ptr [esi+ebx]
					invoke wsprintf, addr buffer, $CTA0("%c"), ecx
					invoke PrintConsole, addr buffer, 0
			
					inc ebx
				.endw

				invoke PrintConsole, $CTA0("\n"), 0
			.endif

			mov ebx, [esi].SerialNumberOffset
			mov al, byte ptr [esi+ebx]
			.if ebx  &&  al != NULL

				invoke PrintConsole, $CTA0("Serial Number   : "), 0

				.while ebx < cbReturned

					.break .if byte ptr [esi+ebx] == NULL

					movzx ecx, byte ptr [esi+ebx]
					invoke wsprintf, addr buffer, $CTA0("%c"), ecx
					invoke PrintConsole, addr buffer, 0
			
					inc ebx
				.endw

				invoke PrintConsole, $CTA0("\n"), 0
			.endif
		.endif
	.endif

	; Close handle the device

	invoke CloseHandle, hDevice

ExitWithTrue:
	xor eax, eax
	inc eax
	ret					; return TRUE

ExitWithFalse:
	xor eax, eax
	ret					; return FALSE

GetDeviceProperty endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       start                                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc uses esi edi ebx

local buffer[128]:BYTE
local hDevInfo:HDEVINFO
local hIntDevInfo:HDEVINFO
    
	invoke GetStdHandle, STD_OUTPUT_HANDLE
	.if eax != INVALID_HANDLE_VALUE

		mov g_hConsoleOutput, eax

		invoke PrintConsole, $CTA0("\nEnumDisk - Enumerates all the disk devices\n\n"), \
											FOREGROUND_BLUE + FOREGROUND_INTENSITY

		; Get device information set that contains all devices present on system

		invoke SetupDiGetClassDevs, addr GUID_DEVCLASS_DISKDRIVE, NULL, NULL, DIGCF_PRESENT 
		.if eax != INVALID_HANDLE_VALUE

			mov hDevInfo, eax

			; Get the interface device information set that contains all devices present on system

			invoke SetupDiGetClassDevs, addr GUID_DEVINTERFACE_DISK, \
                 			NULL, NULL, DIGCF_PRESENT + DIGCF_DEVICEINTERFACE

			.if eax != INVALID_HANDLE_VALUE

				mov hIntDevInfo, eax

				; Enumerate all the disk devices

				xor ebx, ebx
				.while TRUE 

					mov eax, ebx
					inc eax				; make it one-based
					invoke wsprintf, addr buffer, $CTA0("\n\n### Properties for Device %d ###\n"), eax
					invoke PrintConsole, addr buffer, 0
				
					invoke GetRegistryProperty, hDevInfo, ebx
					.break .if eax == FALSE

					invoke GetDeviceProperty, hIntDevInfo, ebx
					.break .if eax == FALSE

					inc ebx

				.endw
    
				invoke PrintConsole, $CTA0("\n\n### End of Device List ###\n"), 0

				invoke SetupDiDestroyDeviceInfoList, hIntDevInfo
			
			.else
				invoke PrintConsole, $CTA0("Error: Can't get interface device information set\n"), 0
			.endif

			invoke SetupDiDestroyDeviceInfoList, hDevInfo
    
		.else
			invoke PrintConsole, $CTA0("Error: Can't get device information set.\n"), 0
		.endif

	.else
		invoke MessageBox, NULL, $CTA0("Can't get console standard output handle."), \
											NULL, MB_OK + MB_ICONERROR
	.endif

	invoke ExitProcess, 0

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start

:make

set exe=EnumDisk

\masm32\bin\ml /nologo /c /coff %exe%.bat
\masm32\bin\link /nologo /out:%exe%.exe /subsystem:console %exe%.obj

del %exe%.obj

echo.
pause
exit
