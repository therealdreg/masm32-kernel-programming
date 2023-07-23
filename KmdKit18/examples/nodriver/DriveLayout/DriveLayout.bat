;@echo off
;goto make

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

include \masm32\include\windows.inc

include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\advapi32.inc
include \masm32\include\comctl32.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\comctl32.lib

;include \masm32\include\w2k\ntddk.inc		
include \masm32\include\w2k\devioctl.inc
;include \masm32\include\w2k\ntddstor.inc
;include \masm32\include\w2k\ntdddisk.inc

include cocomac\cocomac.mac
include cocomac\ListView.mac
include \masm32\Macros\Strings.mac

; ntdddisk.inc

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

IOCTL_DISK_BASE                 equ FILE_DEVICE_DISK
IOCTL_DISK_GET_DRIVE_LAYOUT     equ CTL_CODE(IOCTL_DISK_BASE, 0003, METHOD_BUFFERED, FILE_READ_ACCESS)

; because of bad definition in windows.inc

_LARGE_INTEGER UNION
	struct
		LowPart		DWORD ?
		HighPart	SDWORD ?
	ends
	struct u
		LowPart		DWORD ?
		HighPart	SDWORD ?
	ends
	QuadPart		QWORD ?	; signed
_LARGE_INTEGER ENDS
P_LARGE_INTEGER typedef PTR _LARGE_INTEGER

; The following structure is returned on an IOCTL_DISK_GET_PARTITION_INFO
; and an IOCTL_DISK_GET_DRIVE_LAYOUT request.  It is also used in a request
; to change the drive layout, IOCTL_DISK_SET_DRIVE_LAYOUT.

IFNDEF PARTITION_INFORMATION		; winioctl.inc also
PARTITION_INFORMATION STRUCT		; sizeof = 20h
	StartingOffset		_LARGE_INTEGER	<>
	PartitionLength		_LARGE_INTEGER	<>
	HiddenSectors		DWORD		?
	PartitionNumber		DWORD		?
	PartitionType		BYTE		?
	BootIndicator		BOOLEAN		?
	RecognizedPartition	BOOLEAN		?
	RewritePartition	BOOLEAN		?
						DWORD		?	; padding
PARTITION_INFORMATION ENDS
PPARTITION_INFORMATION typedef ptr PARTITION_INFORMATION
ENDIF

; The following structures is returned on an IOCTL_DISK_GET_DRIVE_LAYOUT
; request and given as input to an IOCTL_DISK_SET_DRIVE_LAYOUT request.

IFNDEF DRIVE_LAYOUT_INFORMATION		; winioctl.inc also
DRIVE_LAYOUT_INFORMATION  STRUCT	; sizeof = 28h
	PartitionCount		DWORD	?
	Signature			DWORD	?
	PartitionEntry		PARTITION_INFORMATION 1 dup(<>)
DRIVE_LAYOUT_INFORMATION ENDS
PDRIVE_LAYOUT_INFORMATION typedef ptr DRIVE_LAYOUT_INFORMATION
ENDIF

; Define the partition types returnable by known disk drivers.

PARTITION_ENTRY_UNUSED          equ 00      ; Entry unused
PARTITION_FAT_12                equ 01      ; 12-bit FAT entries
PARTITION_XENIX_1               equ 02      ; Xenix
PARTITION_XENIX_2               equ 03      ; Xenix
PARTITION_FAT_16                equ 04      ; 16-bit FAT entries
PARTITION_EXTENDED              equ 05      ; Extended partition entry
PARTITION_HUGE                  equ 06      ; Huge partition MS-DOS V4
PARTITION_IFS                   equ 07      ; IFS Partition
PARTITION_OS2BOOTMGR            equ 0Ah     ; OS/2 Boot Manager/OPUS/Coherent swap
PARTITION_FAT32                 equ 0Bh     ; FAT32
PARTITION_FAT32_XINT13          equ 0Ch     ; FAT32 using extended int13 services
PARTITION_XINT13                equ 0Eh     ; Win95 partition using extended int13 services
PARTITION_XINT13_EXTENDED       equ 0Fh     ; Same as type 5 but uses extended int13 services
PARTITION_PREP                  equ 41h     ; PowerPC Reference Platform (PReP) Boot Partition
PARTITION_LDM                   equ 42h     ; Logical Disk Manager partition
PARTITION_UNIX                  equ 63h     ; Unix

VALID_NTFT                      equ C0h     ; NTFT uses high order bits

; The high bit of the partition type code indicates that a partition
; is part of an NTFT mirror or striped array.

PARTITION_NTFT                  equ 80h     ; NTFT partition

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       M A C R O S                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

date MACRO
local pos, month

	;; Day
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 4) OR (pos EQ 5)
			db "&chr"
		ENDIF
		pos = pos + 1
	ENDM

	;; Month
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 1)
			month TEXTEQU @SubStr(%@Date, 1 , 2)
			IF month EQ 01
				db " Jan "	
			ELSEIF month EQ 02
				db " Feb "	
			ELSEIF month EQ 03
				db " Mar "	
			ELSEIF month EQ 04
				db " Apr "	
			ELSEIF month EQ 05
				db " May "	
			ELSEIF month EQ 06
				db " Jun "	
			ELSEIF month EQ 07
				db " Jul "	
			ELSEIF month EQ 08
				db " Aug "	
			ELSEIF month EQ 09
				db " Sep "	
			ELSEIF month EQ 10
				db " Oct "	
			ELSEIF month EQ 11
				db " Nov "	
			ELSEIF month EQ 12
				db " Dec "	
			ENDIF
		ENDIF
		pos = pos + 1
	ENDM

	;; Year
	db "20"
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 7) OR (pos EQ 8)
			db "&chr"
		ENDIF
		pos = pos + 1
	ENDM

ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IDD_MAIN		equ	1000
IDC_LISTVIEW	equ 1001
IDI_ICON		equ 1002
IDM_ABOUT		equ	2000

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_hInstance		HINSTANCE	?
g_hwndDlg		HWND		?
g_hwndListView	HWND		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  R E A D O N L Y  D A T A                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

szAbout						db "About...", 0
szWrittenBy					db "Drive Layout Information v1.0", 0Ah, 0Dh
							db "Built on "
							date
							db 0Ah, 0Dh, 0Ah, 0Dh
							db "Written by Four-F <four-f@mail.ru>", 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        C O D E                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                            malloc                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

malloc proc dwBytes:DWORD

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke GetProcessHeap
	invoke HeapAlloc, eax, 0, [esp+4]
	ret 4

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

malloc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                           delete                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

free proc lpMem:PVOID

option PROLOGUE:NONE
option EPILOGUE:NONE

	invoke GetProcessHeap
	invoke HeapFree, eax, 0, [esp+4]
	ret 4

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

free endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    FillDriveLayoutInfo                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FillDriveLayoutInfo proc uses esi edi ebx

local lvi:LV_ITEM
local i:DWORD
local hDevice:HANDLE
local dwBytesReturned:DWORD
local cb:DWORD
local pdli:PDRIVE_LAYOUT_INFORMATION
local buffer[512]:CHAR

	mov esi, pdli
	assume esi:ptr DRIVE_LAYOUT_INFORMATION

	mov lvi.imask, LVIF_TEXT
	or lvi.iItem, -1

	mov cb, sizeof DRIVE_LAYOUT_INFORMATION + sizeof PARTITION_INFORMATION * 511

	invoke malloc, cb
	.if eax != NULL
	
		mov pdli, eax

		and i, 0
		.while i < 32
				
			invoke wsprintf, addr buffer, $CTA0("\\Device\\Harddisk%d\\Partition%d"), i, 0

			invoke DefineDosDevice, DDD_RAW_TARGET_PATH, $CTA0("HarddiskXPartitionX"), addr buffer
			.if eax != 0

				invoke CreateFile, $CTA0("\\\\.\\HarddiskXPartitionX"), GENERIC_READ, \
							FILE_SHARE_READ + FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL

				.if eax != INVALID_HANDLE_VALUE
					mov hDevice, eax
		
					invoke DeviceIoControl, hDevice, IOCTL_DISK_GET_DRIVE_LAYOUT, NULL, 0, \
												pdli, cb, addr dwBytesReturned, NULL
					.if eax != 0

						mov esi, pdli
						assume esi:ptr DRIVE_LAYOUT_INFORMATION

						lea edi, [esi].PartitionEntry
						assume edi:ptr PARTITION_INFORMATION

						xor ebx, ebx
						.while ebx < [esi].PartitionCount
						
							inc ebx					; Make it one-based

							.if [edi].RecognizedPartition

								invoke wsprintf, addr buffer, \
												$CTA0("\\Device\\Harddisk%d\\Partition%d"), \
												i, [edi].PartitionNumber
			
								inc lvi.iItem
								and lvi.iSubItem, 0
								lea eax, buffer
								mov lvi.pszText, eax
								ListView_InsertItem g_hwndListView, addr lvi


								inc lvi.iSubItem
								.if [edi].StartingOffset.HighPart != 0
									invoke wsprintf, addr buffer, $CTA0("%x%08x"), \
										[edi].StartingOffset.HighPart, [edi].StartingOffset.LowPart
								.else
									invoke wsprintf, addr buffer, $CTA0("%x"), \
										[edi].StartingOffset.LowPart
								.endif
								lea eax, buffer
								mov lvi.pszText, eax
								ListView_SetItem g_hwndListView, addr lvi


								inc lvi.iSubItem
								.if [edi].PartitionLength.HighPart != 0
									invoke wsprintf, addr buffer, $CTA0("%x%08x"), \
										[edi].PartitionLength.HighPart, [edi].PartitionLength.LowPart
								.else
									invoke wsprintf, addr buffer, $CTA0("%x"), \
										[edi].PartitionLength.LowPart
								.endif
								lea eax, buffer
								mov lvi.pszText, eax
								ListView_SetItem g_hwndListView, addr lvi


								inc lvi.iSubItem
								invoke wsprintf, addr buffer, $CTA0("%d"), \
									[edi].HiddenSectors
								lea eax, buffer
								mov lvi.pszText, eax
								ListView_SetItem g_hwndListView, addr lvi
								

								inc lvi.iSubItem
								.if [edi].BootIndicator
									mov lvi.pszText, $CTA0("*")
								.else
									mov lvi.pszText, $CTA0("")
								.endif
								ListView_SetItem g_hwndListView, addr lvi


								inc lvi.iSubItem
								movzx eax, [edi].PartitionType
								.if eax & PARTITION_NTFT
									mov lvi.pszText, $CTA0("*")
								.else
									mov lvi.pszText, $CTA0("")
								.endif
								ListView_SetItem g_hwndListView, addr lvi


								inc lvi.iSubItem
								movzx eax, [edi].PartitionType
								; PARTITION_NTFT is used in combination (that is, bitwise
								; logically ORed) with the other values
								and eax, not PARTITION_NTFT
								.if eax == PARTITION_ENTRY_UNUSED
									mov lvi.pszText, $CTA0("Unused entry")
								.elseif eax == PARTITION_FAT_12
									mov lvi.pszText, $CTA0("A partition with 12-bit FAT entries")
								.elseif eax == PARTITION_XENIX_1
									mov lvi.pszText, $CTA0("A XENIX® Type 1 partition")
								.elseif eax == PARTITION_XENIX_2
									mov lvi.pszText, $CTA0("A XENIX Type 2 partition")
								.elseif eax == PARTITION_FAT_16
									mov lvi.pszText, $CTA0("A partition with 16-bit FAT entries")
								.elseif eax == PARTITION_EXTENDED
									mov lvi.pszText, $CTA0("An MS-DOS® V4 extended partition")
								.elseif eax == PARTITION_HUGE
									mov lvi.pszText, $CTA0("An MS-DOS V4 huge partition")
								.elseif eax == PARTITION_IFS
									mov lvi.pszText, $CTA0("An IFS partition.")
								.elseif eax == PARTITION_OS2BOOTMGR
									mov lvi.pszText, $CTA0("OS/2 Boot Manager/OPUS/Coherent swap")
								.elseif eax == PARTITION_FAT32
									mov lvi.pszText, $CTA0("A FAT32 partition")
								.elseif eax == PARTITION_FAT32_XINT13
									mov lvi.pszText, $CTA0("A FAT32 partition that uses extended INT 13 services")
								.elseif eax == PARTITION_XINT13
									mov lvi.pszText, $CTA0("A partition that uses extended int13 services")
								.elseif eax == PARTITION_XINT13_EXTENDED
									mov lvi.pszText, $CTA0("An MS-DOS® V4 extended partition that uses extended INT 13 services")
								.elseif eax == PARTITION_PREP
									mov lvi.pszText, $CTA0("A PowerPC Reference Platform partition")
								.elseif eax == PARTITION_LDM
									mov lvi.pszText, $CTA0("A logical disk manager partition")
								.elseif eax == PARTITION_UNIX
									mov lvi.pszText, $CTA0("A UNIX partition")
								.else
									mov lvi.pszText, $CTA0("???")
								.endif
								ListView_SetItem g_hwndListView, addr lvi
	
							.endif

							add edi, sizeof PARTITION_INFORMATION

						.endw
						
						assume edi:nothing
						assume esi:nothing

					.endif
					invoke CloseHandle, hDevice
				.endif
				invoke DefineDosDevice, DDD_REMOVE_DEFINITION, $CTA0("HarddiskXPartitionX"), NULL
			.endif
			
			inc i
			
		.endw
		
		invoke free, pdli
		
	.endif

	ret

FillDriveLayoutInfo endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     ListViewInsertColumn                                          
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ListViewInsertColumn proc

local lvc:LV_COLUMN

	mov lvc.imask, LVCF_TEXT + LVCF_WIDTH 
	mov lvc.pszText, $CTA0("Partition")
	mov lvc.lx, 160
	ListView_InsertColumn g_hwndListView, 0, addr lvc

	or lvc.imask, LVCF_FMT
	mov lvc.fmt, LVCFMT_RIGHT
	mov lvc.lx, 85
	mov lvc.pszText, $CTA0("Starting Offset")
	ListView_InsertColumn g_hwndListView, 1, addr lvc

	mov lvc.lx, 90
	mov lvc.pszText, $CTA0("Partition Length")
	ListView_InsertColumn g_hwndListView, 2, addr lvc
	
	mov lvc.pszText, $CTA0("Hidden Sectors")
	ListView_InsertColumn g_hwndListView, 3, addr lvc

	mov lvc.fmt, LVCFMT_CENTER
	mov lvc.lx, 60
	mov lvc.pszText, $CTA0("Bootable")
	ListView_InsertColumn g_hwndListView, 4, addr lvc

	mov lvc.pszText, $CTA0("NTFT")
	mov lvc.lx, 40
	ListView_InsertColumn g_hwndListView, 5, addr lvc

	mov lvc.fmt, LVCFMT_LEFT
	mov lvc.lx, 280
	mov lvc.pszText, $CTA0("Partition Type")
	ListView_InsertColumn g_hwndListView, 6, addr lvc

	ret

ListViewInsertColumn endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               D I A L O G     P R O C E D U R E                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local rect:RECT

	mov eax, uMsg
	.if eax == WM_INITDIALOG

		push hDlg
		pop g_hwndDlg

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

		invoke GetDlgItem, hDlg, IDC_LISTVIEW
		mov g_hwndListView, eax
		invoke SetFocus, g_hwndListView

		invoke GetClientRect, hDlg, addr rect
		invoke MoveWindow, g_hwndListView, rect.left, rect.top, rect.right, rect.bottom, FALSE

		ListView_SetExtendedListViewStyle g_hwndListView, LVS_EX_GRIDLINES + LVS_EX_FULLROWSELECT

		; Add about menu

		invoke GetSystemMenu, hDlg, FALSE
		mov esi, eax
		invoke InsertMenu, esi, -1, MF_BYPOSITION + MF_SEPARATOR, 0, 0
		invoke InsertMenu, esi, -1, MF_BYPOSITION + MF_STRING, IDM_ABOUT, offset szAbout
	
		invoke ListViewInsertColumn
		invoke FillDriveLayoutInfo

	.elseif eax == WM_SIZE

		mov eax, lParam
		mov ecx, eax
		and eax, 0FFFFh
		shr ecx, 16
		invoke MoveWindow, g_hwndListView, 0, 0, eax, ecx, TRUE

	.elseif eax == WM_COMMAND

		mov eax, wParam
		and eax, 0FFFFh

		.if eax == IDCANCEL
			invoke EndDialog, hDlg, 0
		.endif
	
	.elseif eax == WM_SYSCOMMAND

		.if wParam == IDM_ABOUT
			invoke MessageBox, hDlg, addr szWrittenBy, addr szAbout, MB_OK + MB_ICONINFORMATION
		.endif
 		xor eax, eax
 		ret

	.elseif uMsg == WM_GETMINMAXINFO

		mov ecx, lParam
		mov (MINMAXINFO PTR [ecx]).ptMinTrackSize.x, 380
		mov (MINMAXINFO PTR [ecx]).ptMinTrackSize.y, 150

	.else

		xor eax, eax
		ret
	
	.endif

	xor eax, eax
	inc eax
	ret
    
DlgProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         start                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc

	invoke GetModuleHandle, NULL
	mov g_hInstance, eax
	invoke DialogBoxParam, g_hInstance, IDD_MAIN, NULL, addr DlgProc, 0

	invoke ExitProcess, 0
	invoke InitCommonControls

	ret				; Never executed

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end start

:make

set exe=DriveLayout

:makerc
if exist rsrc.obj goto final
	\masm32\bin\rc /v rsrc.rc
	\masm32\bin\cvtres /machine:ix86 rsrc.res
	if errorlevel 0 goto final
		echo.
		pause
		exit

:final

if exist rsrc.res del rsrc.res

\masm32\bin\ml /nologo /c /coff %exe%.bat
\masm32\bin\link /nologo /subsystem:windows %exe%.obj rsrc.obj

del %exe%.obj

echo.
pause
