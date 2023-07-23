comment ^

Module Name:
    native.inc

Abstract:
    This module defines some undocumented W2000 structures and constants.
    See "Windows NT-2000 Native API Reference" by Gary Nebbett

Author:
    Four-F (four-f@mail.ru)

Last Update:
	24-September-2003

IMPORTANT:
	Hand made -> Bugs are very possible :(
	Your bug report is very welcome.

Comments:
    Some fields name have been changed because of collision with masm reserved words
    All such words are prefixed with underscore ('_') symbol.
^

;IFNDEF KAFFINITY
;	include ntddk.inc
;ENDIF


;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Counted String
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFNDEF _STRING
_STRING STRUCT
	_Length			WORD	?
	MaximumLength	WORD	?
	Buffer			DWORD	?	; PCHAR
_STRING ENDS
PSTRING typedef PTR _STRING
ENDIF

IFNDEF ANSI_STRING
ANSI_STRING equ <_STRING>
PANSI_STRING typedef PTR _STRING
ENDIF

IFNDEF OEM_STRING
OEM_STRING equ <_STRING>
POEM_STRING typedef PTR _STRING
ENDIF

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; CONST Counted String
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFNDEF CSTRING
CSTRING STRUCT
	_Length		WORD	?
	MaximumLength	WORD	?
	Buffer			DWORD	?	; CONST char *
CSTRING ENDS
PCSTRING typedef PTR CSTRING
ENDIF

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

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Valid values for the Attributes field
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFNDEF OBJ_INHERIT
OBJ_INHERIT             equ 00000002h
OBJ_PERMANENT           equ 00000010h
OBJ_EXCLUSIVE           equ 00000020h
OBJ_CASE_INSENSITIVE    equ 00000040h
OBJ_OPENIF              equ 00000080h
OBJ_OPENLINK            equ 00000100h
OBJ_KERNEL_HANDLE       equ 00000200h
OBJ_VALID_ATTRIBUTES    equ 000003F2h
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Object Attributes structure
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
IFNDEF OBJECT_ATTRIBUTES
OBJECT_ATTRIBUTES STRUCT		; sizeof = 18h
	_Length						DWORD			? ; original name Length
	RootDirectory				HANDLE			?
	ObjectName					PUNICODE_STRING	?
	Attributes					DWORD			?
	SecurityDescriptor			PVOID			? ; Points to type SECURITY_DESCRIPTOR
	SecurityQualityOfService	PVOID			? ; Points to type SECURITY_QUALITY_OF_SERVICE
OBJECT_ATTRIBUTES ENDS
POBJECT_ATTRIBUTES typedef OBJECT_ATTRIBUTES

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
        
$IsAddr2 MACRO Operand:REQ
	local a
	a = 0

	IF @SizeStr(<Operand>) GT 5
		IFIDNI <addr >, @SubStr(<Operand>, 1 , 5)
			a = 1
		ENDIF
	ENDIF

	IF a
        EXITM <-1>
    ELSE
        EXITM <0>
    ENDIF
ENDM

$IsOffset2 MACRO Operand:REQ
	local a
	a = 0

echo Operand
	IF @SizeStr(<Operand>) GT 7
		IFIDNI <offset >, @SubStr(<Operand>, 1 , 7)
			a = 1
		ENDIF
	ENDIF

	IF a
        EXITM <-1>
    ELSE
        EXITM <0>
    ENDIF
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

InitializeObjectAttributes MACRO p:REQ, n:REQ, a:REQ, r:REQ, s:REQ

;; Be very carefull with this macro !!!
;; It can contain some hiden bugs !!!
;; In ambiguous cases fill OBJECT_ATTRIBUTES structure manually

	local adr, reax, reax
	reax = 0
	recx = 0

	IF (OPATTR (p)) AND 00010000y
		;; is a register value
		IFDIFI <p>, <ecx>		;; not ecx
			mov ecx, p
		ENDIF
	ELSE
		IF $IsAddr2(p)
			adr SUBSTR <p>, 6
			IF $IsStack(adr)			;; is relative to SS
				lea ecx, adr
				recx = 1				;; no more eax
			ELSE
				mov ecx, offset adr
			ENDIF
		ENDIF
	ENDIF

	PUSHCONTEXT ASSUMES
	assume ecx:ptr OBJECT_ATTRIBUTES
	mov [ecx]._Length, sizeof OBJECT_ATTRIBUTES

	IF (OPATTR (r)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push r
		pop [ecx].RootDirectory
	ELSE
		IF r EQ 0
			and [ecx].RootDirectory, 0	;; NULL
		ELSE
			mov [ecx].RootDirectory, r
		ENDIF
	ENDIF

	IF (OPATTR (a)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push a
		pop [ecx].Attributes
	ELSE
		IF a EQ 0
			and [ecx].Attributes, 0
		ELSE
			mov [ecx].Attributes, a
		ENDIF
	ENDIF


	IF (OPATTR (n)) AND 00010000y
		;; is a register value
		;IFDIFI <n>, <eax>		;; not eax
			mov [ecx].ObjectName, n
		;ENDIF
	ELSE
		IF $IsAddr2(n)
			adr SUBSTR <n>, 6
			IF $IsStack(adr)			;; is relative to SS
				lea eax, adr
				mov [ecx].ObjectName, eax
				reax = 1				;; no more eax
			ELSE
				mov [ecx].ObjectName, offset adr
			ENDIF
		ELSEIF (OPATTR (n)) AND 00000010y
			;; is a memory variable or has a relocatable data label
			push n
			pop [ecx].ObjectName
		ELSE
			;; offset
			mov [ecx].ObjectName, n
		ENDIF
	ENDIF

	IF (OPATTR (s)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push s
		pop [ecx].SecurityDescriptor
	ELSE
		IF s EQ 0
			and [ecx].SecurityDescriptor, 0	;; NULL
		ELSE
			mov [ecx].SecurityDescriptor, s
		ENDIF
	ENDIF

	and [ecx].SecurityQualityOfService, 0	;; NULL
	assume ecx:nothing
	POPCONTEXT ASSUMES
ENDM
ENDIF


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Section Information Structures.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
IFNDEF ViewShare
;typedef enum _SECTION_INHERIT {
    ViewShare	equ 1
    ViewUnmap	equ 2
ENDIF

IFNDEF NtCurrentProcess
NtCurrentProcess	equ -1
NtCurrentThread		equ -2
ENDIF

IFNDEF _LARGE_INTEGER
; Damn. LARGE_INTEGER is defined in windows.inc and defined wrong way!
; So i change its name to _LARGE_INTEGER :-(
_LARGE_INTEGER UNION
	struct
		LowPart    DWORD ?
		HighPart   SDWORD ?
	ends
	struct u
		LowPart    DWORD ?
		HighPart   SDWORD ?
	ends
	QuadPart     QWORD ?	; signed
_LARGE_INTEGER ENDS
_PLARGE_INTEGER typedef PTR _LARGE_INTEGER
ENDIF

IFNDEF ULARGE_INTEGER
ULARGE_INTEGER UNION
	struct
		LowPart    DWORD ?
		HighPart   DWORD ?
	ends
	struct u
		LowPart    DWORD ?
		HighPart   DWORD ?
	ends
	QuadPart     QWORD ?
ULARGE_INTEGER ENDS
PULARGE_INTEGER	typedef	PTR ULARGE_INTEGER
ENDIF















;typedef enum _SYSTEM_INFORMATION_CLASS {
												; Query		Set
SystemBasicInformation					equ 0	;	Y		N
SystemProcessorInformation				equ 1	;	Y		N
SystemPerformanceInformation			equ 2	;	Y		N
SystemTimeOfDayInformation				equ 3	;	Y		N
SystemNotImplemented1					equ 4	;	Y		N
SystemProcessesAndThreadsInformation	equ 5	;	Y		N
SystemCallCounts						equ 6	;	Y		N
SystemConfigurationInformation			equ 7	;	Y		N
SystemProcessorTimes					equ 8	;	Y		N
SystemGlobalFlag						equ 9	;	Y		Y
SystemNotImplemented2					equ 10	;	Y		N
SystemModuleInformation					equ 11	;	Y		N
SystemLockInformation					equ 12	;	Y		N
SystemNotImplemented3					equ 13	;	Y		N
SystemNotImplemented4					equ 14	;	Y		N
SystemNotImplemented5					equ 15	;	Y		N
SystemHandleInformation					equ 16	;	Y		N
SystemObjectInformation					equ 17	;	Y		N
SystemPagefileInformation				equ 18	;	Y		N
SystemInstructionEmulationCounts		equ 19	;	Y		N
SystemInvalidInfoClass1					equ 20
SystemCacheInformation					equ 21	;	Y		Y
SystemPoolTagInformation				equ 22	;	Y		N
SystemProcessorStatistics				equ 23	;	Y		N
SystemDpcInformation					equ 24	;	Y		Y
SystemNotImplemented6					equ 25	;	Y		N
SystemLoadImage							equ 26	;	N		Y
SystemUnloadImage						equ 27	;	N		Y
SystemTimeAdjustment					equ 28	;	Y		Y
SystemNotImplemented7					equ 29	;	Y		N
SystemNotImplemented8					equ 30	;	Y		N
SystemNotImplemented9					equ 31	;	Y		N
SystemCrashDumpInformation				equ 32	;	Y		N
SystemExceptionInformation				equ 33	;	Y		N
SystemCrashDumpStateInformation			equ 34	;	Y		Y/N
SystemKernelDebuggerInformation			equ 35	;	Y		N
SystemContextSwitchInformation			equ 36	;	Y		N
SystemRegistryQuotaInformation			equ 37	;	Y		Y
SystemLoadAndCallImage					equ 38	;	N		Y
SystemPrioritySeparation				equ 39	;	N		Y
SystemNotImplemented10					equ 40	;	Y		N
SystemNotImplemented11					equ 41	;	Y		N
SystemInvalidInfoClass2					equ 42
SystemInvalidInfoClass3					equ 43
SystemTimeZoneInformation				equ 44	;	Y		N
SystemLookasideInformation				equ 45	;	Y		N
SystemSetTimeSlipEvent					equ 46	;	N		Y
SystemCreateSession						equ 47	;	N		Y
SystemDeleteSession						equ 48	;	N		Y
SystemInvalidInfoClass4					equ 49
SystemRangeStartInformation				equ 50	;	Y		N
SystemVerifierInformation				equ 51	;	Y		Y
SystemAddVerifier						equ 52	;	N		Y
SystemSessionProcessesInformation		equ 53	;	Y		N

SYSTEM_BASIC_INFORMATION STRUCT			; Information Class 0
	Unknown						DWORD	?
	MaximumIncrement			DWORD	?
	PhysicalPageSize			DWORD	?
	NumberOfPhysicalPages		DWORD	?
	LowestPhysicalPage			DWORD	?
	HighestPhysicalPage			DWORD	?
	AllocationGranularity		DWORD	?
	LowestUserAddress			DWORD	?
	HighestUserAddress			DWORD	?
	ActiveProcessors			DWORD	?
	NumberProcessors			BYTE	?
								db 3 dup(?)
SYSTEM_BASIC_INFORMATION ENDS
PSYSTEM_BASIC_INFORMATION typedef ptr SYSTEM_BASIC_INFORMATION

SYSTEM_MODULE_INFORMATION STRUCT		;Information Class 11
	Reserved			DWORD	2 dup(?)
	Base				PVOID	?
	_Size				DWORD	?	; original name Size
	Flags				DWORD	?
	Index				WORD	?
	Unknown				WORD	?
	LoadCount			WORD	?
	ModuleNameOffset	WORD	?
	ImageName			CHAR 256 dup(?)
SYSTEM_MODULE_INFORMATION ENDS
PSYSTEM_MODULE_INFORMATION typedef PTR SYSTEM_MODULE_INFORMATION

; (Gary Nebbett)

;typedef enum _SECTION_INFORMATION_CLASS {
SectionBasicInformation	equ 0
SectionImageInformation	equ 1

; SectionBasicInformation
SECTION_BASIC_INFORMATION STRUCT		; Information Class 0
	BaseAddress		PVOID	?
	Attributes		DWORD	?
	_Size			LARGE_INTEGER	<>	; original Size
SECTION_BASIC_INFORMATION ENDS
PSECTION_BASIC_INFORMATION typedef ptr SECTION_BASIC_INFORMATION

SEC_BASED		equ 00200000h	; Section should be mapped at same address in each process
SEC_NO_CHANGE	equ 00400000h	; Changes to protection of section pages are disabled
SEC_FILE		equ 00800000h	; Section is backed by a file
SEC_IMAGE		equ 01000000h	; Section is mapped as an image
;SEC_VLM is only valid in Windows 2000 and is not implemented on the Intel platform.
SEC_VLM			equ 02000000h	; Section maps VLM
SEC_RESERVE		equ 04000000h	; Section pages are reserved
SEC_COMMIT		equ 08000000h	; Section pages are committed
SEC_NOCACHE		equ 10000000h	; Section pages are non-cacheable

; SectionImageInformation
SECTION_IMAGE_INFORMATION STRUCT		; Information Class 1
	EntryPoint				PVOID	?
	Unknown1				DWORD	?
	StackReserve			DWORD	?
	StackCommit				DWORD	?
	Subsystem				DWORD	?
	MinorSubsystemVersion	WORD	?
	MajorSubsystemVersion	WORD	?
	Unknown2				DWORD	?
	Characteristics			DWORD	?
	ImageNumber				WORD	?
	Executable				BOOLEAN	?
	Unknown3				BYTE	?
	Unknown4				DWORD 3 dup(?)
SECTION_IMAGE_INFORMATION ENDS
PSECTION_IMAGE_INFORMATION typedef ptr SECTION_IMAGE_INFORMATION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; Windows NT-2000 Native API Reference by Gary Nebbett
;   Chapter 3: Virtual Memory
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _MEMORY_INFORMATION_CLASS {
MemoryBasicInformation		equ 0
MemoryWorkingSetList		equ 1
MemorySectionName			equ 2
MemoryBasicVlmInformation	equ 3

IFNDEF MEMORY_BASIC_INFORMATION		; This structure is defined in windows.inc
MEMORY_BASIC_INFORMATION STRUCT		; Information Class 0
	BaseAddress			PVOID	?
	AllocationBase		PVOID	?
	AllocationProtect	DWORD	?
	RegionSize			DWORD	?
	State				DWORD	?
	Protect				DWORD	?
	_Type				DWORD	?	; Type
MEMORY_BASIC_INFORMATION ENDS
ENDIF
PMEMORY_BASIC_INFORMATION typedef ptr MEMORY_BASIC_INFORMATION

MEMORY_WORKING_SET_LIST STRUCT		; Information Class 1
	NumberOfPages		DWORD	?
	WorkingSetList		DWORD	?
MEMORY_WORKING_SET_LIST ENDS
PMEMORY_WORKING_SET_LIST typedef ptr MEMORY_WORKING_SET_LIST

WSLE_PAGE_READONLY				equ 001h ; Page is read only
WSLE_PAGE_EXECUTE				equ 002h ; Page is executable
WSLE_PAGE_READWRITE				equ 004h ; Page is writeable
WSLE_PAGE_EXECUTE_READ			equ 003h
WSLE_PAGE_WRITECOPY				equ 005h ; Page should be copied on write
WSLE_PAGE_EXECUTE_READWRITE		equ 006h
WSLE_PAGE_EXECUTE_WRITECOPY		equ 007h ; Page should be copied on write
WSLE_PAGE_SHARE_COUNT_MASK		equ 0E0h
WSLE_PAGE_SHAREABLE				equ 100h ; Page is shareable

MEMORY_SECTION_NAME STRUCT			; Information Class 2
	SectionFileName		UNICODE_STRING	<>
MEMORY_SECTION_NAME ENDS
PMEMORY_SECTION_NAME typedef ptr MEMORY_SECTION_NAME

;typedef enum _OBJECT_INFORMATION_CLASS {
ObjectBasicInformation			equ 0	; Y N
ObjectNameInformation			equ 1	; Y N
ObjectTypeInformation			equ 2	; Y N
ObjectAllTypesInformation		equ 3	; Y N
ObjectHandleInformation			equ 4	; Y Y

OBJECT_BASIC_INFORMATION STRUCT			; Information Class 0
	Attributes					DWORD		?
	GrantedAccess				DWORD		?	; ACCESS_MASK
	HandleCount					DWORD		?
	PointerCount				DWORD		?
	PagedPoolUsage				DWORD		?
	NonPagedPoolUsage			DWORD		?
	Reserved					DWORD 3 dup(?)
	NameInformationLength		DWORD		?
	TypeInformationLength		DWORD		?
	SecurityDescriptorLength	DWORD		?
	CreateTime					LARGE_INTEGER	<>
OBJECT_BASIC_INFORMATION ENDS
POBJECT_BASIC_INFORMATION typedef ptr OBJECT_BASIC_INFORMATION

HANDLE_FLAG_INHERIT				equ 1
HANDLE_FLAG_PROTECT_FROM_CLOSE	equ 2
PERMANENT						equ 10h
EXCLUSIVE						equ 20h		; different encoding than in SYSTEM_OBJECT_INFORMATION

IFNDEF OBJECT_NAME_INFORMATION
OBJECT_NAME_INFORMATION STRUCT			; Information Class 1
	_Name		UNICODE_STRING	<>
OBJECT_NAME_INFORMATION ENDS
POBJECT_NAME_INFORMATION typedef ptr OBJECT_NAME_INFORMATION
ENDIF

OBJECT_TYPE_INFORMATION STRUCT			; Information Class 2 (sizeof = 60h)
	_Name					UNICODE_STRING	<>
	ObjectCount				DWORD		?		; 008h
	HandleCount				DWORD		?		; 00Ch
	Reserved1				DWORD 4 dup(?)		; 010h
	PeakObjectCount			DWORD		?		; 020h
	PeakHandleCount			DWORD		?		; 024h
	Reserved2				DWORD 4 dup(?)		; 034h
	InvalidAttributes		DWORD		?		; 038h
	GenericMapping			GENERIC_MAPPING	<>	; 03Ch
	ValidAccess				DWORD		?		; 04Ch
	Unknown					BYTE		?		; 050h
	MaintainHandleDatabase	BYTE	?			; BOOLEAN
							db 2 dup(?)			; padding
	PoolType				DWORD	?			; 054h POOL_TYPE
	PagedPoolUsage			DWORD	?			; 058h
	NonPagedPoolUsage		DWORD	?			; 05Ch
OBJECT_TYPE_INFORMATION ENDS
POBJECT_TYPE_INFORMATION typedef ptr OBJECT_TYPE_INFORMATION


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; This helper macro lets you never forget what you have to improve in source code of driver
;
; Usage:
;    Fix
; or
;    Fix <The system always crushes here>
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFNDEF Fix
Fix MACRO txt:=<Fix this later!!!!>
	local pos, spos

	pos = 0
	spos = 0

	% FORC fix_chr, @FileCur		;; Don't display full path. Easier to read.
		pos = pos + 1
		IF "&fix_chr" EQ 5Ch		;; "/"
			spos = pos
		ENDIF
	ENDM
	% ECHO @CatStr(<Fix: >, @SubStr(%@FileCur, spos+1,), <(%@Line) - txt>)
ENDM
ENDIF

