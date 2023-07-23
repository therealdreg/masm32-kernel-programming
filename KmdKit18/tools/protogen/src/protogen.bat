;@echo off
;goto make

echo
	echo -----------------
IFDEF DEBUG
	echo | DEBUG Build   |
ELSE
	echo | RELEASE Build |
ENDIF
	echo -----------------
echo

.486
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\windows.inc

include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
;include \masm32\include\masm32.inc
include clash\clash.inc

includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
;includelib \masm32\lib\masm32.lib
includelib clash\clash.lib

include \masm32\Macros\Strings.mac

include Macros.mac
include seh.inc
include string.asm
include atodw.asm
include memory.asm

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;             G L O B A L   V A R I A B L E S   F O R   D E B U G   P U R P O S E S                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;GENERATE_INVOKE		equ 1

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                         F U N C T I O N S   P R O T O T Y P E S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

strcmp proto C :LPSTR, :LPSTR
externdef strcmp:proc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          M A C R O S                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		
CC MACRO cc:REQ
	IFNDEF CC_CUR
		CC_CUR = -1		; initialize counter
	ENDIF
	CC_CUR = CC_CUR + 1
	cc equ CC_CUR
ENDM

NewLine MACRO p:REQ
	IF (OPATTR (p)) AND 00010000y
		;; Is a register value
		mov word ptr [p], 0A0Dh
		add p, 2
	ELSE
		mov eax, p
		mov word ptr [eax], 0A0Dh
		add p, 2
	ENDIF
ENDM

il_strlen MACRO s:REQ
	;; fast inline strlen
	IF (OPATTR (s)) AND 00010000y
		;; Is a register value
		IFDIFI <s>, <ecx>
			mov ecx, s
		ENDIF
	ELSE
		mov ecx, s
	ENDIF

	xor	eax, eax
@@:	mov	dl, [ecx+eax]
	inc	eax
	or	dl, dl
	jnz	short @B
	dec	eax

ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         E Q U A T E S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MAX_NUM_OF_PARAMS	equ 256

PAGE_SIZE		equ 1000h
PAGE_SHIFT		equ 12

;:::::::::: Define Calling Conventions ::::::::::
CC CC_CDECL
CC CC_STDCALL
;CC CC_ANSI
;CC CC_UNICODE
CC CC_FASTCALL
;CC CC_VARIABLE		; it's not s calling convention of course
;CC CC_PASCAL
;CC CC_THISCALL
;CC CC_NAKED
IFDEF DEBUG
CC CC_WEIRD
ENDIF

CC_NUM		= CC_CUR + 1	; The number of calling conventions types we are goinng to track


IMPORT_CODE				equ 0	; The import is executable code.
IMPORT_DATA				equ	1	; The import is data.
IMPORT_CONST			equ 2	; The import was specified as CONST in the .def file.

IMPORT_ORDINAL			equ 0	; The import is by ordinal. This indicates that the value in the Ordinal/Hint field of the Import Header is the import's ordinal. If this constant is not specified, then the Ordinal/Hint field should always be interpreted as the import's hint.
IMPORT_NAME				equ 1	; The import name is identical to the public symbol name.
IMPORT_NAME_NOPREFIX	equ 2	; The import name is the public symbol name, but skipping the leading ?, @, or optionally _.
IMPORT_NAME_UNDECORATE	equ 3	; The import name is the public symbol name, but skipping the leading ?, @, or optionally _, and truncating at the first @.


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    S T R U C T U R E S                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

comment ^
SYM_FLAGS_AND_INDEX RECORD \
	symANSI:1,			; 31 - symbol has 'W' at the very end of its name
	symUNIC:1,			; 30 - symbol has 'A' at the very end of its name
	symCDecl:1,			; 29
	symStdCall:1,		; 28
	symFastCall:1,		; 27
	symPaskal:1,		; 26
	symThisCall:1,		; 25
	symNaked:1,			; 24
	symWeird:1,			; 23
	symVariable:1,		; 22
	sym_Imp:1,			; 21 - symbol has prepending '__imp_'
	symReserved:1,		; 20
	symIndex:20			; 0-19 - hope 20 bits (1048575) is enough for index
						; very big chances that libraries never will grow that big

;   31  30  29  28  27  26  25  24  23  22  21  20  19                                           0  
; +---+---+---+---+---+---+---+---+---+---+---+---+------------------------------------------------+
; | A | W | C | S | F | P | T | N | R | V | I | - | Index - of symbol in the library               |
; +---+---+---+---+---+---+---+---+---+---+---+---+------------------------------------------------+

SYM_ENTRY STRUCT
	pSymbolName			LPSTR				?		; VA to symbol name.

	; if we will need more place for flags it's possible to use 4 most significant bits of this field
	; but then we must work with RVA (from start of library file)
	; i guess it's hard to find the library of size 268'435'455 bytes. So 28 bits for RVA should be enough
	; if we decide to work this way it must be consider in the sorce code of course

	union
		dwFlagsAndIndex	DWORD				?
						SYM_FLAGS_AND_INDEX	<>
	ends
SYM_ENTRY ENDS
^

comment ^
SYM_FLAGS_AND_INDEX RECORD \
	symANSI:1,			; 31 - symbol has 'W' at the very end of its name
	symUNIC:1,			; 30 - symbol has 'A' at the very end of its name
	sym__imp_:1,		; 29 - symbol has prepending '__imp_'
	symReserved:9,		; 20-28
	symIndex:20			; 0-19 - hope 20 bits (1048575) is enough for index
						; very big chances that libraries never will grow that big

;   31  30  29  28  27  26  25  24  23  22  21  20  19                                           0  
; +---+---+---+---+---+---+---+---+---+---+---+---+------------------------------------------------+
; | A | W | I | - | - | - | - | - | - | - | - | - | Index - of symbol in the library               |
; +---+---+---+---+---+---+---+---+---+---+---+---+------------------------------------------------+

SYM_ENTRY STRUCT
	pSymbolName			LPSTR				?		; VA to symbol name.

	; if we will need more place for flags it's possible to use 4 most significant bits of this field
	; but then we must work with RVA (from start of library file)
	; i guess it's hard to find the library of size 268'435'455 bytes. So 28 bits for RVA should be enough
	; if we decide to work this way it must be consider in the sorce code of course

	union
		dwFlagsAndIndex	DWORD				?
						SYM_FLAGS_AND_INDEX	<>
	ends
SYM_ENTRY ENDS
PSYM_ENTRY typedef PTR SYM_ENTRY

^
comment ^
SYM_FLAGS_AND_INDEX RECORD \
	symANSI:1,			; 31 - symbol has 'A' at the very end of its name
	symUNIC:1,			; 30 - symbol has 'W' at the very end of its name
	sym__imp_:1,		; 29 - symbol has prepending '__imp_'
	symReserved:9,		; 20-28
	symIndex:20			; 0-19 - hope 20 bits (1048575) is enough for index
						; very big chances that libraries never will grow that big

;   31  30  29  28  27  26  25  24  23  22  21  20  19                                           0  
; +---+---+---+---+---+---+---+---+---+---+---+---+------------------------------------------------+
; | A | W | I | - | - | - | - | - | - | - | - | - | Index - of symbol in the library               |
; +---+---+---+---+---+---+---+---+---+---+---+---+------------------------------------------------+

SYM_ENTRY STRUCT
	pSymbolName			LPSTR				?		; VA to symbol name.

	; if we will need more place for flags it's possible to use 4 most significant bits of this field
	; but then we must work with RVA (from start of library file)
	; i guess it's hard to find the library of size 268'435'455 bytes. So 28 bits for RVA should be enough
	; if we decide to work this way it must be consider in the sorce code of course

	union
		dwFlagsAndIndex	DWORD				?
						SYM_FLAGS_AND_INDEX	<>
	ends
SYM_ENTRY ENDS
PSYM_ENTRY typedef PTR SYM_ENTRY
^

FF_SYM_ANSI					equ 00000001			; symbol has 'A' at the very end of its name
FF_SYM_UNICODE				equ 00000002			; symbol has 'W' at the very end of its name
FF_SYM___IMP_				equ 00000004			; symbol has prepending '__imp_'
FF_SYM_VARIABLE				equ 00000008			; symbol is possibly a variable not a function



SYM_ENTRY STRUCT
													;       _StrFormatByteSize64A@16
													; __imp__DdeCmpStringHandles@8
													;       ^
													;       |
													;       +------------+
													;                    |
	pSymbolName					LPSTR		?		; VA to symbol name -+

													; _StrFormatByteSize64A@16
													;  ^
													;  |
													;  +----------------------------------------------+
													;                                                 |
	pUndecSymbolNameStart		LPSTR		?		; VA to the beginning of undecorated symbol name -+

													; _StrFormatByteSize64A@16
													;  \________  ________/
													;           \/
	ccUndecSymbolNameLength		UINT		?		; num of chars of undecorated symbol name.

	uNumberOfParameters			UINT		?		; Number of accepting by function parameters or -1 if not recognized
													; For _StrFormatByteSize64A@16 uNumberOfParameters = 16/4 = 4

	uIndex						UINT		?		; Index of symbol in the library

	pMemberHeader				LPSTR		?		; VA to symbol's IMAGE_ARCHIVE_MEMBER_HEADER

	dwFlags						DWORD		?		; FF_SYM_XXX

	byImportType				BYTE		?		; IMPORT_CODE, IMPORT_DATA, IMPORT_CONST
	byImportNameType			BYTE		?		; IMPORT_ORDINAL, IMPORT_NAME, IMPORT_NAME_NOPREFIX, IMPORT_NAME_UNDECORATE
	
	wReserved					WORD		?

SYM_ENTRY ENDS
PSYM_ENTRY typedef PTR SYM_ENTRY


CC_ENTRY STRUCT
	uNumEntries	UINT		?		; Number of the SYM_ENTRY in the array
	pSymEntries	PSYM_ENTRY	?		; Points to the beginning of the SYM_ENTRYs array of appropriate CC
CC_ENTRY ENDS
PCC_ENTRY typedef PTR CC_ENTRY



UNDECOR_SYMBOL STRUCT
	; Dont' change this structure
	pUndecName		LPSTR	?
	uUndecLenght	UINT	?
UNDECOR_SYMBOL ENDS
PUNDECOR_SYMBOL typedef PTR UNDECOR_SYMBOL

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
CTA0 ".lib", g_szLibExtension, 4

CTA0 "v1.0", g_szVersion, 4

IFDEF DEBUG
	CTA0 "\nWarning: %s\n\n", g_szDbgWeirdSymbol, 4
;	CTA0 "\nWarning: Too many params %s\n", g_szWeirdNumOfParams, 4
	CTA0 "\nWarning: Corresponding unicode symbol for %s was not found\n\n", g_szDbgUnicSymbolNotFound, 4
ENDIF

CTA0 "\nWarning: argument count of %s is not devisible by 4.\n\n", g_szArgCountNotDivBy4, 4

CTA ";:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n", g_szCommentedLine, 4
CTA "\t", g_szTab, 4
CTA " equ \[", g_szEqu, 4
;CTA "\n", g_szNewLine
CTA "IFDEF UNICODE\n", g_szIfDefUnicode, 4
CTA "ELSE\n", g_szElseUnicode, 4
CTA "ENDIF\n", g_szEndIfUnicode, 4
CTA "\t; possible exported variable", g_szExportedVariable, 4
CTA0 "__imp_", g_sz__imp_, 4

IFDEF GENERATE_INVOKE
CTA "invoke ", g_szInvoke, 4
CTA ", 0", g_szZero, 4
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              I N I T I A L I Z E D  D A T A                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
;g_hConsoleInput		HANDLE ?
PUBLIC g_hConsoleOutput
g_hConsoleOutput	HANDLE	?
g_pszCommandLine	LPVOID	?

g_hLibraryFile		HANDLE	?
g_hLibraryMapping	HANDLE	?
g_pLibraryImage		LPVOID	?

;g_hIncludeFile		HANDLE	?
;g_hIncludeMapping	HANDLE	?
g_pInclude			LPVOID	?
g_cbInclude			UINT	?
g_pIncludeCurrent	LPVOID	?


g_pSymEntries		LPVOID	?
g_cbSymEntries		UINT	?



g_uNumberOfMembers	UINT	?	; Unsigned long containing the number of archive members.
g_uNumberOfSymbols	UINT	?	; Unsigned long containing the number of symbols indexed.
								; Each object-file member typically defines one or more external symbols.


g_uNumOfVariables	UINT	?


align 4
; Array of CC_ENTRYs. Each CC_ENTRY correspond to particular calling convention type we are tracking
g_paCcEntries		db (sizeof CC_ENTRY) * CC_NUM dup(?)

;g_apPointers		db (sizeof CC_ENTRY) * CC_NUM dup(?)

align 4
g_acLibraryPath		CHAR MAX_PATH	dup(?)

IFDEF DEBUG
	g_acDebugMessage	CHAR 256 dup(?)
ENDIF

g_fSortUndecorated	DWORD	?

; Command line switches
; 'm' masm, 'f' fasm, 't' tasm, 'n' nasm
g_keyIncludeType	CHAR	?
g_keySort			CHAR	?
g_keyDirectImport	BYTE	?	; suppress jump table
;g_keyNoLogo			BYTE	?	; suppress logo printing

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
		invoke fstrlen, psz
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
;                                IsLibExtensionSpecified                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IsLibExtensionSpecified proc uses esi ebx pLibraryPath:LPSTR

	_mov ebx, FALSE	; assume error

	mov esi, pLibraryPath
	.if esi != NULL
		invoke fstrlen, esi
		add esi, eax
		sub esi, 4			; ".lib"
		invoke lstrcmpi, addr g_szLibExtension, esi
		.if eax == 0
			inc ebx		; OK
		.endif
	.endif

	return ebx

IsLibExtensionSpecified endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  IsFullPathSpecified                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IsFullPathSpecified proc uses esi pLibraryPath:LPSTR

	mov esi, pLibraryPath
	mov ecx, $invoke(fstrlen, esi)
	; search for back slash
    .while ecx
	    mov al, [esi]
    	.break .if al == '\'
	    inc esi
	    dec ecx
	.endw

	return ecx

IsFullPathSpecified endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       CloseLibrary                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CloseLibrary proc

	.if g_pLibraryImage != NULL
		invoke UnmapViewOfFile, g_pLibraryImage
		_mov g_pLibraryImage, NULL
	.endif

	.if g_hLibraryMapping != NULL
		invoke CloseHandle, g_hLibraryMapping
		_mov g_hLibraryMapping, NULL
	.endif

	.if g_hLibraryFile != INVALID_HANDLE_VALUE
		invoke CloseHandle, g_hLibraryFile
		_mov g_hLibraryFile, INVALID_HANDLE_VALUE
	.endif

	ret

CloseLibrary endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       OpenLibrary                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OpenLibrary proc uses ebx pszLibraryName:LPSTR

	and ebx, FALSE		; clear flag OK

	invoke CreateFile, pszLibraryName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL
	.if eax != INVALID_HANDLE_VALUE
		mov g_hLibraryFile, eax
		invoke CreateFileMapping, g_hLibraryFile, NULL, PAGE_READONLY, 0, 0, NULL
		.if eax != NULL
			mov g_hLibraryMapping, eax
			invoke MapViewOfFile, g_hLibraryMapping, FILE_MAP_READ, 0, 0, 0
			.if eax != NULL
				mov g_pLibraryImage, eax
				inc ebx				; set flag OK
			.endif
		.endif
	.endif

	.if ebx == FALSE
		invoke CloseLibrary
	.endif

	return ebx

	ret

OpenLibrary endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     FindAtBackward                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FindAtBackward proc pString:LPSTR

; returns zero-based position of '@'

option PROLOGUE:NONE
option EPILOGUE:NONE

Fix It may be buggy if @ symbol is first character and symbol name contains no more @ chars

	invoke fstrlen, [esp+4]
	mov edx, [esp+4]

	mov cl, '@'
	.while eax
		dec eax
		.break .if byte ptr [edx][eax] == cl
	.endw

;	.if ZERO?
;		inc eax		; '@' found
;	.endif

	ret (sizeof DWORD)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

FindAtBackward endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       IsNullImport                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IsNullImport proc pSymbol:LPSTR

option PROLOGUE:NONE
option EPILOGUE:NONE

Fix InString strstr

;	assume eax:SDWORD
	mov eax, [esp+4]
	invoke InString, 1, eax, $CTA0("NULL_THUNK_DATA")
	.if sdword ptr eax > 0
		ret (sizeof DWORD)
	.endif

	mov eax, [esp+4]
	invoke InString, 1, eax, $CTA0("IMPORT_DESCRIPTOR")	
	.if sdword ptr eax > 0
		ret (sizeof DWORD)
	.endif

;	assume eax:nothing

;Fix
;	.if $invoke(InString, 1, pSymbol, $CTA0("NULL_IMPORT_DESCRIPTOR"))
;		inc ebx
;	.endif

	xor eax, eax
	ret (sizeof DWORD)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

IsNullImport endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                             ConvertNumOfParamsFromSymbol                                          
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ConvertNumOfParamsFromSymbol proc uses esi ebx pszSymbolName:LPSTR

; Returns number of parameters for this symbol or -1 if error

local acMessage[512]:CHAR

	or ebx, -1					; assume error
	mov esi, pszSymbolName

	invoke FindAtBackward, esi
	.if eax
		add eax, esi
		inc eax
		invoke atodw, eax

;		mov ecx, eax
;		and ecx, 011y
		.if !(eax & 011y)	; is argument count is not devisible by 4 ?
			shr eax, 2		; /sizeof arg = num of params
			.if eax < MAX_NUM_OF_PARAMS
				mov ebx, eax
IFDEF DEBUG
			.else
				; Too many params
				invoke wsprintf, addr g_acDebugMessage, $CTA0("\nWarning: Too many params %s\n\n"), esi
				invoke PrintConsole, addr g_acDebugMessage, 0
ENDIF
			.endif
		.else
			invoke wsprintf, addr acMessage, addr g_szArgCountNotDivBy4, esi
			invoke PrintConsole, addr acMessage, 0
		.endif

	.endif

	mov eax, ebx
	ret

ConvertNumOfParamsFromSymbol endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     GetImportType                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetImportType proc pMemberHeader:PTR IMAGE_ARCHIVE_MEMBER_HEADER

option PROLOGUE:NONE
option EPILOGUE:NONE

; see 8.2. Import Type of Microsoft PExecutable and COFF Specification

	or eax, -1								; assume error
	mov ecx, [esp+4]
	add ecx, sizeof IMAGE_ARCHIVE_MEMBER_HEADER	; eax -> IMPORT_OBJECT_HEADER
	.if word ptr [ecx] == IMAGE_FILE_MACHINE_UNKNOWN		; Sig1	Must be IMAGE_FILE_MACHINE_UNKNOWN.
															; See Section 3.3.1, "Machine Types, " for more information.
		.if word ptr [ecx+2] == 0FFFFh						; Sig2	Must be 0xFFFF.
			movzx eax, (IMPORT_OBJECT_HEADER PTR [ecx]).rImport
			; ImportRec RECORD Reserved:11, NameType:3, Type2:2
			and eax, mask Type2		; return Type
		.endif
	.endif

	ret (sizeof DWORD)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

GetImportType endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   GetImportNameType                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetImportNameType proc pMemberHeader:PTR IMAGE_ARCHIVE_MEMBER_HEADER

option PROLOGUE:NONE
option EPILOGUE:NONE

; see 8.3. Import Name Type of Microsoft PExecutable and COFF Specification

	or eax, -1								; assume error
	mov ecx, [esp+4]
	add ecx, sizeof IMAGE_ARCHIVE_MEMBER_HEADER	; eax -> IMPORT_OBJECT_HEADER
	.if word ptr [ecx] == IMAGE_FILE_MACHINE_UNKNOWN		; Sig1	Must be IMAGE_FILE_MACHINE_UNKNOWN.
															; See Section 3.3.1, "Machine Types, " for more information.
		.if word ptr [ecx+2] == 0FFFFh						; Sig2	Must be 0xFFFF.
			movzx eax, (IMPORT_OBJECT_HEADER PTR [ecx]).rImport
			; ImportRec RECORD Reserved:11, NameType:3, Type2:2
			and eax, mask NameType
			shr eax, 2	; return NameType
		.endif
	.endif

	ret (sizeof DWORD)

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

GetImportNameType endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      FillSymEntries                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FillSymEntries proc uses ebx esi edi p2ndLinkerMember:LPVOID, pLibraryArrayOfSymbols:LPVOID, uNumberOfSymbols:UINT

local fOk:BOOL
local dwSymFlags:DWORD
local uNumberOfMembers:UINT
local paOffsetsToMembers:LPVOID
local paIndices:LPVOID
local dwCcType:DWORD
local uNumberOfParameters:UINT
local uAtPosition:UINT					; from the beginning of the string
local ccUndecSymbolNameLength:UINT
local pUndecSymbolNameStart:LPSTR
local uIndex:UINT
local pMemberHeader:LPVOID			; PTR IMAGE_ARCHIVE_MEMBER_HEADER
local byImportType:BYTE				; IMPORT_CODE, IMPORT_DATA, IMPORT_CONST
local byImportNameType:BYTE			; IMPORT_ORDINAL, IMPORT_NAME, IMPORT_NAME_NOPREFIX, IMPORT_NAME_UNDECORATE

	; Walks through String Table pointed by	pLibraryArrayOfSymbols.
	; String Table is a series of null-terminated strings that name all the symbols in the directory.
	; Each string begins immediately after the null byte in the previous string.

	; So we walks through it trying recognize the calling conversion of each symbol
	; And we fill SYM_ENTRYs

	and fOk, 0						; clear flag OK
	mov eax, p2ndLinkerMember

	push dword ptr [eax]
	pop uNumberOfMembers

	add eax, 4						; skip m = Number of Members
	mov paOffsetsToMembers, eax		; paOffsetsToMembers points to the first member address

	mov ecx, uNumberOfMembers
	shl ecx, 2						; sizeof pointer

	add eax, ecx					; eax -> Number of Symbols
	add eax, 4						; skip m = Number of Symbols
	mov paIndices, eax

	_try

	mov esi, pLibraryArrayOfSymbols
	assume esi:ptr CHAR
	lea edi, g_paCcEntries
	assume edi:ptr CC_ENTRY
	xor ebx, ebx
	.while ebx < uNumberOfSymbols

		; Skip _IMPORT_DESCRIPTOR_XXX, _NULL_IMPORT_DESCRIPTOR, XXX_NULL_THUNK_DATA etc...
		.if $invoke(IsNullImport, esi)
IFDEF DEBUG
			mov dwCcType, CC_WEIRD
			invoke wsprintf, addr g_acDebugMessage, $CTA0("Skip: %s\n"), esi
			invoke PrintConsole, addr g_acDebugMessage, 0
ENDIF
			jmp @F
		.endif

		Fix May be init dwCcType with CC_WEIRD
		or dwCcType, -1					; init calling convention type
		and dwSymFlags, 0							; reset
		or uNumberOfParameters, -1					; reset to not recognized
		and ccUndecSymbolNameLength, 0				; init with 0
		and pUndecSymbolNameStart, NULL
;		or dwImportType, 0							; init with IMPORT_CODE

		mov ecx, ebx
		shl ecx, 1					; * sizeof Indice
		add ecx, paIndices
		movzx ecx, word ptr [ecx]
		mov uIndex, ecx

		; VA to symbol's IMAGE_ARCHIVE_MEMBER_HEADER
		dec ecx						; make it zero-based
		shl ecx, 2					; * sizeof pointer
		add ecx, paOffsetsToMembers
		mov ecx, [ecx]
		add ecx, g_pLibraryImage
		mov pMemberHeader, ecx


		invoke GetImportType, pMemberHeader
		mov byImportType, al
		.if ( eax == IMPORT_OBJECT_DATA ) || ( eax == IMPORT_OBJECT_CONST )
			or dwSymFlags, FF_SYM_VARIABLE
		.endif


		invoke GetImportNameType, pMemberHeader
		mov byImportNameType, al


		invoke InString, 1, esi, addr g_sz__imp_
		.if eax == 1
			or dwSymFlags, FF_SYM___IMP_
			add esi, sizeof g_sz__imp_ - 1			; skip '__imp_'
		.endif


		invoke ConvertNumOfParamsFromSymbol, esi
		mov uNumberOfParameters, eax


		.if [esi] == '@'							; is it fastcall here ?
			invoke FindAtBackward, esi
			.if eax

				mov ecx, eax
				dec eax
				mov ccUndecSymbolNameLength, eax

				mov eax, esi
				inc eax
				mov pUndecSymbolNameStart, eax

				mov dwCcType, CC_FASTCALL
IFDEF DEBUG
			.else
				mov dwCcType, CC_WEIRD
ENDIF
			.endif

		.elseif [esi] == '_'						; is it stdcall or cdecl here ?
			invoke FindAtBackward, esi
			mov uAtPosition, eax
			.if eax
				; stdcall here

				mov ecx, eax
				dec eax
				mov ccUndecSymbolNameLength, eax

				mov dwCcType, CC_STDCALL
			.else
				; cdecl here

				invoke fstrlen, esi
				dec eax
				mov ccUndecSymbolNameLength, eax

				mov dwCcType, CC_CDECL

comment ^

				g_NumOfVariables

				; cdecl here
				; les't decide is it a function or exported variable
				mov eax, pOffsets
				mov edx, ebx
				shl edx, 2
				add eax, edx

				mov edx, [eax]
				bswap edx				; edx -> IMAGE_ARCHIVE_MEMBER_HEADER
				mov eax, g_pLibraryImage
				add eax, edx
				add eax, sizeof IMAGE_ARCHIVE_MEMBER_HEADER	; eax -> IMPORT_OBJECT_HEADER
				assume eax:ptr IMPORT_OBJECT_HEADER
				mov dx, [eax].rImport
				assume eax:nothing

				and edx, mask Type2
				.if edx == IMPORT_OBJECT_CODE
					; The import is executable code.
					mov dwCcType, CC_CDECL
				.elseif edx == IMPORT_OBJECT_DATA
					; The import is data.
					mov dwCcType, CC_VARIABLE
				.elseif edx == IMPORT_OBJECT_CONST
					; The import was specified as CONST in the .def file.
					mov dwCcType, CC_VARIABLE
				.else
					IFDEF DEBUG
						mov dwCcType, CC_WEIRD
;					ELSE
;						_mov dwCcType, -1	; ???
					ENDIF
				.endif		
^
			.endif

			mov eax, esi
			inc eax
			mov pUndecSymbolNameStart, eax

			mov eax, uAtPosition
			.if [esi][eax][-1] == 'A'		; ANSI or Unicode ?
				or dwSymFlags, FF_SYM_ANSI
			.elseif [esi][eax][-1] == 'W'
				or dwSymFlags, FF_SYM_UNICODE
			.endif
IFDEF DEBUG
		.else
			mov dwCcType, CC_WEIRD
ENDIF
		.endif

		mov eax, dwCcType
		.if eax != -1

			; Fill in SYM_ENTRY for this symbol

			mov ecx, [edi][eax*(sizeof CC_ENTRY)].uNumEntries
			inc [edi][eax*(sizeof CC_ENTRY)].uNumEntries		; num = num + 1
			mov edx, [edi][eax*(sizeof CC_ENTRY)].pSymEntries	; edx -> first SYM_ENTRY of particular cc type

			mov eax, ecx
			imul eax, (sizeof SYM_ENTRY)


			mov (SYM_ENTRY PTR [edx])[eax].pSymbolName, esi

			mov ecx, pUndecSymbolNameStart
			mov (SYM_ENTRY PTR [edx])[eax].pUndecSymbolNameStart, ecx


			mov ecx, dwSymFlags
			mov (SYM_ENTRY PTR [edx])[eax].dwFlags, ecx

			mov ecx, uIndex
			mov (SYM_ENTRY PTR [edx])[eax].uIndex, ecx

			; VA to symbol's IMAGE_ARCHIVE_MEMBER_HEADER
			mov ecx, pMemberHeader
			mov (SYM_ENTRY PTR [edx])[eax].pMemberHeader, ecx


			; num of chars of undecorated symbol name.
			mov ecx, ccUndecSymbolNameLength			
			mov (SYM_ENTRY PTR [edx])[eax].ccUndecSymbolNameLength, ecx


			; Number of accepting by function parameters or -1 if not recognized
			mov ecx, uNumberOfParameters			
			mov (SYM_ENTRY PTR [edx])[eax].uNumberOfParameters, ecx


			; Import Type
			mov cl, byImportType			
			mov (SYM_ENTRY PTR [edx])[eax].byImportType, cl


			; Import Name Type
			mov cl, byImportNameType			
			mov (SYM_ENTRY PTR [edx])[eax].byImportNameType, cl

		.endif
@@:
		invoke fstrlen, esi
		add esi, eax
		inc esi							; skip terminating zero
		inc ebx							; next symbol
	.endw

	inc fOk

	_finally

	assume esi:nothing
	assume edi:nothing

	return fOk

FillSymEntries endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         strncmp                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

strncmp proc uses esi edi ps1:LPSTR, ps2:LPSTR, n:UINT

	xor eax, eax
	mov esi, ps1
	mov edi, ps2
	mov ecx, n
	repe cmpsb
	.if !ZERO?
		inc eax
	.endif

	ret

strncmp endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       PrintSummary                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintSummary proc uses esi

local acSummary[256]:CHAR

;	CL_argv[1*(sizeof DWORD)]

;	Dump of file hello2.obj
;	invoke PrintConsole, addr acSummary

	lea esi, g_paCcEntries
	assume esi:ptr CC_ENTRY
	xor edx, edx
	xor ecx,  ecx
	.while ecx < sizeof g_paCcEntries/sizeof CC_ENTRY
		add edx, [esi][(sizeof CC_ENTRY)*ecx].uNumEntries
		inc ecx
	.endw

	CTA "\n", g_szSummaryFormat, 4
	CTA "cdecl           : %d\n"
	CTA "stdcall         : %d\n"
;	CTA "stdcall neutral : %d\n"
;	CTA "stdcall ansi    : %d\n"
;	CTA "stdcall unicode : %d\n"
	CTA "fastcall        : %d\n"
	CTA "variables       : %d\n"
	CTA "----------------------\n"
	CTA "total           : %d\n\n\0"

	invoke wsprintf, addr acSummary, addr g_szSummaryFormat, \
		[esi][(sizeof CC_ENTRY)*CC_CDECL].uNumEntries, \
		[esi][(sizeof CC_ENTRY)*CC_STDCALL].uNumEntries, \

		[esi][(sizeof CC_ENTRY)*CC_FASTCALL].uNumEntries, \
		g_uNumOfVariables, \
;		[esi][(sizeof CC_ENTRY)*CC_VARIABLE].uNumEntries, \
		edx

;;		[esi][(sizeof CC_ENTRY)*CC_ANSI].uNumEntries, \
;;		[esi][(sizeof CC_ENTRY)*CC_UNICODE].uNumEntries, \

	invoke PrintConsole, addr acSummary, 0

	IFDEF DEBUG
		invoke wsprintf, addr acSummary, $CTA0("weird           : %d\n\n"), [esi][(sizeof CC_ENTRY)*CC_WEIRD].uNumEntries
		invoke PrintConsole, addr acSummary, 0
	ENDIF

	assume esi:nothing

	ret

PrintSummary endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        PrintLogo                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintLogo proc

.const
szBuiltOn			db " ("
					date
					db ").", 0
cbBuiltOn			equ $-szBuiltOn
.code

	invoke PrintConsole, $CTA0("\nPrototype Generator"), FOREGROUND_GREEN + FOREGROUND_INTENSITY

	invoke PrintConsole, \
	$CTA0(" - Library file to Include file converter.\n"), FOREGROUND_BLUE + FOREGROUND_GREEN

	invoke PrintConsole, addr g_szVersion, FOREGROUND_BLUE + FOREGROUND_GREEN
	invoke PrintConsole, addr szBuiltOn, FOREGROUND_BLUE + FOREGROUND_GREEN

	invoke PrintConsole, \
	$CTA0(" Copyright (C) 2005, Four-F ( four-f@mail.ru )"), FOREGROUND_BLUE + FOREGROUND_GREEN

	invoke PrintConsole, $CTA0("\n\n"), 0

	ret

PrintLogo endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       PrintUsage                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintUsage proc

local csbi:CONSOLE_SCREEN_BUFFER_INFO

CTA  "  -nologo  disable the display of the copyright banner\n", szUsageOptions
CTA  "  libname  library (*.lib) file to convert\n\n"
CTA0 "Example: protogen kernel32.lib\n\n"

	invoke GetConsoleScreenBufferInfo, g_hConsoleOutput, addr csbi
	invoke PrintConsole, $CTA0("Usage: protogen [-nologo] libname\n\n"), 0

	invoke SetConsoleTextAttribute, g_hConsoleOutput, FOREGROUND_BLUE + FOREGROUND_INTENSITY
	invoke PrintConsole, $CTA0("Options:\n"), 0

	movzx eax, csbi.wAttributes
	invoke SetConsoleTextAttribute, g_hConsoleOutput, eax

	invoke PrintConsole, addr szUsageOptions, 0

	ret

PrintUsage endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 MakePathFullIfNeeded                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MakePathFullIfNeeded proc uses esi pLibName:LPSTR, pac:LPVOID, cb:UINT

; pLibName	- library file name as user specified it in command line
; pac		- pointer to buffer to recieve full patch
; cb		- size of the buffer pointed by pac

	mov esi, pLibName

	; add current directory if needed
	Fix check for arg
	invoke IsFullPathSpecified, esi
	.if eax
		invoke lstrcpy, pac, esi
	.else
		push eax
		invoke GetFullPathName, esi, cb, pac, esp
		pop eax
	.endif

	; add .lib extension if needed
	Fix check for arg
	invoke IsLibExtensionSpecified, esi
	.if eax == FALSE
		invoke lstrcat, pac, addr g_szLibExtension
	.endif

	ret

MakePathFullIfNeeded endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    MakeExtensionInc                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MakeExtensionInc proc uses esi edi ebx pPath:LPSTR
    mov esi, pPath
    .if esi != NULL
		invoke fstrlen, esi
		add esi, eax
		sub esi, 4
		mov dword ptr [esi], "cni."
	.endif
	ret
MakeExtensionInc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    GetNameFromPath                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetNameFromPath proc uses esi pPath:LPSTR


	mov esi, pPath
	.if esi != NULL
		mov ecx, $invoke(fstrlen, esi)
;	    lea edx, [esi][eax-6]
	    lea ecx, [eax-6]
		; search for back slash
	    .while ecx
		    mov al, [esi][ecx]
    		.break .if al == '\'
		    dec ecx
		.endw
	.endif

	.if ecx != 0
		lea eax, [esi][ecx]
		inc eax
	.else
		xor eax, eax
	.endif

    ret

GetNameFromPath endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 PrintLogoIfUserWantTo                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PrintLogoIfUserWantTo proc uses ebx

	; Find 'nologo' switch. If not found prints (C) banner

	mov ebx, CL_argc
	dec ebx
	.while ebx
		mov eax, ebx
		shl eax, 2
		mov eax, CL_argv[eax]
		inc eax							; skeep '/' or '-'
		invoke lstrcmpi, eax, $CTA0("nologo")
		.if eax == 0
			; If 'nologo' switch found break
			.break
		.endif
		dec ebx
	.endw

	.if ebx == 0						; not found 'nologo' ?
		invoke PrintLogo				; Print (C) banner
	.endif

	ret

PrintLogoIfUserWantTo endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  InitCommandSwitches                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

InitCommandSwitches proc

	xor eax, eax
	.if CL_switch['i']
		dec eax							; -1
	.endif
	mov g_keyDirectImport, al

	mov g_keyIncludeType, 'm'			; masm by default

	.if CL_switch['t'] || CL_switch['T']
		mov g_keyIncludeType, 't'
	.elseif CL_switch['f'] || CL_switch['F']
		mov g_keyIncludeType, 'f'
	.elseif CL_switch['n'] || CL_switch['N']
		mov g_keyIncludeType, 'n'
	.endif

	; 0 - no sort, 0Ah - sort ascending, 0Dh - sort descending
	and g_keySort, 0
	.if CL_switch['a'] || CL_switch['A']
		mov g_keySort, 0Ah
	.elseif CL_switch['d'] || CL_switch['D']
		mov g_keySort, 0Dh
	.endif

	ret

InitCommandSwitches endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                WipeStringSpacesFromRight                                          
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

WipeStringSpacesFromRight proc ps:LPSTR, cc:UINT

option PROLOGUE:NONE
option EPILOGUE:NONE

	; An archive member header has the format, in which each field is an ASCII text string
	; that is left justified and padded with spaces to the end of the field.
	; There is no terminating null character in any of these fields.

	; This proc will overwrite those padding spaces with zeroes

	mov eax, [esp+4]			; ps - pointer to the string
	mov ecx, [esp+8]			; cc - number of characters in string
	.while ecx
		dec ecx
		.break .if byte ptr [eax][ecx] != ' '
		and byte ptr [eax][ecx], 0
	.endw

	ret (sizeof DWORD)*2

option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF

WipeStringSpacesFromRight endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   GetMaxSymbolNameLenght                                          
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetMaxSymbolNameLenght proc uses esi edi ebx pSymEntries:LPVOID, uNumberOfSymbols:UINT, fdwAnsiUnicode:DWORD


; fdwAnsiUnicode:	-1				Enum all symbols
;					FF_SYM_ANSI		Enum only ANSI symbols
;					FF_SYM_UNICODE	Enum only UNICODE symbols

	and edi, 0

	mov esi, pSymEntries
	assume esi:ptr SYM_ENTRY

	.if fdwAnsiUnicode == -1

		xor ebx, ebx
		.while ebx < uNumberOfSymbols
			mov eax, [esi].ccUndecSymbolNameLength
			.if edi < eax
				mov edi, eax
			.endif
			inc ebx			; next symbol
			add esi, sizeof SYM_ENTRY
		.endw

	.else

		xor ebx, ebx
		.while ebx < uNumberOfSymbols
			mov eax, fdwAnsiUnicode
			.if [esi].dwFlags & eax			; enum only appropriate symbols
				mov eax, [esi].ccUndecSymbolNameLength
				.if edi < eax
					mov edi, eax
				.endif
			.endif
			inc ebx			; next symbol
			add esi, sizeof SYM_ENTRY
		.endw

	.endif

	mov eax, edi
	ret

GetMaxSymbolNameLenght endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      GenerateBanner                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GenerateBanner proc uses esi edi ebx pBanner:LPSTR, cb:UINT

	lea esi, g_szCommentedLine
	mov ebx, sizeof g_szCommentedLine
	mov edi, g_pIncludeCurrent

	NewLine edi

	invoke fCopyMemory, edi, esi, ebx
	add edi, ebx

	invoke fCopyMemory, edi, pBanner, cb
	add edi, cb

	invoke fCopyMemory, edi, esi, ebx
	add edi, ebx

	NewLine edi

	mov g_pIncludeCurrent, edi

	ret

GenerateBanner endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     DbgPrintSymbolsInfo                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
IFDEF DEBUG
DbgPrintSymbolsInfo proc uses esi edi ebx dwCC:DWORD

local buffer[1024]:CHAR
local uNumOfSymbols:UINT
local uPrintedSymbols:UINT
local uLeftMarguinToUndecSymbol:UINT

; Use it to print all symbols of paticular cc

	and uPrintedSymbols, 0

	invoke PrintConsole, $CTA0("\n;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n\n"), 0
	invoke PrintConsole, $CTA0("Indx Header   Flags ImpType Name Type   Pars Symbol\n"), 0

	lea esi, g_paCcEntries
	assume esi:ptr CC_ENTRY
	mov eax, dwCC
	push [esi][eax*(sizeof CC_ENTRY)].uNumEntries
	pop uNumOfSymbols
	mov esi, [esi][eax*(sizeof CC_ENTRY)].pSymEntries
	assume esi:nothing

	assume esi:ptr SYM_ENTRY
	xor ebx, ebx
	.while ebx < uNumOfSymbols

		; Index of symbol in the library
		invoke wsprintf, addr buffer, $CTA0("%04u "), [esi].uIndex
		invoke lstrcpy, addr g_acDebugMessage, addr buffer

		; RVA to symbol's IMAGE_ARCHIVE_MEMBER_HEADER
		mov eax, [esi].pMemberHeader
		sub eax, g_pLibraryImage				; VA -> RVA
		invoke wsprintf, addr buffer, $CTA0("%08X "), eax
		invoke lstrcat, addr g_acDebugMessage, addr buffer

		; FF_SYM_XXX
		mov edi, [esi].dwFlags
		shl edi, 32-4						; used flags

		; FF_SYM_VARIABLE				equ 00000008			; symbol is possibly a variable not a function
		shl edi, 1
		.if CARRY?
			invoke lstrcat, addr g_acDebugMessage, $CTA0("V")
		.else
			invoke lstrcat, addr g_acDebugMessage, $CTA0("-")
		.endif

		; FF_SYM___IMP_				equ 00000004			; symbol has prepending '__imp_'
		shl edi, 1
		.if CARRY?
			invoke lstrcat, addr g_acDebugMessage, $CTA0("I")
		.else
			invoke lstrcat, addr g_acDebugMessage, $CTA0("-")
		.endif

		; FF_SYM_UNICODE				equ 00000002			; symbol has 'W' at the very end of its name
		shl edi, 1
		.if CARRY?
			invoke lstrcat, addr g_acDebugMessage, $CTA0("W")
		.else
			invoke lstrcat, addr g_acDebugMessage, $CTA0("-")
		.endif

		; FF_SYM_ANSI					equ 00000001			; symbol has 'A' at the very end of its name
		shl edi, 1
		.if CARRY?
			invoke lstrcat, addr g_acDebugMessage, $CTA0("A")
		.else
			invoke lstrcat, addr g_acDebugMessage, $CTA0("-")
		.endif

		; Import Type
comment ^
IMPORT_CODE	0	The import is executable code.
IMPORT_DATA	1	The import is data.
IMPORT_CONST	2	The import was specified as CONST in the .def file.

ImpType 
CODE
DATA
CONST
^
		.if [esi].byImportType == IMPORT_CODE
			invoke lstrcat, addr g_acDebugMessage, $CTA0("  CODE    ")
		.elseif [esi].byImportType == IMPORT_DATA
			invoke lstrcat, addr g_acDebugMessage, $CTA0("  DATA    ")
		.elseif [esi].byImportType == IMPORT_CONST
			invoke lstrcat, addr g_acDebugMessage, $CTA0("  CONST   ")
		.else
			invoke lstrcat, addr g_acDebugMessage, $CTA0("          ")
		.endif

		; Import Name Type
comment ^
IMPORT_ORDINAL	0	The import is by ordinal. This indicates that the value in the Ordinal/Hint field of the Import Header is the import's ordinal. If this constant is not specified, then the Ordinal/Hint field should always be interpreted as the import's hint.
IMPORT_NAME	1	The import name is identical to the public symbol name.
IMPORT_NAME_NOPREFIX	2	The import name is the public symbol name, but skipping the leading ?, @, or optionally _.
IMPORT_NAME_UNDECORATE	3	The import name is the public symbol name, but skipping the leading ?, @, or optionally _, and truncating at the first @.

Name Type   
ORDINAL
NAME
NAME_NOPREF
NAME_UNDEC
^
		.if [esi].byImportNameType == IMPORT_ORDINAL
			invoke lstrcat, addr g_acDebugMessage, $CTA0("ORDINAL     ")
		.elseif [esi].byImportNameType == IMPORT_ORDINAL
			invoke lstrcat, addr g_acDebugMessage, $CTA0("NAME        ")
		.elseif [esi].byImportNameType == IMPORT_NAME_NOPREFIX
			invoke lstrcat, addr g_acDebugMessage, $CTA0("NAME_NOPREF ")
		.elseif [esi].byImportNameType == IMPORT_NAME_UNDECORATE
			invoke lstrcat, addr g_acDebugMessage, $CTA0("NAME_UNDEC  ")
		.else
			invoke lstrcat, addr g_acDebugMessage, $CTA0("            ")
		.endif



		; Number of accepting by function parameters or -1 if not recognized
		invoke wsprintf, addr buffer, $CTA0("%02d   "), [esi].uNumberOfParameters
		invoke lstrcat, addr g_acDebugMessage, addr buffer

		invoke lstrlen, addr g_acDebugMessage
		mov uLeftMarguinToUndecSymbol, eax

		; Print name as is
		mov edi, [esi].pSymbolName
		.if [esi].dwFlags & FF_SYM___IMP_
			sub edi, sizeof g_sz__imp_ - 1
		.endif
		invoke lstrcat, addr g_acDebugMessage, edi

		invoke PrintConsole, addr g_acDebugMessage, 0

		invoke PrintConsole, $CTA0("\n"), 0

		; Print undecorated name
		mov ecx, [esi].pUndecSymbolNameStart
		sub ecx, [esi].pSymbolName
		.if [esi].dwFlags & FF_SYM___IMP_
			add ecx, sizeof g_sz__imp_ - 1
		.endif
		add ecx, uLeftMarguinToUndecSymbol


		lea edi, g_acDebugMessage
		mov al, ' '
		db 0F3h, 0AAh		; repe stosb

		push esi
		mov ecx, [esi].ccUndecSymbolNameLength		; num of chars of undecorated symbol name.
		mov esi, [esi].pUndecSymbolNameStart		; VA to the beginning of undecorated symbol name
		db 0F3h, 0A4h		; repe movsb
		mov byte ptr [edi], 0						; terminate with zero
		pop esi

		invoke PrintConsole, addr g_acDebugMessage, 0


		invoke PrintConsole, $CTA0("\n"), 0


		inc uPrintedSymbols
		inc ebx
		add esi, sizeof SYM_ENTRY
	.endw
	assume esi:nothing

	invoke wsprintf, addr g_acDebugMessage, $CTA0("\n%u symbols\n"), uPrintedSymbols
	invoke PrintConsole, addr g_acDebugMessage, 0

	invoke PrintConsole, $CTA0("\n;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n\n"), 0

	ret

DbgPrintSymbolsInfo endp
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  GenerateProtoStdCall                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GenerateProtoStdCall proc uses esi edi ebx pSymEntries:PTR SYM_ENTRY, uNumberOfSymbols:UINT

IFDEF DEBUG
local uPrintedSymbols:UINT
ENDIF
local uProtoMargin:UINT
local acMessage[256]:CHAR

	.if uNumberOfSymbols != 0
		; write banner
		invoke GenerateBanner, \
		$CTA(";:                                            STDCALL                                             ::\n", g_szStdCallProtoBanner), sizeof g_szStdCallProtoBanner
;		mov g_pIncludeCurrent, eax		; update the pointer
	.else
		Fix
		ret
	.endif

IFDEF DEBUG
	and uPrintedSymbols, 0
ENDIF

	mov esi, pSymEntries
	assume esi:ptr SYM_ENTRY

	invoke GetMaxSymbolNameLenght, esi, uNumberOfSymbols, -1
	mov uProtoMargin, eax

Fix
;	invoke Sort, esi, uNumberOfSymbols, CC_STDCALL

	xor ebx, ebx
	.while ebx < uNumberOfSymbols

		Fix make it possible to generate __imp_ protos
		.if [esi].dwFlags & FF_SYM___IMP_
			; skip this symbol
			jmp @F			; continue
		.endif

IFDEF DEBUG
;			invoke DbgPrintImportTypes, [esi][ebx*(sizeof SYM_ENTRY)].pSymbolName, [esi][ebx*(sizeof SYM_ENTRY)].dwFlagsAndIndex
ENDIF

IFDEF GENERATE_INVOKE
		Fix Generate invoke only for masm
		invoke fCopyMemory, g_pIncludeCurrent, addr g_szInvoke, sizeof g_szInvoke
		add g_pIncludeCurrent, sizeof g_szInvoke
ENDIF

		; Write undecorated symbol name
		invoke fCopyMemory, g_pIncludeCurrent, [esi].pUndecSymbolNameStart, [esi].ccUndecSymbolNameLength
		mov eax, [esi].ccUndecSymbolNameLength
		add g_pIncludeCurrent, eax

	
		; fill with spaces untill proto margin
		mov eax, uProtoMargin
		sub eax, [esi].ccUndecSymbolNameLength

IFNDEF GENERATE_INVOKE
		; not sure we need to do this check here but who cares...
		.if !SIGN?
			push eax
			invoke fFillMemory, g_pIncludeCurrent, eax, ' '
			pop eax
			add g_pIncludeCurrent, eax
		.endif
ENDIF

		; write proto
IFNDEF GENERATE_INVOKE
		invoke fCopyMemory, g_pIncludeCurrent, $CTA(" proto stdcall", g_szProtoStdCall), sizeof g_szProtoStdCall
		add g_pIncludeCurrent, sizeof g_szProtoStdCall
ENDIF

		mov edi, [esi].uNumberOfParameters
		.if edi == -1
			invoke fCopyMemory, g_pIncludeCurrent, $CTA(" ; Weird number of parameters\:", g_szWeirdNumParams), sizeof g_szWeirdNumParams
			add g_pIncludeCurrent, sizeof g_szWeirdNumParams
IFDEF DEBUG
			invoke wsprintf, addr g_acDebugMessage, \
				$CTA0("\n%u stdcall symbols has weird number of parameters = %d\n\n"), \
				uPrintedSymbols, [esi].uNumberOfParameters
			invoke PrintConsole, addr g_acDebugMessage, 0
ENDIF
		.elseif edi > 0
			.while edi > 0
IFDEF GENERATE_INVOKE
				invoke fCopyMemory, g_pIncludeCurrent, addr g_szZero, sizeof g_szZero
				add g_pIncludeCurrent, sizeof g_szDWORD
ELSE
				invoke fCopyMemory, g_pIncludeCurrent, $CTA(" :DWORD,", g_szDWORD), sizeof g_szDWORD
				add g_pIncludeCurrent, sizeof g_szDWORD
ENDIF								
				dec edi				
			.endw
IFNDEF GENERATE_INVOKE
		sub g_pIncludeCurrent, 1				; remove trailing ","
ENDIF
		.endif



		.if [esi].dwFlags & FF_SYM_VARIABLE
			invoke fCopyMemory, g_pIncludeCurrent, addr g_szExportedVariable, sizeof g_szExportedVariable
			add g_pIncludeCurrent, sizeof g_szExportedVariable
		.endif


		NewLine g_pIncludeCurrent

IFDEF DEBUG
		inc uPrintedSymbols
ENDIF

;IFDEF DEBUG
;		.else
;			invoke wsprintf, addr g_acDebugMessage, $CTA0("Skip: %s\n"), [esi].pSymbolName
;			invoke PrintConsole, addr g_acDebugMessage, 0
;ENDIF
;		.endif
@@:
		add esi, sizeof SYM_ENTRY
		inc ebx			; next symbol
	.endw

IFDEF DEBUG
	; Print how many symbols were prototyped
	invoke wsprintf, addr g_acDebugMessage, $CTA0("\nProtos for %u stdcall symbols generated\n\n"), uPrintedSymbols
	invoke PrintConsole, addr g_acDebugMessage, 0
ENDIF
Fix
;	invoke GenerateEquationsStdCall, pSymEntries, uNumberOfSymbols

	ret

GenerateProtoStdCall endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    GenerateProtoCDecl                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GenerateProtoCDecl proc uses esi edi ebx pSymEntries:PTR SYM_ENTRY, uNumberOfSymbols:UINT

IFDEF DEBUG
local uPrintedSymbols:UINT
ENDIF
local uProtoMargin:UINT
local acMessage[256]:CHAR

	.if uNumberOfSymbols != 0
		; write banner
		invoke GenerateBanner, \
		$CTA(";:                                             CDECL                                              ::\n", g_szCDeclProtoBanner), sizeof g_szCDeclProtoBanner
;		mov g_pIncludeCurrent, eax		; update the pointer
	.else
		Fix
		ret
	.endif

IFDEF DEBUG
	and uPrintedSymbols, 0
ENDIF

	mov esi, pSymEntries
	assume esi:ptr SYM_ENTRY

	invoke GetMaxSymbolNameLenght, esi, uNumberOfSymbols, -1
	mov uProtoMargin, eax


Fix
;	invoke Sort, esi, uNumberOfSymbols, CC_CDECL

	xor ebx, ebx
	.while ebx < uNumberOfSymbols

		Fix make it possible to generate __imp_ protos
		.if [esi].dwFlags & FF_SYM___IMP_
			; skip this symbol
			jmp @F
		.endif

IFDEF DEBUG
;			invoke DbgPrintImportTypes, [esi][ebx*(sizeof SYM_ENTRY)].pSymbolName, [esi][ebx*(sizeof SYM_ENTRY)].dwFlagsAndIndex
ENDIF

IFDEF GENERATE_INVOKE
		Fix Generate invoke only for masm
		invoke fCopyMemory, g_pIncludeCurrent, addr g_szInvoke, sizeof g_szInvoke
		add g_pIncludeCurrent, sizeof g_szInvoke
ENDIF

		; Write undecorated symbol name
		invoke fCopyMemory, g_pIncludeCurrent, [esi].pUndecSymbolNameStart, [esi].ccUndecSymbolNameLength
		mov eax, [esi].ccUndecSymbolNameLength
		add g_pIncludeCurrent, eax

	
		; fill with spaces untill proto margin
		mov eax, uProtoMargin
		sub eax, [esi].ccUndecSymbolNameLength

IFNDEF GENERATE_INVOKE
		; not sure we need to do this check here but who cares...
		.if !SIGN?
			push eax
			invoke fFillMemory, g_pIncludeCurrent, eax, ' '
			pop eax
			add g_pIncludeCurrent, eax
		.endif
ENDIF

IFNDEF GENERATE_INVOKE
		; write proto
		invoke fCopyMemory, g_pIncludeCurrent, $CTA(" proto c :VARARG", g_szProtoCDecl), sizeof g_szProtoCDecl
		add g_pIncludeCurrent, sizeof g_szProtoCDecl
ENDIF

		.if [esi].dwFlags & FF_SYM_VARIABLE
			invoke fCopyMemory, g_pIncludeCurrent, addr g_szExportedVariable, sizeof g_szExportedVariable
			add g_pIncludeCurrent, sizeof g_szExportedVariable
		.endif
	
		NewLine g_pIncludeCurrent

IFDEF DEBUG
		inc uPrintedSymbols
ENDIF

;IFDEF DEBUG
;		.else
;			invoke wsprintf, addr g_acDebugMessage, $CTA0("Skip: %s\n"), [esi].pSymbolName
;			invoke PrintConsole, addr g_acDebugMessage, 0
;ENDIF
;		.endif
@@:
		add esi, sizeof SYM_ENTRY
		inc ebx			; next symbol
	.endw

IFDEF DEBUG
	; Print how many symbols were prototyped
	invoke wsprintf, addr g_acDebugMessage, $CTA0("\nProtos for %u cdecl symbols generated\n\n"), uPrintedSymbols
	invoke PrintConsole, addr g_acDebugMessage, 0
ENDIF
Fix
;	invoke GenerateEquationsCDecl, pSymEntries, uNumberOfSymbols

	ret

GenerateProtoCDecl endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 GenerateProtoFastCall                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GenerateProtoFastCall proc uses esi edi ebx pSymEntries:PTR SYM_ENTRY, uNumberOfSymbols:UINT

IFDEF DEBUG
local uPrintedSymbols:UINT
ENDIF
local acUndecSymbolName[256]:CHAR
local acBuffer[512]:CHAR

	.if uNumberOfSymbols != 0
		; write banner
		invoke GenerateBanner, \
		$CTA(";:                                           FASTCALL                                             ::\n", g_szFastCallProtoBanner), sizeof g_szFastCallProtoBanner
;		mov g_pIncludeCurrent, eax		; update the pointer
	.else
		Fix
		ret
	.endif

IFDEF DEBUG
	and uPrintedSymbols, 0
ENDIF

	mov esi, pSymEntries
	assume esi:ptr SYM_ENTRY

Fix
;	invoke Sort, esi, uNumberOfSymbols, CC_FASTCALL

	xor ebx, ebx
	.while ebx < uNumberOfSymbols

		Fix make it possible to generate __imp_ protos
		.if [esi].dwFlags & FF_SYM___IMP_
			; skip this symbol
			jmp @F
		.endif

IFDEF GENERATE_INVOKE
		Fix Generate invoke only for masm
		invoke fCopyMemory, g_pIncludeCurrent, addr g_szInvoke, sizeof g_szInvoke
		add g_pIncludeCurrent, sizeof g_szInvoke

		; Write undecorated symbol name
		invoke fCopyMemory, g_pIncludeCurrent, [esi].pUndecSymbolNameStart, [esi].ccUndecSymbolNameLength
		mov eax, [esi].ccUndecSymbolNameLength
		add g_pIncludeCurrent, eax

		mov edi, [esi].uNumberOfParameters
		.if edi > 0
			.while edi > 0
				invoke fCopyMemory, g_pIncludeCurrent, addr g_szZero, sizeof g_szZero
				add g_pIncludeCurrent, sizeof g_szDWORD
				dec edi				
			.endw
		.endif
ELSE			; Generate Proto

comment ^
		invoke fCopyMemory, g_pIncludeCurrent, $CTA("externdef syscall ", g_szExterndefSyscall, 4), sizeof g_szExterndefSyscall
		add g_pIncludeCurrent, sizeof g_szExterndefSyscall

		invoke fstrlen, [esi].pSymbolName
		push eax
		invoke fCopyMemory, g_pIncludeCurrent, [esi].pSymbolName, eax
		pop eax
		add g_pIncludeCurrent, eax



		invoke fCopyMemory, g_pIncludeCurrent, [esi].pUndecSymbolNameStart, [esi].ccUndecSymbolNameLength
		mov eax, [esi].ccUndecSymbolNameLength
		add g_pIncludeCurrent, eax
^



		invoke fCopyMemory, addr acUndecSymbolName, [esi].pUndecSymbolNameStart, [esi].ccUndecSymbolNameLength
		lea eax, acUndecSymbolName
		add eax, [esi].ccUndecSymbolNameLength
		mov byte ptr [eax], 0				; terminate with zero

		invoke wsprintf, addr acBuffer, $CTA0("externdef syscall %s:proc\n%s textequ \[%s\]\n"), \
								[esi].pSymbolName, addr acUndecSymbolName, [esi].pSymbolName
		push eax
		invoke fCopyMemory, g_pIncludeCurrent, addr acBuffer, eax
		pop eax
		add g_pIncludeCurrent, eax
ENDIF

		Fix If invoking generated variables should be different
		.if [esi].dwFlags & FF_SYM_VARIABLE
			invoke fCopyMemory, g_pIncludeCurrent, addr g_szExportedVariable, sizeof g_szExportedVariable
			add g_pIncludeCurrent, sizeof g_szExportedVariable
		.endif
	
		NewLine g_pIncludeCurrent

IFDEF DEBUG
		inc uPrintedSymbols
ENDIF

@@:
		add esi, sizeof SYM_ENTRY
		inc ebx			; next symbol
	.endw

IFDEF DEBUG
	; Print how many symbols were prototyped
	invoke wsprintf, addr g_acDebugMessage, $CTA0("\nProtos for %u fastcall symbols generated\n\n"), uPrintedSymbols
	invoke PrintConsole, addr g_acDebugMessage, 0
ENDIF

Fix
;	invoke GenerateEquationsFastCall, pSymEntries, uNumberOfSymbols

	ret

GenerateProtoFastCall endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      GenerateProto                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GenerateProto proc uses esi

	lea esi, g_paCcEntries
	assume esi:ptr CC_ENTRY

IFDEF DEBUG
	invoke DbgPrintSymbolsInfo, CC_STDCALL
ENDIF

	; stdcall processing
	mov eax, CC_STDCALL
	invoke GenerateProtoStdCall, [esi][eax*(sizeof CC_ENTRY)].pSymEntries, [esi][eax*(sizeof CC_ENTRY)].uNumEntries

IFDEF DEBUG
	invoke DbgPrintSymbolsInfo, CC_CDECL
ENDIF

	; cdecl processing
	mov eax, CC_CDECL
	invoke GenerateProtoCDecl, [esi][eax*(sizeof CC_ENTRY)].pSymEntries, [esi][eax*(sizeof CC_ENTRY)].uNumEntries

IFDEF DEBUG
	invoke DbgPrintSymbolsInfo, CC_FASTCALL
ENDIF

	; fastcall processing
	mov eax, CC_FASTCALL
	invoke GenerateProtoFastCall, [esi][eax*(sizeof CC_ENTRY)].pSymEntries, [esi][eax*(sizeof CC_ENTRY)].uNumEntries


	assume esi:nothing

	return g_pIncludeCurrent

GenerateProto endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     GenerateEquation                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GenerateEquation proc uses esi pSymEntry:PTR SYM_ENTRY, uEquMargin:UINT

;	AddAtom                        equ <AddAtomW>

	mov esi, pSymEntry
	assume esi:ptr SYM_ENTRY

	invoke fCopyMemory, g_pIncludeCurrent, $CTA0("    "), 4
	add g_pIncludeCurrent, 4

	mov eax, [esi].ccUndecSymbolNameLength
	dec eax									; not including 'A' or 'W'
	invoke fCopyMemory, g_pIncludeCurrent, [esi].pUndecSymbolNameStart, eax
	mov eax, [esi].ccUndecSymbolNameLength
	dec eax
	add g_pIncludeCurrent, eax

	
	; fill with spaces untill proto margin
	mov eax, uEquMargin
	sub eax, [esi].ccUndecSymbolNameLength
	inc eax

	; not sure we need to do this check here but who cares...
	.if !SIGN?
		push eax
		invoke fFillMemory, g_pIncludeCurrent, eax, ' '
		pop eax
		add g_pIncludeCurrent, eax
	.endif

	invoke fCopyMemory, g_pIncludeCurrent, $CTA0("equ \["), 5
	add g_pIncludeCurrent, 5

	invoke fCopyMemory, g_pIncludeCurrent, [esi].pUndecSymbolNameStart, [esi].ccUndecSymbolNameLength
	mov eax, [esi].ccUndecSymbolNameLength
	add g_pIncludeCurrent, eax

	invoke fCopyMemory, g_pIncludeCurrent, $CTA0("\]"), 1
	add g_pIncludeCurrent, 1

	NewLine g_pIncludeCurrent

	ret

GenerateEquation endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        IsSpecialCase                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IsSpecialCase proc uses esi edi ebx pSymEntries:PTR SYM_ENTRY, uNumberOfSymbols:UINT, pSymEntry:PTR SYM_ENTRY

local fRet:BOOL
IFDEF DEBUG
local buffer[128]:CHAR
ENDIF

	; pSymEntry points to SYM_ENTRY for ANSI or UNICODE symbol

	; We need to do some check while generating equations.
	; If we meet one of such case we return TRUE or FALSE otherwise.

	and fRet, FALSE				; assume no special cases

	.if uNumberOfSymbols != 0

		mov edi, pSymEntry
		assume edi:ptr SYM_ENTRY

		mov eax, [edi].dwFlags
		and eax, FF_SYM_ANSI + FF_SYM_UNICODE
		.if !ZERO?					; Make sure is pSymEntry really points to ANSI or UNICODE entry.
									; We need this check because we going to trancate symbol name at 'A' or 'W'
			mov esi, pSymEntries
			assume esi:ptr SYM_ENTRY

			; *** SPECIAL CASE #1

			; user32.lib
			; BroadcastSystemMessage
			; BroadcastSystemMessageA
			; BroadcastSystemMessageW
			;
			; So we must not equate BroadcastSystemMessage to BroadcastSystemMessageA and to BroadcastSystemMessageW
			; otherwise we'll get symbol redefinition error

			xor ebx, ebx
			.while ebx < uNumberOfSymbols

				.if esi != edi				; skip comparing with the same symbol
					; Compare only symbols that have the same name lenght
					mov eax, [edi].ccUndecSymbolNameLength
					dec eax
					.if [esi].ccUndecSymbolNameLength == eax
						invoke strncmp, [esi].pUndecSymbolNameStart, [edi].pUndecSymbolNameStart, eax
						.if eax == 0
IFDEF DEBUG
							invoke RtlZeroMemory, addr g_acDebugMessage, sizeof g_acDebugMessage
							invoke lstrcpy, addr g_acDebugMessage, $CTA0("Special case #1 found: ")
							invoke fCopyMemory, addr buffer, [edi].pUndecSymbolNameStart, [edi].ccUndecSymbolNameLength
							invoke lstrcat, addr g_acDebugMessage, addr buffer
							invoke lstrcat, addr g_acDebugMessage, $CTA0(" has corresponding ")

							invoke fCopyMemory, addr buffer, [esi].pUndecSymbolNameStart, [esi].ccUndecSymbolNameLength
							invoke lstrcat, addr g_acDebugMessage, addr buffer

							invoke lstrcat, addr g_acDebugMessage, $CTA0("\n")
							invoke PrintConsole, addr g_acDebugMessage, 0
ENDIF
							mov fRet, TRUE				; found special case #1
							jmp RetFromIsSpecialCase
						.endif
					.endif
				.endif

				add esi, sizeof SYM_ENTRY
				inc ebx			; next symbol
			.endw



			Fix add check
			; *** SPECIAL CASE #2

			; sulwapi.lib
			; StrFormatByteSize64A doesn't have corresponding StrFormatByteSize64W symbol
			; but doesn't not have StrFormatByteSizeW
			; So we have to equate StrFormatByteSize to StrFormatByteSizeA

		.endif		; if FF_SYM_ANSI or FF_SYM_UNICODE
		assume esi:nothing
		assume edi:nothing
	.endif			; if uNumberOfSymbols != 0

comment ^
	; make symbol unicode and trancate at '@' pos
	; _GetStringTypeA@20 -> _GetStringTypeW
	; We have to search for symbol this way because of:
	; _GetStringTypeA@20
	; _GetStringTypeW@16

	; Now we have to check to see whether we encounter something like this:
	; _MoveFileW _MoveFileWithProgressW

	; Mmm... It can be so btw:
	; _SomeFuncW _SomeFuncW@SomeSuffix but not for the system library

	; But it will only work till we meet something like this:
	; _SomeSymbol@10 != _SomeSymbolA@14
	; I'm not shure if it can be, but who cares...

^
RetFromIsSpecialCase:
	mov eax, fRet
	ret

IsSpecialCase endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   GenerateEquationsStdCall                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GenerateEquationsStdCall proc uses esi edi ebx pSymEntries:PTR SYM_ENTRY, uNumberOfSymbols:UINT

local uUnicodeEquMargin:UINT
local uAnsiEquMargin:UINT

local acMessage[256]:CHAR

	.if uNumberOfSymbols != 0
		; write banner
		invoke GenerateBanner, \
		$CTA(";:                                       STDCALL EQUATIONS                                        ::\n", g_szStdCallEquBanner), sizeof g_szStdCallEquBanner
	.else
		Fix
		ret
	.endif


	; Write "IFDEF UNICODE"
	invoke fCopyMemory, g_pIncludeCurrent, addr g_szIfDefUnicode, sizeof g_szIfDefUnicode
	add g_pIncludeCurrent, sizeof g_szIfDefUnicode

	mov esi, pSymEntries
	assume esi:ptr SYM_ENTRY

	invoke GetMaxSymbolNameLenght, esi, uNumberOfSymbols, FF_SYM_UNICODE
	mov uUnicodeEquMargin, eax

	xor ebx, ebx
	.while ebx < uNumberOfSymbols

		Fix make it possible to generate __imp_ protos
		.if [esi].dwFlags & FF_SYM___IMP_
			; skip this symbol
			jmp @F			; continue
		.endif

		.if !([esi].dwFlags & FF_SYM_UNICODE)
			; Not unicode -> skip this symbol
			jmp @F			; continue
		.endif

		invoke IsSpecialCase, pSymEntries, uNumberOfSymbols, esi
		.if eax == FALSE
			invoke GenerateEquation, esi, uUnicodeEquMargin
		.endif

@@:
		add esi, sizeof SYM_ENTRY
		inc ebx			; next symbol
	.endw



	; "ELSE"
	invoke fCopyMemory, g_pIncludeCurrent, addr g_szElseUnicode, sizeof g_szElseUnicode
	add g_pIncludeCurrent, sizeof g_szElseUnicode

	mov esi, pSymEntries
	assume esi:ptr SYM_ENTRY

	invoke GetMaxSymbolNameLenght, esi, uNumberOfSymbols, FF_SYM_ANSI
	mov uAnsiEquMargin, eax

	xor ebx, ebx
	.while ebx < uNumberOfSymbols

		Fix make it possible to generate __imp_ protos
		.if [esi].dwFlags & FF_SYM___IMP_
			; skip this symbol
			jmp @F			; continue
		.endif

		.if !([esi].dwFlags & FF_SYM_ANSI)
			; Not unicode -> skip this symbol
			jmp @F			; continue
		.endif

		invoke IsSpecialCase, pSymEntries, uNumberOfSymbols, esi
		.if eax == FALSE
			invoke GenerateEquation, esi, uAnsiEquMargin
		.endif

@@:
		add esi, sizeof SYM_ENTRY
		inc ebx			; next symbol
	.endw

	assume esi:nothing


	; ENDIF
	invoke fCopyMemory, g_pIncludeCurrent, addr g_szEndIfUnicode, sizeof g_szEndIfUnicode
	add g_pIncludeCurrent, sizeof g_szEndIfUnicode

	ret

GenerateEquationsStdCall endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      GenerateEquations                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GenerateEquations proc uses esi

	lea esi, g_paCcEntries
	assume esi:ptr CC_ENTRY

	; stdcall processing
	mov eax, CC_STDCALL
	invoke GenerateEquationsStdCall, [esi][eax*(sizeof CC_ENTRY)].pSymEntries, [esi][eax*(sizeof CC_ENTRY)].uNumEntries

	assume esi:nothing

	return g_pIncludeCurrent

	ret

GenerateEquations endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     AllocateSymEntries                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

AllocateSymEntries proc uses edi ebx uNumberOfSymbols:UINT

local uListSize:UINT
local uTotalSize:UINT
local pSymEntries:LPVOID

;	invoke RtlZeroMemory, addr g_paCcEntries, sizeof g_paCcEntries

	and pSymEntries, NULL				; assume we can't allocate memory

	; We don't know how many symbols of which type we really have
	; So, we need to reserve  uNumberOfSymbols * sizeof SYM_ENTRY * CC_NUM  bytes of memory

	mov eax, uNumberOfSymbols

	; Calculate how many memory for each SYM_ENTRYs we need
	imul eax, (sizeof SYM_ENTRY)
	; Round up to page size
	add eax, PAGE_SIZE - 1
	and eax, - PAGE_SIZE
	mov uListSize, eax

;add eax, 3 
;and eax, -4


	; Calculate how many memory for all SYM_ENTRYs we need
	imul eax, CC_NUM						; sizeof g_paCcEntries / sizeof CC_ENTRY
	mov uTotalSize, eax

comment ^
	; Calculate how many memory pages we need
	shr eax, (PAGE_SHIFT - 2)				; * PAGE_SIZE / sizeof LPVOID
	inc eax
	shl eax, PAGE_SHIFT						; * PAGE_SIZE

	imul eax, (sizeof SYM_ENTRY)/(sizeof LPVOID)
	mov uListSize, eax

	imul eax, (sizeof g_paCcEntries)/(sizeof CC_ENTRY)
	mov uTotalSize, eax
^
	; We don't know how many symbols of which type we really have
	; So, we only reserve memory here
	; To commit is the exception handler's job (see seh.inc)
	invoke VirtualAlloc, NULL, uTotalSize, MEM_RESERVE, PAGE_READWRITE
	.if eax != NULL
		mov pSymEntries, eax

		push uTotalSize
		pop g_cbSymEntries

		; Initialize array of CC_ENTRY
		lea edi, g_paCcEntries
		assume edi:ptr CC_ENTRY
		mov edx, uListSize
		xor ecx,  ecx
		.while ecx < CC_NUM					; sizeof g_paCcEntries / sizeof CC_ENTRY
			mov [edi][(sizeof CC_ENTRY)*ecx].pSymEntries, eax
			and [edi][(sizeof CC_ENTRY)*ecx].uNumEntries, 0
			add eax, edx
			inc ecx
		.endw
		assume edi:nothing
		; Now every CC_ENTRY.pSymEntries points to appropriate address in allocated blok of memory
		; It's the start of SYM_ENTRY array
	.endif

	mov eax, pSymEntries
	ret

AllocateSymEntries endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                             VerifyAndSkeep1stLinkerMemberHeader                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

VerifyAndSkeep1stLinkerMemberHeader proc uses esi edi ebx p1stLinkerMemberHeader:PTR IMAGE_ARCHIVE_MEMBER_HEADER

; If 1st Linker Member Header format is OK returns pointer to 2nd Linker Member Header
; or NULL otherwise

local acBuffer[16]:CHAR
local uLibraryFileSize:UINT
local u1stLinkerMemberSize:UINT
local p2ndLinkerMemberHeader:PTR IMAGE_ARCHIVE_MEMBER_HEADER

	and p2ndLinkerMemberHeader, NULL			; assume invalid library format

	mov esi, p1stLinkerMemberHeader

	; Is it 1st Linker Member Header ?
	invoke strncmp, esi, $CTA0(%IMAGE_ARCHIVE_LINKER_MEMBER), 16
	.if eax == 0											; Yes. It's 1st Linker Member Header

		; IMAGE_ARCHIVE_MEMBER_HEADER.Size1 - ASCII decimal representation of the total size
		; of the archive member, not including the size of the header
		invoke fCopyMemory, addr acBuffer, addr (IMAGE_ARCHIVE_MEMBER_HEADER ptr [esi]).Size1, \
										sizeof IMAGE_ARCHIVE_MEMBER_HEADER.Size1
		invoke WipeStringSpacesFromRight, addr acBuffer, sizeof IMAGE_ARCHIVE_MEMBER_HEADER.Size1
		invoke atodw, addr acBuffer							; convert size of 1st Linker Member from dec string to number
		mov u1stLinkerMemberSize, eax						; size of 1st Linker Member

		push eax											; reserve place on stack for lpFileSizeHigh
		invoke GetFileSize, g_hLibraryFile, esp
		pop ecx												; pop lpFileSizeHigh from stack

		.if eax != -1
			; do some stupid error checking
			sub eax, IMAGE_ARCHIVE_START_SIZE				; + archive file signature size
			sub eax, sizeof IMAGE_ARCHIVE_MEMBER_HEADER
			sub eax, uLibraryFileSize
			; If IMAGE_ARCHIVE_START_SIZE + sizeof IMAGE_ARCHIVE_MEMBER_HEADER + size of 1st Linker Member
			; more or equal to whole library file size something wrong with file format
			.if !SIGN?
				add esi, sizeof IMAGE_ARCHIVE_MEMBER_HEADER	; skeep 1st Linker Member Header. esi -> 1st Linker Member
				add esi, u1stLinkerMemberSize				; skeep 1st Linker Member. esi -> 2nd Linker Member Header

				; 1st Linker Member Header is OK.
				mov p2ndLinkerMemberHeader, esi

			.else
				invoke PrintConsole, $CTA0("Error: "), FOREGROUND_RED + FOREGROUND_INTENSITY
				invoke PrintConsole, $CTA0("The 1st Linker Member size of "), 0
				invoke PrintConsole, addr g_acLibraryPath, 0
				invoke PrintConsole, $CTA0(" is wrong.\n"), 0
			.endif
		.else
			invoke PrintConsole, $CTA0("Error: "), FOREGROUND_RED + FOREGROUND_INTENSITY
			invoke PrintConsole, $CTA0("Couldn't get file size of "), 0
			invoke PrintConsole, addr g_acLibraryPath, 0
			invoke PrintConsole, $CTA0(".\n"), 0
		.endif
	.else
		invoke PrintConsole, $CTA0("Error: "), FOREGROUND_RED + FOREGROUND_INTENSITY
		invoke PrintConsole, $CTA0("Couldn't find 1st Linker Member in "), 0
		invoke PrintConsole, addr g_acLibraryPath, 0
		invoke PrintConsole, $CTA0(".\n"), 0
	.endif

	mov eax, p2ndLinkerMemberHeader
	ret

VerifyAndSkeep1stLinkerMemberHeader endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                Verify2ndLinkerMemberHeader                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Verify2ndLinkerMemberHeader proc uses esi edi ebx p2ndLinkerMemberHeader:PTR IMAGE_ARCHIVE_MEMBER_HEADER

; If 2nd Linker Member Header format is OK returns TRUE
; or FALSE otherwise

local acBuffer[16]:CHAR
local uLibraryFileSize:UINT
local u2ndLinkerMemberSize:UINT
local fOk:BOOL

	and fOk, FALSE			; assume invalid library format

	mov esi, p2ndLinkerMemberHeader

	; Is it 2nd Linker Member Header ?
	invoke strncmp, esi, $CTA0(%IMAGE_ARCHIVE_LINKER_MEMBER), 16
	.if eax == 0											; Yes. It's 2nd Linker Member Header

		; IMAGE_ARCHIVE_MEMBER_HEADER.Size1 - ASCII decimal representation of the total size
		; of the archive member, not including the size of the header
		invoke fCopyMemory, addr acBuffer, addr (IMAGE_ARCHIVE_MEMBER_HEADER ptr [esi]).Size1, \
										sizeof IMAGE_ARCHIVE_MEMBER_HEADER.Size1
		invoke WipeStringSpacesFromRight, addr acBuffer, sizeof IMAGE_ARCHIVE_MEMBER_HEADER.Size1
		invoke atodw, addr acBuffer							; convert size of 2nd Linker Member from dec string to number
		mov u2ndLinkerMemberSize, eax						; size of 2nd Linker Member

		push eax											; reserve place on stack for lpFileSizeHigh
		invoke GetFileSize, g_hLibraryFile, esp
		pop ecx												; pop lpFileSizeHigh from stack

		.if eax != -1
			; do some stupid error checking
			add eax, g_pLibraryImage

			mov ecx, esi
			add ecx, sizeof IMAGE_ARCHIVE_MEMBER_HEADER
			add ecx, u2ndLinkerMemberSize

			sub eax, ecx
			; If p2ndLinkerMemberHeader + sizeof IMAGE_ARCHIVE_MEMBER_HEADER + size of 2nd Linker Member
			; more or equal to g_pLibraryImage + whole library file size, something wrong with file format
			.if !SIGN?

				; 2nd Linker Member Header is OK.
				inc fOk

			.else
				invoke PrintConsole, $CTA0("Error: "), FOREGROUND_RED + FOREGROUND_INTENSITY
				invoke PrintConsole, $CTA0("The 2nd Linker Member size of "), 0
				invoke PrintConsole, addr g_acLibraryPath, 0
				invoke PrintConsole, $CTA0(" is wrong.\n"), 0
			.endif
		.else
			invoke PrintConsole, $CTA0("Error: "), FOREGROUND_RED + FOREGROUND_INTENSITY
			invoke PrintConsole, $CTA0("Couldn't get file size of "), 0
			invoke PrintConsole, addr g_acLibraryPath, 0
			invoke PrintConsole, $CTA0(".\n"), 0
		.endif
	.else
		invoke PrintConsole, $CTA0("Error: "), FOREGROUND_RED + FOREGROUND_INTENSITY
		invoke PrintConsole, $CTA0("Couldn't find 2nd Linker Member in "), 0
		invoke PrintConsole, addr g_acLibraryPath, 0
		invoke PrintConsole, $CTA0(".\n"), 0
	.endif

	mov eax, fOk
	ret

Verify2ndLinkerMemberHeader endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   GetCurrentDateString                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetCurrentDateString proc pchCurrentDate:LPVOID, cbCurrentDate:DWORD

local LocalTime:SYSTEMTIME

	.const

	g_aszMonths	label LPSTR
	LPSTR	$CTA0("Jan")
	LPSTR	$CTA0("Feb")
	LPSTR	$CTA0("Mar")
	LPSTR	$CTA0("Apr")
	LPSTR	$CTA0("May")
	LPSTR	$CTA0("Jun")
	LPSTR	$CTA0("Jul")
	LPSTR	$CTA0("Aug")
	LPSTR	$CTA0("Sep")
	LPSTR	$CTA0("Oct")
	LPSTR	$CTA0("Nov")
	LPSTR	$CTA0("Dec")
	g_cbaMonths	equ $-g_aszMonths

	.code

	_try

	mov eax, pchCurrentDate
	and byte ptr [eax], 0

	.if cbCurrentDate > 12

		invoke GetLocalTime, addr LocalTime

		movzx ecx, LocalTime.wYear

		movzx eax, LocalTime.wMonth
		dec eax			; make it zero based
		.if eax < 12
			lea eax, g_aszMonths[eax * sizeof LPSTR]
		.else
			mov eax, $CTA0("???")
		.endif

		movzx edx, LocalTime.wDay

		invoke wsprintf, pchCurrentDate, $CTA0("%02d %s %04d"), edx, eax, ecx

	.endif

	_finally

	ret

GetCurrentDateString endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                   Parse2ndLinkerMember                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Parse2ndLinkerMember proc uses esi edi ebx p2ndLinkerMember:LPVOID

local acIncludePath[MAX_PATH]:CHAR
local buffer[256]:CHAR
local achCurrentDate[32]:CHAR
local dwNumberOfBytesWritten:DWORD
local hIncludeFile:HANDLE
local pSymEntries:LPVOID

	mov esi, p2ndLinkerMember		; esi -> 2nd Linker Member

	mov eax, [esi]		; Unsigned long containing the number of archive members.
	mov g_uNumberOfMembers, eax

	add esi, sizeof DWORD	; esi -> Array of file offsets to archive member headers, arranged in ascending order. 
	shl eax, 2				; 4 * g_uNumberOfMembers
							; eax = sizeof Offsets Array

	add esi, eax			; esi -> Number of Symbols
	mov eax, [esi]			; Unsigned long containing the number of symbols indexed.
	mov g_uNumberOfSymbols, eax				; Total in library

	add esi, 4			; skip Number of Symbols
	shl eax, 1			; (Number of Symbols) * 2 = sizeof Indices Array
	add esi, eax		; esi -> Array of Symbols

	invoke AllocateSymEntries, g_uNumberOfSymbols
	.if eax != NULL

		mov g_pSymEntries, eax

		invoke FillSymEntries, p2ndLinkerMember, esi, g_uNumberOfSymbols
		;invoke ParseSymbols, esi, g_uNumberOfSymbols
		.if eax

			invoke fstrcpy, addr acIncludePath, addr g_acLibraryPath
			invoke MakeExtensionInc, addr acIncludePath

			; Allocate memory to write parsed info to
			; Reserve a region big enough
			invoke GetFileSize, g_hLibraryFile, NULL
			shl eax, 1
			mov g_cbInclude, eax				; hope sizeof(*.lib) * 2 is enough ;)
			; we only reserve memory here
			; to commit is the exception handler's job (see seh.inc)
			invoke VirtualAlloc, NULL, g_cbInclude, MEM_RESERVE + MEM_TOP_DOWN, PAGE_READWRITE
			.if eax != NULL

				mov g_pInclude, eax
				mov g_pIncludeCurrent, eax
				or g_fSortUndecorated, -1			; set

				invoke GetCurrentDateString, addr achCurrentDate, sizeof achCurrentDate

				invoke  wsprintf, addr buffer, \
					$CTA0(";:           This file was created on %s with Protogen %s by Four-F.                   ::\n"), \
					addr achCurrentDate,
					addr g_szVersion

				invoke lstrlen, addr buffer
				mov ecx, eax

				_try

				invoke GenerateBanner, addr buffer, ecx

;				invoke GetNameFromPath, addr g_acLibraryPath
;				.if eax != 0
;					invoke GenerateBanner, eax
;				.endif

				invoke GenerateProto
				invoke GenerateEquations

				_finally

				mov eax, g_pIncludeCurrent
				.if eax > g_pInclude
					; there is some info to save

					; calculate actual size
					sub eax, g_pInclude
					.if eax < g_cbInclude
						mov g_cbInclude, eax
					.endif

					Fix CREATE_NEW
					invoke CreateFile, addr acIncludePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
					.if eax != INVALID_HANDLE_VALUE
						mov hIncludeFile, eax

						invoke WriteFile, hIncludeFile, g_pInclude, g_cbInclude, addr dwNumberOfBytesWritten, NULL
						.if eax != 0
							invoke PrintSummary
						.else
							invoke PrintConsole, $CTA0("Error: Can't write to inc file.\n"), 0
						.endif
						invoke CloseHandle, hIncludeFile
					.else
						invoke PrintConsole, $CTA0("Error: Can't create inc file.\n"), 0
					.endif
				.else
					invoke PrintConsole, $CTA0("Error: Something went wrong. The lib file was not parsed properly\:\n"), 0
				.endif

				; Free
				invoke VirtualFree, g_pInclude, 0, MEM_RELEASE

			.endif

		.else
			invoke PrintConsole, $CTA0("Error: Can't parse library.\n"), 0
		.endif

		; release all memory we have
		invoke VirtualFree, g_pSymEntries, 0, MEM_RELEASE

	.else
		invoke PrintConsole, $CTA0("Error: Can't allocate memory.\n"), 0
	.endif

	ret

Parse2ndLinkerMember endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       ParseLibrary                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ParseLibrary proc uses esi edi ebx; pszLibraryPath:LPSTR

local acMessage[512]:CHAR
local acBuffer[16]:CHAR
;local uLibraryFileSize:UINT
;local u1stLinkerMemberSize:UINT
;local u2ndLinkerMemberSize:UINT

;	Fix remove this two lines
;	invoke PrintConsole, addr g_acLibraryPath, 0
;	invoke PrintConsole, $CTA0("\n"), 0

	; open lib file
	invoke OpenLibrary, addr g_acLibraryPath
	.if eax == TRUE

		.data
			szArchiveSignature	db IMAGE_ARCHIVE_START
		.code

		mov esi, g_pLibraryImage
		; The archive file signature identifies the file type.
		; Any utility (for example, a linker) expecting an archive file as input
		; can check the file type by reading this signature. 
		invoke strncmp, g_pLibraryImage, addr szArchiveSignature, IMAGE_ARCHIVE_START_SIZE
		.if eax == 0												; Signature is OK

			add esi, sizeof IMAGE_ARCHIVE_START_SIZE				; skip archive file signature

			.if esi & 01y
				; Each member header starts on the first even address
				; after the end of the previous archive member.
				inc esi
			.endif

			; Is it 1st Linker Member Header ?
			invoke VerifyAndSkeep1stLinkerMemberHeader, esi
			.if eax != NULL
				; eax -> 2nd Linker Member Header
				mov esi, eax

				.if esi & 01y
					; Each member header starts on the first even address
					; after the end of the previous archive member.
					inc esi
				.endif

				; Although both the linker members provide a directory of symbols and archive members that contain them,
				; the second linker member is used in preference to the first by all current linkers. 

				invoke Verify2ndLinkerMemberHeader, esi
				.if eax == TRUE
					add esi, sizeof IMAGE_ARCHIVE_MEMBER_HEADER
					invoke Parse2ndLinkerMember, esi
				.endif
			.endif
		.else
			invoke PrintConsole, $CTA0("Error: "), FOREGROUND_RED + FOREGROUND_INTENSITY
			invoke PrintConsole, $CTA0("Couldn't find signature \=\:\[arch\]\\n\= in "), 0
			invoke PrintConsole, addr g_acLibraryPath, 0
			invoke PrintConsole, $CTA0(".\n"), 0
		.endif
		invoke CloseLibrary
	.else
		invoke PrintConsole, $CTA0("Error: "), FOREGROUND_RED + FOREGROUND_INTENSITY
		invoke PrintConsole, $CTA0("Couldn't open library file "), 0
		invoke PrintConsole, addr g_acLibraryPath, 0
		invoke PrintConsole, $CTA0(".\n"), 0
	.endif

	ret

ParseLibrary endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          start                                                    
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

start proc uses esi edi ebx

local buffer[128]:BYTE 
local dwNumberOfCharsRead:DWORD
;local acLibraryPath[MAX_PATH]:CHAR
local acIncludePath[MAX_PATH]:CHAR
;local acFileName[MAX_PATH]:CHAR
local hIncludeFile:HANDLE
local dwNumberOfBytesWritten:DWORD

;local acFileName[MAX_PATH]:CHAR
local wfd:WIN32_FIND_DATA
local hFindFile:HANDLE

	invoke GetStdHandle, STD_OUTPUT_HANDLE
	.if eax != INVALID_HANDLE_VALUE
		mov g_hConsoleOutput, eax

		mov g_pszCommandLine, $invoke(GetCommandLine)

		; parse command line
		invoke CL_ScanArgsX, g_pszCommandLine

		; if '-h' or '-?' was specified print help
		.if eax == 1 || CL_switch['h'] || CL_switch['H'] || CL_switch['?']
			invoke PrintLogo
			invoke PrintUsage
		.else

			; initialize global vars with error values
			or g_hLibraryFile,		INVALID_HANDLE_VALUE
			and g_hLibraryMapping,	NULL
			and g_pLibraryImage,	NULL

			invoke InitCommandSwitches

			invoke PrintLogoIfUserWantTo


			; get pointer to library file name
			mov eax, CL_argc
			dec eax
			shl eax, 2
			mov esi, CL_argv[eax]
			.if byte ptr [esi] == '*'
				; Parse all libs in directory

				invoke FindFirstFile, $CTA0("*.lib"), addr wfd
				.if eax != INVALID_HANDLE_VALUE
					mov hFindFile, eax
					.while TRUE

						invoke lstrcpy, addr g_acLibraryPath, addr wfd.cFileName
						invoke ParseLibrary;, addr g_acLibraryPath

						invoke FindNextFile, hFindFile, addr wfd
						.if eax != 0


						.else
							invoke GetLastError
							.if eax == ERROR_NO_MORE_FILES
								.break
							.else
								invoke PrintConsole, $CTA0("Error: Some error has occured while enumerating files.\n"), 0
								.break
							.endif
						.endif
					.endw
					invoke FindClose, hFindFile
				.else
					invoke PrintConsole, $CTA0("Error: Couldn't find any *.lib file.\n"), 0
				.endif

			.else
				invoke MakePathFullIfNeeded, esi, addr g_acLibraryPath, sizeof g_acLibraryPath
				invoke ParseLibrary;, addr g_acLibraryPath
			.endif

		.endif
	.else
		invoke MessageBox, NULL, $CTA0("Can't get console standard output handle."), NULL, MB_OK + MB_ICONERROR
	.endif

	xor eax, eax
	ret

start endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end	start

:make

set exe=protogen

:makerc
if exist rsrc.obj goto final
	\masm32\bin\rc /v rsrc.rc
	\masm32\bin\cvtres /machine:ix86 rsrc.res
	if errorlevel 0 goto final
		echo.
		pause
		exit

:final

rem Use DEBUG or RELEASE to make appropriate build
set conf=RELEASE

if exist rsrc.res del rsrc.res

\masm32\bin\ml /nologo /c /coff /D%conf% %exe%.bat
\masm32\bin\link /nologo /out:%exe%.exe /subsystem:console /merge:.idata=.text /merge:.rdata=.text /merge:.data=.text /section:.text,EWR /ignore:4078 %exe%.obj strcmp.obj  rsrc.obj

del %exe%.obj

echo.
pause
exit
