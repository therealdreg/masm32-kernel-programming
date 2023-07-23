;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  Global Descriptor Table Dumper - Let you browse Global Descriptor Table content.
;
; To understand it better read
;   IA-32 Intel Architecture Software Developer’s Manual
;   Volume 3 : System Programming Guide
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
include \masm32\include\gdi32.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\comctl32.lib
includelib \masm32\lib\gdi32.lib

include \masm32\include\winioctl.inc

include \masm32\Macros\Strings.mac

include ..\common.inc
include gdt.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 S T R U C T U R E S                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                         F U N C T I O N S   P R O T O T Y P E S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc	proto :HWND, :UINT, :WPARAM, :LPARAM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        M A C R O S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

$invoke MACRO vars:VARARG
     invoke vars
     EXITM <eax>
ENDM

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
IDE_GDT			equ 1001

IDM_ABOUT		equ	2000

IDI_ICON		equ 3000

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  R E A D O N L Y  D A T A                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

g_szAppName	db "Global Descriptor Table Dumper", 0

szAbout				db "About...", 0
szWrittenBy			db "Global Descriptor Table Dumper v1.1", 0Ah, 0Dh
					db "Built on "
					date
					db 0Ah, 0Dh, 0Ah, 0Dh
					db "Written by Four-F <four-f@mail.ru>", 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_hInstance					HINSTANCE	?
g_hDlg						HWND		?
g_hwndEditGdt				HWND		?

g_hFontOld					HFONT		?
g_hFontNew					HFONT		?

g_pBuffer					LPVOID		?

g_cbBytesReturned			DWORD		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         PrintGdtDump                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintGdtDump proc uses esi edi ebx

local buffer[256]:CHAR
local dwSelector:DWORD

	invoke GetProcessHeap
	invoke HeapAlloc, eax, HEAP_NO_SERIALIZE + HEAP_ZERO_MEMORY, 1000h * 10
	.if eax == NULL
		ret
	.endif
	mov edi, eax

	mov esi, g_pBuffer


	invoke wsprintf, addr buffer, $CTA0("Global Descriptor Table\nBase: %08X   Limit: %08X\n"), dword ptr [esi], dword ptr [esi][4]
	invoke lstrcpy, edi, addr buffer



	CTA "\nBase\t— Segment base address\n", g_szDescription, 4
	CTA "Limit\t— Segment Limit\n"
	CTA "DPL\t— Descriptor privilege level\n"
	CTA "P\t— Segment present\n"
	CTA0 "G\t— Granularity\n\n"
	invoke lstrcat, edi, addr g_szDescription


	CTA0 "-------------------------------------------------------------------------------\n", g_szLine, 4
	invoke lstrcat, edi, addr g_szLine
	invoke lstrcat, edi, $CTA0("Sel.  Base      Limit     DPL  P   G    Description\n")
	invoke lstrcat, edi, addr g_szLine
;	invoke SendMessage, g_hwndEditGdt, EM_REPLACESEL, FALSE, edi

	mov ebx, [esi][4]				; Gdt.Limit
	; The GDT limit is always be one less than an integral multiple of eight (that is, 8N – 1).
	inc ebx
	shr ebx, 3					; / sizeof SEGMENT_DESCRIPTOR	
	dec ebx						; take into account skipped first selector

	mov dwSelector, 8

	add esi, sizeof DWORD * 2
	assume esi:ptr KGDTENTRY

	.while ebx

		; Selector #
		invoke wsprintf, addr buffer, $CTA0("%04X  "), dwSelector
		invoke lstrcat, edi, addr buffer

		add esi, sizeof KGDTENTRY			; skip reserved descriptor for the first circle


		; Segment Base
		mov ch, [esi]._HighWord.Bytes.BaseHi
		mov cl, [esi]._HighWord.Bytes.BaseMid
		shl ecx, 16
		mov cx, [esi].BaseLow

		invoke wsprintf, addr buffer, $CTA0("%08X  "), ecx
		invoke lstrcat, edi, addr buffer


		; Segment Limit
		mov ecx, [esi]._HighWord.gdtBits
		and ecx, mask gdtLimitHi		; and ecx, 011110000000000000000y
		mov cx, [esi].LimitLow

		; if granularity = 1 the limit is in 4Kb units
		mov eax, [esi]._HighWord.gdtBits
		and eax, mask gdtGranularity	; and ecx, 00100000000000000000000000y
		shr eax, 23
		.if eax == 1
			inc ecx
			shl ecx, 12					; * PAGE_SIZE
			dec ecx
		.endif

		invoke wsprintf, addr buffer, $CTA0("%08X   "), ecx
		invoke lstrcat, edi, addr buffer


		; Segment DPL
		mov ecx, [esi]._HighWord.gdtBits
		and ecx, mask gdtDpl		; and ecx, 0110000000000000y
		shr ecx, 13

		invoke wsprintf, addr buffer, $CTA0("%d   "), ecx
		invoke lstrcat, edi, addr buffer


		; Segment present flag
		mov ecx, [esi]._HighWord.gdtBits
		and ecx, mask gdtPres		; and ecx, 01000000000000000y
		shr ecx, 15
		.if ecx == 1
			mov eax, $CTA0("P   ")
		.else
			mov eax, $CTA0("NP  ")
		.endif
		invoke lstrcat, edi, eax

		; Segment granularity
		mov ecx, [esi]._HighWord.gdtBits
		and ecx, mask gdtGranularity	; and ecx, 00100000000000000000000000y
		shr ecx, 23
		.if ecx == 1
			mov eax, $CTA0("4Kb  ")
		.else
			mov eax, $CTA0("1b   ")
		.endif
		invoke lstrcat, edi, eax

		; Segment description
		mov ecx, [esi]._HighWord.gdtBits
		and ecx, mask gdtType	; 
		and ecx, 01111100000000y
		shr ecx, 8
		mov eax, ecx
		and ecx, 01111y			; clear S flag
		shr eax, 4				; extract S (descriptor type) flag
		.if eax == 1
			; The segment descriptor is for a code or data segment (S flag is set).

comment ^
Code- and Data-Segment Types
+-----------------------+------------+-----------------------------------------+
|      Type Field       |            |                                         |
+---------+-------------+ Descriptor |              Description                |
| Decimal | 0B 0A 09 08 |    Type    |                                         |
|         |    E  W  A  |            |                                         |
+---------+-------------+------------+-----------------------------------------+
|    0    | 0  0  0  0  |    Data    | Read-Only                               |
|    1    | 0  0  0  1  |    Data    | Read-Only, accessed                     |
|    2    | 0  0  1  0  |    Data    | Read/Write                              |
|    3    | 0  0  1  1  |    Data    | Read/Write, accessed                    |
|    4    | 0  1  0  0  |    Data    | Read-Only, expand-down                  |
|    5    | 0  1  0  1  |    Data    | Read-Only, expand-down, accessed        |
|    6    | 0  1  1  0  |    Data    | Read/Write, expand-down                 |
|    7    | 0  1  1  1  |    Data    | Read/Write, expand-down, accessed       |
+---------+-------------+------------+-----------------------------------------+
|         |    C  R  A  |            |                                         |
+---------+-------------+------------+-----------------------------------------+
|    8    | 1  0  0  0  |    Code    | Execute-Only                            |
|    9    | 1  0  0  1  |    Code    | Execute-Only, accessed                  |
|    10   | 1  0  1  0  |    Code    | Execute/Read                            |
|    11   | 1  0  1  1  |    Code    | Execute/Read, accessed                  |
|    12   | 1  1  0  0  |    Code    | Execute-Only, conforming                |
|    13   | 1  1  0  1  |    Code    | Execute-Only, conforming, accessed      |
|    14   | 1  1  1  0  |    Code    | Execute/Read-Only, conforming           |
|    15   | 1  1  1  1  |    Code    | Execute/Read-Only, conforming, accessed |
+---------+-------------+------------+-----------------------------------------+
^

			.if cl == 0
				mov eax, $CTA0("Read-Only")
			.elseif cl == 1
				mov eax, $CTA0("Read-Only, accessed")
			.elseif cl == 2
				mov eax, $CTA0("Read/Write")
			.elseif cl == 3
				mov eax, $CTA0("Read/Write, accessed")
			.elseif cl == 4
				mov eax, $CTA0("Read-Only, expand-down")
			.elseif cl == 5
				mov eax, $CTA0("Read-Only, expand-down, accessed")
			.elseif cl == 6
				mov eax, $CTA0("Read/Write, expand-down")
			.elseif cl == 7
				mov eax, $CTA0("Read/Write, expand-down, accessed")
			.elseif cl == 8
				mov eax, $CTA0("Execute-Only")
			.elseif cl == 9
				mov eax, $CTA0("Execute-Only, accessed")
			.elseif cl == 10
				mov eax, $CTA0("Execute/Read")
			.elseif cl == 11
				mov eax, $CTA0("Execute/Read, accessed")
			.elseif cl == 12
				mov eax, $CTA0("Execute-Only, conforming")
			.elseif cl == 13
				mov eax, $CTA0("Execute-Only, conforming, accessed")
			.elseif cl == 14
				mov eax, $CTA0("Execute/Read-Only, conforming")
			.elseif cl == 15
				mov eax, $CTA0("Execute/Read-Only, conforming, accessed")
			.endif


		.else
			; The segment descriptor is for a system segment (S flag is clear).

comment ^
System-Segment and Gate-Descriptor Types
+-----------------------+--------------------------+
|      Type Field       |                          |
+---------+-------------+      Description         +
| Decimal | 0B 0A 09 08 |                          |
+---------+-------------+--------------------------+
|    0    | 0  0  0  0  | Reserved                 |
|    1    | 0  0  0  1  | 16-Bit TSS (Available)   |
|    2    | 0  0  1  0  | LDT                      |
|    3    | 0  0  1  1  | 16-Bit TSS (Busy)        |
|    4    | 0  1  0  0  | 16-Bit Call Gate         |
|    5    | 0  1  0  1  | Task Gate                |
|    6    | 0  1  1  0  | 16-Bit Interrupt Gate    |
|    7    | 0  1  1  1  | 16-Bit Trap Gate         |
|    8    | 1  0  0  0  | Reserved                 |
|    9    | 1  0  0  1  | 32-Bit TSS (Available)   |
|    10   | 1  0  1  0  | Reserved                 |
|    11   | 1  0  1  1  | 32-Bit TSS (Busy)        |
|    12   | 1  1  0  0  | 32-Bit Call Gate         |
|    13   | 1  1  0  1  | Reserved                 |
|    14   | 1  1  1  0  | 32-Bit Interrupt Gate    |
|    15   | 1  1  1  1  | 32-Bit Trap Gate         |
+---------+-------------+--------------------------+
^
			.if cl == 0
				mov eax, $CTA0("Reserved")
			.elseif cl == 1
				mov eax, $CTA0("16-Bit TSS (Available)")
			.elseif cl == 2
				mov eax, $CTA0("LDT")
			.elseif cl == 3
				mov eax, $CTA0("16-Bit TSS (Busy)")
			.elseif cl == 4
				mov eax, $CTA0("16-Bit Call Gate")
			.elseif cl == 5
				mov eax, $CTA0("Task Gate")
			.elseif cl == 6
				mov eax, $CTA0("16-Bit Interrupt Gate")
			.elseif cl == 7
				mov eax, $CTA0("16-Bit Trap Gate")
			.elseif cl == 8
				mov eax, $CTA0("Reserved")
			.elseif cl == 9
				mov eax, $CTA0("32-Bit TSS (Available)")
			.elseif cl == 10
				mov eax, $CTA0("Reserved")
			.elseif cl == 11
				mov eax, $CTA0("32-Bit TSS (Busy)")
			.elseif cl == 12
				mov eax, $CTA0("32-Bit Call Gate")
			.elseif cl == 13
				mov eax, $CTA0("Reserved")
			.elseif cl == 14
				mov eax, $CTA0("32-Bit Interrupt Gate")
			.elseif cl == 15
				mov eax, $CTA0("32-Bit Trap Gate")
			.endif

		.endif
		invoke lstrcat, edi, eax




		invoke lstrcat, edi, $CTA0("\n")
;		invoke SendMessage, g_hwndEditGdt, EM_REPLACESEL, FALSE, edi

		add dwSelector, sizeof KGDTENTRY
		dec ebx

	.endw

	assume esi:nothing

	invoke SetWindowText, g_hwndEditGdt, edi

	invoke GetProcessHeap
	invoke HeapFree, eax, HEAP_NO_SERIALIZE, edi

	ret

PrintGdtDump endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               D I A L O G     P R O C E D U R E                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DlgProc proc uses esi edi hDlg:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

local rect:RECT
local lf:LOGFONT

	mov eax, uMsg
	.if eax == WM_COMMAND
		mov eax, wParam
		.if ax == IDCANCEL
			invoke EndDialog, hDlg, 0
		.endif

	.elseif eax == WM_SIZE

		mov eax, lParam
		mov ecx, eax
		and eax, 0FFFFh
		shr ecx, 16
		invoke MoveWindow, g_hwndEditGdt, 0, 0, eax, ecx, TRUE

	.elseif eax == WM_INITDIALOG

		; Initialize global variables
		push hDlg
		pop g_hDlg

		invoke SetWindowText, hDlg, addr g_szAppName

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

		; Add "About..." to sys menu
		invoke GetSystemMenu, hDlg, FALSE
		push eax
		invoke InsertMenu, eax, -1, MF_BYPOSITION + MF_SEPARATOR, 0, 0
		pop eax
		invoke InsertMenu, eax, -1, MF_BYPOSITION + MF_STRING, IDM_ABOUT, offset szAbout


		mov g_hwndEditGdt, $invoke(GetDlgItem, hDlg, IDE_GDT)
		invoke SendMessage, g_hwndEditGdt, EM_SETLIMITTEXT, 65535, 0

		mov	g_hFontOld, $invoke(SendMessage, g_hwndEditGdt, WM_GETFONT, 0, 0)
		invoke GetObject, g_hFontOld, sizeof LOGFONT, addr lf

		lea ecx, lf.lfFaceName
		invoke lstrcpy, ecx, $CTA0("Courier New")
		invoke CreateFontIndirect, addr lf		
		mov	g_hFontNew, eax

		invoke SendMessage, g_hwndEditGdt, WM_SETFONT, g_hFontNew, FALSE

		invoke PrintGdtDump

		; The following four lines is needed because of while processing
		; the WM_INITDIALOG message of a dialog box, sending an EM_SETSEL
		; fails to remove the highlight from (unselect) the edit control text.
		
		; Read Microsoft Knowledge Base Article - 96674
		; "Unselecting Edit Control Text at Dialog Box Initialization"
		; for more info
  
 		invoke SetFocus, g_hwndEditGdt
		invoke PostMessage, g_hwndEditGdt, EM_SETSEL, -1, -1
		xor eax, eax
		ret

	.elseif uMsg == WM_DESTROY

		invoke SendMessage, g_hwndEditGdt, WM_SETFONT, g_hFontOld, FALSE
		invoke DeleteObject, g_hFontNew

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
;                                         CallDriver                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CallDriver proc

local fOk:BOOL

local hSCManager:HANDLE
local hService:HANDLE
local acModulePath[MAX_PATH]:CHAR
local _ss:SERVICE_STATUS
local hDevice:HANDLE

	and fOk, FALSE				; assume error

	invoke OpenSCManager, NULL, NULL, SC_MANAGER_ALL_ACCESS
	.if eax != NULL
		mov hSCManager, eax

		push eax
		invoke GetFullPathName, $CTA0("GdtDump.sys"), sizeof acModulePath, addr acModulePath, esp
    	pop eax

		invoke CreateService, hSCManager, $CTA0("GdtDump"), addr g_szAppName, \
			SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
			SERVICE_ERROR_IGNORE, addr acModulePath, NULL, NULL, NULL, NULL, NULL

		.if eax != NULL
			mov hService, eax

			invoke StartService, hService, 0, NULL
			.if eax != 0

				invoke CreateFile, $CTA0("\\\\.\\GdtDump"), GENERIC_READ, \
										0, NULL, OPEN_EXISTING, 0, NULL

				.if eax != INVALID_HANDLE_VALUE
					mov hDevice, eax

					;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

					invoke DeviceIoControl, hDevice, IOCTL_DUMP_GDT, NULL, 0, \
							g_pBuffer, BUFFER_SIZE, addr g_cbBytesReturned, NULL
					.if ( eax != 0 ) && ( g_cbBytesReturned != 0 )
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
;                                           start                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start:

	invoke VirtualAlloc, NULL, BUFFER_SIZE, MEM_COMMIT, PAGE_READWRITE
	.if eax != NULL
		mov g_pBuffer, eax
		invoke CallDriver
		.if eax == TRUE
			mov g_hInstance, $invoke(GetModuleHandle, NULL)
			invoke DialogBoxParam, g_hInstance, IDD_MAIN, NULL, addr DlgProc, 0
		.endif
		invoke VirtualFree, g_pBuffer, 0, MEM_RELEASE
	.else
		invoke MessageBox, NULL, $CTA0("Couldn't allocate memory."), NULL, MB_OK + MB_ICONERROR
	.endif

	invoke ExitProcess, 0

end start

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:make

set exe=GdtDump

if exist ..\%exe%.exe del ..\%exe%.exe

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

move %exe%.exe ..
del %exe%.obj
if exist %exe%.exe del %exe%.exe

echo.
pause
