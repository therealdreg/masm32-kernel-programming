;@echo off
;goto make

.486
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\windows.inc

include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include clash.inc

includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
includelib clash.lib

include \masm32\mProgs\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                         F U N C T I O N S   P R O T O T Y P E S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
g_hConsoleOutput	HANDLE	?
g_pszCommandLine	LPVOID	?


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          start                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc uses esi

	invoke GetCommandLine
	mov g_pszCommandLine, eax
int 3
	; parse command line
	invoke CL_ScanArgsX, g_pszCommandLine

mov eax, CL_argc
	; if '-h' or '-?' was specified print help
	.if eax == 1 || CL_switch['h'] || CL_switch['?']
;		invoke PrintConsole, $CTA0("Usage: lic libname\n")
	.endif

lea eax, CL_argv
lea eax, CL_switch

	.if CL_switch['a']
       movzx eax, CL_switch['a']  ; get arg number of '-o'
       lea eax, [eax*4+4]         ; calculate (eax+1)*4
       mov eax, CL_argv[eax]      ; get pointer to next arg
	.endif

	.if CL_switch['z']
		xor ecx, ecx
	.endif


	ret

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end	start

:make

set exe=clash

\masm32\bin\ml /nologo /c /coff %exe%.bat
\masm32\bin\link /nologo /out:%exe%.exe /subsystem:console /merge:.idata=.text /merge:.rdata=.text /merge:.data=.text /section:.text,EWR /ignore:4078 %exe%.obj

del %exe%.obj

echo.
pause
