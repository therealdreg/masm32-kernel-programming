;@echo off
;goto make

.386
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\windows.inc

include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include \masm32\include\w2k\ntdll.inc
include \masm32\include\w2k\ntstatus.inc
include \masm32\include\winioctl.inc
include clash\clash.inc

includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\w2k\ntdll.lib
includelib clash\clash.lib

;include native.inc
include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         E Q U A T E S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         M A C R O S                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

$invoke MACRO vars:VARARG
     invoke vars
     EXITM <eax>
ENDM

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

PUBLIC g_hConsoleOutput
g_hConsoleOutput	HANDLE	?
;g_pszCommandLine	LPVOID	?

g_fbFlushRemoveableMedia	BOOL	?
g_fbEjectRemoveableMedia	BOOL	?
		

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

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
comment ^
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        PrintStatus                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintStatus proc dwStatus:DWORD

local hModule:HINSTANCE
local pBuffer:LPSTR

	mov hModule, $invoke(GetModuleHandle, $CTA0("ntdll.dll"))

	invoke FormatMessage, \
	FORMAT_MESSAGE_FROM_SYSTEM + FORMAT_MESSAGE_FROM_HMODULE + FORMAT_MESSAGE_IGNORE_INSERTS + FORMAT_MESSAGE_ALLOCATE_BUFFER, \
	hModule, dwStatus, SUBLANG_DEFAULT SHL 10 + LANG_NEUTRAL, addr pBuffer, 0, NULL
	.if eax != 0
		invoke PrintConsole, pBuffer, FOREGROUND_RED
		invoke LocalFree, pBuffer	
	.endif

	ret

PrintStatus endp
^
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       PrintLastError                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintLastError proc

	sub esp, 800h

	invoke GetLastError
	mov ecx, esp
	invoke FormatMessage, FORMAT_MESSAGE_FROM_SYSTEM, NULL, eax, \
					SUBLANG_DEFAULT SHL 10 + LANG_NEUTRAL, ecx, 800h, NULL
	.if eax != 0
		mov ecx, esp
		invoke PrintConsole, ecx, FOREGROUND_RED + FOREGROUND_INTENSITY
	.else
		invoke PrintConsole, $CTA0("Sorry. Error number not found."), \
						FOREGROUND_RED + FOREGROUND_INTENSITY
	.endif
 
	add esp, 800h

	ret

PrintLastError endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        PrintLogo                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintLogo proc

	invoke PrintConsole, $CTA0("\nSync 1.0"), FOREGROUND_BLUE + FOREGROUND_GREEN	;FOREGROUND_GREEN + FOREGROUND_INTENSITY

	invoke PrintConsole, \
	$CTA0(" - Disk Flusher\n"), FOREGROUND_BLUE + FOREGROUND_GREEN

	invoke PrintConsole, \
	$CTA0("Copyright (C) 2004, Four-F ( four-f@mail.ru )\n\n"), FOREGROUND_BLUE + FOREGROUND_GREEN

	ret

PrintLogo endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       PrintUsage                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintUsage proc

;local csbi:CONSOLE_SCREEN_BUFFER_INFO

CTA		"  -r     flush removeable media\n", szUsageOptions
CTA		"  -e     eject removeable media\n"
CTA		"Specifying explicit drive letters will flush only those drives\n"
CTA		"\n"
CTA		"Example:\n"
CTA		"  sync -r ace     flushes drives a, c, e\n"
CTA0	"  sync            flushes all fixed drives\n"

	;invoke GetConsoleScreenBufferInfo, g_hConsoleOutput, addr csbi
	invoke PrintConsole, $CTA0("Usage: sync [-r | -e | drive letters]\n\n"), 0

	;invoke SetConsoleTextAttribute, g_hConsoleOutput, FOREGROUND_BLUE + FOREGROUND_INTENSITY
	invoke PrintConsole, $CTA0("Options:\n"), 0

	;movzx eax, csbi.wAttributes
	;invoke SetConsoleTextAttribute, g_hConsoleOutput, eax

	invoke PrintConsole, addr szUsageOptions, 0

	ret

PrintUsage endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  InitCommandSwitches                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

InitCommandSwitches proc

	and g_fbFlushRemoveableMedia, FALSE
	.if CL_switch['r'] || CL_switch['R']
		mov g_fbFlushRemoveableMedia, TRUE
	.endif

	and g_fbEjectRemoveableMedia, FALSE
	.if CL_switch['e'] || CL_switch['E']
		mov g_fbEjectRemoveableMedia, TRUE
	.endif

	ret

InitCommandSwitches endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     EjectMedia                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EjectMedia proc hVolume:HANDLE

local cb:DWORD
local fOk:BOOL

	and fOk, FALSE

	invoke DeviceIoControl, hVolume, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, addr cb, NULL
	.if eax != 0

		invoke DeviceIoControl, hVolume, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, addr cb, NULL
		.if eax != 0

			invoke DeviceIoControl, hVolume, IOCTL_DISK_EJECT_MEDIA, NULL, 0, NULL, 0, addr cb, NULL
			.if eax != 0
				mov fOk, TRUE
			.else
				invoke PrintLastError
			.endif
		.else
			invoke PrintLastError
		.endif
	.else
		invoke PrintLastError
	.endif

	mov eax, fOk
	ret

EjectMedia endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        FlushVolume                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FlushVolume proc dwDriveLetter:DWORD, fbEject:BOOL

local hVolume:HANDLE
local buffer[32]:CHAR

	mov eax, dwDriveLetter
	and eax, 0FFh
	invoke wsprintf, addr buffer, $CTA0("\\\\.\\%c:"), eax

	invoke CreateFile, addr buffer, GENERIC_READ + GENERIC_WRITE,
			FILE_SHARE_READ + FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL
	.if eax != INVALID_HANDLE_VALUE

		mov hVolume, eax

		mov eax, dwDriveLetter
		and eax, 0FFh
		invoke wsprintf, addr buffer, $CTA0("Flushing drive %c\n"), eax
		invoke PrintConsole, addr buffer, 0

		invoke FlushFileBuffers, hVolume
		.if eax != 0
			.if fbEject
				invoke EjectMedia, hVolume
				.if eax == TRUE
					mov eax, dwDriveLetter
					and eax, 0FFh
					invoke wsprintf, addr buffer, $CTA0("Drive %c ejected\n"), eax
					invoke PrintConsole, addr buffer, 0					
				.endif
			.endif
		.else
			invoke PrintLastError
		.endif
		invoke ZwClose, hVolume

	.else
		invoke PrintLastError
	.endif

	ret

FlushVolume endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         FlushAll                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FlushAll proc uses esi edi ebx
	
	mov esi, $invoke(GetLogicalDrives)

	xor ebx, ebx
	.while ebx < 32					; I know they are 26 :)

		.if esi & 1
			mov eax, ebx
			add eax, 'A'
			mov ah, ':'
			push eax
			invoke GetDriveType, esp
			pop ecx
			mov ecx, ebx
			add ecx, 'A'
			.if eax == DRIVE_REMOVABLE && g_fbFlushRemoveableMedia
				invoke FlushVolume, ecx, g_fbEjectRemoveableMedia
			.elseif eax == DRIVE_FIXED
				invoke FlushVolume, ecx, FALSE
			.endif
                
		.endif

		shr esi, 1		; Next drive
		inc ebx
	.endw
		
	ret

FlushAll endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          start                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc uses ebx

local buffer[128]:BYTE 

	invoke GetStdHandle, STD_OUTPUT_HANDLE
	.if eax != INVALID_HANDLE_VALUE
		mov g_hConsoleOutput, eax

		; parse command line
		invoke CL_ScanArgsX, $invoke(GetCommandLine)

		; if '-h' or '-?' was specified print help
		.if CL_switch['h'] || CL_switch['H'] || CL_switch['?']
			invoke PrintLogo
			invoke PrintUsage
		.else

			invoke InitCommandSwitches
			invoke PrintLogo

			; get pointer to list of drives to flush if any
			mov eax, CL_argc
			dec eax
			.if !ZERO?
				shl eax, 2
				mov ebx, CL_argv[eax]
				.if byte ptr [ebx] != '-' && byte ptr [ebx] != '/'
					; Seems edx points to list of drives to flush

					.while byte ptr [ebx] != 0

						xor eax, eax
						mov al, [ebx]
						and al, 11011111y		; To upper case
						cmp al, "A"
						jb @F
						cmp al, "Z"
						ja @F
	
						mov ah, ':'

						push eax
						invoke GetDriveType, esp
						pop ecx

						xor ecx, ecx
						mov cl, [ebx]
						and cl, 11011111y		; To upper case
	
						.if eax == DRIVE_REMOVABLE && g_fbFlushRemoveableMedia
							invoke FlushVolume, ecx, g_fbEjectRemoveableMedia
						.elseif eax == DRIVE_FIXED
							invoke FlushVolume, ecx, FALSE
						.endif
					@@:
						inc ebx
					.endw
	
				.else
					; list of drives to flush not specified - flush all
					invoke FlushAll					
				.endif
			.else
				invoke FlushAll
			.endif
				
		.endif
	.endif

	invoke PrintConsole, $CTA0("\n"), 0

	xor eax, eax
	ret

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end	start

:make

set exe=Sync

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
\masm32\bin\link /nologo /out:%exe%.exe /subsystem:console /merge:.idata=.text /merge:.rdata=.text /merge:.data=.text /section:.text,EWR /ignore:4078 %exe%.obj rsrc.obj

del %exe%.obj

echo.
pause
exit
