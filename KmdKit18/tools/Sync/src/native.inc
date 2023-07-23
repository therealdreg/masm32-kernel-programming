

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



IO_STATUS_BLOCK STRUCT		; sizeof = 08h
	Status		SDWORD		?	; 0000h  NTSTATUS
	Information	DWORD		?	; 0004h
IO_STATUS_BLOCK ENDS
PIO_STATUS_BLOCK typedef PTR IO_STATUS_BLOCK


; Define the create/open option flags

FILE_SYNCHRONOUS_IO_ALERT               equ 00000010h
FILE_SYNCHRONOUS_IO_NONALERT            equ 00000020h


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
