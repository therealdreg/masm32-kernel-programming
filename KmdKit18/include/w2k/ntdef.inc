comment ^
Module Name:
    ntdef.inc

Abstract:
    Type definitions for the basic types.

Author:
    Four-F (four-f@mail.ru)

Last updated:    
    27-August-2004
    
IMPORTANT:
	Hand made -> Bugs are very possible :(
	Your bug report is very welcome.

Comments:
    Some fields name have been changed because of collision with masm reserved words.
    All such words are prefixed with underscore ('_') symbol.
^

IFNDEF _NTDEF_
_NTDEF_ equ 1

CALLBACK	typedef		proto STDCALL
WINAPI		typedef		proto STDCALL
NTAPI		typedef		proto STDCALL

BOOL		typedef		DWORD

PBYTE		typedef		PTR BYTE
PWORD		typedef		PTR DORD
PDWORD		typedef		PTR DWORD
PVOID		typedef		PTR
PVOID64		typedef		PTR
PCVOID		typedef		PTR
PSTR		typedef		PTR BYTE
PCSTR		typedef		PTR BYTE
PWSTR		typedef		PTR WORD

PDWORD_PTR	typedef		PTR DWORD

UINT		typedef		DWORD

HFILE		typedef		DWORD

IFNDEF ANYSIZE_ARRAY
	ANYSIZE_ARRAY	equ 1	; winnt
ENDIF

; Basics

char	typedef BYTE
CHAR	typedef BYTE
long	typedef	SDWORD
LONG	typedef	SDWORD

; UNICODE (Wide Character) types

WCHAR	typedef	WORD	; wc,   16-bit UNICODE character
wchar_t	typedef	WORD	; wc,   16-bit UNICODE character

PWCHAR	typedef	PTR WORD
LPWCH	typedef	PTR WORD
PWCH	typedef	PTR WORD

LPCWCH	typedef	PTR WORD
PCWCH	typedef	PTR WORD

NWPSTR	typedef	PTR WORD

LPWSTR	typedef	PTR WORD
PWSTR	typedef	PTR WORD

LPCWSTR	typedef	PTR WORD
PCWSTR	typedef	PTR WORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; ANSI (Multi-byte Character) types
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PCHAR	typedef	PTR BYTE
LPCH	typedef	PTR BYTE
PCH		typedef	PTR BYTE

LPCCH	typedef	PTR BYTE
PCCH	typedef	PTR BYTE

NPSTR	typedef	PTR BYTE

LPSTR	typedef	PTR BYTE
PSTR	typedef	PTR BYTE

LPCSTR	typedef	PTR BYTE
PCSTR	typedef	PTR BYTE

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Neutral ANSI/UNICODE types and macros
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFDEF UNICODE

	TCHAR	typedef	WORD
	PTCHAR	typedef	PTR WORD

	TUCHAR	typedef	WORD
	PTUCHAR	typedef	PTR WORD

	LPTCH	typedef	PTR WORD
	PTCH	typedef	PTR WORD

	PTSTR	typedef	PTR WORD
	LPTSTR	typedef	PTR WORD

	PCTSTR	typedef	PTR WORD
	LPCTSTR	typedef	PTR WORD

	LP		typedef	PTR WORD

ELSE		; UNICODE

	TCHAR	typedef	SBYTE
	PTCHAR	typedef	PTR SBYTE

	TUCHAR	typedef	BYTE
	PTUCHAR	typedef	PTR BYTE

ENDIF		; UNICODE
comment ^
typedef LPSTR LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR;
typedef LPCSTR PCTSTR, LPCTSTR;
^
double	typedef	QWORD
DOUBLE	typedef	QWORD

QUAD STRUCT							; QUAD is for those times we want
	DoNotUseThisField	QWORD	?	; an 8 byte aligned 8 byte long structure
QUAD ENDS							; which is NOT really a floating point
									; number.  Use DOUBLE if you want an FP
									; number.

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Pointer to Basics
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PSHORT	typedef	SWORD
PLONG	typedef	PTR SDWORD
PQUAD	typedef	PTR QWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Unsigned Basics
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; Tell windef.h that some types are already defined.

; #define BASETYPES

UCHAR	typedef	BYTE
USHORT	typedef	WORD
ULONG	typedef	DWORD
UQUAD	typedef	QWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Pointer to Unsigned Basics
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PUCHAR	typedef	PTR BYTE
PUSHORT	typedef	PTR WORD
PULONG	typedef	PTR DWORD
PUQUAD	typedef	PTR QWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Signed characters
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SCHAR	typedef SBYTE
PSCHAR	typedef PTR SBYTE

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Handle to an Object
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

HANDLE	typedef	DWORD
PHANDLE	typedef	PTR DWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Flag (bit) fields
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FCHAR	typedef	BYTE
FSHORT	typedef	WORD
FLONG	typedef	DWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Low order two bits of a handle are ignored by the system and available
; for use by application code as tag bits.  The remaining bits are opaque
; and used to store a serial number and table index.
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Cardinal Data Types [0 - 2**N-2)
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CCHAR	typedef	BYTE
CSHORT	typedef	SWORD
CLONG	typedef	SDWORD

PCCHAR	typedef	PTR BYTE
PCSHORT	typedef	PTR SWORD
PCLONG	typedef	PTR SDWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; NLS basics (Locale and Language Ids)
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

LCID	typedef DWORD
PLCID	typedef PTR DWORD
LANGID	typedef WORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Logical Data Type - These are 32-bit logical values.
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

LOGICAL		typedef	DWORD
PLOGICAL	typedef	PTR DWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; NTSTATUS
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

NTSTATUS	typedef	SDWORD
PNTSTATUS	typedef	PTR SDWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Status values are 32 bit values layed out as follows:
;
;   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
;   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
;  +---+-+-------------------------+-------------------------------+
;  |Sev|C|       Facility          |               Code            |
;  +---+-+-------------------------+-------------------------------+
;
;  where
;
;      Sev - is the severity code
;
;          00 - Success
;          01 - Informational
;          10 - Warning
;          11 - Error
;
;      C - is the Customer code flag
;
;      Facility - is the facility code
;
;      Code - is the facility's status code
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

APPLICATION_ERROR_MASK       equ 020000000h
ERROR_SEVERITY_SUCCESS       equ 000000000h
ERROR_SEVERITY_INFORMATIONAL equ 040000000h
ERROR_SEVERITY_WARNING       equ 080000000h
ERROR_SEVERITY_ERROR         equ 0C0000000h

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Large (64-bit) integer types and operations
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TIME UNION
	struct
		union
			LowPart		DWORD ?
			LowTime		DWORD ?
		ends
		union
			HighPart	SDWORD ?
			HighTime	SDWORD ?
		ends
	ends
	struct u
		union
			LowPart    DWORD ?
			LowTime    DWORD ?
		ends
		union
			HighPart   SDWORD ?
			HighTime   SDWORD ?
		ends
	ends
	QuadPart     QWORD ?	; signed
TIME ENDS
PTIME typedef PTR TIME

LONGLONG	typedef QWORD
ULONGLONG	typedef QWORD

PLONGLONG	typedef PTR QWORD
PULONGLONG	typedef PTR QWORD

USN			typedef QWORD

LARGE_INTEGER UNION
	struct
		LowPart		DWORD ?
		HighPart	SDWORD ?
	ends
	struct u
		LowPart		DWORD ?
		HighPart	SDWORD ?
	ends
	QuadPart		QWORD ?	; signed
LARGE_INTEGER ENDS
PLARGE_INTEGER typedef PTR LARGE_INTEGER

ULARGE_INTEGER UNION
	struct
		LowPart		DWORD ?
		HighPart	DWORD ?
	ends
	struct u
		LowPart		DWORD ?
		HighPart	DWORD ?
	ends
	QuadPart		QWORD ?
ULARGE_INTEGER ENDS
PULARGE_INTEGER	typedef	PTR ULARGE_INTEGER

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Locally Unique Identifier
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

LUID STRUCT
	LowPart		DWORD	?
	HighPart	SDWORD	?
LUID ENDS
PLUID typedef PTR LUID

DWORDLONG	typedef QWORD
PDWORDLONG	typedef PTR QWORD

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Physical address.
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PHYSICAL_ADDRESS UNION
	struct
		LowPart    DWORD ?
		HighPart   SDWORD ?
	ends
	struct u
		LowPart    DWORD ?
		HighPart   SDWORD ?
	ends
	QuadPart     QWORD ?	; signed
PHYSICAL_ADDRESS ENDS
PPHYSICAL_ADDRESS typedef PTR PHYSICAL_ADDRESS

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define operations to logically shift an int64 by 0..31 bits and to multiply
; 32-bits by 32-bits to form a 64-bit product.
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Event type
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _EVENT_TYPE {
	NotificationEvent		equ 0
	SynchronizationEvent	equ 1

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Timer type
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _TIMER_TYPE {
	NotificationTimer		equ 0
	SynchronizationTimer	equ 1

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Wait type
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _WAIT_TYPE {
	WaitAll	equ 0
	WaitAny	equ 1

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Pointer to an Asciiz string
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PSZ		typedef	PTR BYTE
PCSZ	typedef	PTR BYTE

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Counted String
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_STRING STRUCT
	_Length			WORD	?
	MaximumLength	WORD	?
	Buffer			DWORD	?	; PCHAR
_STRING ENDS
PSTRING typedef PTR _STRING


ANSI_STRING equ <_STRING>
PANSI_STRING typedef PTR _STRING

;typedef STRING ANSI_STRING;
;typedef PSTRING PANSI_STRING;

OEM_STRING equ <_STRING>
POEM_STRING typedef PTR _STRING

;typedef STRING OEM_STRING;
;typedef PSTRING POEM_STRING;

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; CONSTCounted String
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CSTRING STRUCT
	_Length		WORD	?
	MaximumLength	WORD	?
	Buffer			DWORD	?	; CONST char *
CSTRING ENDS
PCSTRING typedef PTR CSTRING

ANSI_NULL	equ 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Unicode strings are counted 16-bit character strings. If they are
; NULL terminated, Length does not include trailing NULL.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFNDEF UNICODE_STRING
	UNICODE_STRING STRUCT
		_Length		WORD	?		; len of string in bytes (not chars)
		MaximumLength	WORD	?		; len of Buffer in bytes (not chars)
		Buffer			PWSTR	?		; pointer to string
	UNICODE_STRING ENDS
	PUNICODE_STRING	typedef	PTR UNICODE_STRING
ENDIF

UNICODE_NULL	equ 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Boolean
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BOOLEAN		typedef		BYTE
PBOOLEAN	typedef		PTR BYTE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Doubly linked list structure.  Can be used as either a list head, or
;  as link words.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

LIST_ENTRY STRUCT
	Flink	DWORD	?	; PTR LIST_ENTRY
	Blink	DWORD	?	; PTR LIST_ENTRY
LIST_ENTRY ENDS
PLIST_ENTRY typedef PTR LIST_ENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Singly linked list structure. Can be used as either a list head, or
;  as link words.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SINGLE_LIST_ENTRY STRUCT
	Next	DWORD	?	; PTR SINGLE_LIST_ENTRY
SINGLE_LIST_ENTRY ENDS
PSINGLE_LIST_ENTRY typedef PTR SINGLE_LIST_ENTRY

; These are needed for portable debugger support.

LIST_ENTRY32 STRUCT
	Flink	DWORD	?
	Blink	DWORD	?
LIST_ENTRY32 ENDS
PLIST_ENTRY32 typedef PTR LIST_ENTRY32

LIST_ENTRY64 STRUCT
	Flink	QWORD	?
	Blink	QWORD	?
LIST_ENTRY64 ENDS
PLIST_ENTRY64 typedef PTR LIST_ENTRY64

STRING32 STRUCT
	_Length			WORD	?
	MaximumLength	WORD	?
	Buffer			DWORD	?
STRING32 ENDS
PSTRING32 typedef ptr STRING32

UNICODE_STRING32 equ <STRING32>
PUNICODE_STRING32 typedef ptr UNICODE_STRING32

ANSI_STRING32 equ <STRING32>
PANSI_STRING32 typedef ptr ANSI_STRING32

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Valid values for the Attributes field
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OBJ_INHERIT             equ 00000002h
OBJ_PERMANENT           equ 00000010h
OBJ_EXCLUSIVE           equ 00000020h
OBJ_CASE_INSENSITIVE    equ 00000040h
OBJ_OPENIF              equ 00000080h
OBJ_OPENLINK            equ 00000100h
OBJ_KERNEL_HANDLE       equ 00000200h
OBJ_VALID_ATTRIBUTES    equ 000003F2h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Object Attributes structure
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OBJECT_ATTRIBUTES STRUCT		; sizeof = 18h
	_Length						DWORD			? ; original name Length
	RootDirectory				HANDLE			?
	ObjectName					PUNICODE_STRING	?
	Attributes					DWORD			?
	SecurityDescriptor			PVOID			? ; Points to type SECURITY_DESCRIPTOR
	SecurityQualityOfService	PVOID			? ; Points to type SECURITY_QUALITY_OF_SERVICE
OBJECT_ATTRIBUTES ENDS
POBJECT_ATTRIBUTES typedef ptr OBJECT_ATTRIBUTES

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; VOID
; InitializeObjectAttributes(
;     OUT POBJECT_ATTRIBUTES p,
;     IN PUNICODE_STRING n,
;     IN ULONG a,
;     IN HANDLE r,
;     IN PSECURITY_DESCRIPTOR s
;     )
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;#define InitializeObjectAttributes( p, n, a, r, s ) { \
;    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
;    (p)->RootDirectory = r;                             \
;    (p)->Attributes = a;                                \
;    (p)->ObjectName = n;                                \
;    (p)->SecurityDescriptor = s;                        \
;    (p)->SecurityQualityOfService = NULL;               \
;    }

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; +
; The following $IsXxx macros is not a part of original ntdef.h

$IsImm MACRO Operand:REQ
	IF (OPATTR (Operand)) AND 00000100y
		;; Is an immediate value
        EXITM <-1>
    ELSE
        EXITM <0>
    ENDIF
ENDM

$IsMem MACRO Operand:REQ
	IF (OPATTR (Operand)) AND 00000010y
		;; Is a memory variable or has a relocatable data label
		EXITM <-1>              ;; True
	ELSE
		EXITM <0>               ;; False
	ENDIF
ENDM

$IsReg MACRO Operand:REQ
	IF (OPATTR (Operand)) AND 00010000y
		;; Is a register value
        EXITM <-1>
    ELSE
        EXITM <0>
    ENDIF
ENDM

$IsStack MACRO Operand:REQ
	IF (OPATTR (Operand)) AND 01000000y
		;; relative to SS
        EXITM <-1>
    ELSE
        EXITM <0>
    ENDIF
ENDM

$IsAddr2 MACRO Operand:REQ
;	local a
;	a = 0
	IF @SizeStr(<Operand>) GT 5
		IFIDNI <addr >, @SubStr(<Operand>, 1 , 5)
			EXITM <-1>
;;			a = 1
		ENDIF
	ENDIF
;;	IF a
;;        EXITM <-1>
;;    ELSE
        EXITM <0>
;;    ENDIF
ENDM

$IsOffset2 MACRO Operand:REQ
;	local a
;	a = 0
	IF @SizeStr(<Operand>) GT 7
		echo *************
		IFIDNI <offset >, @SubStr(<Operand>, 1 , 7)
		echo *************
			EXITM <-1>
;;			a = 1
		ENDIF
	ENDIF

;;	IF a
;;        EXITM <-1>
;;    ELSE
        EXITM <0>
;;    ENDIF
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

InitializeObjectAttributes MACRO p:REQ, n:REQ, a:REQ, r:REQ, s:REQ

;; ECX is used to hold a pointer to OBJECT_ATTRIBUTES
;; EAX is used if stack variable passed

;; p - Pointer to the OBJECT_ATTRIBUTES structure to initialize
;; n - ObjectName
;; a - Attributes
;; r - RootDirectory
;; s - SecurityDescriptor


;; Be very carefull with this macro !!!
;; It can contain some hidden bugs !!!
;; In ambiguous cases fill OBJECT_ATTRIBUTES structure manually

	local adr, reax, reax, line
	reax = 0
	recx = 0

	IF $IsAddr2(p)
		adr SUBSTR <p>, 6
		IF $IsStack(adr)			;; is relative to SS
			lea ecx, adr
		ELSE
			mov ecx, offset adr
		ENDIF
		recx = 1				;; no more ecx
	ELSEIF (OPATTR (p)) AND 00010000y
		;; is a register value
		IFDIFI <p>, <ecx>		;; not ecx
			mov ecx, p
		ENDIF
	ELSEIF (OPATTR (p)) AND 00000010y
		;; is a memory variable or has a relocatable data label (offset)
		mov ecx, p
	ELSEIF (OPATTR (p)) AND 01000000y	;; ELSEIF $IsStack(p)
		;; relative to SS
		mov ecx, p
		recx = 1					;; no more ecx
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! Pointer to OBJECT_ATTRIBUTES structure improperly specified.
	ENDIF

	PUSHCONTEXT ASSUMES
	assume ecx:ptr OBJECT_ATTRIBUTES
	mov [ecx]._Length, sizeof OBJECT_ATTRIBUTES


	;; RootDirectory - Specifies a handle to the root object directory for the path name specified in the ObjectName parameter.
	;; If ObjectName parameter is a fully-qualified object name, RootDirectory is NULL.
	IF (OPATTR (r)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push r
		pop [ecx].RootDirectory
	ELSEIF (OPATTR (r)) AND 00010000y
		;; is a register value
		IFDIFI <r>, <ecx>
			mov [ecx].RootDirectory, r
		ELSE
			line TEXTEQU %@Line
			.ERR
			% ECHO @FileCur(line) : ERROR! ECX register value overwritten by InitializeObjectAttributes macro.
		ENDIF
	ELSEIF (OPATTR (r)) AND 01000000y	;; ELSEIF $IsStack(r)
		;; relative to SS
		push r
		pop [ecx].RootDirectory
	ELSEIF (OPATTR (r)) AND 00000100y	;; ELSEIF $IsImm(r)
		;; Is an immediate value
		IF r EQ 0
			and [ecx].RootDirectory, 0	;; NULL
		ELSE
			mov [ecx].RootDirectory, r
		ENDIF
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! RootDirectory improperly specified.
	ENDIF


	;; Attributes - Specifies one or more flags:
	IF (OPATTR (a)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push a
		pop [ecx].Attributes
	ELSEIF (OPATTR (a)) AND 00010000y
		;; is a register value
		IFDIFI <a>, <ecx>		;; not ecx
			mov [ecx].Attributes, a
		ELSE
			line TEXTEQU %@Line
			.ERR
			% ECHO @FileCur(line) : ERROR! ECX register value overwritten by InitializeObjectAttributes macro.
		ENDIF
	ELSEIF (OPATTR (a)) AND 01000000y	;; ELSEIF $IsStack(a)
		;; relative to SS
		push a
		pop [ecx].Attributes
	ELSEIF (OPATTR (a)) AND 00000100y	;; ELSEIF $IsImm(a)
		;; Is an immediate value
		IF a EQ 0
			and [ecx].Attributes, 0	;; NULL
		ELSE
			mov [ecx].Attributes, a
		ENDIF
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! Attributes improperly specified.
	ENDIF


	;; SecurityDescriptor - Specifies a security descriptor to apply to an object when it is created.
	IF (OPATTR (s)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push s
		pop [ecx].SecurityDescriptor
	ELSEIF (OPATTR (s)) AND 00010000y
		;; is a register value
		IFIDNI <s>, <ecx>		;; ecx ?
			line TEXTEQU %@Line
			.ERR
			% ECHO @FileCur(line) : ERROR! ECX register value overwritten by InitializeObjectAttributes macro.
		ELSE
			mov [ecx].SecurityDescriptor, s
		ENDIF
	ELSEIF (OPATTR (s)) AND 01000000y	;; ELSEIF $IsStack(s)
		;; relative to SS
		push s
		pop [ecx].SecurityDescriptor
	ELSEIF (OPATTR (s)) AND 00000100y	;; ELSEIF $IsImm(s)
		;; Is an immediate value
		IF s EQ 0
			and [ecx].SecurityDescriptor, 0	;; NULL
		ELSE
			mov [ecx].SecurityDescriptor, s
		ENDIF
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! SecurityDescriptor improperly specified.
	ENDIF

	;; ObjectName - Specifies the Unicode string name of the object for which a handle is to be opened.
	IF $IsAddr2(n)
		adr SUBSTR <n>, 6
		IF $IsStack(adr)			;; is relative to SS
			lea eax, adr
			mov [ecx].ObjectName, eax
			reax = 1				;; no more eax
		ELSE
			mov [ecx].ObjectName, offset adr
		ENDIF
	ELSEIF (OPATTR (n)) AND 00010000y
		;; is a register value
		IFDIFI <n>, <ecx>		;; not ecx
			mov [ecx].ObjectName, n
		ELSE
			line TEXTEQU %@Line
			.ERR
			% ECHO @FileCur(line) : ERROR! ECX register value overwritten by InitializeObjectAttributes macro.
		ENDIF
	ELSEIF (OPATTR (n)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push n
		pop [ecx].ObjectName
	ELSEIF (OPATTR (n)) AND 00000100y
		;; Is an immediate value
		IF n EQ 0
			and [ecx].ObjectName, 0	;; NULL
		ELSE
			mov [ecx].ObjectName, n
		ENDIF
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! ObjectName improperly specified.
	ENDIF
	
	and [ecx].SecurityQualityOfService, 0	;; NULL
	assume ecx:nothing
	POPCONTEXT ASSUMES
ENDM

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Constants
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


FALSE	equ 0
TRUE	equ 1

NULL	equ 0
NULL64	equ 0

MINCHAR     equ 080h
MAXCHAR     equ 07fh
MINSHORT    equ 08000h
MAXSHORT    equ 07fffh
MINLONG     equ 080000000h
MAXLONG     equ 07fffffffh
MAXUCHAR    equ 0ffh
MAXUSHORT   equ 0ffffh
MAXULONG    equ 0ffffffffh

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Useful Helper Macros
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; CONTAINING_RECORD calculates the address of the base of the structure given its type,
; and an address of a field within the structure.

CONTAINING_RECORD MACRO Address:REQ, _Type:REQ, Field:REQ

	local adr, line

	IF $IsAddr2(Address)
		adr SUBSTR <Address>, 6
		IF (OPATTR (adr)) AND 01000000y
			;; relative to SS
			lea eax, adr
		ELSE
			mov eax, offset adr
		ENDIF
	ELSEIF (OPATTR (Address)) AND 00010000y
		;; is a register value
		IFDIFI <Address>, <eax>		;; not eax
			mov eax, Address
		ENDIF
	ELSEIF (OPATTR (Address)) AND 00000010y
		;; is a memory variable or has a relocatable data label (offset)
		mov eax, Address
	ELSEIF (OPATTR (Address)) AND 01000000y
		;; relative to SS
		mov eax, Address
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! Pointer to a field in an instance of a structure of type _Type is improperly specified.
	ENDIF
	
	IFE (OPATTR (_Type)) AND 00000100y
		;; Is not an immediate value.
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! The second parameter of CONTAINING_RECORD macro must be a structure name whose base address is to be returned.
	ENDIF

	IF (OPATTR (Field))
		;; OPATTR doesn't recognizes structure members.  So if it returns something
		;; Field is not a name of structure member.
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! The third parameter of CONTAINING_RECORD macro must be a name of the member pointed to by Address and which is contained in a structure of type _Type. 
	ENDIF

	lea eax, (_Type PTR [eax-_Type.Field])
ENDM

$CONTAINING_RECORD MACRO Address:REQ, _Type:REQ, Field:REQ

	local adr, line

	IF $IsAddr2(Address)
		adr SUBSTR <Address>, 6
		IF (OPATTR (adr)) AND 01000000y
			;; relative to SS
			lea eax, adr
		ELSE
			mov eax, offset adr
		ENDIF
	ELSEIF (OPATTR (Address)) AND 00010000y
		;; is a register value
		IFDIFI <Address>, <eax>		;; not eax
			mov eax, Address
		ENDIF
	ELSEIF (OPATTR (Address)) AND 00000010y
		;; is a memory variable or has a relocatable data label (offset)
		mov eax, Address
	ELSEIF (OPATTR (Address)) AND 01000000y
		;; relative to SS
		mov eax, Address
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! Pointer to a field in an instance of a structure of type _Type is improperly specified.
	ENDIF
	
	IFE (OPATTR (_Type)) AND 00000100y
		;; Is not an immediate value.
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! The second parameter of CONTAINING_RECORD macro must be a structure name whose base address is to be returned.
	ENDIF

	IF (OPATTR (Field))
		;; OPATTR doesn't recognizes structure members.  So if it returns something
		;; Field is not a name of structure member.
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! The third parameter of CONTAINING_RECORD macro must be a name of the member pointed to by Address and which is contained in a structure of type _Type. 
	ENDIF

	lea eax, (_Type PTR [eax-_Type.Field])
	EXITM <eax>
ENDM

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Interrupt Request Level (IRQL)
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KIRQL	typedef BYTE
PKIRQL	typedef PTR BYTE

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Product types
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _NT_PRODUCT_TYPE {
    NtProductWinNt		equ 1
    NtProductLanManNt	equ 2
    NtProductServer		equ 3

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; the bit mask, SharedUserData->SuiteMask, is a ULONG
; so there can be a maximum of 32 entries in this enum.
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _SUITE_TYPE {
    SmallBusiness			equ 0
    Enterprise				equ 1
    BackOffice				equ 2
    CommunicationServer		equ 3
    TerminalServer			equ 4
    SmallBusinessRestricted	equ 5
    EmbeddedNT				equ 6
    DataCenter				equ 7
    SingleUserTS			equ 8
    Personal				equ 9
    ServerAppliance			equ 10
    MaxSuiteType			equ 11

VER_SERVER_NT                       equ 80000000h
VER_WORKSTATION_NT                  equ 40000000h
VER_SUITE_SMALLBUSINESS             equ 00000001h
VER_SUITE_ENTERPRISE                equ 00000002h
VER_SUITE_BACKOFFICE                equ 00000004h
VER_SUITE_COMMUNICATIONS            equ 00000008h
VER_SUITE_TERMINAL                  equ 00000010h
VER_SUITE_SMALLBUSINESS_RESTRICTED  equ 00000020h
VER_SUITE_EMBEDDEDNT                equ 00000040h
VER_SUITE_DATACENTER                equ 00000080h
VER_SUITE_SINGLEUSERTS              equ 00000100h
VER_SUITE_PERSONAL                  equ 00000200h
VER_SUITE_SERVERAPPLIANCE           equ 00000400h

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Language IDs.
;
;  The following two combinations of primary language ID and
;  sublanguage ID have special semantics:
;
;    Primary Language ID   Sublanguage ID      Result
;    -------------------   ---------------     ------------------------
;    LANG_NEUTRAL          SUBLANG_NEUTRAL     Language neutral
;    LANG_NEUTRAL          SUBLANG_DEFAULT     User default language
;    LANG_NEUTRAL          SUBLANG_SYS_DEFAULT System default language
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Primary language IDs.
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

LANG_NEUTRAL                     equ 00

LANG_AFRIKAANS                   equ 36h
LANG_ALBANIAN                    equ 1ch
LANG_ARABIC                      equ 01h
LANG_ARMENIAN                    equ 2bh
LANG_ASSAMESE                    equ 4dh
LANG_AZERI                       equ 2ch
LANG_BASQUE                      equ 2dh
LANG_BELARUSIAN                  equ 23h
LANG_BENGALI                     equ 45h
LANG_BULGARIAN                   equ 02h
LANG_CATALAN                     equ 03h
LANG_CHINESE                     equ 04h
LANG_CROATIAN                    equ 1ah
LANG_CZECH                       equ 05h
LANG_DANISH                      equ 06h
LANG_DUTCH                       equ 13h
LANG_ENGLISH                     equ 09h
LANG_ESTONIAN                    equ 25h
LANG_FAEROESE                    equ 38h
LANG_FARSI                       equ 29h
LANG_FINNISH                     equ 0bh
LANG_FRENCH                      equ 0ch
LANG_GEORGIAN                    equ 37h
LANG_GERMAN                      equ 07h
LANG_GREEK                       equ 08h
LANG_GUJARATI                    equ 47h
LANG_HEBREW                      equ 0dh
LANG_HINDI                       equ 39h
LANG_HUNGARIAN                   equ 0eh
LANG_ICELANDIC                   equ 0fh
LANG_INDONESIAN                  equ 21h
LANG_ITALIAN                     equ 10h
LANG_JAPANESE                    equ 11h
LANG_KANNADA                     equ 4bh
LANG_KASHMIRI                    equ 60h
LANG_KAZAK                       equ 3fh
LANG_KONKANI                     equ 57h
LANG_KOREAN                      equ 12h
LANG_LATVIAN                     equ 26h
LANG_LITHUANIAN                  equ 27h
LANG_MACEDONIAN                  equ 2fh   ; the Former Yugoslav Republic of Macedonia
LANG_MALAY                       equ 3eh
LANG_MALAYALAM                   equ 4ch
LANG_MANIPURI                    equ 58h
LANG_MARATHI                     equ 4eh
LANG_NEPALI                      equ 61h
LANG_NORWEGIAN                   equ 14h
LANG_ORIYA                       equ 48h
LANG_POLISH                      equ 15h
LANG_PORTUGUESE                  equ 16h
LANG_PUNJABI                     equ 46h
LANG_ROMANIAN                    equ 18h
LANG_RUSSIAN                     equ 19h
LANG_SANSKRIT                    equ 4fh
LANG_SERBIAN                     equ 1ah
LANG_SINDHI                      equ 59h
LANG_SLOVAK                      equ 1bh
LANG_SLOVENIAN                   equ 24h
LANG_SPANISH                     equ 0ah
LANG_SWAHILI                     equ 41h
LANG_SWEDISH                     equ 1dh
LANG_TAMIL                       equ 49h
LANG_TATAR                       equ 44h
LANG_TELUGU                      equ 4ah
LANG_THAI                        equ 1eh
LANG_TURKISH                     equ 1fh
LANG_UKRAINIAN                   equ 22h
LANG_URDU                        equ 20h
LANG_UZBEK                       equ 43h
LANG_VIETNAMESE                  equ 2ah

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Sublanguage IDs.
;
;  The name immediately following SUBLANG_ dictates which primary
;  language ID that sublanguage ID can be combined with to form a
;  valid language ID.
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SUBLANG_NEUTRAL                  equ 00    ; language neutral
SUBLANG_DEFAULT                  equ 01    ; user default
SUBLANG_SYS_DEFAULT              equ 02    ; system default

SUBLANG_ARABIC_SAUDI_ARABIA      equ 01    ; Arabic (Saudi Arabia)
SUBLANG_ARABIC_IRAQ              equ 02    ; Arabic (Iraq)
SUBLANG_ARABIC_EGYPT             equ 03    ; Arabic (Egypt)
SUBLANG_ARABIC_LIBYA             equ 04    ; Arabic (Libya)
SUBLANG_ARABIC_ALGERIA           equ 05    ; Arabic (Algeria)
SUBLANG_ARABIC_MOROCCO           equ 06    ; Arabic (Morocco)
SUBLANG_ARABIC_TUNISIA           equ 07    ; Arabic (Tunisia)
SUBLANG_ARABIC_OMAN              equ 08    ; Arabic (Oman)
SUBLANG_ARABIC_YEMEN             equ 09    ; Arabic (Yemen)
SUBLANG_ARABIC_SYRIA             equ 0ah   ; Arabic (Syria)
SUBLANG_ARABIC_JORDAN            equ 0bh   ; Arabic (Jordan)
SUBLANG_ARABIC_LEBANON           equ 0ch   ; Arabic (Lebanon)
SUBLANG_ARABIC_KUWAIT            equ 0dh   ; Arabic (Kuwait)
SUBLANG_ARABIC_UAE               equ 0eh   ; Arabic (U.A.E)
SUBLANG_ARABIC_BAHRAIN           equ 0fh   ; Arabic (Bahrain)
SUBLANG_ARABIC_QATAR             equ 10h   ; Arabic (Qatar)
SUBLANG_AZERI_LATIN              equ 01    ; Azeri (Latin)
SUBLANG_AZERI_CYRILLIC           equ 02    ; Azeri (Cyrillic)
SUBLANG_CHINESE_TRADITIONAL      equ 01    ; Chinese (Taiwan)
SUBLANG_CHINESE_SIMPLIFIED       equ 02    ; Chinese (PR China)
SUBLANG_CHINESE_HONGKONG         equ 03    ; Chinese (Hong Kong S.A.R., P.R.C.)
SUBLANG_CHINESE_SINGAPORE        equ 04    ; Chinese (Singapore)
SUBLANG_CHINESE_MACAU            equ 05    ; Chinese (Macau)
SUBLANG_DUTCH                    equ 01    ; Dutch
SUBLANG_DUTCH_BELGIAN            equ 02    ; Dutch (Belgian)
SUBLANG_ENGLISH_US               equ 01    ; English (USA)
SUBLANG_ENGLISH_UK               equ 02    ; English (UK)
SUBLANG_ENGLISH_AUS              equ 03    ; English (Australian)
SUBLANG_ENGLISH_CAN              equ 04    ; English (Canadian)
SUBLANG_ENGLISH_NZ               equ 05    ; English (New Zealand)
SUBLANG_ENGLISH_EIRE             equ 06    ; English (Irish)
SUBLANG_ENGLISH_SOUTH_AFRICA     equ 07    ; English (South Africa)
SUBLANG_ENGLISH_JAMAICA          equ 08    ; English (Jamaica)
SUBLANG_ENGLISH_CARIBBEAN        equ 09    ; English (Caribbean)
SUBLANG_ENGLISH_BELIZE           equ 0ah   ; English (Belize)
SUBLANG_ENGLISH_TRINIDAD         equ 0bh   ; English (Trinidad)
SUBLANG_ENGLISH_ZIMBABWE         equ 0ch   ; English (Zimbabwe)
SUBLANG_ENGLISH_PHILIPPINES      equ 0dh   ; English (Philippines)
SUBLANG_FRENCH                   equ 01    ; French
SUBLANG_FRENCH_BELGIAN           equ 02    ; French (Belgian)
SUBLANG_FRENCH_CANADIAN          equ 03    ; French (Canadian)
SUBLANG_FRENCH_SWISS             equ 04    ; French (Swiss)
SUBLANG_FRENCH_LUXEMBOURG        equ 05    ; French (Luxembourg)
SUBLANG_FRENCH_MONACO            equ 06    ; French (Monaco)
SUBLANG_GERMAN                   equ 01    ; German
SUBLANG_GERMAN_SWISS             equ 02    ; German (Swiss)
SUBLANG_GERMAN_AUSTRIAN          equ 03    ; German (Austrian)
SUBLANG_GERMAN_LUXEMBOURG        equ 04    ; German (Luxembourg)
SUBLANG_GERMAN_LIECHTENSTEIN     equ 05    ; German (Liechtenstein)
SUBLANG_ITALIAN                  equ 01    ; Italian
SUBLANG_ITALIAN_SWISS            equ 02    ; Italian (Swiss)
SUBLANG_KASHMIRI_INDIA           equ 02    ; Kashmiri (India)
SUBLANG_KOREAN                   equ 01    ; Korean (Extended Wansung)
SUBLANG_LITHUANIAN               equ 01    ; Lithuanian
SUBLANG_MALAY_MALAYSIA           equ 01    ; Malay (Malaysia)
SUBLANG_MALAY_BRUNEI_DARUSSALAM  equ 02    ; Malay (Brunei Darussalam)
SUBLANG_NEPALI_INDIA             equ 02    ; Nepali (India)
SUBLANG_NORWEGIAN_BOKMAL         equ 01    ; Norwegian (Bokmal)
SUBLANG_NORWEGIAN_NYNORSK        equ 02    ; Norwegian (Nynorsk)
SUBLANG_PORTUGUESE               equ 02    ; Portuguese
SUBLANG_PORTUGUESE_BRAZILIAN     equ 01    ; Portuguese (Brazilian)
SUBLANG_SERBIAN_LATIN            equ 02    ; Serbian (Latin)
SUBLANG_SERBIAN_CYRILLIC         equ 03    ; Serbian (Cyrillic)
SUBLANG_SPANISH                  equ 01    ; Spanish (Castilian)
SUBLANG_SPANISH_MEXICAN          equ 02    ; Spanish (Mexican)
SUBLANG_SPANISH_MODERN           equ 03    ; Spanish (Modern)
SUBLANG_SPANISH_GUATEMALA        equ 04    ; Spanish (Guatemala)
SUBLANG_SPANISH_COSTA_RICA       equ 05    ; Spanish (Costa Rica)
SUBLANG_SPANISH_PANAMA           equ 06    ; Spanish (Panama)
SUBLANG_SPANISH_DOMINICAN_REPUBLIC equ 07  ; Spanish (Dominican Republic)
SUBLANG_SPANISH_VENEZUELA        equ 08    ; Spanish (Venezuela)
SUBLANG_SPANISH_COLOMBIA         equ 09    ; Spanish (Colombia)
SUBLANG_SPANISH_PERU             equ 0ah   ; Spanish (Peru)
SUBLANG_SPANISH_ARGENTINA        equ 0bh   ; Spanish (Argentina)
SUBLANG_SPANISH_ECUADOR          equ 0ch   ; Spanish (Ecuador)
SUBLANG_SPANISH_CHILE            equ 0dh   ; Spanish (Chile)
SUBLANG_SPANISH_URUGUAY          equ 0eh   ; Spanish (Uruguay)
SUBLANG_SPANISH_PARAGUAY         equ 0fh   ; Spanish (Paraguay)
SUBLANG_SPANISH_BOLIVIA          equ 10h   ; Spanish (Bolivia)
SUBLANG_SPANISH_EL_SALVADOR      equ 11h   ; Spanish (El Salvador)
SUBLANG_SPANISH_HONDURAS         equ 12h   ; Spanish (Honduras)
SUBLANG_SPANISH_NICARAGUA        equ 13h   ; Spanish (Nicaragua)
SUBLANG_SPANISH_PUERTO_RICO      equ 14h   ; Spanish (Puerto Rico)
SUBLANG_SWEDISH                  equ 01    ; Swedish
SUBLANG_SWEDISH_FINLAND          equ 02    ; Swedish (Finland)
SUBLANG_URDU_PAKISTAN            equ 01    ; Urdu (Pakistan)
SUBLANG_URDU_INDIA               equ 02    ; Urdu (India)
SUBLANG_UZBEK_LATIN              equ 01    ; Uzbek (Latin)
SUBLANG_UZBEK_CYRILLIC           equ 02    ; Uzbek (Cyrillic)

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Sorting IDs.
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SORT_DEFAULT                     equ 0     ; sorting default

SORT_JAPANESE_XJIS               equ 0     ; Japanese XJIS order
SORT_JAPANESE_UNICODE            equ 1     ; Japanese Unicode order

SORT_CHINESE_BIG5                equ 0     ; Chinese BIG5 order
SORT_CHINESE_PRCP                equ 0     ; PRC Chinese Phonetic order
SORT_CHINESE_UNICODE             equ 1     ; Chinese Unicode order
SORT_CHINESE_PRC                 equ 2     ; PRC Chinese Stroke Count order
SORT_CHINESE_BOPOMOFO            equ 3     ; Traditional Chinese Bopomofo order

SORT_KOREAN_KSC                  equ 0     ; Korean KSC order
SORT_KOREAN_UNICODE              equ 1     ; Korean Unicode order

SORT_GERMAN_PHONE_BOOK           equ 1     ; German Phone Book order

SORT_HUNGARIAN_DEFAULT           equ 0     ; Hungarian Default order
SORT_HUNGARIAN_TECHNICAL         equ 1     ; Hungarian Technical order

SORT_GEORGIAN_TRADITIONAL        equ 0     ; Georgian Traditional order
SORT_GEORGIAN_MODERN             equ 1     ; Georgian Modern order

ENDIF ; _NTDEF_