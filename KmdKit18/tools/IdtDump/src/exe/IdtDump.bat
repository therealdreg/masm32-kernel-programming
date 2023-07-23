;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  Interrupt Descriptor Table Dumper - Let you browse Interrupt Descriptor Table content.
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
include idt.inc

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
IDE_IDT			equ 1001

IDM_ABOUT		equ	2000

IDI_ICON		equ 3000

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  R E A D O N L Y  D A T A                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

g_szAppName	db "Interrupt Descriptor Table Dumper", 0

szAbout				db "About...", 0
szWrittenBy			db "Interrupt Descriptor Table Dumper v1.1", 0Ah, 0Dh
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
g_hwndEditIdt				HWND		?

g_hFontOld					HFONT		?
g_hFontNew					HFONT		?

g_pBuffer					LPVOID		?

g_cbBytesReturned			DWORD		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         PrintIdtDump                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintIdtDump proc uses esi edi ebx

local buffer[256]:CHAR
local dwVector:DWORD

	invoke GetProcessHeap
	invoke HeapAlloc, eax, HEAP_NO_SERIALIZE + HEAP_ZERO_MEMORY, 1000h * 15
	.if eax == NULL
		ret
	.endif
	mov edi, eax

	mov esi, g_pBuffer


	invoke wsprintf, addr buffer, $CTA0("Interrupt Descriptor Table\nBase: %08X   Limit: %08X\n"), dword ptr [esi], dword ptr [esi][4]
	invoke lstrcpy, edi, addr buffer

	CTA "\nSelector\t- Segment Selector for destination code segment\n", g_szDescription, 4
	CTA "Offset\t\t- Offset to procedure entry point\n"
	CTA "DPL\t\t— Descriptor privilege level\n"
	CTA "P\t\t— Segment present\n"
	CTA0 "D\t\t- Size of gate: 1 = 32 bits; 0 = 16 bits D\n\n"
	invoke lstrcat, edi, addr g_szDescription


	CTA0 "--------------------------------------------------------------------------------------------------------------\n", g_szLine, 4
	invoke lstrcat, edi, addr g_szLine
	invoke lstrcat, edi, $CTA0("Int.  Sel.:Offset    DPL  P   D        Descriptor Type   Description\n")
	invoke lstrcat, edi, addr g_szLine

	mov ebx, [esi][4]				; Idt.Limit
	; The IDT limit is always be one less than an integral multiple of eight (that is, 8N – 1).
	inc ebx
	shr ebx, 3					; / sizeof KIDTENTRY

	and dwVector, 0

	add esi, sizeof DWORD * 2
	assume esi:ptr KIDTENTRY

	.while ebx

		; Int #
		invoke wsprintf, addr buffer, $CTA0("%04X  "), dwVector
		invoke lstrcat, edi, addr buffer

		; Selector
		movzx ecx, [esi].Selector
		invoke wsprintf, addr buffer, $CTA0("%04X:"), ecx
		invoke lstrcat, edi, addr buffer


		; Offset
		mov cx, [esi].ExtendedOffset
		shl ecx, 16
		mov cx, [esi]._Offset
		invoke wsprintf, addr buffer, $CTA0("%08X   "), ecx
		invoke lstrcat, edi, addr buffer


		; DPL
		movzx ecx, [esi].Access
		shr ecx, 13
		and ecx, 011y
		invoke wsprintf, addr buffer, $CTA0("%d   "), ecx
		invoke lstrcat, edi, addr buffer


		; Segment present flag
		movzx ecx, [esi].Access
		shr ecx, 15
		.if ecx == 1
			mov eax, $CTA0("P   ")
		.else
			mov eax, $CTA0("NP  ")
		.endif
		invoke lstrcat, edi, eax

		; Size of gate
		movzx ecx, [esi].Access
		shr ecx, 8
		mov eax, ecx
		and eax, 0111y
		.if eax != 0101y			; D flag is not valid for Task Gate
			shr ecx, 3
			and ecx, 01y			; mask D flag
			.if ecx == 1
				mov eax, $CTA0("32 bits  ")
			.else
				mov eax, $CTA0("16 bits  ")
			.endif
		.else
			mov eax, $CTA0("         ")
		.endif
		invoke lstrcat, edi, eax

		; Descriptor Type
		movzx ecx, [esi].Access
		shr ecx, 8
		and ecx, 0111y
		.if ecx == 0101y
			; Task Gate
			mov eax, $CTA0("Task Gate        ")
		.elseif ecx == 0110y
			; Interrupt Gate
			mov eax, $CTA0("Interrupt Gate   ")
		.elseif ecx == 0111y
			; Trap Gate
			mov eax, $CTA0("Trap Gate        ")
		.else
			mov eax, $CTA0("                 ")
		.endif
		invoke lstrcat, edi, eax

		; Description
		mov ecx, dwVector
		.if ecx == 0
			mov eax, $CTA0("Fault       Divide Error")
		.elseif ecx == 1
			mov eax, $CTA0("Fault/Trap  Debug")
		.elseif ecx == 2
			mov eax, $CTA0("Interrupt   NMI Interrupt")
		.elseif ecx == 3
			mov eax, $CTA0("Trap        Breakpoint")
		.elseif ecx == 4
			mov eax, $CTA0("Trap        Overflow")
		.elseif ecx == 5
			mov eax, $CTA0("Fault       BOUND Range Exceeded")
		.elseif ecx == 6
			mov eax, $CTA0("Fault       Invalid Opcode (Undefined Opcode). Was introduced in the Pentium Pro processor")
		.elseif ecx == 7
			mov eax, $CTA0("Fault       Device Not Available (No Math Coprocessor)")
		.elseif ecx == 8
			mov eax, $CTA0("Abort       Double Fault")
		.elseif ecx == 9
			mov eax, $CTA0("Fault       Coprocessor Segment Overrun (reserved). IA-32 processors after the Intel386 processor do not generate this exception.")
		.elseif ecx == 10
			mov eax, $CTA0("Fault       Invalid TSS")
		.elseif ecx == 11
			mov eax, $CTA0("Fault       Segment Not Present")
		.elseif ecx == 12
			mov eax, $CTA0("Fault       Stack-Segment Fault")
		.elseif ecx == 13
			mov eax, $CTA0("Fault       General Protection")
		.elseif ecx == 14
			mov eax, $CTA0("Fault       Page Fault")
		.elseif ecx == 15
			mov eax, $CTA0("            Intel reserved. Do not use")
		.elseif ecx == 16
			mov eax, $CTA0("Fault       x87 FPU Floating-Point Error (Math Fault)")
		.elseif ecx == 17
			mov eax, $CTA0("Fault       Alignment Check. Was introduced in the Intel486 processor.")
		.elseif ecx == 18
			mov eax, $CTA0("Abort       Machine Check. Was introduced in the Pentium processor and enhanced in the P6 family processors.")
		.elseif ecx == 19
			mov eax, $CTA0("Fault       SIMD Floating-Point Exception. Was introduced in the Pentium III processor.")
		.else
			mov eax, $CTA0("")			; for shure
			.if ecx >= 20  &&  ecx <= 31
				mov eax, $CTA0("            Intel reserved. Do not use")
			.endif
			.if ecx >= 32
				mov eax, $CTA0("            User Defined (Non-reserved) Interrupts")
			.endif
		.endif
		invoke lstrcat, edi, eax
			

		invoke lstrcat, edi, $CTA0("\n")

		add esi, sizeof KIDTENTRY
		inc dwVector
		dec ebx

	.endw

	assume esi:nothing

	invoke SetWindowText, g_hwndEditIdt, edi

	invoke GetProcessHeap
	invoke HeapFree, eax, HEAP_NO_SERIALIZE, edi

	ret

PrintIdtDump endp

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
		invoke MoveWindow, g_hwndEditIdt, 0, 0, eax, ecx, TRUE

	.elseif eax == WM_INITDIALOG

		; Initialize global variables
		push hDlg
		pop g_hDlg

		invoke LoadIcon, g_hInstance, IDI_ICON
		invoke SendMessage, hDlg, WM_SETICON, ICON_BIG, eax

		invoke SetWindowText, hDlg, addr g_szAppName

		; Add "About..." to sys menu
		invoke GetSystemMenu, hDlg, FALSE
		push eax
		invoke InsertMenu, eax, -1, MF_BYPOSITION + MF_SEPARATOR, 0, 0
		pop eax
		invoke InsertMenu, eax, -1, MF_BYPOSITION + MF_STRING, IDM_ABOUT, offset szAbout


		mov g_hwndEditIdt, $invoke(GetDlgItem, hDlg, IDE_IDT)
		invoke SendMessage, g_hwndEditIdt, EM_SETLIMITTEXT, 65535, 0

		mov	g_hFontOld, $invoke(SendMessage, g_hwndEditIdt, WM_GETFONT, 0, 0)
		invoke GetObject, g_hFontOld, sizeof LOGFONT, addr lf

		lea ecx, lf.lfFaceName
		invoke lstrcpy, ecx, $CTA0("Courier New")
		invoke CreateFontIndirect, addr lf		
		mov	g_hFontNew, eax

		invoke SendMessage, g_hwndEditIdt, WM_SETFONT, g_hFontNew, FALSE

		invoke PrintIdtDump

		; The following four lines is needed because of while processing
		; the WM_INITDIALOG message of a dialog box, sending an EM_SETSEL
		; fails to remove the highlight from (unselect) the edit control text.
		
		; Read Microsoft Knowledge Base Article - 96674
		; "Unselecting Edit Control Text at Dialog Box Initialization"
		; for more info
  
 		invoke SetFocus, g_hwndEditIdt
		invoke PostMessage, g_hwndEditIdt, EM_SETSEL, -1, -1
		xor eax, eax
		ret

	.elseif uMsg == WM_DESTROY

		invoke SendMessage, g_hwndEditIdt, WM_SETFONT, g_hFontOld, FALSE
		invoke DeleteObject, g_hFontNew

	.elseif uMsg == WM_GETMINMAXINFO

		mov ecx, lParam
		mov (MINMAXINFO PTR [ecx]).ptMinTrackSize.x, 380
		mov (MINMAXINFO PTR [ecx]).ptMinTrackSize.y, 150

	.elseif eax == WM_SYSCOMMAND
		.if wParam == IDM_ABOUT
			invoke MessageBox, hDlg, addr szWrittenBy, addr szAbout, MB_OK + MB_ICONINFORMATION
		.endif
		xor eax, eax
		ret

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
		invoke GetFullPathName, $CTA0("IdtDump.sys"), sizeof acModulePath, addr acModulePath, esp
    	pop eax

		invoke CreateService, hSCManager, $CTA0("IdtDump"), addr g_szAppName, \
			SERVICE_START + SERVICE_STOP + DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, \
			SERVICE_ERROR_IGNORE, addr acModulePath, NULL, NULL, NULL, NULL, NULL

		.if eax != NULL
			mov hService, eax

			invoke StartService, hService, 0, NULL
			.if eax != 0

				invoke CreateFile, $CTA0("\\\\.\\IdtDump"), GENERIC_READ, \
										0, NULL, OPEN_EXISTING, 0, NULL

				.if eax != INVALID_HANDLE_VALUE
					mov hDevice, eax

					;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

					invoke DeviceIoControl, hDevice, IOCTL_DUMP_IDT, NULL, 0, \
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

set exe=IdtDump

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
