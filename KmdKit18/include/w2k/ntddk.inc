comment ^
Module Name:
    ntddk.inc

Abstract:
    This module defines the NT types, constants, and functions that are
    exposed to device drivers.

Author:
    Four-F (four-f@mail.ru)

Last update:
	11-Januar-2005

IMPORTANT:
	Hand made -> Bugs are very possible :(
	Your bug report is very welcome.

Comments:
    Some fields name have been changed because of collision with masm reserved words.
    All such words are prefixed with underscore ('_') symbol.
^

IFNDEF _NTDDK_
_NTDDK_ equ 1

include ntdef.inc
include ntstatus.inc

; +
; The following $IsXxx macros is not a part of original ntdddk.h

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

$IsAddr MACRO Operand:REQ
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

; The preceding $IsXxx macros is not a part of original ntdddk.h
; -

IFNDEF PDWORD
	PDWORD				typedef PTR DWORD
ENDIF

; Kernel Mutex Level Numbers (must be globallly assigned within executive)
; The third token in the name is the sub-component name that defines and
; uses the level number.

; Used by Vdm for protecting io simulation structures

MUTEX_LEVEL_VDM_IO       equ 00000001
MUTEX_LEVEL_EX_PROFILE   equ 00000040h

; The LANMAN Redirector uses the file system major function, but defines
; it's own mutex levels.  We can do this safely because we know that the
; local filesystem will never call the remote filesystem and vice versa.

MUTEX_LEVEL_RDR_FILESYS_DATABASE    equ 10100000h
MUTEX_LEVEL_RDR_FILESYS_SECURITY    equ 10100001h

; File System levels.

MUTEX_LEVEL_FILESYSTEM_RAW_VCB      equ 11000006h

; In the NT STREAMS environment, a mutex is used to serialize open, close
; and Scheduler threads executing in a subsystem-parallelized stack.

MUTEX_LEVEL_STREAMS_SUBSYS          equ 11001001h

; Mutex level used by LDT support on x86

MUTEX_LEVEL_PS_LDT                  equ 1F000000h

PsGetCurrentProcess TEXTEQU <IoGetCurrentProcess>
PsGetCurrentThread TEXTEQU <KeGetCurrentThread>

SLIST_HEADER UNION
	Alignment		QWORD				?	; ULONGLONG
	struct
		Next		SINGLE_LIST_ENTRY	<>
		Depth		WORD				?
		Sequence	WORD				?
	ends
SLIST_HEADER ENDS
PSLIST_HEADER typedef ptr SLIST_HEADER

; Kernel definitions that need to be here for forward reference purposes

; Processor modes.

KPROCESSOR_MODE typedef BYTE

;typedef enum _MODE {
	KernelMode	equ 0
	UserMode	equ 1
	MaximumMode	equ 2

; Asynchronous Procedure Call (APC) object

KAPC STRUCT		; sizeof = 030h
	_Type			SWORD		?	; 0000h  (org field name Type)
	_Size			SWORD		?	; 0002h  (org field name Size)
	Spare0			DWORD		?	; 0004h
	Thread			PVOID		?	; 0008h  PTR KTHREAD
	ApcListEntry	LIST_ENTRY <>	; 000Ch
	KernelRoutine	PVOID		?	; 0014h  KiSuspendNop
	RundownRoutine	PVOID		?	; 0018h
	NormalRoutine	PVOID		?	; 001Ch	 KiSuspendThread
	NormalContext	PVOID		?	; 0020h

	; N.B. The following two members MUST be together.

	SystemArgument1	PVOID		?	; 0024h
	SystemArgument2	PVOID		?	; 0028h
	ApcStateIndex	BYTE		?	; 002Ch
	ApcMode			BYTE		?	; 002Dh  KPROCESSOR_MODE
	Inserted		BYTE		?	; 002Eh  BOOLEAN
					db			?	; padding
KAPC ENDS
PKAPC typedef PTR KAPC

; Asynchronous Procedure Call (APC) object

; +
; Define DPC importance.
; LowImportance - Queue DPC at end of target DPC queue.
; MediumImportance - Queue DPC at end of target DPC queue.
; HighImportance - Queue DPC at front of target DPC DPC queue.
;
; If there is currently a DPC active on the target processor, or a DPC
; interrupt has already been requested on the target processor when a
; DPC is queued, then no further action is necessary. The DPC will be
; executed on the target processor when its queue entry is processed.
;
; If there is not a DPC active on the target processor and a DPC interrupt
; has not been requested on the target processor, then the exact treatment
; of the DPC is dependent on whether the host system is a UP system or an
; MP system.
;
; UP system.
;
; If the DPC is of medium or high importance, the current DPC queue depth
; is greater than the maximum target depth, or current DPC request rate is
; less the minimum target rate, then a DPC interrupt is requested on the
; host processor and the DPC will be processed when the interrupt occurs.
; Otherwise, no DPC interupt is requested and the DPC execution will be
; delayed until the DPC queue depth is greater that the target depth or the
; minimum DPC rate is less than the target rate.
;
; MP system.
;
; If the DPC is being queued to another processor and the depth of the DPC
; queue on the target processor is greater than the maximum target depth or
; the DPC is of high importance, then a DPC interrupt is requested on the
; target processor and the DPC will be processed when the interrupt occurs.
; Otherwise, the DPC execution will be delayed on the target processor until
; the DPC queue depth on the target processor is greater that the maximum
; target depth or the minimum DPC rate on the target processor is less than
; the target mimimum rate.
;
; If the DPC is being queued to the current processor and the DPC is not of
; low importance, the current DPC queue depth is greater than the maximum
; target depth, or the minimum DPC rate is less than the minimum target rate,
; then a DPC interrupt is request on the current processor and the DPV will
; be processed whne the interrupt occurs. Otherwise, no DPC interupt is
; requested and the DPC execution will be delayed until the DPC queue depth
; is greater that the target depth or the minimum DPC rate is less than the
; target rate.
; -

;typedef enum _KDPC_IMPORTANCE {
    LowImportance		equ 0
    MediumImportance	equ 1
    HighImportance		equ 2

; Deferred Procedure Call (DPC) object

KDPC STRUCT			; sizeof = 020h
	_Type			SWORD		?	; 0000h  (IO_TYPE_DPC	= 13h) (original name Type)
	Number			BYTE		?	; 0002h
	Importance		BYTE		?	; 0003h
	DpcListEntry	LIST_ENTRY <>	; 0004h
	DeferredRoutine	PVOID		?	; 000Ch
	DeferredContext	PVOID		?	; 0010h
	SystemArgument1	PVOID		?	; 0014h
	SystemArgument2	PVOID		?	; 0018h
	_Lock			PVOID		?	; 001Ch  (original Lock)
KDPC ENDS
PKDPC typedef PTR KDPC

PKIPI_CONTEXT typedef DWORD

; Define interprocessor interrupt performance counters.

KIPI_COUNTS STRUCT
	Freeze				DWORD	?
	Packet				DWORD	?
	DPC					DWORD	?
	APC					DWORD	?
	FlushSingleTb		DWORD	?
	FlushMultipleTb		DWORD	?
	FlushEntireTb		DWORD	?
	GenericCall			DWORD	?
	ChangeColor			DWORD	?
	SweepDcache			DWORD	?
	SweepIcache			DWORD	?
	SweepIcacheRange	DWORD	?
	FlushIoBuffers		DWORD	?
	GratuitousDPC		DWORD	?
KIPI_COUNTS ENDS
PKIPI_COUNTS  typedef ptr KIPI_COUNTS

; +
; I/O system definitions.
;
; Define a Memory Descriptor List (MDL)
;
; An MDL describes pages in a virtual buffer in terms of physical pages.  The
; pages associated with the buffer are described in an array that is allocated
; just after the MDL header structure itself.  In a future compiler this will
; be placed at:
;
;      ULONG Pages[];
;
; Until this declaration is permitted, however, one simply calculates the
; base of the array by adding one to the base MDL pointer:
;
;      Pages = (PULONG) (Mdl + 1);
;
; Notice that while in the context of the subject thread, the base virtual
; address of a buffer mapped by an MDL may be referenced using the following:
;
;      Mdl->StartVa | Mdl->ByteOffset
; -

MDL STRUCT			; sizeof = 01Ch
	Next			PVOID		?	; 0000h  PTR MDL
	_Size			SWORD		?	; 0004h  (original name Size)
	MdlFlags		SWORD		?	; 0006h
	Process			PVOID		?	; 0008h  PTR EPROCESS
	MappedSystemVa	PVOID		?	; 000Ch
	StartVa			PVOID		?	; 0010h
	ByteCount		DWORD		?	; 0014h
	ByteOffset		DWORD		?	; 0018h
MDL ENDS
PMDL typedef PTR MDL

MDL_MAPPED_TO_SYSTEM_VA     equ 0001h
MDL_PAGES_LOCKED            equ 0002h
MDL_SOURCE_IS_NONPAGED_POOL equ 0004h
MDL_ALLOCATED_FIXED_SIZE    equ 0008h
MDL_PARTIAL                 equ 0010h
MDL_PARTIAL_HAS_BEEN_MAPPED equ 0020h
MDL_IO_PAGE_READ            equ 0040h
MDL_WRITE_OPERATION         equ 0080h
MDL_PARENT_MAPPED_SYSTEM_VA equ 0100h
MDL_LOCK_HELD               equ 0200h
MDL_PHYSICAL_VIEW           equ 0400h
MDL_IO_SPACE                equ 0800h
MDL_NETWORK_HEADER          equ 1000h
MDL_MAPPING_CAN_FAIL        equ 2000h
MDL_ALLOCATED_MUST_SUCCEED  equ 4000h

MDL_MAPPING_FLAGS equ MDL_MAPPED_TO_SYSTEM_VA or MDL_PAGES_LOCKED or MDL_SOURCE_IS_NONPAGED_POOL or MDL_PARTIAL_HAS_BEEN_MAPPED or MDL_PARENT_MAPPED_SYSTEM_VA or MDL_LOCK_HELD or MDL_SYSTEM_VA or MDL_IO_SPACE

;  Define an access token from a programmer's viewpoint.  The structure is
;  completely opaque and the programer is only allowed to have pointers
;  to tokens.

PACCESS_TOKEN	typedef	PTR	; winnt

; Pointer to a SECURITY_DESCRIPTOR  opaque data type.

PSECURITY_DESCRIPTOR	typedef	PTR	; winnt

; Define a pointer to the Security ID data type (an opaque data type)

PSID	typedef	PTR	; winnt

ACCESS_MASK	typedef DWORD
PACCESS_MASK typedef PTR ACCESS_MASK

;  The following are masks for the predefined standard access types

DELETE                           equ 00010000h
READ_CONTROL                     equ 00020000h
WRITE_DAC                        equ 00040000h
WRITE_OWNER                      equ 00080000h
SYNCHRONIZE                      equ 00100000h

STANDARD_RIGHTS_REQUIRED         equ 000F0000h

STANDARD_RIGHTS_READ             equ READ_CONTROL
STANDARD_RIGHTS_WRITE            equ READ_CONTROL
STANDARD_RIGHTS_EXECUTE          equ READ_CONTROL

STANDARD_RIGHTS_ALL              equ 001F0000h

SPECIFIC_RIGHTS_ALL              equ 0000FFFFh

; AccessSystemAcl access type

ACCESS_SYSTEM_SECURITY           equ 01000000h

; MaximumAllowed access type

MAXIMUM_ALLOWED                  equ 02000000h

;  These are the generic rights.

GENERIC_READ                     equ 80000000h
GENERIC_WRITE                    equ 40000000h
GENERIC_EXECUTE                  equ 20000000h
GENERIC_ALL                      equ 10000000h

;  Define the generic mapping array.  This is used to denote the
;  mapping of each generic access right to a specific access mask.

GENERIC_MAPPING STRUCT				; sizeof = 010h
	GenericRead		DWORD		?	; 0000h ACCESS_MASK
	GenericWrite	DWORD		?	; 0004h ACCESS_MASK
	GenericExecute	DWORD		?	; 0008h ACCESS_MASK
	GenericAll		DWORD		?	; 000Ch ACCESS_MASK
GENERIC_MAPPING ENDS
PGENERIC_MAPPING typedef PTR GENERIC_MAPPING

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                                                    ::
;:                        LUID_AND_ATTRIBUTES                         ::
;:                                                                    ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;#include <pshpack4.h>

LUID_AND_ATTRIBUTES STRUCT		; sizeof = 0Ch
	Luid1		LUID 		<>	; 0000h
	Attributes	DWORD		?	; 0008h
LUID_AND_ATTRIBUTES ENDS
PLUID_AND_ATTRIBUTES typedef PTR LUID_AND_ATTRIBUTES

;#include <poppack.h>

; This is the *current* ACL revision

ACL_REVISION     equ 2
ACL_REVISION_DS  equ 4

; This is the history of ACL revisions.  Add a new one whenever
; ACL_REVISION is updated

ACL_REVISION1   equ 1
ACL_REVISION2   equ 2
MIN_ACL_REVISION equ ACL_REVISION2
ACL_REVISION3   equ 3
ACL_REVISION4   equ 4
MAX_ACL_REVISION equ ACL_REVISION4

ACL STRUCT		; sizeof = 08h
	AclRevision	BYTE		?	; 0000h
	Sbz1		BYTE		?	; 0001h
	AclSize		WORD		?	; 0002h
	AceCount	WORD		?	; 0004h
	Sbz2		WORD		?	; 0006h
ACL ENDS
PACL typedef PTR ACL

; Current security descriptor revision value

SECURITY_DESCRIPTOR_REVISION	equ 1
SECURITY_DESCRIPTOR_REVISION1	equ 1

; Privilege attributes

SE_PRIVILEGE_ENABLED_BY_DEFAULT equ 00000001
SE_PRIVILEGE_ENABLED            equ 00000002
SE_PRIVILEGE_USED_FOR_ACCESS    equ 80000000h

; Privilege Set Control flags

PRIVILEGE_SET_ALL_NECESSARY    equ 1

; -
;  Privilege Set - This is defined for a privilege set of one.
;                  If more than one privilege is needed, then this structure
;                  will need to be allocated with more space.
;
;  Note: don't change this structure without fixing the INITIAL_PRIVILEGE_SET
;  structure (defined in se.h)
; -

PRIVILEGE_SET STRUCT		; sizeof = 014h
	PrivilegeCount	DWORD		?	; 0000h
	Control			DWORD		?	; 0004h
	Privilege		LUID_AND_ATTRIBUTES	1 dup(<>)	; 0008h
PRIVILEGE_SET ENDS
PPRIVILEGE_SET typedef PTR PRIVILEGE_SET

; These must be converted to LUIDs before use.

SE_MIN_WELL_KNOWN_PRIVILEGE       equ 2
SE_CREATE_TOKEN_PRIVILEGE         equ 2
SE_ASSIGNPRIMARYTOKEN_PRIVILEGE   equ 3
SE_LOCK_MEMORY_PRIVILEGE          equ 4
SE_INCREASE_QUOTA_PRIVILEGE       equ 5

; Unsolicited Input is obsolete and unused.

SE_UNSOLICITED_INPUT_PRIVILEGE    equ 6

SE_MACHINE_ACCOUNT_PRIVILEGE      equ 6
SE_TCB_PRIVILEGE                  equ 7
SE_SECURITY_PRIVILEGE             equ 8
SE_TAKE_OWNERSHIP_PRIVILEGE       equ 9
SE_LOAD_DRIVER_PRIVILEGE          equ 10
SE_SYSTEM_PROFILE_PRIVILEGE       equ 11
SE_SYSTEMTIME_PRIVILEGE           equ 12
SE_PROF_SINGLE_PROCESS_PRIVILEGE  equ 13
SE_INC_BASE_PRIORITY_PRIVILEGE    equ 14
SE_CREATE_PAGEFILE_PRIVILEGE      equ 15
SE_CREATE_PERMANENT_PRIVILEGE     equ 16
SE_BACKUP_PRIVILEGE               equ 17
SE_RESTORE_PRIVILEGE              equ 18
SE_SHUTDOWN_PRIVILEGE             equ 19
SE_DEBUG_PRIVILEGE                equ 20
SE_AUDIT_PRIVILEGE                equ 21
SE_SYSTEM_ENVIRONMENT_PRIVILEGE   equ 22
SE_CHANGE_NOTIFY_PRIVILEGE        equ 23
SE_REMOTE_SHUTDOWN_PRIVILEGE      equ 24
SE_UNDOCK_PRIVILEGE               equ 25
SE_SYNC_AGENT_PRIVILEGE           equ 26
SE_ENABLE_DELEGATION_PRIVILEGE    equ 27
SE_MAX_WELL_KNOWN_PRIVILEGE       equ SE_ENABLE_DELEGATION_PRIVILEGE

; +
; Impersonation Level
;
; Impersonation level is represented by a pair of bits in Windows.
; If a new impersonation level is added or lowest value is changed from
; 0 to something else, fix the Windows CreateFile call.
; -

;typedef enum _SECURITY_IMPERSONATION_LEVEL {
    SecurityAnonymous		equ 0
    SecurityIdentification	equ 1
    SecurityImpersonation	equ 2
    SecurityDelegation		equ 3

SECURITY_MAX_IMPERSONATION_LEVEL	equ SecurityDelegation

DEFAULT_IMPERSONATION_LEVEL			equ SecurityImpersonation

; Security Tracking Mode

SECURITY_DYNAMIC_TRACKING      equ TRUE)
SECURITY_STATIC_TRACKING       equ FALSE

SECURITY_CONTEXT_TRACKING_MODE  typedef BOOLEAN
PSECURITY_CONTEXT_TRACKING_MODE typedef PTR SECURITY_CONTEXT_TRACKING_MODE

; Quality Of Service

SECURITY_QUALITY_OF_SERVICE STRUCT		; sizeof = 0Ch
	_Length				DWORD		?	; 0000h  (org name Length)
	ImpersonationLevel	SDWORD		?	; 0004h  enum SECURITY_IMPERSONATION_LEVEL
	ContextTrackingMode	BYTE		?	; 0008h  SECURITY_CONTEXT_TRACKING_MODE
	EffectiveOnly		BYTE		?	; 0009h  BOOLEAN
						db	2 dup(?)	; padding
SECURITY_QUALITY_OF_SERVICE ENDS
PSECURITY_QUALITY_OF_SERVICE typedef PTR SECURITY_QUALITY_OF_SERVICE

; Used to represent information related to a thread impersonation

SE_IMPERSONATION_STATE STRUCT
	Token			PVOID	?	; 0000h  PACCESS_TOKEN
	CopyOnOpen		BYTE	?	; 0004h  BOOLEAN
	EffectiveOnly	BYTE	?	; 0005h  BOOLEAN
					db 2 dup(?)	; padding
	Level			SDWORD	?	; 0008  enum SECURITY_IMPERSONATION_LEVEL
SE_IMPERSONATION_STATE ENDS
PSE_IMPERSONATION_STATE typedef PTR SE_IMPERSONATION_STATE

SECURITY_INFORMATION  typedef DWORD
PSECURITY_INFORMATION typedef PTR SECURITY_INFORMATION

OWNER_SECURITY_INFORMATION       equ 00000001
GROUP_SECURITY_INFORMATION       equ 00000002
DACL_SECURITY_INFORMATION        equ 00000004
SACL_SECURITY_INFORMATION        equ 00000008

PROTECTED_DACL_SECURITY_INFORMATION     equ 80000000h
PROTECTED_SACL_SECURITY_INFORMATION     equ 40000000h
UNPROTECTED_DACL_SECURITY_INFORMATION   equ 20000000h
UNPROTECTED_SACL_SECURITY_INFORMATION   equ 10000000h

LOW_PRIORITY			equ 0	; Lowest thread priority level
LOW_REALTIME_PRIORITY	equ 16	; Lowest realtime priority level
HIGH_PRIORITY			equ 31	; Highest thread priority level
MAXIMUM_PRIORITY		equ 32	; Number of thread priority levels
; begin_winnt
MAXIMUM_WAIT_OBJECTS	equ 64	; Maximum number of wait objects

MAXIMUM_SUSPEND_COUNT	equ MAXCHAR	; Maximum times thread can be suspended
; end_winnt

; Thread affinity

KAFFINITY	typedef	DWORD
PKAFFINITY	typedef PTR KAFFINITY

; Thread priority

KPRIORITY	typedef SDWORD

; Spin Lock

KSPIN_LOCK	typedef DWORD	; ULONG_PTR
PKSPIN_LOCK	typedef PTR DWORD

;  Doubly-linked list manipulation routines.  Implemented as macros
;  but logically these are procedures.

; VOID InitializeListHead( PLIST_ENTRY ListHead );

InitializeListHead MACRO ListHead:REQ
local a
	IF $IsAddr(ListHead)
		a SUBSTR <ListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
	ELSE
		.ERR
	ENDIF
	; (LIST_ENTRY PTR [eax]).Flink = (LIST_ENTRY PTR [eax]).Blink = eax
	mov (LIST_ENTRY PTR [eax]).Flink, eax
	mov (LIST_ENTRY PTR [eax]).Blink, eax
ENDM

;  BOOLEAN IsListEmpty( PLIST_ENTRY ListHead );

IsListEmpty MACRO ListHead:REQ
local a
	IF $IsAddr(ListHead)
		a SUBSTR <ListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
	ELSE
		.ERR
	ENDIF
	.if [LIST_ENTRY PTR [eax]].Flink == eax
		xor eax, eax
		inc eax						;; return TRUE
	.else
		xor eax, eax				;; return FALSE
	.endif
ENDM

$IsListEmpty MACRO ListHead:REQ
local a
	IF $IsAddr(ListHead)
		a SUBSTR <ListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
	ELSE
		.ERR
	ENDIF
	.if [LIST_ENTRY PTR [eax]].Flink == eax
		xor eax, eax
		inc eax						;; return TRUE
	.else
		xor eax, eax				;; return FALSE
	.endif
	EXITM <eax>
ENDM

;  VOID RemoveEntryList( PLIST_ENTRY Entry );

RemoveEntryList MACRO Entry:REQ
local a
	IF $IsAddr(Entry)
		a SUBSTR <Entry>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(Entry)
		IFDIFI <Entry>, <eax>		;; don't move eax onto itself
			mov eax, Entry
		ENDIF
	ELSEIF $IsStack(Entry)
		mov eax, Entry
	ELSE
		.ERR
	ENDIF

	mov ecx, (LIST_ENTRY PTR [eax]).Flink
	mov eax, (LIST_ENTRY PTR [eax]).Blink

	mov (LIST_ENTRY PTR [eax]).Flink, ecx
	mov (LIST_ENTRY PTR [ecx]).Blink, eax
ENDM

;  PLIST_ENTRY RemoveHeadList( PLIST_ENTRY ListHead );

RemoveHeadList MACRO ListHead:REQ
local a
	IF $IsAddr(ListHead)
		a SUBSTR <ListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
	ELSE
		.ERR
	ENDIF
	mov ecx, (LIST_ENTRY PTR [eax]).Flink
	push ecx

	mov ecx, (LIST_ENTRY PTR [ecx]).Flink
	mov (LIST_ENTRY PTR [eax]).Flink, ecx

	mov (LIST_ENTRY PTR [ecx]).Blink, eax

	pop eax
ENDM

$RemoveHeadList MACRO ListHead:REQ
local a
	IF $IsAddr(ListHead)
		a SUBSTR <ListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
	ELSE
		.ERR
	ENDIF
	mov ecx, (LIST_ENTRY PTR [eax]).Flink
	push ecx

	mov ecx, (LIST_ENTRY PTR [ecx]).Flink
	mov (LIST_ENTRY PTR [eax]).Flink, ecx

	mov (LIST_ENTRY PTR [ecx]).Blink, eax

	pop eax
	EXITM <eax>
ENDM

;  PLIST_ENTRY RemoveTailList( PLIST_ENTRY ListHead );

RemoveTailList MACRO ListHead:REQ
local a
	IF $IsAddr(ListHead)
		a SUBSTR <ListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
	ELSE
		.ERR
	ENDIF
	mov ecx, (LIST_ENTRY PTR [eax]).Blink
	push ecx

	mov ecx, (LIST_ENTRY PTR [ecx]).Blink
	mov (LIST_ENTRY PTR [eax]).Blink, ecx

	mov (LIST_ENTRY PTR [ecx]).Flink, eax

	pop eax
ENDM

$RemoveTailList MACRO ListHead:REQ
local a
	IF $IsAddr(ListHead)
		a SUBSTR <ListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
	ELSE
		.ERR
	ENDIF
	mov ecx, (LIST_ENTRY PTR [eax]).Blink
	push ecx

	mov ecx, (LIST_ENTRY PTR [ecx]).Blink
	mov (LIST_ENTRY PTR [eax]).Blink, ecx

	mov (LIST_ENTRY PTR [ecx]).Flink, eax

	pop eax
	EXITM <eax>
ENDM


;  VOID InsertTailList( PLIST_ENTRY ListHead, PLIST_ENTRY Entry );

InsertTailList MACRO ListHead:REQ, Entry:REQ
local h, e, reax
	reax = 0
	IF $IsAddr(ListHead)
		h SUBSTR <ListHead>, 6
		IF $IsStack(h)				;; Is relative to SS
			lea eax, h
			reax = 1				;; no more eax
		ELSE
			mov eax, offset h
			reax = 1				;; no more eax
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
			reax = 1				;; no more eax
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
		reax = 1					;; no more eax
	ELSE
		.ERR
	ENDIF

	IF $IsAddr(Entry)
		e SUBSTR <Entry>, 6
		IF $IsStack(e)				;; Is relative to SS
			lea ecx, e
		ELSE
			mov ecx, offset e
		ENDIF
	ELSEIF $IsReg(Entry)

		IF reax							;; if eax was used report error
			IFIDNI <Entry>, <eax>
				line TEXTEQU %@Line
				% ECHO @FileCur(line) : ERROR! Register value overwritten by InsertTailList macro.
				.ERR
			ENDIF		
		ENDIF

		IFDIFI <Entry>, <ecx>	;; don't move eax onto itself
			mov ecx, Entry
		ENDIF
	ELSEIF $IsStack(Entry)
		mov ecx, Entry
	ELSE
		.ERR
	ENDIF

	mov (LIST_ENTRY PTR [ecx]).Flink, eax
	push (LIST_ENTRY PTR [eax]).Blink
	pop (LIST_ENTRY PTR [ecx]).Blink

	push (LIST_ENTRY PTR [eax]).Blink
	mov (LIST_ENTRY PTR [eax]).Blink, ecx
	pop eax
	mov (LIST_ENTRY PTR [eax]).Flink, ecx
ENDM

;  VOID InsertHeadList( PLIST_ENTRY ListHead, PLIST_ENTRY Entry );

InsertHeadList MACRO ListHead:REQ, Entry:REQ
local h, e, reax
	reax = 0
	IF $IsAddr(ListHead)
		h SUBSTR <ListHead>, 6
		IF $IsStack(h)				;; Is relative to SS
			lea eax, h
			reax = 1				;; no more eax
		ELSE
			mov eax, offset h
			reax = 1				;; no more eax
		ENDIF
	ELSEIF $IsReg(ListHead)
		IFDIFI <ListHead>, <eax>	;; don't move eax onto itself
			mov eax, ListHead
			reax = 1				;; no more eax
		ENDIF
	ELSEIF $IsStack(ListHead)
		mov eax, ListHead
		reax = 1					;; no more eax
	ELSE
		.ERR
	ENDIF

	IF $IsAddr(Entry)
		e SUBSTR <Entry>, 6
		IF $IsStack(e)				;; Is relative to SS
			lea ecx, e
		ELSE
			mov ecx, offset e
		ENDIF
	ELSEIF $IsReg(Entry)

		IF reax							;; if eax was used report error
			IFIDNI <Entry>, <eax>
				line TEXTEQU %@Line
				% ECHO @FileCur(line) : ERROR! Register value overwritten by InsertTailList macro.
				.ERR
			ENDIF		
		ENDIF

		IFDIFI <Entry>, <ecx>	;; don't move eax onto itself
			mov ecx, Entry
		ENDIF
	ELSEIF $IsStack(Entry)
		mov ecx, Entry
	ELSE
		.ERR
	ENDIF

	mov (LIST_ENTRY PTR [ecx]).Blink, eax
	push (LIST_ENTRY PTR [eax]).Flink
	pop (LIST_ENTRY PTR [ecx]).Flink

	push (LIST_ENTRY PTR [eax]).Flink
	mov (LIST_ENTRY PTR [eax]).Flink, ecx
	pop eax
	mov (LIST_ENTRY PTR [eax]).Blink, ecx
ENDM

; Subroutines for dealing with the Registry

RTL_QUERY_REGISTRY_TABLE STRUCT
	QueryRoutine	PVOID	?	; 0000h  PRTL_QUERY_REGISTRY_ROUTINE
	Flags			DWORD	?	; 0004h
	_Name			PWSTR	?	; 0008h  (org name Name)
	EntryContext	PVOID	?	; 000Ch
	DefaultType		DWORD	?	; 0010h
	DefaultData		PVOID	?	; 0014h
	DefaultLength	DWORD	?	; 0018h
RTL_QUERY_REGISTRY_TABLE ENDS
PRTL_QUERY_REGISTRY_TABLE typedef PTR RTL_QUERY_REGISTRY_TABLE

; The following flags specify how the Name field of a RTL_QUERY_REGISTRY_TABLE
; entry is interpreted.  A NULL name indicates the end of the table.

RTL_QUERY_REGISTRY_SUBKEY   equ 00000001h	; Name is a subkey and remainder of
											; table or until next subkey are value
											; names for that subkey to look at.

RTL_QUERY_REGISTRY_TOPKEY   equ 00000002h	; Reset current key to original key for
											; this and all following table entries.

RTL_QUERY_REGISTRY_REQUIRED equ 00000004h	; Fail if no match found for this table entry.

RTL_QUERY_REGISTRY_NOVALUE  equ 00000008h	; Used to mark a table entry that has no
											; value name, just wants a call out, not
											; an enumeration of all values.

RTL_QUERY_REGISTRY_NOEXPAND equ 00000010h	; Used to suppress the expansion of
											; REG_MULTI_SZ into multiple callouts or
											; to prevent the expansion of environment
											; variable values in REG_EXPAND_SZ

RTL_QUERY_REGISTRY_DIRECT   equ 00000020h	; QueryRoutine field ignored.  EntryContext
											; field points to location to store value.
											; For null terminated strings, EntryContext
											; points to UNICODE_STRING structure that
											; that describes maximum size of buffer.
											; If .Buffer field is NULL then a buffer is
											; allocated.

RTL_QUERY_REGISTRY_DELETE   equ 00000040h	; Used to delete value keys after they
											; are queried.

; The following values for the RelativeTo parameter determine what the
; Path parameter to RtlQueryRegistryValues is relative to.

RTL_REGISTRY_ABSOLUTE     equ 0		; Path is a full path
RTL_REGISTRY_SERVICES     equ 1		; \Registry\Machine\System\CurrentControlSet\Services
RTL_REGISTRY_CONTROL      equ 2		; \Registry\Machine\System\CurrentControlSet\Control
RTL_REGISTRY_WINDOWS_NT   equ 3		; \Registry\Machine\Software\Microsoft\Windows NT\CurrentVersion
RTL_REGISTRY_DEVICEMAP    equ 4		; \Registry\Machine\Hardware\DeviceMap
RTL_REGISTRY_USER         equ 5		; \Registry\User\CurrentUser
RTL_REGISTRY_MAXIMUM      equ 6
RTL_REGISTRY_HANDLE       equ 40000000h	; Low order bits are registry handle
RTL_REGISTRY_OPTIONAL     equ 80000000h	; Indicates the key node is optional

;  Time conversion routines

TIME_FIELDS STRUCT		; sizeof = 010h
	Year			SWORD		?	; 0000h range [1601...]
	Month			SWORD		?	; 0002h range [1..12]
	Day				SWORD		?	; 0004h range [1..31]
	Hour			SWORD		?	; 0006h range [0..23]
	Minute			SWORD		?	; 0008h range [0..59]
	Second			SWORD		?	; 000Ah range [0..59]
	Milliseconds	SWORD		?	; 000Ch range [0..999]
	Weekday			SWORD		?	; 000Eh range [0..6] == [Sunday..Saturday]
TIME_FIELDS ENDS
PTIME_FIELDS typedef PTR TIME_FIELDS

; +
; The following macros store and retrieve USHORTS and ULONGS from potentially
; unaligned addresses, avoiding alignment faults.  they should probably be
; rewritten in assembler
; -

SHORT_SIZE				equ sizeof USHORT
SHORT_MASK				equ (SHORT_SIZE - 1)
LONG_SIZE				equ sizeof LONG
LONG_MASK				equ (LONG_SIZE - 1)
LONGLONG_SIZE			equ sizeof LONGLONG
LONGLONG_MASK			equ (LONGLONG_SIZE - 1)
LOWBYTE_MASK			equ 00FFh

; +
;  BitMap routines.  The following structure, routines, and macros are
;  for manipulating bitmaps.  The user is responsible for allocating a bitmap
;  structure (which is really a header) and a buffer (which must be longword
;  aligned and multiple longwords in size).
; -

RTL_BITMAP STRUCT				; sizeof = 8
	SizeOfBitMap	DWORD	?	; 00h  Number of bits in bit map
	Buffer			PVOID	?	; 04h  Pointer to the bit map itself
RTL_BITMAP ENDS
PRTL_BITMAP typedef PTR RTL_BITMAP

; +
;  The following routine locates a set of contiguous regions of clear
;  bits within the bitmap.  The caller specifies whether to return the
;  longest runs or just the first found lcoated.  The following structure is
;  used to denote a contiguous run of bits.  The two routines return an array
;  of this structure, one for each run located.
; -

RTL_BITMAP_RUN STRUCT			; sizeof = 8
	StartingIndex	DWORD	?	; 00h
	NumberOfBits	DWORD	?	; 04h
RTL_BITMAP_RUN ENDS
PRTL_BITMAP_RUN typedef ptr RTL_BITMAP_RUN

; Range list package

RTL_RANGE STRUCT				; sizeof = 20h

	; The start of the range
	_Start			QWORD	?	; 000h Read only

	; The end of the range
	_End			QWORD	?	; 008h Read only

	; Data the user passed in when they created the range
	UserData		PVOID	?	; 010h Read/Write

	; The owner of the range
	Owner			PVOID	?	; 014h Read/Write

	; User defined flags the user specified when they created the range
	Attributes		BYTE	?	; 018h Read/Write

	; Flags (RTL_RANGE_*)
	Flags			BYTE	?	; 019h Read only

					db 6 dup(?)	; padding

RTL_RANGE ENDS
PRTL_RANGE typedef ptr RTL_RANGE

RTL_RANGE_SHARED	equ 01
RTL_RANGE_CONFLICT	equ 02

RTL_RANGE_LIST STRUCT				; sizeof = 14h

	; The list of ranges
	ListHead			LIST_ENTRY	<>	; 000h

	; These always come in useful
	Flags				DWORD		?	; 008h use RANGE_LIST_FLAG_*

	; The number of entries in the list
	Count				DWORD		?	; 0C0h

	; Every time an add/delete operation is performed on the list this is
	; incremented.  It is checked during iteration to ensure that the list
	; hasn't changed between GetFirst/GetNext or GetNext/GetNext calls
	Stamp				DWORD		?	; 010h

RTL_RANGE_LIST ENDS
PRTL_RANGE_LIST typedef ptr RTL_RANGE_LIST


RTL_RANGE_LIST_ITERATOR STRUCT
	RangeListHead	PVOID	?	; 000h PLIST_ENTRY
	MergedHead		PVOID	?	; 004h PLIST_ENTRY
	Current			PVOID	?	; 008h
	Stamp			DWORD	?	; 00Ch
RTL_RANGE_LIST_ITERATOR ENDS
PRTL_RANGE_LIST_ITERATOR typedef ptr RTL_RANGE_LIST_ITERATOR


RTL_RANGE_LIST_ADD_IF_CONFLICT		equ 00000001
RTL_RANGE_LIST_ADD_SHARED			equ 00000002

RTL_RANGE_LIST_SHARED_OK			equ 00000001
RTL_RANGE_LIST_NULL_CONFLICT_OK		equ 00000002

OSVERSIONINFOA STRUCT
	dwOSVersionInfoSize		DWORD	?		; 000h
	dwMajorVersion			DWORD	?		; 004h
	dwMinorVersion			DWORD	?		; 008h
	dwBuildNumber			DWORD	?		; 00Ch
	dwPlatformId			DWORD	?		; 010h
	szCSDVersion			BYTE 128 dup(?)	; 014h Maintenance string for PSS usage
OSVERSIONINFOA ENDS
POSVERSIONINFOA typedef ptr OSVERSIONINFOA
LPOSVERSIONINFOA typedef ptr OSVERSIONINFOA

OSVERSIONINFOW STRUCT
	dwOSVersionInfoSize		DWORD	?		; 000h
	dwMajorVersion			DWORD	?		; 004h
	dwMinorVersion			DWORD	?		; 008h
	dwBuildNumber			DWORD	?		; 00Ch
	dwPlatformId			DWORD	?		; 010h
	szCSDVersion			WORD 128 dup(?)	; 014h Maintenance string for PSS usage
OSVERSIONINFOW ENDS
POSVERSIONINFOW typedef ptr OSVERSIONINFOW
LPOSVERSIONINFOW typedef ptr OSVERSIONINFOW

RTL_OSVERSIONINFOW equ <OSVERSIONINFOW>
PRTL_OSVERSIONINFOW typedef ptr OSVERSIONINFOW

IFDEF UNICODE
OSVERSIONINFO equ <OSVERSIONINFOW>
POSVERSIONINFO typedef ptr OSVERSIONINFOW
LPOSVERSIONINFO typedef ptr OSVERSIONINFOW
ELSE
OSVERSIONINFO equ <OSVERSIONINFOA>
POSVERSIONINFO typedef ptr OSVERSIONINFOA
LPOSVERSIONINFO typedef ptr OSVERSIONINFOA
ENDIF

OSVERSIONINFOEXA  STRUCT
	dwOSVersionInfoSize	DWORD	?		; 000h
	dwMajorVersion		DWORD	?		; 004h
	dwMinorVersion		DWORD	?		; 008h
	dwBuildNumber		DWORD	?		; 00Ch
	dwPlatformId		DWORD	?		; 010h
	szCSDVersion		BYTE 128 dup(?)	; 014h Maintenance string for PSS usage
	wServicePackMajor	WORD	?		; 094
	wServicePackMinor	WORD	?		; 096
	wSuiteMask			WORD	?		; 098
	wProductType		BYTE	?		; 09A
	wReserved			BYTE	?		; 09B
OSVERSIONINFOEXA ENDS
POSVERSIONINFOEXA typedef ptr OSVERSIONINFOEXA
LPOSVERSIONINFOEXA typedef ptr OSVERSIONINFOEXA

OSVERSIONINFOEXW  STRUCT
	dwOSVersionInfoSize	DWORD	?		; 000h
	dwMajorVersion		DWORD	?		; 004h
	dwMinorVersion		DWORD	?		; 008h
	dwBuildNumber		DWORD	?		; 00Ch
	dwPlatformId		DWORD	?		; 010h
	szCSDVersion		WORD 128 dup(?)	; 014h Maintenance string for PSS usage
	wServicePackMajor	WORD	?		; 114h
	wServicePackMinor	WORD	?		; 116h
	wSuiteMask			WORD	?		; 118h
	wProductType		BYTE	?		; 11Ah
	wReserved			BYTE	?		; 11Bh
OSVERSIONINFOEXW ENDS
POSVERSIONINFOEXW typedef ptr OSVERSIONINFOEXW
LPOSVERSIONINFOEXW typedef ptr OSVERSIONINFOEXW

RTL_OSVERSIONINFOEXW equ <OSVERSIONINFOEXW>
PRTL_OSVERSIONINFOEXW typedef ptr OSVERSIONINFOEXW

IFDEF UNICODE
OSVERSIONINFOEX equ <OSVERSIONINFOEXW>
POSVERSIONINFOEX typedef ptr OSVERSIONINFOEXW
LPOSVERSIONINFOEX typedef ptr OSVERSIONINFOEXW
ELSE
OSVERSIONINFOEX equ <OSVERSIONINFOEXA>
POSVERSIONINFOEX typedef ptr OSVERSIONINFOEXA
LPOSVERSIONINFOEX typedef ptr OSVERSIONINFOEXA
ENDIF

; RtlVerifyVersionInfo() conditions

VER_EQUAL                       equ 1
VER_GREATER                     equ 2
VER_GREATER_EQUAL               equ 3
VER_LESS                        equ 4
VER_LESS_EQUAL                  equ 5
VER_AND                         equ 6
VER_OR                          equ 7

VER_CONDITION_MASK              equ 7
VER_NUM_BITS_PER_CONDITION_MASK equ 3

; RtlVerifyVersionInfo() type mask bits

VER_MINORVERSION                equ 0000001h
VER_MAJORVERSION                equ 0000002h
VER_BUILDNUMBER                 equ 0000004h
VER_PLATFORMID                  equ 0000008h
VER_SERVICEPACKMINOR            equ 0000010h
VER_SERVICEPACKMAJOR            equ 0000020h
VER_SUITENAME                   equ 0000040h
VER_PRODUCT_TYPE                equ 0000080h

; RtlVerifyVersionInfo() os product type values

VER_NT_WORKSTATION              equ 0000001
VER_NT_DOMAIN_CONTROLLER        equ 0000002
VER_NT_SERVER                   equ 0000003

; dwPlatformId defines:

VER_PLATFORM_WIN32s             equ 0
VER_PLATFORM_WIN32_WINDOWS      equ 1
VER_PLATFORM_WIN32_NT           equ 2

; +
; Define the various device type values.  Note that values used by Microsoft
; Corporation are in the range 0-32767, and 32768-65535 are reserved for use
; by customers.
; -

DEVICE_TYPE	typedef	DWORD

FILE_DEVICE_BEEP                equ 01
FILE_DEVICE_CD_ROM              equ 02
FILE_DEVICE_CD_ROM_FILE_SYSTEM  equ 03
FILE_DEVICE_CONTROLLER          equ 04
FILE_DEVICE_DATALINK            equ 05
FILE_DEVICE_DFS                 equ 06
FILE_DEVICE_DISK                equ 07
FILE_DEVICE_DISK_FILE_SYSTEM    equ 08
FILE_DEVICE_FILE_SYSTEM         equ 09
FILE_DEVICE_INPORT_PORT         equ 0ah
FILE_DEVICE_KEYBOARD            equ 0bh
FILE_DEVICE_MAILSLOT            equ 0ch
FILE_DEVICE_MIDI_IN             equ 0dh
FILE_DEVICE_MIDI_OUT            equ 0eh
FILE_DEVICE_MOUSE               equ 0fh
FILE_DEVICE_MULTI_UNC_PROVIDER  equ 10h
FILE_DEVICE_NAMED_PIPE          equ 11h
FILE_DEVICE_NETWORK             equ 12h
FILE_DEVICE_NETWORK_BROWSER     equ 13h
FILE_DEVICE_NETWORK_FILE_SYSTEM equ 14h
FILE_DEVICE_NULL                equ 15h
FILE_DEVICE_PARALLEL_PORT       equ 16h
FILE_DEVICE_PHYSICAL_NETCARD    equ 17h
FILE_DEVICE_PRINTER             equ 18h
FILE_DEVICE_SCANNER             equ 19h
FILE_DEVICE_SERIAL_MOUSE_PORT   equ 1ah
FILE_DEVICE_SERIAL_PORT         equ 1bh
FILE_DEVICE_SCREEN              equ 1ch
FILE_DEVICE_SOUND               equ 1dh
FILE_DEVICE_STREAMS             equ 1eh
FILE_DEVICE_TAPE                equ 1fh
FILE_DEVICE_TAPE_FILE_SYSTEM    equ 20h
FILE_DEVICE_TRANSPORT           equ 21h
FILE_DEVICE_UNKNOWN             equ 22h
FILE_DEVICE_VIDEO               equ 23h
FILE_DEVICE_VIRTUAL_DISK        equ 24h
FILE_DEVICE_WAVE_IN             equ 25h
FILE_DEVICE_WAVE_OUT            equ 26h
FILE_DEVICE_8042_PORT           equ 27h
FILE_DEVICE_NETWORK_REDIRECTOR  equ 28h
FILE_DEVICE_BATTERY             equ 29h
FILE_DEVICE_BUS_EXTENDER        equ 2ah
FILE_DEVICE_MODEM               equ 2bh
FILE_DEVICE_VDM                 equ 2ch
FILE_DEVICE_MASS_STORAGE        equ 2dh
FILE_DEVICE_SMB                 equ 2eh
FILE_DEVICE_KS                  equ 2fh
FILE_DEVICE_CHANGER             equ 30h
FILE_DEVICE_SMARTCARD           equ 31h
FILE_DEVICE_ACPI                equ 32h
FILE_DEVICE_DVD                 equ 33h
FILE_DEVICE_FULLSCREEN_VIDEO    equ 34h
FILE_DEVICE_DFS_FILE_SYSTEM     equ 35h
FILE_DEVICE_DFS_VOLUME          equ 36h
FILE_DEVICE_SERENUM             equ 37h
FILE_DEVICE_TERMSRV             equ 38h
FILE_DEVICE_KSEC                equ 39h

; +
; Macro definition for defining IOCTL and FSCTL function control codes.  Note
; that function codes 0-2047 are reserved for Microsoft Corporation, and
; 2048-4095 are reserved for customers.
; -

CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM

; Macro to extract device type out of the device io control code

DEVICE_TYPE_FROM_CTL_CODE MACRO ctrlCode:REQ
	EXITM %(((ctrlCode) AND 0ffff0000h) SHR 16)
ENDM

; Define the method codes for how buffers are passed for I/O and FS controls

METHOD_BUFFERED                 equ 0
METHOD_IN_DIRECT                equ 1
METHOD_OUT_DIRECT               equ 2
METHOD_NEITHER                  equ 3

; Define the access check value for any access
;
; The FILE_READ_ACCESS and FILE_WRITE_ACCESS constants are also defined in
; ntioapi.h as FILE_READ_DATA and FILE_WRITE_DATA. The values for these
; constants *MUST* always be in sync.
;
; FILE_SPECIAL_ACCESS is checked by the NT I/O system the same as FILE_ANY_ACCESS.
; The file systems, however, may add additional access checks for I/O and FS controls
; that use this value.

FILE_ANY_ACCESS			equ 0
FILE_SPECIAL_ACCESS		equ FILE_ANY_ACCESS
FILE_READ_ACCESS		equ 1	; file & pipe
FILE_WRITE_ACCESS		equ 2	; file & pipe

; Define access rights to files and directories

; +
; The FILE_READ_DATA and FILE_WRITE_DATA constants are also defined in
; devioctl.h as FILE_READ_ACCESS and FILE_WRITE_ACCESS. The values for these
; constants *MUST* always be in sync.
; The values are redefined in devioctl.h because they must be available to
; both DOS and NT.
; -

FILE_READ_DATA				equ 0001	; file & pipe
FILE_LIST_DIRECTORY			equ 0001	; directory

FILE_WRITE_DATA				equ 0002	; file & pipe
FILE_ADD_FILE				equ 0002	; directory

FILE_APPEND_DATA			equ 0004	; file
FILE_ADD_SUBDIRECTORY		equ 0004	; directory
FILE_CREATE_PIPE_INSTANCE	equ 0004	; named pipe

FILE_READ_EA				equ 0008	; file & directory

FILE_WRITE_EA				equ 0010h	; file & directory

FILE_EXECUTE				equ 0020h	; file
FILE_TRAVERSE				equ 0020h	; directory

FILE_DELETE_CHILD			equ 0040h	; directory

FILE_READ_ATTRIBUTES		equ 0080h	; all

FILE_WRITE_ATTRIBUTES		equ 0100h	; all

FILE_ALL_ACCESS				equ (STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 1FFh)

FILE_GENERIC_READ equ (STANDARD_RIGHTS_READ or FILE_READ_DATA or FILE_READ_ATTRIBUTES or FILE_READ_EA or SYNCHRONIZE)

FILE_GENERIC_WRITE equ (STANDARD_RIGHTS_WRITE or FILE_WRITE_DATA or FILE_WRITE_ATTRIBUTES or FILE_WRITE_EA or FILE_APPEND_DATA or SYNCHRONIZE)

FILE_GENERIC_EXECUTE equ (STANDARD_RIGHTS_EXECUTE or FILE_READ_ATTRIBUTES or FILE_EXECUTE or SYNCHRONIZE)

; Define share access rights to files and directories

FILE_SHARE_READ                 equ 1	; winnt
FILE_SHARE_WRITE                equ 2	; winnt
FILE_SHARE_DELETE               equ 4	; winnt
FILE_SHARE_VALID_FLAGS          equ 7

; +
; Define the file attributes values
;
; Note:  0x00000008 is reserved for use for the old DOS VOLID (volume ID)
;         and is therefore not considered valid in NT.
;
; Note:  0x00000010 is reserved for use for the old DOS SUBDIRECTORY flag
;         and is therefore not considered valid in NT.  This flag has
;         been disassociated with file attributes since the other flags are
;         protected with READ_ and WRITE_ATTRIBUTES access to the file.
;
; Note:  Note also that the order of these flags is set to allow both the
;         FAT and the Pinball File Systems to directly set the attributes
;         flags in attributes words without having to pick each flag out
;         individually.  The order of these flags should not be changed!
; -

FILE_ATTRIBUTE_READONLY             equ 00000001h  ; winnt
FILE_ATTRIBUTE_HIDDEN               equ 00000002h  ; winnt
FILE_ATTRIBUTE_SYSTEM               equ 00000004h  ; winnt

FILE_ATTRIBUTE_DIRECTORY            equ 00000010h  ; winnt
FILE_ATTRIBUTE_ARCHIVE              equ 00000020h  ; winnt
FILE_ATTRIBUTE_DEVICE               equ 00000040h  ; winnt
FILE_ATTRIBUTE_NORMAL               equ 00000080h  ; winnt

FILE_ATTRIBUTE_TEMPORARY            equ 00000100h  ; winnt
FILE_ATTRIBUTE_SPARSE_FILE          equ 00000200h  ; winnt
FILE_ATTRIBUTE_REPARSE_POINT        equ 00000400h  ; winnt
FILE_ATTRIBUTE_COMPRESSED           equ 00000800h  ; winnt

FILE_ATTRIBUTE_OFFLINE              equ 00001000h  ; winnt
FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  equ 00002000h  ; winnt
FILE_ATTRIBUTE_ENCRYPTED            equ 00004000h  ; winnt

; This definition is old and will disappear shortly

FILE_ATTRIBUTE_CONTENT_INDEXED  equ FILE_ATTRIBUTE_NOT_CONTENT_INDEXED

FILE_ATTRIBUTE_VALID_FLAGS      equ 00007fb7h
FILE_ATTRIBUTE_VALID_SET_FLAGS  equ 000031a7h

; Define the create disposition values

FILE_SUPERSEDE                  equ 0
FILE_OPEN                       equ 1
FILE_CREATE                     equ 2
FILE_OPEN_IF                    equ 3
FILE_OVERWRITE                  equ 4
FILE_OVERWRITE_IF               equ 5
FILE_MAXIMUM_DISPOSITION        equ 5

; Define the create/open option flags

FILE_DIRECTORY_FILE                     equ 00000001h
FILE_WRITE_THROUGH                      equ 00000002h
FILE_SEQUENTIAL_ONLY                    equ 00000004h
FILE_NO_INTERMEDIATE_BUFFERING          equ 00000008h

FILE_SYNCHRONOUS_IO_ALERT               equ 00000010h
FILE_SYNCHRONOUS_IO_NONALERT            equ 00000020h
FILE_NON_DIRECTORY_FILE                 equ 00000040h
FILE_CREATE_TREE_CONNECTION             equ 00000080h

FILE_COMPLETE_IF_OPLOCKED               equ 00000100h
FILE_NO_EA_KNOWLEDGE                    equ 00000200h
FILE_OPEN_FOR_RECOVERY                  equ 00000400h
FILE_RANDOM_ACCESS                      equ 00000800h

FILE_DELETE_ON_CLOSE                    equ 00001000h
FILE_OPEN_BY_FILE_ID                    equ 00002000h
FILE_OPEN_FOR_BACKUP_INTENT             equ 00004000h
FILE_NO_COMPRESSION                     equ 00008000h

FILE_RESERVE_OPFILTER                   equ 00100000h
FILE_OPEN_REPARSE_POINT                 equ 00200000h
FILE_OPEN_NO_RECALL                     equ 00400000h
FILE_OPEN_FOR_FREE_SPACE_QUERY          equ 00800000h

FILE_COPY_STRUCTURED_STORAGE            equ 00000041h
FILE_STRUCTURED_STORAGE                 equ 00000441h

FILE_VALID_OPTION_FLAGS                 equ 00ffffffh
FILE_VALID_PIPE_OPTION_FLAGS            equ 00000032h
FILE_VALID_MAILSLOT_OPTION_FLAGS        equ 00000032h
FILE_VALID_SET_FLAGS                    equ 00000036h

; Define the I/O status information return values for NtCreateFile/NtOpenFile

FILE_SUPERSEDED                 equ 0
FILE_OPENED                     equ 1
FILE_CREATED                    equ 2
FILE_OVERWRITTEN                equ 3
FILE_EXISTS                     equ 4
FILE_DOES_NOT_EXIST             equ 5

; Define special ByteOffset parameters for read and write operations

FILE_WRITE_TO_END_OF_FILE       equ 0ffffffffh
FILE_USE_FILE_POINTER_POSITION  equ 0fffffffeh

; Define alignment requirement values

FILE_BYTE_ALIGNMENT             equ 00000000h
FILE_WORD_ALIGNMENT             equ 00000001h
FILE_LONG_ALIGNMENT             equ 00000003h
FILE_QUAD_ALIGNMENT             equ 00000007h
FILE_OCTA_ALIGNMENT             equ 0000000fh
FILE_32_BYTE_ALIGNMENT          equ 0000001fh
FILE_64_BYTE_ALIGNMENT          equ 0000003fh
FILE_128_BYTE_ALIGNMENT         equ 0000007fh
FILE_256_BYTE_ALIGNMENT         equ 000000ffh
FILE_512_BYTE_ALIGNMENT         equ 000001ffh

; Define the maximum length of a filename string

MAXIMUM_FILENAME_LENGTH         equ 256

; Define the various device characteristics flags

FILE_REMOVABLE_MEDIA            equ 00000001h
FILE_READ_ONLY_DEVICE           equ 00000002h
FILE_FLOPPY_DISKETTE            equ 00000004h
FILE_WRITE_ONCE_MEDIA           equ 00000008h
FILE_REMOTE_DEVICE              equ 00000010h
FILE_DEVICE_IS_MOUNTED          equ 00000020h
FILE_VIRTUAL_VOLUME             equ 00000040h
FILE_AUTOGENERATED_DEVICE_NAME  equ 00000080h
FILE_DEVICE_SECURE_OPEN         equ 00000100h

; end_wdm

; flags specified here will be propagated up and down a device stack
; after FDO and all filter devices are added, but before the device
; stack is started

FILE_CHARACTERISTICS_PROPAGATED equ FILE_REMOVABLE_MEDIA or FILE_READ_ONLY_DEVICE or FILE_FLOPPY_DISKETTE or FILE_WRITE_ONCE_MEDIA or FILE_DEVICE_SECURE_OPEN

; Define the base asynchronous I/O argument types

IO_STATUS_BLOCK STRUCT		; sizeof = 08h
	Status		SDWORD		?	; 0000h  NTSTATUS
	Information	DWORD		?	; 0004h
IO_STATUS_BLOCK ENDS
PIO_STATUS_BLOCK typedef PTR IO_STATUS_BLOCK

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the file information class values
;
; WARNING:  The order of the following values are assumed by the I/O system.
;           Any changes made here should be reflected there as well.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation      equ 1
    FileFullDirectoryInformation  equ 2
    FileBothDirectoryInformation  equ 3
    FileBasicInformation          equ 4  ; wdm
    FileStandardInformation       equ 5  ; wdm
    FileInternalInformation       equ 6
    FileEaInformation             equ 7
    FileAccessInformation         equ 8
    FileNameInformation           equ 9
    FileRenameInformation         equ 10
    FileLinkInformation           equ 11
    FileNamesInformation          equ 12
    FileDispositionInformation    equ 13
    FilePositionInformation       equ 14 ; wdm
    FileFullEaInformation         equ 15
    FileModeInformation           equ 16
    FileAlignmentInformation      equ 17
    FileAllInformation            equ 18
    FileAllocationInformation     equ 19
    FileEndOfFileInformation      equ 20 ; wdm
    FileAlternateNameInformation  equ 21
    FileStreamInformation         equ 22
    FilePipeInformation           equ 23
    FilePipeLocalInformation      equ 24
    FilePipeRemoteInformation     equ 25
    FileMailslotQueryInformation  equ 26
    FileMailslotSetInformation    equ 27
    FileCompressionInformation    equ 28
    FileObjectIdInformation       equ 29
    FileCompletionInformation     equ 30
    FileMoveClusterInformation    equ 31
    FileQuotaInformation          equ 32
    FileReparsePointInformation   equ 33
    FileNetworkOpenInformation    equ 34
    FileAttributeTagInformation   equ 35
    FileTrackingInformation       equ 36
    FileMaximumInformation        equ 37

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the various structures which are returned on query operations
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FILE_BASIC_INFORMATION STRUCT		; sizeof = 028h
	CreationTime		LARGE_INTEGER	<>
	LastAccessTime		LARGE_INTEGER	<>
	LastWriteTime		LARGE_INTEGER	<>
	ChangeTime			LARGE_INTEGER	<>
	FileAttributes		DWORD			?
						DWORD			?	; padding
FILE_BASIC_INFORMATION ENDS
PFILE_BASIC_INFORMATION typedef ptr FILE_BASIC_INFORMATION
                                                            
FILE_STANDARD_INFORMATION STRUCT			; sizeof = 018h
	AllocationSize		LARGE_INTEGER	<>
	EndOfFile			LARGE_INTEGER	<>
	NumberOfLinks		DWORD			?
	DeletePending		BOOLEAN			?
	Directory			BOOLEAN			?
    					db 		2 dup(?)	; padding
FILE_STANDARD_INFORMATION ENDS
PFILE_STANDARD_INFORMATION typedef ptr FILE_STANDARD_INFORMATION
                                                            
FILE_POSITION_INFORMATION STRUCT
	CurrentByteOffset	LARGE_INTEGER	<>
FILE_POSITION_INFORMATION ENDS
PFILE_POSITION_INFORMATION typedef ptr FILE_POSITION_INFORMATION
                                                            
FILE_ALIGNMENT_INFORMATION  STRUCT
	AlignmentRequirement	DWORD	?
FILE_ALIGNMENT_INFORMATION ENDS
PFILE_ALIGNMENT_INFORMATION typedef ptr FILE_ALIGNMENT_INFORMATION
                                                            
FILE_NAME_INFORMATION  STRUCT
	FileNameLength	DWORD	?
	FileName		WCHAR	1 dup(?)
FILE_NAME_INFORMATION ENDS
PFILE_NAME_INFORMATION typedef ptr FILE_NAME_INFORMATION
                                                            
FILE_NETWORK_OPEN_INFORMATION  STRUCT		; sizeof = 038h
	CreationTime	LARGE_INTEGER	<>
	LastAccessTime	LARGE_INTEGER	<>
	LastWriteTime	LARGE_INTEGER	<>
	ChangeTime		LARGE_INTEGER	<>
	AllocationSize	LARGE_INTEGER	<>
	EndOfFile		LARGE_INTEGER	<>
	FileAttributes	DWORD			?
					DWORD			?	; padding
FILE_NETWORK_OPEN_INFORMATION ENDS
PFILE_NETWORK_OPEN_INFORMATION typedef ptr FILE_NETWORK_OPEN_INFORMATION

FILE_ATTRIBUTE_TAG_INFORMATION STRUCT		; sizeof=8
	FileAttributes	DWORD	?
	ReparseTag		DWORD	?
FILE_ATTRIBUTE_TAG_INFORMATION ENDS
PFILE_ATTRIBUTE_TAG_INFORMATION typedef PTR FILE_ATTRIBUTE_TAG_INFORMATION

FILE_DISPOSITION_INFORMATION STRUCT			; sizeof=1  :-(	I'm 99% sure it cann't be so
	DeleteFile		BOOLEAN	?
FILE_DISPOSITION_INFORMATION ENDS
PFILE_DISPOSITION_INFORMATION typedef PTR FILE_DISPOSITION_INFORMATION

FILE_END_OF_FILE_INFORMATION STRUCT			; sizeof=8
	EndOfFile		LARGE_INTEGER	<>
FILE_END_OF_FILE_INFORMATION ENDS
PFILE_END_OF_FILE_INFORMATION typedef PTR FILE_END_OF_FILE_INFORMATION

FILE_FULL_EA_INFORMATION STRUCT		; sizeof = 0Ch
	NextEntryOffset		DWORD	?	; 00h
	qwFlags				BYTE	?	; 04h
	EaNameLength		BYTE	?	; 05h
	EaValueLength		WORD	?	; 06h
	EaName				BYTE	?	; 08h
						db 3 dup(?)
FILE_FULL_EA_INFORMATION ENDS
PFILE_FULL_EA_INFORMATION typedef PTR FILE_FULL_EA_INFORMATION

; +
; Define the file system information class values
;
; WARNING:  The order of the following values are assumed by the I/O system.
;           Any changes made here should be reflected there as well.
; -

;typedef enum _FSINFOCLASS {
    FileFsVolumeInformation      equ 1
    FileFsLabelInformation       equ 2
    FileFsSizeInformation        equ 3
    FileFsDeviceInformation      equ 4
    FileFsAttributeInformation   equ 5
    FileFsControlInformation     equ 6
    FileFsFullSizeInformation    equ 7
    FileFsObjectIdInformation    equ 8
    FileFsMaximumInformation     equ 9

FILE_FS_DEVICE_INFORMATION STRUCT	; sizeof = 8
	DeviceType			DWORD	?	; 00h DEVICE_TYPE
	Characteristics		DWORD	?	; 04h
FILE_FS_DEVICE_INFORMATION ENDS
PFILE_FS_DEVICE_INFORMATION typedef PTR FILE_FS_DEVICE_INFORMATION  

; Define segement buffer structure for scatter/gather read/write.

FILE_SEGMENT_ELEMENT UNION
	Buffer		QWORD	?	; PVOID64
	Alignment	QWORD	?
FILE_SEGMENT_ELEMENT ENDS
PFILE_SEGMENT_ELEMENT typedef ptr FILE_SEGMENT_ELEMENT

; Define the I/O bus interface types.

;typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined	equ -1
    Internal				equ 0
    Isa						equ 1
    Eisa					equ 2
    MicroChannel			equ 3
    TurboChannel			equ 4
    PCIBus					equ 5
    VMEBus					equ 6
    NuBus					equ 7
    PCMCIABus				equ 8
    CBus					equ 9
    MPIBus					equ 10
    MPSABus					equ 11
    ProcessorInternal		equ 12
    InternalPowerBus		equ 13
    PNPISABus				equ 14
    PNPBus					equ 15
    MaximumInterfaceType	equ 16

; Define the DMA transfer widths.

;typedef enum _DMA_WIDTH {
    Width8Bits				equ 0
    Width16Bits				equ 1
    Width32Bits				equ 2
    MaximumDmaWidth			equ 3

; Define DMA transfer speeds.

;typedef enum _DMA_SPEED {
    Compatible				equ 0
    TypeA					equ 1
    TypeB					equ 2
    TypeC					equ 3
    TypeF					equ 4
    MaximumDmaSpeed			equ 5

; Define types of bus information.

;typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined		equ -1
    Cmos							equ 0
    EisaConfiguration				equ 1
    Pos								equ 2
    CbusConfiguration				equ 3
    PCIConfiguration				equ 4
    VMEConfiguration				equ 5
    NuBusConfiguration				equ 6
    PCMCIAConfiguration				equ 7
    MPIConfiguration				equ 8
    MPSAConfiguration				equ 9
    PNPISAConfiguration				equ 10
    SgiInternalConfiguration		equ 11
    MaximumBusDataType				equ 12
;} BUS_DATA_TYPE, *PBUS_DATA_TYPE;

; Define I/O Driver error log packet structure.  This structure is filled in
; by the driver.

IO_ERROR_LOG_PACKET STRUCT					; sizeof = 30h
	MajorFunctionCode			BYTE	?
	RetryCount					BYTE	?
	DumpDataSize				WORD	?
	NumberOfStrings				WORD	?
	StringOffset				WORD	?
	EventCategory				WORD	?
								WORD	?
	ErrorCode					SDWORD	?	; 00C NTSTATUS
	UniqueErrorValue			DWORD	?	; 010
	FinalStatus					SDWORD	?	; 014 NTSTATUS
	SequenceNumber				DWORD	?	; 018
	IoControlCode				DWORD	?	; 01C
	DeviceOffset				LARGE_INTEGER	<>	; 020
	DumpData					DWORD	1 dup(?)	; 028
								DWORD	?
IO_ERROR_LOG_PACKET ENDS
PIO_ERROR_LOG_PACKET typedef PTR IO_ERROR_LOG_PACKET

; Define the I/O error log message.  This message is sent by the error log
; thread over the lpc port.

IO_ERROR_LOG_MESSAGE STRUCT
	_Type				WORD	?
	_Size				WORD	?
	DriverNameLength	WORD	?
						WORD	?	; padding
	TimeStamp			LARGE_INTEGER	<>
	DriverNameOffset	DWORD	?
	EntryData			IO_ERROR_LOG_PACKET	<>
IO_ERROR_LOG_MESSAGE ENDS
PIO_ERROR_LOG_MESSAGE typedef PTR IO_ERROR_LOG_MESSAGE

; Define the maximum message size that will be sent over the LPC to the
; application reading the error log entries.

; Regardless of LPC size restrictions, ERROR_LOG_MAXIMUM_SIZE must remain
; a value that can fit in a UCHAR.

ERROR_LOG_LIMIT_SIZE		equ (256-16)

; This limit, exclusive of IO_ERROR_LOG_MESSAGE_HEADER_LENGTH, also applies
; to IO_ERROR_LOG_MESSAGE_LENGTH

IO_ERROR_LOG_MESSAGE_HEADER_LENGTH	equ ( (sizeof IO_ERROR_LOG_MESSAGE) - (sizeof IO_ERROR_LOG_PACKET) +  (sizeof WORD)*40 )

ERROR_LOG_MESSAGE_LIMIT_SIZE		equ ( ERROR_LOG_LIMIT_SIZE + IO_ERROR_LOG_MESSAGE_HEADER_LENGTH )

IFDEF _WIN64
	PORT_MAXIMUM_MESSAGE_LENGTH	equ 512
ELSE
	PORT_MAXIMUM_MESSAGE_LENGTH	equ 256
ENDIF

; IO_ERROR_LOG_MESSAGE_LENGTH is
; min(PORT_MAXIMUM_MESSAGE_LENGTH, ERROR_LOG_MESSAGE_LIMIT_SIZE)

IF PORT_MAXIMUM_MESSAGE_LENGTH GT ERROR_LOG_MESSAGE_LIMIT_SIZE
	IO_ERROR_LOG_MESSAGE_LENGTH			equ ERROR_LOG_MESSAGE_LIMIT_SIZE
ELSE
	IO_ERROR_LOG_MESSAGE_LENGTH			equ PORT_MAXIMUM_MESSAGE_LENGTH
ENDIF
;IO_ERROR_LOG_MESSAGE_LENGTH			equ ( (PORT_MAXIMUM_MESSAGE_LENGTH > ERROR_LOG_MESSAGE_LIMIT_SIZE) ?           \
;        ERROR_LOG_MESSAGE_LIMIT_SIZE :                                        \
;        PORT_MAXIMUM_MESSAGE_LENGTH)

; Define the maximum packet size a driver can allocate.

ERROR_LOG_MAXIMUM_SIZE				equ ( IO_ERROR_LOG_MESSAGE_LENGTH - IO_ERROR_LOG_MESSAGE_HEADER_LENGTH )

; Registry Specific Access Rights.

KEY_QUERY_VALUE         equ 0001h
KEY_SET_VALUE           equ 0002h
KEY_CREATE_SUB_KEY      equ 0004h
KEY_ENUMERATE_SUB_KEYS  equ 0008h
KEY_NOTIFY              equ 0010h
KEY_CREATE_LINK         equ 0020h

KEY_READ	equ (STANDARD_RIGHTS_READ or KEY_QUERY_VALUE or KEY_ENUMERATE_SUB_KEYS or KEY_NOTIFY) AND NOT SYNCHRONIZE
KEY_WRITE	equ (STANDARD_RIGHTS_WRITE or KEY_SET_VALUE or KEY_CREATE_SUB_KEY) AND NOT SYNCHRONIZE
KEY_EXECUTE	equ (KEY_READ AND NOT SYNCHRONIZE)
KEY_ALL_ACCESS	equ (STANDARD_RIGHTS_ALL or KEY_QUERY_VALUE or KEY_SET_VALUE or KEY_CREATE_SUB_KEY or KEY_ENUMERATE_SUB_KEYS or KEY_NOTIFY or KEY_CREATE_LINK) AND NOT SYNCHRONIZE

; Open/Create Options

REG_OPTION_RESERVED         equ 00000000   ; Parameter is reserved
REG_OPTION_NON_VOLATILE     equ 00000000   ; Key is preserved when system is rebooted
REG_OPTION_VOLATILE         equ 00000001   ; Key is not preserved when system is rebooted
REG_OPTION_CREATE_LINK      equ 00000002   ; Created key is a symbolic link
REG_OPTION_BACKUP_RESTORE   equ 00000004   ; open for backup or restore special access rules privilege required
REG_OPTION_OPEN_LINK        equ 00000008   ; Open symbolic link
REG_LEGAL_OPTION			equ (REG_OPTION_RESERVED or REG_OPTION_NON_VOLATILE or REG_OPTION_VOLATILE or REG_OPTION_CREATE_LINK or REG_OPTION_BACKUP_RESTORE or REG_OPTION_OPEN_LINK)

; Key creation/open disposition

REG_CREATED_NEW_KEY         equ 00000001   ; New Registry Key created
REG_OPENED_EXISTING_KEY     equ 00000002   ; Existing Key opened

; Key restore flags

REG_WHOLE_HIVE_VOLATILE     equ 00000001   ; Restore whole hive volatile
REG_REFRESH_HIVE            equ 00000002   ; Unwind changes to last flush
REG_NO_LAZY_FLUSH           equ 00000004   ; Never lazy flush this hive
REG_FORCE_RESTORE           equ 00000008   ; Force the restore process even when we have open handles on subkeys

; Key query structures

KEY_BASIC_INFORMATION STRUCT
	LastWriteTime		LARGE_INTEGER	<>	; 00h
	TitleIndex			DWORD			?	; 08h
	NameLength			DWORD			?	; 0Ch
	_Name				WORD			?	; 10h  WCHAR Variable length string  (original field name Name)
						WORD			?	; 12h  padding
KEY_BASIC_INFORMATION ENDS
PKEY_BASIC_INFORMATION typedef ptr KEY_BASIC_INFORMATION

KEY_NODE_INFORMATION STRUCT
	LastWriteTime		LARGE_INTEGER	<>	; 00h
	TitleIndex			DWORD			?	; 08h
	ClassOffset			DWORD			?	; 0Ch
	ClassLength			DWORD			?	; 10h
	NameLength			DWORD			?	; 14h
	_Name				WORD			?	; 18h  WCHAR Variable length string  (original field name Name)
						WORD			?	; 1Ah  padding
;          Class[1];           // Variable length string not declared
KEY_NODE_INFORMATION ENDS
PKEY_NODE_INFORMATION typedef ptr KEY_NODE_INFORMATION

KEY_FULL_INFORMATION STRUCT
	LastWriteTime		LARGE_INTEGER	<>	; 00h
	TitleIndex			DWORD			?	; 08h
	ClassOffset			DWORD			?	; 0Ch
	ClassLength			DWORD			?	; 10h
	SubKeys				DWORD			?	; 14h
	MaxNameLen			DWORD			?	; 18h
	MaxClassLen			DWORD			?	; 1Ch
	Values				DWORD			?	; 20h
	MaxValueNameLen		DWORD			?	; 24h
	MaxValueDataLen		DWORD			?	; 28h
	Class				WORD			?	; 2Ch  WCHAR Variable length
						WORD			?	; 2Eh  padding
KEY_FULL_INFORMATION ENDS
PKEY_FULL_INFORMATION typedef ptr KEY_FULL_INFORMATION

KEY_NAME_INFORMATION STRUCT
	NameLength		DWORD			?	; 00h
	_Name			WORD			?	; 04h  WCHAR Variable length string  (original field name Name)
					WORD			?	; 06h  padding
KEY_NAME_INFORMATION ENDS
PKEY_NAME_INFORMATION typedef ptr KEY_NAME_INFORMATION

;typedef enum _KEY_INFORMATION_CLASS {
    KeyBasicInformation			equ 0
    KeyNodeInformation			equ 1
    KeyFullInformation			equ 2
    KeyNameInformation			equ 3

KEY_WRITE_TIME_INFORMATION STRUCT
	LastWriteTime	LARGE_INTEGER	<>
KEY_WRITE_TIME_INFORMATION ENDS
PKEY_WRITE_TIME_INFORMATION typedef ptr KEY_WRITE_TIME_INFORMATION

;typedef enum _KEY_SET_INFORMATION_CLASS {
    KeyWriteTimeInformation			equ 0

; Value entry query structures

KEY_VALUE_BASIC_INFORMATION STRUCT	; sizeof = 10h
	TitleIndex	DWORD	?	; 00h
	_Type		DWORD	?	; 04h  (original field name Type)
	NameLength	DWORD	?	; 08h
	_Name		WORD	?	; 0Ch  WCHAR Variable size (original field name Name)
				WORD	?	; padding
KEY_VALUE_BASIC_INFORMATION ENDS
PKEY_VALUE_BASIC_INFORMATION typedef PTR KEY_VALUE_BASIC_INFORMATION

KEY_VALUE_FULL_INFORMATION STRUCT	; sizeof = 18h
	TitleIndex	DWORD	?	; 00h
	_Type		DWORD	?	; 04h  (original field name Type)
	DataOffset	DWORD	?	; 08h
	DataLength	DWORD	?	; 0Ch
	NameLength	DWORD	?	; 10h
	_Name		WORD	?	; 14h  WCHAR Variable size  (original field name Name)
				WORD	?	; 16h  padding
	;Data[1];            // Variable size data not declared
KEY_VALUE_FULL_INFORMATION ENDS
PKEY_VALUE_FULL_INFORMATION typedef PTR KEY_VALUE_FULL_INFORMATION

KEY_VALUE_PARTIAL_INFORMATION STRUCT	; sizeof = 10h
	TitleIndex	DWORD	?
	_Type		DWORD	?	; original field name Type
	DataLength	DWORD	?
	Data		BYTE	?	; Variable size
				db 3 dup(?)	; padding
KEY_VALUE_PARTIAL_INFORMATION ENDS
PKEY_VALUE_PARTIAL_INFORMATION typedef PTR KEY_VALUE_PARTIAL_INFORMATION

KEY_VALUE_PARTIAL_INFORMATION_ALIGN64 STRUCT	; sizeof = 0Ch
	_Type		DWORD	?	; (original field name Type)
	DataLength	DWORD	?
	Data		BYTE	?	; Variable size
				db 3 dup(?)	; padding
KEY_VALUE_PARTIAL_INFORMATION_ALIGN64 ENDS
PKEY_VALUE_PARTIAL_INFORMATION_ALIGN64 typedef PTR KEY_VALUE_PARTIAL_INFORMATION_ALIGN64

KEY_VALUE_ENTRY STRUCT	; sizeof = 10h
	ValueName	PVOID	?	; PTR UNICODE_STRING
	DataLength	DWORD	?
	DataOffset	DWORD	?
	_Type		DWORD	?	; (original field name Type)
KEY_VALUE_ENTRY ENDS
PKEY_VALUE_ENTRY typedef PTR KEY_VALUE_ENTRY

;typedef enum _KEY_VALUE_INFORMATION_CLASS {
KeyValueBasicInformation			equ 0
KeyValueFullInformation				equ 1
KeyValuePartialInformation			equ 2
KeyValueFullInformationAlign64		equ 3
KeyValuePartialInformationAlign64	equ 4

;OBJ_NAME_PATH_SEPARATOR		dw "\", "\"

; Object Manager Object Type Specific Access Rights.

OBJECT_TYPE_CREATE		equ 0001

OBJECT_TYPE_ALL_ACCESS	equ (STANDARD_RIGHTS_REQUIRED or 1)

; Object Manager Directory Specific Access Rights.

DIRECTORY_QUERY                 equ 0001
DIRECTORY_TRAVERSE              equ 0002
DIRECTORY_CREATE_OBJECT         equ 0004
DIRECTORY_CREATE_SUBDIRECTORY   equ 0008

DIRECTORY_ALL_ACCESS			equ (STANDARD_RIGHTS_REQUIRED or 0Fh)

; Object Manager Symbolic Link Specific Access Rights.

SYMBOLIC_LINK_QUERY				equ 0001

SYMBOLIC_LINK_ALL_ACCESS		equ (STANDARD_RIGHTS_REQUIRED or 1)

OBJECT_NAME_INFORMATION STRUCT
	_Name	UNICODE_STRING	<>	; (org name Name)
OBJECT_NAME_INFORMATION ENDS
POBJECT_NAME_INFORMATION typedef ptr OBJECT_NAME_INFORMATION

DUPLICATE_CLOSE_SOURCE      equ 00000001  ; winnt
DUPLICATE_SAME_ACCESS       equ 00000002  ; winnt
DUPLICATE_SAME_ATTRIBUTES   equ 00000004

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Section Information Structures.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _SECTION_INHERIT {
    ViewShare	equ 1
    ViewUnmap	equ 2

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Section Access Rights.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;; begin_winnt
SECTION_QUERY       equ 0001h
SECTION_MAP_WRITE   equ 0002h
SECTION_MAP_READ    equ 0004h
SECTION_MAP_EXECUTE equ 0008h
SECTION_EXTEND_SIZE equ 0010h

SECTION_ALL_ACCESS			equ (STANDARD_RIGHTS_REQUIRED or SECTION_QUERY or SECTION_MAP_WRITE or SECTION_MAP_READ or SECTION_MAP_EXECUTE or SECTION_EXTEND_SIZE)

;; end_winnt

SEGMENT_ALL_ACCESS			equ SECTION_ALL_ACCESS

PAGE_NOACCESS			equ 01h     ; winnt
PAGE_READONLY			equ 02h     ; winnt
PAGE_READWRITE			equ 04h     ; winnt
PAGE_WRITECOPY			equ 08h     ; winnt
PAGE_EXECUTE			equ 10h     ; winnt
PAGE_EXECUTE_READ		equ 20h     ; winnt
PAGE_EXECUTE_READWRITE	equ 40h     ; winnt
PAGE_EXECUTE_WRITECOPY	equ 80h     ; winnt
PAGE_GUARD				equ 100h     ; winnt
PAGE_NOCACHE			equ 200h     ; winnt
PAGE_WRITECOMBINE		equ 400h     ; winnt

MEM_COMMIT				equ 1000h
MEM_RESERVE				equ 2000h
MEM_DECOMMIT			equ 4000h
MEM_RELEASE				equ 8000h
MEM_FREE				equ 10000h
MEM_PRIVATE				equ 20000h
MEM_MAPPED				equ 40000h
MEM_RESET				equ 80000h
MEM_TOP_DOWN			equ 100000h
MEM_LARGE_PAGES			equ 20000000h
MEM_4MB_PAGES			equ 80000000h
SEC_RESERVE				equ 4000000h
PROCESS_DUP_HANDLE		equ 0040h  ; winnt
PROCESS_ALL_ACCESS		equ (STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 0FFFh)

MAXIMUM_PROCESSORS		equ 32

; Thread Specific Access Rights

THREAD_TERMINATE			equ 0001h  ; winnt
THREAD_SET_INFORMATION		equ 0020h  ; winnt

THREAD_ALL_ACCESS			equ (STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 3FFh)

; ClientId

CLIENT_ID STRUCT	; sizeof = 8 
	UniqueProcess	HANDLE	?
	UniqueThread	HANDLE	?
CLIENT_ID ENDS
PCLIENT_ID typedef PTR CLIENT_ID

; Thread Environment Block (and portable part of Thread Information Block)

; +
;  NT_TIB - Thread Information Block - Portable part.
;
;      This is the subsystem portable part of the Thread Information Block.
;      It appears as the first part of the TEB for all threads which have
;      a user mode component.
; -

NT_TIB STRUCT				; sizeof = 1Ch
	ExceptionList			PVOID	?	; PTR EXCEPTION_REGISTRATION_RECORD
	StackBase				PVOID	?	; 04h
	StackLimit				PVOID	?	; 08h
	SubSystemTib			PVOID	?	; 0Ch
	union
		FiberData			PVOID	?	; 10h
		Version				DWORD	?	; 10h
	ends
	ArbitraryUserPointer	PVOID	?	; 14h
	Self					PVOID	?	; 18h PTR NT_TIB
NT_TIB ENDS
PNT_TIB typedef PTR NT_TIB

; Process Information Classes

; typedef enum _PROCESSINFOCLASS
    ProcessBasicInformation			equ 0
    ProcessQuotaLimits				equ 1
    ProcessIoCounters				equ 2
    ProcessVmCounters				equ 3
    ProcessTimes					equ 4
    ProcessBasePriority				equ 5
    ProcessRaisePriority			equ 6
    ProcessDebugPort				equ 7
    ProcessExceptionPort			equ 8
    ProcessAccessToken				equ 9
    ProcessLdtInformation			equ 10
    ProcessLdtSize					equ 11
    ProcessDefaultHardErrorMode		equ 12
    ProcessIoPortHandlers			equ 13 ; Note: this is kernel mode only
    ProcessPooledUsageAndLimits		equ 14
    ProcessWorkingSetWatch			equ 15
    ProcessUserModeIOPL				equ 16
    ProcessEnableAlignmentFaultFixup equ 17
    ProcessPriorityClass			equ 18
    ProcessWx86Information			equ 19
    ProcessHandleCount				equ 20
    ProcessAffinityMask				equ 21
    ProcessPriorityBoost			equ 22
    ProcessDeviceMap				equ 23
    ProcessSessionInformation		equ 24
    ProcessForegroundInformation	equ 25
    ProcessWow64Information			equ 26
    MaxProcessInfoClass				equ 27

; Thread Information Classes

; typedef enum _THREADINFOCLASS
    ThreadBasicInformation			equ 0
    ThreadTimes						equ 1
    ThreadPriority					equ 2
    ThreadBasePriority				equ 3
    ThreadAffinityMask				equ 4
    ThreadImpersonationToken		equ 5
    ThreadDescriptorTableEntry		equ 6
    ThreadEnableAlignmentFaultFixup	equ 7
    ThreadEventPair_Reusable		equ 8
    ThreadQuerySetWin32StartAddress	equ 9
    ThreadZeroTlsCell				equ 10
    ThreadPerformanceCount			equ 11
    ThreadAmILastThread				equ 12
    ThreadIdealProcessor			equ 13
    ThreadPriorityBoost				equ 14
    ThreadSetTlsArrayAddress		equ 15
    ThreadIsIoPending				equ 16
    ThreadHideFromDebugger			equ 17
    MaxThreadInfoClass				equ 18

; Process Information Structures

; +
; PageFaultHistory Information
;  NtQueryInformationProcess using ProcessWorkingSetWatch
; -

PROCESS_WS_WATCH_INFORMATION STRUCT	; sizeof = 8
	FaultingPc	PVOID	?
	FaultingVa	PVOID	?
PROCESS_WS_WATCH_INFORMATION ENDS
PPROCESS_WS_WATCH_INFORMATION typedef PTR PROCESS_WS_WATCH_INFORMATION

; +
; Basic Process Information
;  NtQueryInformationProcess using ProcessBasicInfo
; -

PROCESS_BASIC_INFORMATION STRUCT	; sizeof = 18h
	ExitStatus				NTSTATUS	?
	PebBaseAddress			PVOID		? ; PPEB
	AffinityMask			DWORD		?
	BasePriority			DWORD		? ; KPRIORITY
	UniqueProcessId			DWORD		?
	InheritedFromUniqueProcessId	DWORD		?
PROCESS_BASIC_INFORMATION ENDS
PPROCESS_BASIC_INFORMATION typedef PTR PROCESS_BASIC_INFORMATION

; +
; Process Device Map information
;  NtQueryInformationProcess using ProcessDeviceMap
;  NtSetInformationProcess using ProcessDeviceMap
; -

PROCESS_DEVICEMAP_INFORMATION STRUCT
	union
		struct Set
			DirectoryHandle		DWORD	?	; HANDLE
		ends
		struct Query
			DriveMap			DWORD	?
			DriveType			BYTE 32 dup(?)
		ends
	ends
PROCESS_DEVICEMAP_INFORMATION ENDS
PPROCESS_DEVICEMAP_INFORMATION typedef ptr PROCESS_DEVICEMAP_INFORMATION

; +
; Multi-User Session specific Process Information
;  NtQueryInformationProcess using ProcessSessionInformation
; -

PROCESS_SESSION_INFORMATION STRUCT
	SessionId	DWORD	?
PROCESS_SESSION_INFORMATION ENDS
PPROCESS_SESSION_INFORMATION typedef ptr PROCESS_SESSION_INFORMATION

; +
; Process Quotas
;  NtQueryInformationProcess using ProcessQuotaLimits
;  NtQueryInformationProcess using ProcessPooledQuotaLimits
;  NtSetInformationProcess using ProcessQuotaLimits
; -

QUOTA_LIMITS  STRUCT
	PagedPoolLimit			DWORD			?	; SIZE_T
	NonPagedPoolLimit		DWORD			?	; SIZE_T
	MinimumWorkingSetSize	DWORD			?	; SIZE_T
	MaximumWorkingSetSize	DWORD			?	; SIZE_T
	PagefileLimit			DWORD			?	; SIZE_T
							DWORD			?
	TimeLimit				LARGE_INTEGER	<>
QUOTA_LIMITS ENDS
PQUOTA_LIMITS typedef ptr QUOTA_LIMITS

; +
; Process I/O Counters
;  NtQueryInformationProcess using ProcessIoCounters
; -

IO_COUNTERS STRUCT		; sizeof = 30h
	ReadOperationCount	LARGE_INTEGER	<>	; 00h
	WriteOperationCount	LARGE_INTEGER	<>	; 08h
	OtherOperationCount	LARGE_INTEGER	<>	; 10h
	ReadTransferCount	LARGE_INTEGER	<>	; 18h
	WriteTransferCount	LARGE_INTEGER	<>	; 20h
	OtherTransferCount	LARGE_INTEGER	<>	; 28h
IO_COUNTERS ENDS
PIO_COUNTERS typedef PTR IO_COUNTERS

; Process Virtual Memory Counters
;  NtQueryInformationProcess using ProcessVmCounters

VM_COUNTERS STRUCT
	PeakVirtualSize				DWORD	?	; SIZE_T
	VirtualSize					DWORD	?	; SIZE_T
	PageFaultCount				DWORD	?
	PeakWorkingSetSize			DWORD	?	; SIZE_T
	WorkingSetSize				DWORD	?	; SIZE_T
	QuotaPeakPagedPoolUsage		DWORD	?	; SIZE_T
	QuotaPagedPoolUsage			DWORD	?	; SIZE_T
	QuotaPeakNonPagedPoolUsage	DWORD	?	; SIZE_T
	QuotaNonPagedPoolUsage		DWORD	?	; SIZE_T
	PagefileUsage				DWORD	?	; SIZE_T
	PeakPagefileUsage			DWORD	?	; SIZE_T
VM_COUNTERS ENDS
PVM_COUNTERS typedef PTR VM_COUNTERS

; Process Pooled Quota Usage and Limits
;  NtQueryInformationProcess using ProcessPooledUsageAndLimits

POOLED_USAGE_AND_LIMITS  STRUCT
	PeakPagedPoolUsage		DWORD	?	; SIZE_T
	PagedPoolUsage			DWORD	?	; SIZE_T
	PagedPoolLimit			DWORD	?	; SIZE_T
	PeakNonPagedPoolUsage	DWORD	?	; SIZE_T
	NonPagedPoolUsage		DWORD	?	; SIZE_T
	NonPagedPoolLimit		DWORD	?	; SIZE_T
	PeakPagefileUsage		DWORD	?	; SIZE_T
	PagefileUsage			DWORD	?	; SIZE_T
	PagefileLimit			DWORD	?	; SIZE_T
POOLED_USAGE_AND_LIMITS ENDS
PPOOLED_USAGE_AND_LIMITS typedef PTR POOLED_USAGE_AND_LIMITS

; Process Security Context Information
;  NtSetInformationProcess using ProcessAccessToken
; PROCESS_SET_ACCESS_TOKEN access to the process is needed
; to use this info level.

PROCESS_ACCESS_TOKEN  STRUCT

	; Handle to Primary token to assign to the process.
	; TOKEN_ASSIGN_PRIMARY access to this token is needed.

	Token	HANDLE	?

	; Handle to the initial thread of the process.
	; A process's access token can only be changed if the process has
	; no threads or one thread.  If the process has no threads, this
	; field must be set to NULL.  Otherwise, it must contain a handle
	; open to the process's only thread.  THREAD_QUERY_INFORMATION access
	; is needed via this handle.

	Thread	HANDLE	?

PROCESS_ACCESS_TOKEN ENDS
PPROCESS_ACCESS_TOKEN typedef PTR PROCESS_ACCESS_TOKEN

; Process/Thread System and User Time
;  NtQueryInformationProcess using ProcessTimes
;  NtQueryInformationThread using ThreadTimes

KERNEL_USER_TIMES STRUCT
	CreateTime	LARGE_INTEGER	<>
	ExitTime	LARGE_INTEGER	<>
	KernelTime	LARGE_INTEGER	<>
	UserTime	LARGE_INTEGER	<>
KERNEL_USER_TIMES ENDS
PKERNEL_USER_TIMES typedef ptr KERNEL_USER_TIMES

NtCurrentProcess	equ -1
NtCurrentThread		equ -2

;typedef enum _SYSTEM_POWER_STATE {
    PowerSystemUnspecified	equ 0
    PowerSystemWorking		equ 1
    PowerSystemSleeping1	equ 2
    PowerSystemSleeping2	equ 3
    PowerSystemSleeping3	equ 4
    PowerSystemHibernate	equ 5
    PowerSystemShutdown		equ 6
    PowerSystemMaximum		equ 7

;typedef enum _POWER_ACTION {
    PowerActionNone				equ 0
    PowerActionReserved			equ 1
    PowerActionSleep			equ 2
    PowerActionHibernate		equ 3
    PowerActionShutdown			equ 4
    PowerActionShutdownReset	equ 5
    PowerActionShutdownOff		equ 6
    PowerActionWarmEject		equ 7

;typedef enum _DEVICE_POWER_STATE {
    PowerDeviceUnspecified	equ 0
    PowerDeviceD0			equ 1
    PowerDeviceD1			equ 2
    PowerDeviceD2			equ 3
    PowerDeviceD3			equ 4
    PowerDeviceMaximum		equ 5

POWER_STATE UNION
	SystemState		DWORD	?	; SYSTEM_POWER_STATE enum
	DeviceState		DWORD	?	; DEVICE_POWER_STATE enum
POWER_STATE ENDS
PPOWER_STATE typedef ptr POWER_STATE

;typedef enum _POWER_STATE_TYPE {
    SystemPowerState	equ 0
    DevicePowerState	equ 1

; Generic power related IOCTLs

IOCTL_QUERY_DEVICE_POWER_STATE	equ CTL_CODE(FILE_DEVICE_BATTERY, 0, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_SET_DEVICE_WAKE			equ CTL_CODE(FILE_DEVICE_BATTERY, 1, METHOD_BUFFERED, FILE_WRITE_ACCESS)
IOCTL_CANCEL_DEVICE_WAKE		equ CTL_CODE(FILE_DEVICE_BATTERY, 2, METHOD_BUFFERED, FILE_WRITE_ACCESS)


; Defines for W32 interfaces

ES_SYSTEM_REQUIRED	equ 00000001
ES_DISPLAY_REQUIRED	equ 00000002
ES_USER_PRESENT		equ 00000004
ES_CONTINUOUS		equ 80000000h

EXECUTION_STATE typedef DWORD

;typedef enum _LATENCY_TIME {
    LT_DONT_CARE 		equ 0
    LT_LOWEST_LATENCY	equ 1

;typedef enum {
    SystemPowerPolicyAc			equ 0
    SystemPowerPolicyDc			equ 1
    VerifySystemPolicyAc		equ 2
    VerifySystemPolicyDc		equ 3
    SystemPowerCapabilities		equ 4
    SystemBatteryState			equ 5
    SystemPowerStateHandler		equ 6
    ProcessorStateHandler		equ 7
    SystemPowerPolicyCurrent	equ 8
    AdministratorPowerPolicy	equ 9
    SystemReserveHiberFile		equ 10
    ProcessorInformation		equ 11
    SystemPowerInformation		equ 12

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Interrupt Request Level definitions
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PASSIVE_LEVEL		equ 0			; Passive release level
LOW_LEVEL			equ 0			; Lowest interrupt level
APC_LEVEL			equ 1			; APC interrupt level
DISPATCH_LEVEL		equ 2			; Dispatcher level

PROFILE_LEVEL		equ 27			; timer used for profiling.
CLOCK1_LEVEL		equ 28			; Interval clock 1 level - Not used on x86
CLOCK2_LEVEL		equ 28			; Interval clock 2 level
IPI_LEVEL			equ 29			; Interprocessor interrupt level
POWER_LEVEL			equ 30			; Power failure level
HIGH_LEVEL			equ 31			; Highest interrupt level
SYNCH_LEVEL			equ (IPI_LEVEL-1)	; synchronization level

; Processor Control Region Structure Definition

PCR_MINOR_VERSION	equ 1
PCR_MAJOR_VERSION	equ 1

; base address 0FFDFF000h

KPCR STRUCT			; sizeof = 54h

	; Start of the architecturally defined section of the PCR. This section
	; may be directly addressed by vendor/platform specific HAL code and will
	; not change from version to version of NT.

	NtTib			NT_TIB	<>
	SelfPcr			PVOID		? 	; 1Ch PTR KPCR flat address of this PCR
	Prcb			PVOID		? 	; 20h pointer to Prcb
	Irql			BYTE		? 	; 24h KIRQL
					db	3 dup(?)	; padding
	IRR				DWORD		? 	; 28h
	IrrActive		DWORD		? 	; 2Ch
	IDR				DWORD		? 	; 30h
	Reserved2		DWORD		?

	IDT				PVOID		? 	; 38h PTR KIDTENTRY
	GDT				PVOID		? 	; 3Ch PTR KGDTENTRY
	TSS				PVOID		? 	; 40h PTR KTSS
	MajorVersion	WORD		? 	; 44h
	MinorVersion	WORD		? 	; 46h
	SetMember		KAFFINITY	? 	; 48h
	StallScaleFactor DWORD		? 	; 4Ch
	DebugActive		BYTE		? 	; 50h
	Number			BYTE		? 	; 51h
					db 	2 dup(?)	; 052 padding
KPCR ENDS
PKPCR typedef PTR KPCR

; The non-volatile 387 state

KFLOATING_SAVE STRUCT
	ControlWord		DWORD	?
	StatusWord		DWORD	?
	ErrorOffset		DWORD	?
	ErrorSelector	DWORD	?
	DataOffset		DWORD	?	; Not used in wdm
	DataSelector	DWORD	?
	Cr0NpxState		DWORD	?
	Spare1			DWORD	?	; Not used in wdm
KFLOATING_SAVE ENDS
PKFLOATING_SAVE typedef ptr KFLOATING_SAVE

; i386 Specific portions of mm component

; Define the page size for the Intel 386 as 4096 (0x1000).

PAGE_SIZE	equ 1000h

; Define the number of trailing zeroes in a page aligned virtual address.
; This is used as the shift count when shifting virtual addresses to
; virtual page numbers.

PAGE_SHIFT	equ 12

; Define the number of bits to shift to right justify the Page Directory Index
; field of a PTE.

PDI_SHIFT_X86		equ 22
PDI_SHIFT_X86PAE	equ 21
PPI_SHIFT			equ 30	; X86PAE

; Define the number of bits to shift to right justify the Page Table Index
; field of a PTE.

PTI_SHIFT	equ 12

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; The lowest user address reserves the low 64k.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MM_LOWEST_USER_ADDRESS	equ 10000h

; The lowest address for system space.

MM_LOWEST_SYSTEM_ADDRESS equ 0C0C00000h

KI_USER_SHARED_DATA	equ 0ffdf0000h
SharedUserData		equ KI_USER_SHARED_DATA

; +
; Result type definition for i386.  (Machine specific enumerate type
; which is return type for portable exinterlockedincrement/decrement
; procedures.)  In general, you should use the enumerated type defined
; in ex.h instead of directly referencing these constants.
; -

; Flags loaded into AH by LAHF instruction

EFLAG_SIGN		equ 8000h
EFLAG_ZERO		equ 4000h
EFLAG_SELECT	equ EFLAG_SIGN or EFLAG_ZERO

; Define system time structure.

KSYSTEM_TIME STRUCT		; sizeof = 0Ch
	LowPart		DWORD		?	; 0000h
	High1Time	SDWORD		?	; 0004h
	High2Time	SDWORD		?	; 0008h
KSYSTEM_TIME ENDS
PKSYSTEM_TIME typedef PTR KSYSTEM_TIME

; Size of kernel mode stack.

KERNEL_STACK_SIZE			equ 12288

; Define size of large kernel mode stack for callbacks.

KERNEL_LARGE_STACK_SIZE		equ 61440

; Define number of pages to initialize in a large kernel stack.

KERNEL_LARGE_STACK_COMMIT	equ 12288

;  Define the size of the 80387 save area, which is in the context frame.

SIZE_OF_80387_REGISTERS		equ 80

; The following flags control the contents of the CONTEXT structure.

CONTEXT_i386				equ 00010000h    ; this assumes that i386 and
CONTEXT_i486				equ 00010000h    ; i486 have identical context records

CONTEXT_CONTROL				equ (CONTEXT_i386 or 00000001h) ; SS:SP, CS:IP, FLAGS, BP
CONTEXT_INTEGER				equ (CONTEXT_i386 or 00000002h) ; AX, BX, CX, DX, SI, DI
CONTEXT_SEGMENTS			equ (CONTEXT_i386 or 00000004h) ; DS, ES, FS, GS
CONTEXT_FLOATING_POINT		equ (CONTEXT_i386 or 00000008h) ; 387 state
CONTEXT_DEBUG_REGISTERS		equ (CONTEXT_i386 or 00000010h) ; DB 0-3,6,7
CONTEXT_EXTENDED_REGISTERS	equ (CONTEXT_i386 or 00000020h) ; cpu specific extensions

CONTEXT_FULL				equ (CONTEXT_CONTROL or CONTEXT_INTEGER or CONTEXT_SEGMENTS)

MAXIMUM_SUPPORTED_EXTENSION	equ 512


FLOATING_SAVE_AREA STRUCT		; sizeof = 070h
	ControlWord		DWORD		?	; 0000h
	StatusWord		DWORD		?	; 0004h
	TagWord			DWORD		?	; 0008h
	ErrorOffset		DWORD		?	; 000Ch
	ErrorSelector	DWORD		?	; 0010h
	DataOffset		DWORD		?	; 0014h
	DataSelector	DWORD		?	; 0018h
	RegisterArea	BYTE		SIZE_OF_80387_REGISTERS dup(?)	; 001Ch
	Cr0NpxState		DWORD		?	; 006Ch
FLOATING_SAVE_AREA ENDS
PFLOATING_SAVE_AREA typedef PTR FLOATING_SAVE_AREA


; +
; Context Frame
;
;  This frame has a several purposes: 1) it is used as an argument to
;  NtContinue, 2) is is used to constuct a call frame for APC delivery,
;  and 3) it is used in the user level thread creation routines.
;
;  The layout of the record conforms to a standard call frame.
; -

CONTEXT STRUCT			; sizeof = 02CCh

	; The flags values within this flag control the contents of
	; a CONTEXT record.
	;
	; If the context record is used as an input parameter, then
	; for each portion of the context record controlled by a flag
	; whose value is set, it is assumed that that portion of the
	; context record contains valid context. If the context record
	; is being used to modify a threads context, then only that
	; portion of the threads context will be modified.
	;
	; If the context record is used as an IN OUT parameter to capture
	; the context of a thread, then only those portions of the thread's
	; context corresponding to set flags will be returned.
	;
	; The context record is never used as an OUT only parameter.

	ContextFlags		DWORD		?	; 0000h

	; This section is specified/returned if CONTEXT_DEBUG_REGISTERS is
	; set in ContextFlags.  Note that CONTEXT_DEBUG_REGISTERS is NOT
	; included in CONTEXT_FULL.

	regDr0				DWORD		?	; 0004h
	regDr1				DWORD		?	; 0008h
	regDr2				DWORD		?	; 000Ch
	regDr3				DWORD		?	; 0010h
	regDr6				DWORD		?	; 0014h
	regDr7				DWORD		?	; 0018h

	; This section is specified/returned if the
	; ContextFlags word contians the flag CONTEXT_FLOATING_POINT.

	FloatSave			FLOATING_SAVE_AREA <>	; 001Ch

	; This section is specified/returned if the
	; ContextFlags word contians the flag CONTEXT_SEGMENTS.

	regSegGs			DWORD		?	; 008Ch
	regSegFs			DWORD		?	; 0090h
	regSegEs			DWORD		?	; 0094h
	regSegDs			DWORD		?	; 0098h

	; This section is specified/returned if the
	; ContextFlags word contians the flag CONTEXT_INTEGER.

	regEdi				DWORD		?	; 009Ch
	regEsi				DWORD		?	; 00A0h
	regEbx				DWORD		?	; 00A4h
	regEdx				DWORD		?	; 00A8h
	regEcx				DWORD		?	; 00ACh
	regEax				DWORD		?	; 00B0h

	; This section is specified/returned if the
	; ContextFlags word contians the flag CONTEXT_CONTROL.

	regEbp				DWORD		?	; 00B4h
	regEip				DWORD		?	; 00B8h
	regSegCs			DWORD		?	; 00BCh  MUST BE SANITIZED
	regEFlags			DWORD		?	; 00C0h  MUST BE SANITIZED
	regEsp				DWORD		?	; 00C4h
	regSegSs			DWORD		?	; 00C8h


	; This section is specified/returned if the ContextFlags word
	; contains the flag CONTEXT_EXTENDED_REGISTERS.
	; The format and contexts are processor specific

	ExtendedRegisters	BYTE		MAXIMUM_SUPPORTED_EXTENSION dup(?)	; 00CCh

CONTEXT ENDS

;/ Event Specific Access Rights.

EVENT_QUERY_STATE       equ 0001
EVENT_MODIFY_STATE      equ 0002  ; winnt
EVENT_ALL_ACCESS		equ (STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 3) ; winnt

; Semaphore Specific Access Rights.

SEMAPHORE_QUERY_STATE       equ 0001
SEMAPHORE_MODIFY_STATE      equ 0002  ; winnt

SEMAPHORE_ALL_ACCESS		equ (STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 3) ; winnt

; Defined processor features

PF_FLOATING_POINT_PRECISION_ERRATA  equ 0
PF_FLOATING_POINT_EMULATED          equ 1
PF_COMPARE_EXCHANGE_DOUBLE          equ 2
PF_MMX_INSTRUCTIONS_AVAILABLE       equ 3
PF_PPC_MOVEMEM_64BIT_OK             equ 4
PF_ALPHA_BYTE_INSTRUCTIONS          equ 5
PF_XMMI_INSTRUCTIONS_AVAILABLE      equ 6
PF_3DNOW_INSTRUCTIONS_AVAILABLE     equ 7
PF_RDTSC_INSTRUCTION_AVAILABLE      equ 8
PF_PAE_ENABLED                      equ 9

;typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE {
	StandardDesign	equ 0	; None == 0 == standard design
	NEC98x86		equ 1	; NEC PC98xx series on X86
	EndAlternatives	equ 2	; past end of known alternatives

PROCESSOR_FEATURE_MAX	equ 64

;
; Define data shared between kernel and user mode.
;
; N.B. User mode has read only access to this data
;

KUSER_SHARED_DATA STRUCT										; sizeof = 02D8h

	; Current low 32-bit of tick count and tick count multiplier.
	;
	; N.B. The tick count is updated each time the clock ticks.

	TickCountLow					DWORD			?			; 0000h
	TickCountMultiplier				DWORD			?			; 0004h

	; Current 64-bit interrupt time in 100ns units.

	InterruptTime					KSYSTEM_TIME 	<>			; 0008h

	; Current 64-bit system time in 100ns units.

	SystemTime						KSYSTEM_TIME 	<>			; 0014h

	; Current 64-bit time zone bias.

	TimeZoneBias					KSYSTEM_TIME 	<>			; 0020h

	; Support image magic number range for the host system.
	;
	; N.B. This is an inclusive range.

	ImageNumberLow					WORD			?			; 002Ch
	ImageNumberHigh					WORD			?			; 002Eh

	; Copy of system root in Unicode

	NtSystemRoot					WORD		260 dup(?)		; 0030h

	; Maximum stack trace depth if tracing enabled.

	MaxStackTraceDepth				DWORD			?			; 0238h

	; Crypto Exponent

	CryptoExponent					DWORD			?			; 023Ch

	; TimeZoneId

	TimeZoneId						DWORD			?			; 0240h

	Reserved2						DWORD		8 dup(?)		; 0244h

	; product type

	NtProductType					SDWORD			?			; 0264h NT_PRODUCT_TYPE
	ProductTypeIsValid				BOOLEAN			?			; 0268h
									db 3 dup(?)					; padding

	; NT Version. Note that each process sees a version from its PEB, but
	; if the process is running with an altered view of the system version,
	; the following two fields are used to correctly identify the version

	NtMajorVersion					DWORD			?			; 026Ch
	NtMinorVersion					DWORD			?			; 0270h

	; Processor Feature Bits

	ProcessorFeatures				BOOLEAN	PROCESSOR_FEATURE_MAX dup(?)	; 0274h

	; Reserved fields - do not use

	Reserved1						DWORD			?			; 02B4h
	Reserved3						DWORD			?			; 02B8h

	; Time slippage while in debugger

	TimeSlip						DWORD			?			; 02BCh

	; Alternative system architecture.  Example: NEC PC98xx on x86

	AlternativeArchitecture			SDWORD			?			; 02C0h ALTERNATIVE_ARCHITECTURE_TYPE
									db 4 dup(?)					; padding


	; If the system is an evaluation unit, the following field contains the
	; date and time that the evaluation unit expires. A value of 0 indicates
	; that there is no expiration. A non-zero value is the UTC absolute time
	; that the system expires.

	SystemExpirationDate			LARGE_INTEGER 	<>			; 02C8h

	; Suite Support

	SuiteMask						DWORD			?			; 02D0h

	; TRUE if a kernel debugger is connected/enabled

	KdDebuggerEnabled				BOOLEAN			?			; 02D4h
									db 3 bup(?)					; padding

KUSER_SHARED_DATA ENDS
PKUSER_SHARED_DATA typedef ptr KUSER_SHARED_DATA


; Predefined Value Types.

REG_NONE						equ 0 ; No value type
REG_SZ							equ 1 ; Unicode nul terminated string
REG_EXPAND_SZ					equ 2 ; Unicode nul terminated string (with environment variable references)
REG_BINARY						equ 3 ; Free form binary
REG_DWORD						equ 4 ; 32-bit number
REG_DWORD_LITTLE_ENDIAN			equ 4 ; 32-bit number (same as REG_DWORD)
REG_DWORD_BIG_ENDIAN			equ 5 ; 32-bit number
REG_LINK						equ 6 ; Symbolic Link (unicode)
REG_MULTI_SZ					equ 7 ; Multiple Unicode strings
REG_RESOURCE_LIST				equ 8 ; Resource list in the resource map
REG_FULL_RESOURCE_DESCRIPTOR	equ 9 ; Resource list in the hardware description
REG_RESOURCE_REQUIREMENTS_LIST	equ 10
REG_QWORD						equ 11 ; 64-bit number
REG_QWORD_LITTLE_ENDIAN			equ 11 ; 64-bit number (same as REG_QWORD)

; Service Types (Bit Mask)

SERVICE_KERNEL_DRIVER          equ 00000001
SERVICE_FILE_SYSTEM_DRIVER     equ 00000002
SERVICE_ADAPTER                equ 00000004
SERVICE_RECOGNIZER_DRIVER      equ 00000008

SERVICE_DRIVER                 equ (SERVICE_KERNEL_DRIVER or SERVICE_FILE_SYSTEM_DRIVER or SERVICE_RECOGNIZER_DRIVER)

SERVICE_WIN32_OWN_PROCESS      equ 00000010h
SERVICE_WIN32_SHARE_PROCESS    equ 00000020h
SERVICE_WIN32                  equ (SERVICE_WIN32_OWN_PROCESS or SERVICE_WIN32_SHARE_PROCESS)

SERVICE_INTERACTIVE_PROCESS    equ 00000100h

SERVICE_TYPE_ALL               equ (SERVICE_WIN32 or SERVICE_ADAPTER or SERVICE_DRIVER or SERVICE_INTERACTIVE_PROCESS)

; Start Type

SERVICE_BOOT_START             equ 00000000
SERVICE_SYSTEM_START           equ 00000001
SERVICE_AUTO_START             equ 00000002
SERVICE_DEMAND_START           equ 00000003
SERVICE_DISABLED               equ 00000004

; Error control type

SERVICE_ERROR_IGNORE           equ 00000000
SERVICE_ERROR_NORMAL           equ 00000001
SERVICE_ERROR_SEVERE           equ 00000002
SERVICE_ERROR_CRITICAL         equ 00000003

; Define the registry driver node enumerations

;typedef enum _CM_SERVICE_NODE_TYPE {
    DriverType               equ SERVICE_KERNEL_DRIVER
    FileSystemType           equ SERVICE_FILE_SYSTEM_DRIVER
    Win32ServiceOwnProcess   equ SERVICE_WIN32_OWN_PROCESS
    Win32ServiceShareProcess equ SERVICE_WIN32_SHARE_PROCESS
    AdapterType              equ SERVICE_ADAPTER
    RecognizerType           equ SERVICE_RECOGNIZER_DRIVER

;typedef enum _CM_SERVICE_LOAD_TYPE {
    BootLoad    equ SERVICE_BOOT_START
    SystemLoad  equ SERVICE_SYSTEM_START
    AutoLoad    equ SERVICE_AUTO_START
    DemandLoad  equ SERVICE_DEMAND_START
    DisableLoad equ SERVICE_DISABLED

;typedef enum _CM_ERROR_CONTROL_TYPE {
    IgnoreError   equ SERVICE_ERROR_IGNORE
    NormalError   equ SERVICE_ERROR_NORMAL
    SevereError   equ SERVICE_ERROR_SEVERE
    CriticalError equ SERVICE_ERROR_CRITICAL

; Resource List definitions

; +
; Defines the Type in the RESOURCE_DESCRIPTOR
;
; NOTE:  For all CM_RESOURCE_TYPE values, there must be a
; corresponding ResType value in the 32-bit ConfigMgr headerfile
; (cfgmgr32.h).  Values in the range [0x6,0x80) use the same values
; as their ConfigMgr counterparts.  CM_RESOURCE_TYPE values with
; the high bit set (i.e., in the range [0x80,0xFF]), are
; non-arbitrated resources.  These correspond to the same values
; in cfgmgr32.h that have their high bit set (however, since
; cfgmgr32.h uses 16 bits for ResType values, these values are in
; the range [0x8000,0x807F).  Note that ConfigMgr ResType values
; cannot be in the range [0x8080,0xFFFF), because they would not
; be able to map into CM_RESOURCE_TYPE values.  (0xFFFF itself is
; a special value, because it maps to CmResourceTypeDeviceSpecific.)
; -

CM_RESOURCE_TYPE typedef SDWORD

; CmResourceTypeNull is reserved

CmResourceTypeNull                equ 0   ; ResType_All or ResType_None (0x0000)
CmResourceTypePort                equ 1   ; ResType_IO (0x0002)
CmResourceTypeInterrupt           equ 2   ; ResType_IRQ (0x0004)
CmResourceTypeMemory              equ 3   ; ResType_Mem (0x0001)
CmResourceTypeDma                 equ 4   ; ResType_DMA (0x0003)
CmResourceTypeDeviceSpecific      equ 5   ; ResType_ClassSpecific (0xFFFF)
CmResourceTypeBusNumber           equ 6   ; ResType_BusNumber (0x0006)

CmResourceTypeMaximum             equ 7
CmResourceTypeAssignedResource    equ 8   ; BUGBUG--remove
CmResourceTypeSubAllocateFrom     equ 9   ; BUGBUG--remove

CmResourceTypeNonArbitrated     equ 128   ; Not arbitrated if 0x80 bit set
CmResourceTypeConfigData        equ 128   ; ResType_Reserved (0x8000)
CmResourceTypeDevicePrivate     equ 129   ; ResType_DevicePrivate (0x8001)
CmResourceTypePcCardConfig      equ 130   ; ResType_PcCardConfig (0x8002)
CmResourceTypeMfCardConfig      equ 131   ; ResType_MfCardConfig (0x8003)

; Defines the ShareDisposition in the RESOURCE_DESCRIPTOR

;typedef enum _CM_SHARE_DISPOSITION {
    CmResourceShareUndetermined		equ 0	; Reserved
    CmResourceShareDeviceExclusive	equ 1
    CmResourceShareDriverExclusive	equ 2
    CmResourceShareShared			equ 3

; +
; Define the bit masks for Flags common for all CM_RESOURCE_TYPE
;
; BUGBUG--remove the following 3 flags...
; -

CM_RESOURCE_COMMON_COMPUTE_LENGTH_FROM_DEPENDENTS   equ 8000h
CM_RESOURCE_COMMON_NOT_REASSIGNED                   equ 4000h
CM_RESOURCE_COMMON_SUBSTRACTIVE                     equ 2000h

; Define the bit masks for Flags when type is CmResourceTypeInterrupt

CM_RESOURCE_INTERRUPT_LEVEL_SENSITIVE equ 0
CM_RESOURCE_INTERRUPT_LATCHED         equ 1

; Define the bit masks for Flags when type is CmResourceTypeMemory

CM_RESOURCE_MEMORY_READ_WRITE       equ 0000
CM_RESOURCE_MEMORY_READ_ONLY        equ 0001
CM_RESOURCE_MEMORY_WRITE_ONLY       equ 0002
CM_RESOURCE_MEMORY_PREFETCHABLE     equ 0004

CM_RESOURCE_MEMORY_COMBINEDWRITE    equ 0008
CM_RESOURCE_MEMORY_24               equ 0010h
CM_RESOURCE_MEMORY_CACHEABLE        equ 0020h

; Define the bit masks for Flags when type is CmResourceTypePort

CM_RESOURCE_PORT_MEMORY                             equ 0000
CM_RESOURCE_PORT_IO                                 equ 0001

CM_RESOURCE_PORT_FORWARD_FIRST_256_OF_EACH_1024     equ 0002  ; BUGBUG--remove

CM_RESOURCE_PORT_10_BIT_DECODE                      equ 0004
CM_RESOURCE_PORT_12_BIT_DECODE                      equ 0008
CM_RESOURCE_PORT_16_BIT_DECODE                      equ 0010h
CM_RESOURCE_PORT_POSITIVE_DECODE                    equ 0020h
CM_RESOURCE_PORT_PASSIVE_DECODE                     equ 0040h
CM_RESOURCE_PORT_WINDOW_DECODE                      equ 0080h

; Define the bit masks for Flags when type is CmResourceTypeDma

CM_RESOURCE_DMA_8                   equ 0000
CM_RESOURCE_DMA_16                  equ 0001
CM_RESOURCE_DMA_32                  equ 0002
CM_RESOURCE_DMA_8_AND_16            equ 0004
CM_RESOURCE_DMA_BUS_MASTER          equ 0008
CM_RESOURCE_DMA_TYPE_A              equ 0010h
CM_RESOURCE_DMA_TYPE_B              equ 0020h
CM_RESOURCE_DMA_TYPE_F              equ 0040h

; Define the bit masks for Flags when type is CmResourceTypeBusNumber

CM_RESOURCE_BUSNUMBER_SUBALLOCATE_FIRST_VALUE   equ 0001  ; BUGBUG--remove

; Define the bit masks for Flags when type is CmResourceTypeSubAllocateFrom

CM_RESOURCE_SUBALLOCATEFROM_FIXED_TRANSLATION   equ 0001  ; BUGBUG--remove
CM_RESOURCE_SUBALLOCATEFROM_WIRED_TRANSLATION   equ 0002  ; BUGBUG--remove

; +
; This structure defines one type of resource used by a driver.
;
; There can only be *1* DeviceSpecificData block. It must be located at
; the end of all resource descriptors in a full descriptor block.
; -

; Make sure alignment is made properly by compiler; otherwise move
; flags back to the top of the structure (common to all members of the
; union).

;;#include "pshpack4.h"

CM_PARTIAL_RESOURCE_DESCRIPTOR STRUCT		; sizeof = 010h
	_Type				BYTE		?	; 0000h
	ShareDisposition	BYTE		?	; 0001h
	Flags				WORD		?	; 0002h
	union u								; 0004h

		; Range of resources, inclusive.  These are physical, bus relative.
		; It is known that Port and Memory below have the exact same layout
		; as Generic.

		struct Generic
			_Start		PHYSICAL_ADDRESS	<>
			_Length		DWORD				?
		ends	; Generic

		; Range of port numbers, inclusive. These are physical, bus
		; relative. The value should be the same as the one passed to
		; HalTranslateBusAddress()./

        struct Port
			_Start		PHYSICAL_ADDRESS	<>
			_Length		DWORD				?
		ends	; Port

		; IRQL and vector. Should be same values as were passed to
		; HalGetInterruptVector().

        struct Interrupt
			Level		DWORD				?
			Vector		DWORD				?
			Affinity	DWORD				?
		ends	; Interrupt

		; Range of memory addresses, inclusive. These are physical, bus
		; relative. The value should be the same as the one passed to
		; HalTranslateBusAddress().

        struct Memory
			_Start		PHYSICAL_ADDRESS	<>	; 64 bit physical addresses.
			_Length		DWORD				?
		ends	; Memory

		; Physical DMA channel.

        struct Dma
			Channel		DWORD				?
			Port		DWORD				?
			Reserved1	DWORD				?
		ends	; Dma

		; Device driver private data, usually used to help it figure
		; what the resource assignments decisions that were made.

        struct DevicePrivate
			Data		DWORD 3 dup(?)
		ends	; DevicePrivate

		; Bus Number information.

        struct BusNumber
			_Start		DWORD				?
			_Length		DWORD				?
			Reserved	DWORD				?
		ends	; BusNumber

		; Device Specific information defined by the driver.
		; The DataSize field indicates the size of the data in bytes. The
		; data is located immediately after the DeviceSpecificData field in
		; the structure.

        struct DeviceSpecificData
			DataSize	DWORD				?
			Reserved1	DWORD				?
			Reserved2	DWORD				?
		ends	; DeviceSpecificData
	ends	; u
CM_PARTIAL_RESOURCE_DESCRIPTOR ENDS
PCM_PARTIAL_RESOURCE_DESCRIPTOR typedef ptr CM_PARTIAL_RESOURCE_DESCRIPTOR
;#include "poppack.h"

; +
; A Partial Resource List is what can be found in the ARC firmware
; or will be generated by ntdetect.com.
; The configuration manager will transform this structure into a Full
; resource descriptor when it is about to store it in the regsitry.
;
; Note: There must a be a convention to the order of fields of same type,
; (defined on a device by device basis) so that the fields can make sense
; to a driver (i.e. when multiple memory ranges are necessary).
; -

CM_PARTIAL_RESOURCE_LIST STRUCT										; sizeof = 018h
	Version				WORD									?	; 0000h
	Revision			WORD									?	; 0002h
	Count				DWORD									?	; 0004h
	PartialDescriptors	CM_PARTIAL_RESOURCE_DESCRIPTOR	1 dup(<?>)	; 0008h
CM_PARTIAL_RESOURCE_LIST ENDS
PCM_PARTIAL_RESOURCE_LIST typedef ptr CM_PARTIAL_RESOURCE_LIST

; +
; A Full Resource Descriptor is what can be found in the registry.
; This is what will be returned to a driver when it queries the registry
; to get device information; it will be stored under a key in the hardware
; description tree.
;
; end_wdm
; Note: The BusNumber and Type are redundant information, but we will keep
; it since it allows the driver _not_ to append it when it is creating
; a resource list which could possibly span multiple buses.
;
; begin_wdm
; Note: There must a be a convention to the order of fields of same type,
; (defined on a device by device basis) so that the fields can make sense
; to a driver (i.e. when multiple memory ranges are necessary).
; -

CM_FULL_RESOURCE_DESCRIPTOR STRUCT		; sizeof = 020h
	InterfaceType		SDWORD		?	; 0000h INTERFACE_TYPE unused for WDM
	BusNumber			DWORD		?	; 0004h unused for WDM
	PartialResourceList	CM_PARTIAL_RESOURCE_LIST <>	; 0008h
CM_FULL_RESOURCE_DESCRIPTOR ENDS
PCM_FULL_RESOURCE_DESCRIPTOR typedef ptr CM_FULL_RESOURCE_DESCRIPTOR

; The Resource list is what will be stored by the drivers into the
; resource map via the IO API.

CM_RESOURCE_LIST STRUCT								; sizeof = 024h
	Count	DWORD								?	; 0000h
	List	CM_FULL_RESOURCE_DESCRIPTOR	1 dup(<?>)	; 0004h
CM_RESOURCE_LIST ENDS
PCM_RESOURCE_LIST typedef ptr CM_RESOURCE_LIST

; Define the structures used to interpret configuration data of
; \\Registry\machine\hardware\description tree.
; Basically, these structures are used to interpret component
; sepcific data.

; Define DEVICE_FLAGS
; All fields prefixed with 'df' because of record fields should be unique :-(

DEVICE_FLAGS RECORD \
	dfReserved:25,		; bits 7-31
	dfOutput:1,			; bit 6
	dfInput:1,			; bit 5
	dfConsoleOut:1,		; bit 4
	dfConsoleIn:1,		; bit 3
	dfRemovable:1,		; bit 2
	dfReadOnly:1,		; bit 1
	dfFailed:1			; bit 0

; Define Component Information structure

CM_COMPONENT_INFORMATION STRUCT
	Flags			DEVICE_FLAGS	<>
	Version			DWORD			?
	Key				DWORD			?
	AffinityMask	DWORD			?
CM_COMPONENT_INFORMATION ENDS
PCM_COMPONENT_INFORMATION typedef ptr CM_COMPONENT_INFORMATION

; The following structures are used to interpret x86
; DeviceSpecificData of CM_PARTIAL_RESOURCE_DESCRIPTOR.
; (Most of the structures are defined by BIOS.  They are
; not aligned on word (or dword) boundary.

; Define the Rom Block structure

CM_ROM_BLOCK STRUCT
	Address		DWORD	?
	_Size		DWORD	?
CM_ROM_BLOCK ENDS
PCM_ROM_BLOCK typedef ptr CM_ROM_BLOCK

; Exception flag definitions.

EXCEPTION_NONCONTINUABLE 			equ 1	; Noncontinuable exception

; Define maximum number of exception parameters.

EXCEPTION_MAXIMUM_PARAMETERS		equ 15	; maximum number of exception parameters

; Exception record definition.

EXCEPTION_RECORD STRUCT		; sizeof = 050h
	ExceptionCode			SDWORD		?	; 0000h
	ExceptionFlags			DWORD		?	; 0004h
	ExceptionRecord			PVOID		?	; 0008h PTR EXCEPTION_RECORD
	ExceptionAddress		PVOID		?	; 000Ch
	NumberParameters		DWORD		?	; 0010h
	ExceptionInformation	DWORD		EXCEPTION_MAXIMUM_PARAMETERS dup(?)	; 0014h
EXCEPTION_RECORD ENDS
PEXCEPTION_RECORD typedef PTR EXCEPTION_RECORD

EXCEPTION_RECORD32 STRUCT
	ExceptionCode			NTSTATUS	?
	ExceptionFlags			DWORD	?
	ExceptionRecord			DWORD	?
	ExceptionAddress		DWORD	?
	NumberParameters		DWORD	?
	ExceptionInformation	DWORD EXCEPTION_MAXIMUM_PARAMETERS dup(?)
EXCEPTION_RECORD32 ENDS
PEXCEPTION_RECORD32 typedef PTR EXCEPTION_RECORD32

; Typedef for pointer returned by exception_info()

EXCEPTION_POINTERS STRUCT
	ExceptionRecord	PVOID	?	; PTR EXCEPTION_RECORD
	ContextRecord	PVOID	?	; PTR CONTEXT
EXCEPTION_POINTERS ENDS
PEXCEPTION_POINTERS typedef PTR EXCEPTION_POINTERS


; +
; Define configuration routine types.
;
; Configuration information.
; -

;typedef enum _CONFIGURATION_TYPE {
    ArcSystem					equ 0
    CentralProcessor			equ 1
    FloatingPointProcessor		equ 2
    PrimaryIcache				equ 3
    PrimaryDcache				equ 4
    SecondaryIcache				equ 5
    SecondaryDcache				equ 6
    SecondaryCache				equ 7
    EisaAdapter					equ 8
    TcAdapter					equ 9
    ScsiAdapter					equ 10
    DtiAdapter					equ 11
    MultiFunctionAdapter		equ 12
    DiskController				equ 13
    TapeController				equ 14
    CdromController				equ 15
    WormController				equ 16
    SerialController			equ 17
    NetworkController			equ 18
    DisplayController			equ 19
    ParallelController			equ 20
    PointerController			equ 21
    KeyboardController			equ 22
    AudioController				equ 23
    OtherController				equ 24
    DiskPeripheral				equ 25
    FloppyDiskPeripheral		equ 26
    TapePeripheral				equ 27
    ModemPeripheral				equ 28
    MonitorPeripheral			equ 29
    PrinterPeripheral			equ 30
    PointerPeripheral			equ 31
    KeyboardPeripheral			equ 32
    TerminalPeripheral			equ 33
    OtherPeripheral				equ 34
    LinePeripheral				equ 35
    NetworkPeripheral			equ 36
    SystemMemory				equ 37
    DockingInformation			equ 38
    RealModeIrqRoutingTable		equ 39
    MaximumType					equ 40

THREAD_WAIT_OBJECTS equ 3           ; Builtin usable wait blocks

; Interrupt modes.

;typedef enum _KINTERRUPT_MODE {
    LevelSensitive	equ 0
    Latched			equ 1

; Wait reasons

;typedef enum _KWAIT_REASON {
    Executive			equ 0
    FreePage			equ 1
    PageIn				equ 2
    PoolAllocation		equ 3
    DelayExecution		equ 4
    Suspended			equ 5
    UserRequest			equ 6
    WrExecutive			equ 7
    WrFreePage			equ 8
    WrPageIn			equ 9
    WrPoolAllocation	equ 10
    WrDelayExecution	equ 11
    WrSuspended			equ 12
    WrUserRequest		equ 13
    WrEventPair			equ 14
    WrQueue				equ 15
    WrLpcReceive		equ 16
    WrLpcReply			equ 17
    WrVirtualMemory		equ 18
    WrPageOut			equ 19
    WrRendezvous		equ 20
    Spare2				equ 21
    Spare3				equ 22
    Spare4				equ 23
    Spare5				equ 24
    Spare6				equ 25
    WrKernel			equ 26
    MaximumWaitReason	equ 27

; +
; Common dispatcher object header
; 
; N.B. The size field contains the number of dwords in the structure.
; -

DISPATCHER_HEADER STRUCT			; sizeof = 010h
	_Type			BYTE		?	; 0000h  DISP_TYPE_* (original name Type)
	Absolute		BYTE		?	; 0001h
	_Size			BYTE		?	; 0002h  (original name Size)
	Inserted		BYTE		?	; 0003h
	SignalState		SDWORD		?	; 0004h
	WaitListHead	LIST_ENTRY <>	; 0008h
DISPATCHER_HEADER ENDS

KWAIT_BLOCK STRUCT					; sizeof = 018h
	WaitListEntry	LIST_ENTRY <>	; 0000h
	Thread			PVOID		?	; 0008h  PTR KTHREAD
	Object			PVOID		?	; 000Ch
	NextWaitBlock	PVOID		?	; 0010h  PTR KWAIT_BLOCK
	WaitKey			WORD		?	; 0014h
	WaitType		WORD		?	; 0016h
KWAIT_BLOCK ENDS
PKWAIT_BLOCK  typedef PTR KWAIT_BLOCK
PRKWAIT_BLOCK typedef PTR KWAIT_BLOCK

; Kernel object structure definitions

; Device Queue object and entry

KDEVICE_QUEUE STRUCT					; sizeof = 014h
	_Type			SWORD		?		; 0000h  (original name Type)
	_Size			SWORD		?		; 0002h  (original name Size)
	DeviceListHead	LIST_ENTRY <>		; 0004h
	slLock			DWORD		?		; 000Ch  KSPIN_LOCK  (original name Lock)
	Busy			BYTE		?		; 0010h  BOOLEAN
					db			3 dup(?) ; padding
KDEVICE_QUEUE ENDS
PKDEVICE_QUEUE	typedef PTR KDEVICE_QUEUE
PRKDEVICE_QUEUE typedef PTR KDEVICE_QUEUE	; *RESTRICTED_POINTER

KDEVICE_QUEUE_ENTRY STRUCT			; sizeof = 010h
	DeviceListEntry	LIST_ENTRY <>	; 0000h
	SortKey			DWORD		?	; 0008h
	Inserted		BYTE		?	; 000Ch
					db			3 dup(?) ; padding
KDEVICE_QUEUE_ENTRY ENDS
PKDEVICE_QUEUE_ENTRY  typedef PTR KDEVICE_QUEUE_ENTRY
PRKDEVICE_QUEUE_ENTRY typedef PTR KDEVICE_QUEUE_ENTRY	; *RESTRICTED_POINTER

; Event object

KEVENT STRUCT	; sizeof = 10h
	Header	DISPATCHER_HEADER	<>
KEVENT ENDS
PKEVENT typedef PTR KEVENT
PRKEVENT typedef PTR KEVENT	; RESTRICTED_POINTER

; Mutant object

KMUTANT STRUCT		; sizeof = 020h
	Header			DISPATCHER_HEADER <>; 0000h
	MutantListEntry	LIST_ENTRY 		<>	; 0010h
	OwnerThread		PVOID			?	; 0018h PTR KTHREAD *RESTRICTED_POINTER
	Abandoned		BYTE			?	; 001Ch  BOOLEAN
	ApcDisable		BYTE			?	; 001Dh
					WORD			?	; 001Eh  padding
KMUTANT ENDS
PKMUTANT typedef PTR KMUTANT
PRKMUTANT typedef PTR KMUTANT

KMUTEX textequ <KMUTANT>
PKMUTEX typedef PTR KMUTANT
PRKMUTEX typedef PTR KMUTANT

; Semaphore object

KSEMAPHORE STRUCT	; sizeof = 14h
	Header	DISPATCHER_HEADER	<>
	Limit	SDWORD				?
KSEMAPHORE ENDS
PKSEMAPHORE  typedef PTR KSEMAPHORE
PRKSEMAPHORE typedef PTR KSEMAPHORE	; *RESTRICTED_POINTER

; Timer object

KTIMER STRUCT		; sizeof = 028h
	Header			DISPATCHER_HEADER <>; 0000h
	DueTime			ULARGE_INTEGER 	<>	; 0010h
	TimerListEntry	LIST_ENTRY 		<>	; 0018h
	Dpc				PVOID			?	; 0020h PTR KDPC
	Period			SDWORD			?	; 0024h
KTIMER ENDS
PKTIMER typedef PTR KTIMER
PRKTIMER typedef PTR KTIMER	; *RESTRICTED_POINTER

;typedef enum _MEMORY_CACHING_TYPE_ORIG
    MmFrameBufferCached			equ 2

;typedef enum _MEMORY_CACHING_TYPE
    MmNonCached					equ 0	; = FALSE
    MmCached					equ 1	; = TRUE
    MmWriteCombined				equ 2	; = MmFrameBufferCached
    MmHardwareCoherentCached	equ 3
    MmNonCachedUnordered		equ 4
    MmUSWCCached				equ 5
    MmMaximumCacheType			equ 6

; Pool Allocation routines (in pool.c)

;typedef enum _POOL_TYPE {
    NonPagedPool							equ 0
    PagedPool								equ 1
    NonPagedPoolMustSucceed					equ 2
    DontUseThisType							equ 3
    NonPagedPoolCacheAligned				equ 4
    PagedPoolCacheAligned					equ 5
    NonPagedPoolCacheAlignedMustS			equ 6
    MaxPoolType								equ 7

	; Note these per session types are carefully chosen so that the appropriate
	; masking still applies as well as MaxPoolType above.

    NonPagedPoolSession						equ 32
    PagedPoolSession						equ 33
    NonPagedPoolMustSucceedSession			equ 34
    DontUseThisTypeSession					equ 35
    NonPagedPoolCacheAlignedSession			equ 36
    PagedPoolCacheAlignedSession			equ 37
    NonPagedPoolCacheAlignedMustSSession	equ 38

; +
; _EX_POOL_PRIORITY_ provides a method for the system to handle requests
; intelligently in low resource conditions.
;
; LowPoolPriority should be used when it is acceptable to the driver for the
; mapping request to fail if the system is low on resources.  An example of
; this could be for a non-critical network connection where the driver can
; handle the failure case when system resources are close to being depleted.
;
; NormalPoolPriority should be used when it is acceptable to the driver for the
; mapping request to fail if the system is very low on resources.  An example
; of this could be for a non-critical local filesystem request.
;
; HighPoolPriority should be used when it is unacceptable to the driver for the
; mapping request to fail unless the system is completely out of resources.
; An example of this would be the paging file path in a driver.
;
; SpecialPool can be specified to bound the allocation at a page end (or
; beginning).  This should only be done on systems being debugged as the
; memory cost is expensive.
;
; N.B.  These values are very carefully chosen so that the pool allocation
;       code can quickly crack the priority request.
; -

;typedef enum _EX_POOL_PRIORITY {
    LowPoolPriority							equ 0
    LowPoolPrioritySpecialPoolOverrun		equ 8
    LowPoolPrioritySpecialPoolUnderrun		equ 9
    NormalPoolPriority						equ 16
    NormalPoolPrioritySpecialPoolOverrun	equ 24
    NormalPoolPrioritySpecialPoolUnderrun	equ 25
    HighPoolPriority						equ 32
    HighPoolPrioritySpecialPoolOverrun		equ 40
    HighPoolPrioritySpecialPoolUnderrun		equ 41

; Routines to support fast mutexes.

FAST_MUTEX STRUCT		; sizeof = 020h
	Count		SDWORD		?	; 0000h
	Owner		PVOID		?	; 0004h PTR KTHREAD
	Contention	DWORD		?	; 0008h
	Event		KEVENT 		<>	; 000Ch
	OldIrql		DWORD		?	; 001Ch
FAST_MUTEX ENDS
PFAST_MUTEX typedef PTR FAST_MUTEX

; +
; Define interlocked sequenced listhead functions.
;
; A sequenced interlocked list is a singly linked list with a header that
; contains the current depth and a sequence number. Each time an entry is
; inserted or removed from the list the depth is updated and the sequence
; number is incremented. This enables MIPS, Alpha, and Pentium and later
; machines to insert and remove from the list without the use of spinlocks.
; The PowerPc, however, must use a spinlock to synchronize access to the
; list.
;
; N.B. A spinlock must be specified with SLIST operations. However, it may
;      not actually be used.
; -

; +
; VOID ExInitializeSListHead ( IN PSLIST_HEADERT SListHead )
; 
; Routine Description:
;     This function initializes a sequenced singly linked listhead.
; 
; Arguments:
;     SListHead - Supplies a pointer to a sequenced singly linked listhead.
; 
; Return Value:
;     None.
; -

ExInitializeSListHead MACRO SListHead:REQ
local a

	IF $IsAddr(SListHead)
		a SUBSTR <SListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(SListHead)
		IFDIFI <SListHead>, <eax>	;; don't move eax onto itself
			mov eax, SListHead
		ENDIF
	ELSE
		.ERR
	ENDIF
	; (SLIST_HEADER PTR [eax]).Alignment = 0
	and dword ptr [eax], 0
	and dword ptr [eax][4], 0
ENDM

; +
; USHORT ExQueryDepthSList ( IN PSLIST_HEADERT SListHead )
; 
; Routine Description:
;     This function queries the current number of entries contained in a
;     sequenced single linked list.
; 
; Arguments:
;     SListHead - Supplies a pointer to the sequenced listhead which is
;        be queried.
; 
; Return Value:
;     The current number of entries in the sequenced singly linked list is
;     returned as the function value.
; -

ExQueryDepthSList MACRO SListHead:REQ
local a
	IF $IsAddr(SListHead)
		a SUBSTR <SListHead>, 6
		IF $IsStack(a)				;; Is relative to SS
			lea eax, a
		ELSE
			mov eax, offset a
		ENDIF
	ELSEIF $IsReg(SListHead)
		IFDIFI <SListHead>, <eax>	;; don't move eax onto itself
			mov eax, SListHead
		ENDIF
	ELSE
		.ERR
	ENDIF
	movzx eax, (SLIST_HEADER PTR [eax]).Depth
ENDM

GENERAL_LOOKASIDE STRUCT					; sizeof = 048h
	ListHead				SLIST_HEADER <>	; 0000h
	Depth					WORD		?	; 0008h
	MaximumDepth			WORD		?	; 000Ah
	TotalAllocates			DWORD		?	; 000Ch
	union
		AllocateMisses		DWORD		?	; 0010h
		AllocateHits		DWORD		?	; 0010h
	ends
	TotalFrees				DWORD		?	; 0014h
	union
		FreeMisses			DWORD		?	; 0018h
		FreeHits			DWORD		?	; 0018h
	ends
	_Type					SDWORD		?	; 001Ch POOL_TYPE
	Tag						DWORD		?	; 0020h
	_Size					DWORD		?	; 0024h
	Allocate				PVOID		?	; 0028h PALLOCATE_FUNCTION
	Free					PVOID		?	; 002Ch PFREE_FUNCTION
	ListEntry				LIST_ENTRY <>	; 0030h
	LastTotalAllocates		DWORD		?	; 0038h
	union
		LastAllocateMisses	DWORD		?	; 003Ch
		LastAllocateHits	DWORD		?	; 003Ch
	ends
	Future					DWORD 2 dup(?)	; 0040h
GENERAL_LOOKASIDE ENDS
PGENERAL_LOOKASIDE typedef ptr GENERAL_LOOKASIDE


NPAGED_LOOKASIDE_LIST STRUCT		; sizeof = 050h
	L		GENERAL_LOOKASIDE	<>	; 0000h
	_Lock	DWORD				?	; 0048h
NPAGED_LOOKASIDE_LIST ENDS
PNPAGED_LOOKASIDE_LIST typedef ptr NPAGED_LOOKASIDE_LIST

PAGED_LOOKASIDE_LIST STRUCT
	L		GENERAL_LOOKASIDE	<>	; 0000h
	_Lock	FAST_MUTEX			<>	; 0048h
PAGED_LOOKASIDE_LIST ENDS
PPAGED_LOOKASIDE_LIST typedef ptr PAGED_LOOKASIDE_LIST


; Worker Thread

;typedef enum _WORK_QUEUE_TYPE {
    CriticalWorkQueue		equ 0
    DelayedWorkQueue		equ 1
    HyperCriticalWorkQueue	equ 2
    MaximumWorkQueue		equ 3

;  Define executive resource data structures.

ERESOURCE_THREAD	typedef DWORD	; ULONG_PTR
PERESOURCE_THREAD	typedef PTR DWORD

OWNER_ENTRY STRUCT				; sizeof = 08h
	OwnerThread	DWORD		?	; 0000h
	union
		OwnerCount	SDWORD	?	; 0004h
		TableSize	DWORD	?	; 0004h
	ends
OWNER_ENTRY ENDS
POWNER_ENTRY typedef PTR OWNER_ENTRY

ERESOURCE STRUCT		; sizeof = 038h
	SystemResourcesList			LIST_ENTRY <>	; 0000h
	OwnerTable					PVOID		?	; 0008h PTR OWNER_ENTRY
	ActiveCount					SWORD		?	; 000Ch
	Flag						WORD		?	; 000Eh
	SharedWaiters				PVOID		?	; 0010h PTR KSEMAPHORE
	ExclusiveWaiters			PVOID		?	; 0014h PTR KEVENT
	OwnerThreads				OWNER_ENTRY	2 dup(<>)	; 0018h
	ContentionCount				DWORD		?	; 0028h
	NumberOfSharedWaiters		WORD		?	; 002Ch
	NumberOfExclusiveWaiters	WORD		?	; 002Eh
	union
		Address					PVOID		?	; 0030h
		CreatorBackTraceIndex	DWORD		?	; 0030h
	ends
	SpinLock					DWORD		?	; 0034h  KSPIN_LOCK
ERESOURCE ENDS
PERESOURCE typedef PTR ERESOURCE

;  Values for ERESOURCE.Flag

ResourceNeverExclusive       equ 10h
ResourceReleaseByOtherThread equ 20h
ResourceOwnedExclusive       equ 80h

RESOURCE_HASH_TABLE_SIZE     equ 64

; Priority increment definitions.  The comment for each definition gives
; the names of the system services that use the definition when satisfying
; a wait.

; Priority increment used when satisfying a wait on an executive event
; (NtPulseEvent and NtSetEvent)

EVENT_INCREMENT			equ 1

; Priority increment when no I/O has been done.  This is used by device
; and file system drivers when completing an IRP (IoCompleteRequest).

IO_NO_INCREMENT			equ 0

; Priority increment for completing CD-ROM I/O.  This is used by CD-ROM device
; and file system drivers when completing an IRP (IoCompleteRequest)

IO_CD_ROM_INCREMENT		equ 1

; Priority increment for completing disk I/O.  This is used by disk device
; and file system drivers when completing an IRP (IoCompleteRequest)

IO_DISK_INCREMENT		equ 1

; Priority increment for completing keyboard I/O.  This is used by keyboard
; device drivers when completing an IRP (IoCompleteRequest)

IO_KEYBOARD_INCREMENT	equ 6

; Priority increment for completing mailslot I/O.  This is used by the mail-
; slot file system driver when completing an IRP (IoCompleteRequest).

IO_MAILSLOT_INCREMENT	equ 2

; Priority increment for completing mouse I/O.  This is used by mouse device
; drivers when completing an IRP (IoCompleteRequest)

IO_MOUSE_INCREMENT		equ 6

; Priority increment for completing named pipe I/O.  This is used by the
; named pipe file system driver when completing an IRP (IoCompleteRequest).

IO_NAMED_PIPE_INCREMENT	equ 2

; Priority increment for completing network I/O.  This is used by network
; device and network file system drivers when completing an IRP
; (IoCompleteRequest).

IO_NETWORK_INCREMENT	equ 2

; Priority increment for completing parallel I/O.  This is used by parallel
; device drivers when completing an IRP (IoCompleteRequest)

IO_PARALLEL_INCREMENT	equ 1

; Priority increment for completing serial I/O.  This is used by serial device
; drivers when completing an IRP (IoCompleteRequest)

IO_SERIAL_INCREMENT		equ 2

; Priority increment for completing sound I/O.  This is used by sound device
; drivers when completing an IRP (IoCompleteRequest)

IO_SOUND_INCREMENT		equ 8

; Priority increment for completing video I/O.  This is used by video device
; drivers when completing an IRP (IoCompleteRequest)

IO_VIDEO_INCREMENT		equ 1

; Priority increment used when satisfying a wait on an executive semaphore
; (NtReleaseSemaphore)

SEMAPHORE_INCREMENT		equ 1

; MmQuerySystemSize returns one of these value

;typedef enum _MM_SYSTEM_SIZE {
    MmSmallSystem	equ 0
    MmMediumSystem	equ 1
    MmLargeSystem	equ 2

;typedef enum _LOCK_OPERATION {
    IoReadAccess	equ 0
    IoWriteAccess	equ 1
    IoModifyAccess	equ 2


; +
; _MM_PAGE_PRIORITY_ provides a method for the system to handle requests
; intelligently in low resource conditions.
;
; LowPagePriority should be used when it is acceptable to the driver for the
; mapping request to fail if the system is low on resources.  An example of
; this could be for a non-critical network connection where the driver can
; handle the failure case when system resources are close to being depleted.
;
; NormalPagePriority should be used when it is acceptable to the driver for the
; mapping request to fail if the system is very low on resources.  An example
; of this could be for a non-critical local filesystem request.
;
; HighPagePriority should be used when it is unacceptable to the driver for the
; mapping request to fail unless the system is completely out of resources.
; An example of this would be the paging file path in a driver.
; -

;typedef enum _MM_PAGE_PRIORITY
    LowPagePriority			equ 0
    NormalPagePriority		equ 16
    HighPagePriority		equ 32

PHYSICAL_MEMORY_RANGE STRUCT
	BaseAddress		PHYSICAL_ADDRESS	<>
	NumberOfBytes	LARGE_INTEGER		<>
PHYSICAL_MEMORY_RANGE ENDS
PPHYSICAL_MEMORY_RANGE typedef ptr PHYSICAL_MEMORY_RANGE

;  Security operation codes

;typedef enum _SECURITY_OPERATION_CODE {
    SetSecurityDescriptor		equ 0
    QuerySecurityDescriptor		equ 1
    DeleteSecurityDescriptor	equ 2
    AssignSecurityDescriptor	equ 3

; +
;  Data structure used to capture subject security context
;  for access validations and auditing.
;
;  THE FIELDS OF THIS DATA STRUCTURE SHOULD BE CONSIDERED OPAQUE
;  BY ALL EXCEPT THE SECURITY ROUTINES.
; -

SECURITY_SUBJECT_CONTEXT STRUCT			; sizeof = 010h
	ClientToken			PVOID		?	; 0000h PACCESS_TOKEN
	ImpersonationLevel	SDWORD		?	; 0004h SECURITY_IMPERSONATION_LEVEL
	PrimaryToken		PVOID		?	; 0008h PACCESS_TOKEN
	ProcessAuditId		PVOID		?	; 000Ch
SECURITY_SUBJECT_CONTEXT ENDS
PSECURITY_SUBJECT_CONTEXT typedef ptr SECURITY_SUBJECT_CONTEXT

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                                                           ::
;:                  ACCESS_STATE and related structures                      ::
;:                                                                           ::
;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;  Initial Privilege Set - Room for three privileges, which should
;  be enough for most applications.  This structure exists so that
;  it can be imbedded in an ACCESS_STATE structure.  Use PRIVILEGE_SET
;  for all other references to Privilege sets.

INITIAL_PRIVILEGE_COUNT         equ 3

INITIAL_PRIVILEGE_SET STRUCT											; sizeof = 02Ch
	PrivilegeCount	DWORD		?										; 0000h
	Control			DWORD		?										; 0004h
	Privilege		LUID_AND_ATTRIBUTES	INITIAL_PRIVILEGE_COUNT dup(<>)	; 0008h
INITIAL_PRIVILEGE_SET ENDS
PINITIAL_PRIVILEGE_SET typedef ptr INITIAL_PRIVILEGE_SET

; Combine the information that describes the state
; of an access-in-progress into a single structure

ACCESS_STATE STRUCT										; sizeof = 074h
	OperationID				LUID 					<>	; 0000h
	SecurityEvaluated		BOOLEAN					?	; 0008h
	GenerateAudit			BOOLEAN					?	; 0009h
	GenerateOnClose			BOOLEAN					?	; 000Ah
	PrivilegesAllocated		BOOLEAN					?	; 000Bh
	Flags					DWORD					?	; 000Ch
	RemainingDesiredAccess	DWORD					?	; 0010h ACCESS_MASK
	PreviouslyGrantedAccess	DWORD					?	; 0014h ACCESS_MASK
	OriginalDesiredAccess	DWORD					?	; 0018h ACCESS_MASK
	SubjectSecurityContext	SECURITY_SUBJECT_CONTEXT <>	; 001Ch
	SecurityDescriptor		PVOID					?	; 002Ch PSECURITY_DESCRIPTOR
	AuxData					PVOID					?	; 0030h
	union Privileges									; 0034h
		InitialPrivilegeSet	INITIAL_PRIVILEGE_SET	<>	; 0034h
		PrivilegeSet		PRIVILEGE_SET			<>	; 0034h
	ends
	AuditPrivileges			BOOLEAN					?	; 0060h
							db 3 dup(?)					; padding
	ObjectName				UNICODE_STRING 			<>	; 0064h
	ObjectTypeName			UNICODE_STRING 			<>	; 006Ch
ACCESS_STATE ENDS
PACCESS_STATE typedef ptr ACCESS_STATE


; Structures for Load Image Notify

; All fields prefixed with 'iif' because of record fields should be unique :-(
IMAGE_INFO_FLAGS RECORD \
	iifReserved:22,				; bits 10-31
	iifImageMappedToAllPids:1,	; bit 9
	iifSystemModeImage:1,		; bit 8
	iifImageAddressingMode:8	; bits 0-7

IMAGE_INFO STRUCT									; sizeof = 014h
	union
		Properties			DWORD				?	; 0000h
		Flags				IMAGE_INFO_FLAGS	<>	; 0000h not named in original ntddk.h
	ends
	ImageBase				PVOID				?	; 0004h
	ImageSelector			DWORD				?	; 0008h
	ImageSize				DWORD				?	; 000Ch SIZE_T
	ImageSectionNumber		DWORD				?	; 0010h
IMAGE_INFO ENDS
PIMAGE_INFO typedef ptr IMAGE_INFO

IMAGE_ADDRESSING_MODE_32BIT		equ 3


; +
; Define I/O system data structure type codes.  Each major data structure in
; the I/O system has a type code  The type field in each structure is at the
; same offset.  The following values can be used to determine which type of
; data structure a pointer refers to.
; -

IO_TYPE_ADAPTER					equ 1
IO_TYPE_CONTROLLER				equ 2
IO_TYPE_DEVICE					equ 3
IO_TYPE_DRIVER					equ 4
IO_TYPE_FILE					equ 5
IO_TYPE_IRP						equ 6
IO_TYPE_MASTER_ADAPTER			equ 7
IO_TYPE_OPEN_PACKET				equ 8
IO_TYPE_TIMER					equ 9
IO_TYPE_VPB						equ 0Ah
IO_TYPE_ERROR_LOG				equ 0Bh
IO_TYPE_ERROR_MESSAGE			equ 0Ch
IO_TYPE_DEVICE_OBJECT_EXTENSION	equ 0Dh

; Define the major function codes for IRPs.

IRP_MJ_CREATE					equ 0
IRP_MJ_CREATE_NAMED_PIPE		equ 1
IRP_MJ_CLOSE					equ 2
IRP_MJ_READ						equ 3
IRP_MJ_WRITE					equ 4
IRP_MJ_QUERY_INFORMATION		equ 5
IRP_MJ_SET_INFORMATION			equ 6
IRP_MJ_QUERY_EA					equ 7
IRP_MJ_SET_EA					equ 8
IRP_MJ_FLUSH_BUFFERS			equ 9
IRP_MJ_QUERY_VOLUME_INFORMATION	equ 0Ah
IRP_MJ_SET_VOLUME_INFORMATION	equ 0Bh
IRP_MJ_DIRECTORY_CONTROL		equ 0Ch
IRP_MJ_FILE_SYSTEM_CONTROL		equ 0Dh
IRP_MJ_DEVICE_CONTROL			equ 0Eh
IRP_MJ_INTERNAL_DEVICE_CONTROL	equ 0Fh
IRP_MJ_SHUTDOWN					equ 10h
IRP_MJ_LOCK_CONTROL				equ 11h
IRP_MJ_CLEANUP					equ 12h
IRP_MJ_CREATE_MAILSLOT			equ 13h
IRP_MJ_QUERY_SECURITY			equ 14h
IRP_MJ_SET_SECURITY				equ 15h
IRP_MJ_POWER					equ 16h
IRP_MJ_SYSTEM_CONTROL			equ 17h
IRP_MJ_DEVICE_CHANGE			equ 18h
IRP_MJ_QUERY_QUOTA				equ 19h
IRP_MJ_SET_QUOTA				equ 1Ah
IRP_MJ_PNP						equ 1Bh
IRP_MJ_PNP_POWER				equ IRP_MJ_PNP	; Obsolete....
IRP_MJ_MAXIMUM_FUNCTION			equ 1Bh

; Make the Scsi major code the same as internal device control.

IRP_MJ_SCSI                     equ IRP_MJ_INTERNAL_DEVICE_CONTROL

; Define the minor function codes for IRPs.  The lower 128 codes, from 0x00 to
; 0x7f are reserved to Microsoft.  The upper 128 codes, from 0x80 to 0xff, are
; reserved to customers of Microsoft.


; Directory control minor function codes

IRP_MN_QUERY_DIRECTORY          equ 01
IRP_MN_NOTIFY_CHANGE_DIRECTORY  equ 02

; File system control minor function codes.  Note that "user request" is
; assumed to be zero by both the I/O system and file systems.  Do not change

IRP_MN_USER_FS_REQUEST          equ 00
IRP_MN_MOUNT_VOLUME             equ 01
IRP_MN_VERIFY_VOLUME            equ 02
IRP_MN_LOAD_FILE_SYSTEM         equ 03
IRP_MN_TRACK_LINK               equ 04    ; To be obsoleted soon
IRP_MN_KERNEL_CALL              equ 04

; Lock control minor function codes

IRP_MN_LOCK                     equ 01
IRP_MN_UNLOCK_SINGLE            equ 02
IRP_MN_UNLOCK_ALL               equ 03
IRP_MN_UNLOCK_ALL_BY_KEY        equ 04

; +
; Read and Write minor function codes for file systems supporting Lan Manager
; software.  All of these subfunction codes are invalid if the file has been
; opened with FO_NO_INTERMEDIATE_BUFFERING.  They are also invalid in combi-
; nation with synchronous calls (Irp Flag or file open option).
;
; Note that "normal" is assumed to be zero by both the I/O system and file
; systems.  Do not change this value.
; -

IRP_MN_NORMAL                   equ 00
IRP_MN_DPC                      equ 01
IRP_MN_MDL                      equ 02
IRP_MN_COMPLETE                 equ 04
IRP_MN_COMPRESSED               equ 08

IRP_MN_MDL_DPC                  equ IRP_MN_MDL or IRP_MN_DPC
IRP_MN_COMPLETE_MDL             equ IRP_MN_COMPLETE or IRP_MN_MDL
IRP_MN_COMPLETE_MDL_DPC         equ IRP_MN_COMPLETE_MDL or IRP_MN_DPC

; Device Control Request minor function codes for SCSI support. Note that
; user requests are assumed to be zero.

IRP_MN_SCSI_CLASS               equ 01

; PNP minor function codes.

IRP_MN_START_DEVICE                 equ 00
IRP_MN_QUERY_REMOVE_DEVICE          equ 01
IRP_MN_REMOVE_DEVICE                equ 02
IRP_MN_CANCEL_REMOVE_DEVICE         equ 03
IRP_MN_STOP_DEVICE                  equ 04
IRP_MN_QUERY_STOP_DEVICE            equ 05
IRP_MN_CANCEL_STOP_DEVICE           equ 06

IRP_MN_QUERY_DEVICE_RELATIONS       equ 07
IRP_MN_QUERY_INTERFACE              equ 08
IRP_MN_QUERY_CAPABILITIES           equ 09
IRP_MN_QUERY_RESOURCES              equ 0Ah
IRP_MN_QUERY_RESOURCE_REQUIREMENTS  equ 0Bh
IRP_MN_QUERY_DEVICE_TEXT            equ 0Ch
IRP_MN_FILTER_RESOURCE_REQUIREMENTS equ 0Dh

IRP_MN_READ_CONFIG                  equ 0Fh
IRP_MN_WRITE_CONFIG                 equ 10h
IRP_MN_EJECT                        equ 11h
IRP_MN_SET_LOCK                     equ 12h
IRP_MN_QUERY_ID                     equ 13h
IRP_MN_QUERY_PNP_DEVICE_STATE       equ 14h
IRP_MN_QUERY_BUS_INFORMATION        equ 15h
IRP_MN_DEVICE_USAGE_NOTIFICATION    equ 16h
IRP_MN_SURPRISE_REMOVAL             equ 17h

IRP_MN_QUERY_LEGACY_BUS_INFORMATION equ 18h

; POWER minor function codes

IRP_MN_WAIT_WAKE                    equ 00
IRP_MN_POWER_SEQUENCE               equ 01
IRP_MN_SET_POWER                    equ 02
IRP_MN_QUERY_POWER                  equ 03

; WMI minor function codes under IRP_MJ_SYSTEM_CONTROL

IRP_MN_QUERY_ALL_DATA               equ 00
IRP_MN_QUERY_SINGLE_INSTANCE        equ 01
IRP_MN_CHANGE_SINGLE_INSTANCE       equ 02
IRP_MN_CHANGE_SINGLE_ITEM           equ 03
IRP_MN_ENABLE_EVENTS                equ 04
IRP_MN_DISABLE_EVENTS               equ 05
IRP_MN_ENABLE_COLLECTION            equ 06
IRP_MN_DISABLE_COLLECTION           equ 07
IRP_MN_REGINFO                      equ 08
IRP_MN_EXECUTE_METHOD               equ 09


; Define option flags for IoCreateFile.  Note that these values must be
; exactly the same as the SL_... flags for a create function.  Note also
; that there are flags that may be passed to IoCreateFile that are not
; placed in the stack location for the create IRP.  These flags start in
; the next byte.

IO_FORCE_ACCESS_CHECK           equ 0001
IO_OPEN_PAGING_FILE             equ 0002
IO_OPEN_TARGET_DIRECTORY        equ 0004
IO_NO_PARAMETER_CHECKING        equ 0100h

; Define Information fields for whether or not a REPARSE or a REMOUNT has
; occurred in the file system.

IO_REPARSE                      equ 0
IO_REMOUNT                      equ 1

; Defines the order of the information in the array of
; PKEY_VALUE_FULL_INFORMATION.

;typedef enum _IO_QUERY_DEVICE_DATA_FORMAT {
    IoQueryDeviceIdentifier					equ 0
    IoQueryDeviceConfigurationData			equ 1
    IoQueryDeviceComponentInformation		equ 2
    IoQueryDeviceMaxData						equ 3

; Define the objects that can be created by IoCreateFile.

;typedef enum _CREATE_FILE_TYPE {
    CreateFileTypeNone			equ 0
    CreateFileTypeNamedPipe		equ 1
    CreateFileTypeMailslot		equ 2

; Define the structure to describe the Fast I/O dispatch routines.  Any
; additions made to this structure MUST be added monotonically to the end
; of the structure, and fields CANNOT be removed from the middle.

FAST_IO_DISPATCH STRUCT								; sizeof = 070h
	SizeOfFastIoDispatch			DWORD		?	; 0000h
	FastIoCheckIfPossible			PVOID		?	; 0004h PFAST_IO_CHECK_IF_POSSIBLE
	FastIoRead						PVOID		?	; 0008h PFAST_IO_READ
	FastIoWrite						PVOID		?	; 000Ch PFAST_IO_WRITE
	FastIoQueryBasicInfo			PVOID		?	; 0010h PFAST_IO_QUERY_BASIC_INFO
	FastIoQueryStandardInfo			PVOID		?	; 0014h PFAST_IO_QUERY_STANDARD_INFO
	FastIoLock						PVOID		?	; 0018h PFAST_IO_LOCK
	FastIoUnlockSingle				PVOID		?	; 001Ch PFAST_IO_UNLOCK_SINGLE
	FastIoUnlockAll					PVOID		?	; 0020h PFAST_IO_UNLOCK_ALL
	FastIoUnlockAllByKey			PVOID		?	; 0024h PFAST_IO_UNLOCK_ALL_BY_KEY
	FastIoDeviceControl				PVOID		?	; 0028h PFAST_IO_DEVICE_CONTROL
	AcquireFileForNtCreateSection	PVOID		?	; 002Ch PFAST_IO_ACQUIRE_FILE
	ReleaseFileForNtCreateSection	PVOID		?	; 0030h PFAST_IO_RELEASE_FILE
	FastIoDetachDevice				PVOID		?	; 0034h PFAST_IO_DETACH_DEVICE
	FastIoQueryNetworkOpenInfo		PVOID		?	; 0038h PFAST_IO_QUERY_NETWORK_OPEN_INFO
	AcquireForModWrite				PVOID		?	; 003Ch PFAST_IO_ACQUIRE_FOR_MOD_WRITE
	MdlRead							PVOID		?	; 0040h PFAST_IO_MDL_READ
	MdlReadComplete					PVOID		?	; 0044h PFAST_IO_MDL_READ_COMPLETE
	PrepareMdlWrite					PVOID		?	; 0048h PFAST_IO_PREPARE_MDL_WRITE
	MdlWriteComplete				PVOID		?	; 004Ch PFAST_IO_MDL_WRITE_COMPLETE
	FastIoReadCompressed			PVOID		?	; 0050h PFAST_IO_READ_COMPRESSED
	FastIoWriteCompressed			PVOID		?	; 0054h PFAST_IO_WRITE_COMPRESSED
	MdlReadCompleteCompressed		PVOID		?	; 0058h PFAST_IO_MDL_READ_COMPLETE_COMPRESSED
	MdlWriteCompleteCompressed		PVOID		?	; 005Ch PFAST_IO_MDL_WRITE_COMPLETE_COMPRESSED
	FastIoQueryOpen					PVOID		?	; 0060h PFAST_IO_QUERY_OPEN
	ReleaseForModWrite				PVOID		?	; 0064h PFAST_IO_RELEASE_FOR_MOD_WRITE
	AcquireForCcFlush				PVOID		?	; 0068h PFAST_IO_ACQUIRE_FOR_CCFLUSH
	ReleaseForCcFlush				PVOID		?	; 006Ch PFAST_IO_RELEASE_FOR_CCFLUSH
FAST_IO_DISPATCH ENDS
PFAST_IO_DISPATCH typedef ptr FAST_IO_DISPATCH

; Define the actions that a driver execution routine may request of the
; adapter/controller allocation routines upon return.

;typedef enum _IO_ALLOCATION_ACTION {
    KeepObject						equ 1
    DeallocateObject				equ 2
    DeallocateObjectKeepRegisters	equ 3

; Define device driver adapter/controller execution routine.

; Define the I/O system's security context type for use by file system's
; when checking access to volumes, files, and directories.

IO_SECURITY_CONTEXT STRUCT
	SecurityQos			PVOID	?	; PSECURITY_QUALITY_OF_SERVICE
	AccessState			PVOID	?	; PACCESS_STATE
	DesiredAccess		DWORD	?	; ACCESS_MASK
	FullCreateOptions	DWORD	?
IO_SECURITY_CONTEXT ENDS
PIO_SECURITY_CONTEXT typedef ptr IO_SECURITY_CONTEXT

; Define Volume Parameter Block (VPB) flags.

VPB_MOUNTED                     equ 00000001
VPB_LOCKED                      equ 00000002
VPB_PERSISTENT                  equ 00000004
VPB_REMOVE_PENDING              equ 00000008
VPB_RAW_MOUNT                   equ 00000010h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Volume Parameter Block (VPB)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MAXIMUM_VOLUME_LABEL_LENGTH	equ (32 * sizeof(WCHAR))	; 32 characters

VPB STRUCT				; sizeof = 058h
	_Type				SWORD		?	; 0000h  (IO_TYPE_VPB = 0Ah)  (original field name Type)
	_Size				SWORD		?	; 0002h  (original field name Size)
	Flags				WORD		?	; 0004h
	VolumeLabelLength	WORD		?	; 0006h  (in bytes without terminat. zero)
	DeviceObject		PVOID		?	; 0008h PTR DEVICE_OBJECT
	RealDevice			PVOID		?	; 000Ch PTR DEVICE_OBJECT
	SerialNumber		DWORD		?	; 0010h
	ReferenceCount		DWORD		?	; 0014h
	VolumeLabel			WORD (MAXIMUM_VOLUME_LABEL_LENGTH / (sizeof WCHAR)) dup(?); 0018h
VPB ENDS
PVPB typedef PTR VPB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define Wait Context Block (WCB)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

WAIT_CONTEXT_BLOCK STRUCT		; sizeof = 028h
	WaitQueueEntry			KDEVICE_QUEUE_ENTRY <>	; 0000h
	DeviceRoutine			PVOID		?	; 0010h  PDRIVER_CONTROL
	DeviceContext			PVOID		?	; 0014h
	NumberOfMapRegisters	DWORD		?	; 0018h
	DeviceObject			PVOID		?	; 001Ch
	CurrentIrp				PVOID		?	; 0020h
	BufferChainingDpc		PVOID		?	; 0024h PTR KDPC
WAIT_CONTEXT_BLOCK ENDS
PWAIT_CONTEXT_BLOCK typedef PTR WAIT_CONTEXT_BLOCK

CONTROLLER_OBJECT STRUCT
	_Type				SWORD			?	; 000h   (original name Type)
	_Size				SWORD			?	; 002h   (original name Size)
	ControllerExtension	PVOID			?	; 004h
	DeviceWaitQueue		KDEVICE_QUEUE	<>	; 008h

	Spare1				DWORD			?	; 01Ch
	_Spare2				LARGE_INTEGER	<>	; 020h  (original name Spare2)

CONTROLLER_OBJECT ENDS
PCONTROLLER_OBJECT typedef PTR CONTROLLER_OBJECT

; Define Device Object (DO) flags

DO_VERIFY_VOLUME                equ 00000002h
DO_BUFFERED_IO                  equ 00000004h
DO_EXCLUSIVE                    equ 00000008h
DO_DIRECT_IO                    equ 00000010h
DO_MAP_IO_BUFFER                equ 00000020h
DO_DEVICE_HAS_NAME              equ 00000040h
DO_DEVICE_INITIALIZING          equ 00000080h
DO_SYSTEM_BOOT_PARTITION        equ 00000100h
DO_LONG_TERM_REQUESTS           equ 00000200h
DO_NEVER_LAST_DEVICE            equ 00000400h
DO_SHUTDOWN_REGISTERED          equ 00000800h
DO_BUS_ENUMERATED_DEVICE        equ 00001000h
DO_POWER_PAGABLE                equ 00002000h
DO_POWER_INRUSH                 equ 00004000h
DO_LOW_PRIORITY_FILESYSTEM      equ 00010000h

; Device Object structure definition

DEVICE_OBJECT STRUCT		; sizeof = 0B8h
	_Type					SWORD		?	; 0000h  (IO_TYPE_DEVICE = 3)  (original field name Type)
	_Size					WORD		?	; 0002h  (original name Size)
	ReferenceCount			SDWORD		?	; 0004h
	DriverObject			PVOID		?	; 0008h  PTR DRIVER_OBJECT
	NextDevice				PVOID		?	; 000Ch  PTR DEVICE_OBJECT
	AttachedDevice			PVOID		?	; 0010h  PTR DEVICE_OBJECT
	CurrentIrp				PVOID		?	; 0014h  PTR IRP
	Timer					PVOID		?	; 0018h  PTR IO_TIMER
	Flags					DWORD		?	; 001Ch  DO_*
	Characteristics			DWORD		?	; 0020h  FILE_*
	Vpb						PVOID		?	; 0024h  PTR VPB
	DeviceExtension			PVOID		?	; 0028h
	DeviceType				DWORD		?	; 002Ch  DEVICE_TYPE
	StackSize				BYTE		?	; 0030h
							db 		3 dup(?); padding
	UNION Queue
		ListEntry			LIST_ENTRY			<>	; 0034h
		Wcb					WAIT_CONTEXT_BLOCK	<>	; 0034h
	ENDS ; Queue
	AlignmentRequirement	DWORD		?	; 005Ch
	DeviceQueue				KDEVICE_QUEUE <>; 0060h
	Dpc						KDPC 		<>	; 0074h

	; The following field is for exclusive use by the filesystem to keep
	; track of the number of Fsp threads currently using the device

	ActiveThreadCount		DWORD		?	; 0094h
	SecurityDescriptor		PVOID		?	; 0098h  PSECURITY_DESCRIPTOR
	DeviceLock				KEVENT 		<>	; 009Ch

	SectorSize				WORD		?	; 00ACh
	Spare1					WORD		?	; 00AEh

	DeviceObjectExtension	PVOID		?	; 00B0h PTR DEVOBJ_EXTENSION
	Reserved				PVOID		?	; 00B4h
DEVICE_OBJECT ENDS
PDEVICE_OBJECT typedef PTR DEVICE_OBJECT

;;struct  _DEVICE_OBJECT_POWER_EXTENSION;

DEVOBJ_EXTENSION STRUCT			; sizeof = 024h
	_Type			SWORD		?	; 0000h  (original name Type)
	_Size			WORD		?	; 0002h  (original name Size)

	; Public part of the DeviceObjectExtension structure

	DeviceObject	PVOID		?	; 0004h PTR DEVICE_OBJECT owning device object

	; *!!!* The fields below are not defined in original ntddk.h file *!!!*
	; Use it on your own

	PowerFlags		DWORD		?	; 0008h
	Dope			PVOID		?	; 000Ch PTR DEVICE_OBJECT_POWER_EXTENSION
	ExtensionFlags	DWORD		?	; 0010h
	DeviceNode		PVOID		?	; 0014h
	AttachedTo		PVOID		?	; 0018h PTR DEVICE_OBJECT
	FileObjectList	LIST_ENTRY <>	; 001Ch
DEVOBJ_EXTENSION ENDS
PDEVOBJ_EXTENSION typedef PTR DEVOBJ_EXTENSION

; Define Driver Object (DRVO) flags

DRVO_UNLOAD_INVOKED             equ 00000001
DRVO_LEGACY_DRIVER              equ 00000002
DRVO_BUILTIN_DRIVER             equ 00000004	; Driver objects for Hal, PnP Mgr

DRVO_REINIT_REGISTERED          equ 00000008
DRVO_INITIALIZED                equ 00000010h
DRVO_BOOTREINIT_REGISTERED      equ 00000020h
DRVO_LEGACY_RESOURCES           equ 00000040h

DRIVER_EXTENSION STRUCT		; sizeof = 14h

	; Back pointer to Driver Object

	DriverObject	PVOID			?	; 00h PDRIVER_OBJECT

	; The AddDevice entry point is called by the Plug & Play manager
	; to inform the driver when a new device instance arrives that this
	; driver must control.

	AddDevice		PVOID			?	; 04h PDRIVER_ADD_DEVICE

	; The count field is used to count the number of times the driver has
	; had its registered reinitialization routine invoked.

	Count			DWORD			?	; 08h

	; The service name field is used by the pnp manager to determine
	; where the driver related info is stored in the registry.

	ServiceKeyName	UNICODE_STRING	<>	; 0Ch

	; Note: any new shared fields get added here.

	; *!!!* The field below is not defined in original ntddk.h file *!!!*
	; Use it on your own

	ClientDriverExtension	PVOID		?	; 0014h PTR IO_CLIENT_EXTENSION

DRIVER_EXTENSION ENDS
PDRIVER_EXTENSION typedef PTR DRIVER_EXTENSION

; 

DRIVER_OBJECT STRUCT	; sizeof= 0A8h
	_Type			SWORD			?	; 0000h  (IO_TYPE_DRIVER = 4)  (original field name Type)
	_Size			SWORD			?	; 0004h  (original name Size)

	; The following links all of the devices created by a single driver
	; together on a list, and the Flags word provides an extensible flag
	; location for driver objects.

	DeviceObject		PVOID		?	; 0004h PTR DEVICE_OBJECT
	Flags				DWORD		?	; 0008h

	; The following section describes where the driver is loaded.  The count
	; field is used to count the number of times the driver has had its
	; registered reinitialization routine invoked.

	DriverStart			PVOID		?	; 000Ch
	DriverSize			DWORD		?	; 0010h
	DriverSection		PVOID		?	; 0014h
	DriverExtension		PVOID		?	; 0018h PTR DRIVER_EXTENSION

	; The driver name field is used by the error log thread
	; determine the name of the driver that an I/O request is/was bound.

	DriverName			UNICODE_STRING <>	; 001Ch
	
	; The following section is for registry support.  Thise is a pointer
	; to the path to the hardware information in the registry

	HardwareDatabase	PVOID		?	; 0024h PTR UNICODE_STRING

	; The following section contains the optional pointer to an array of
	; alternate entry points to a driver for "fast I/O" support.  Fast I/O
	; is performed by invoking the driver routine directly with separate
	; parameters, rather than using the standard IRP call mechanism.  Note
	; that these functions may only be used for synchronous I/O, and when
	; the file is cached.

	FastIoDispatch		PVOID		?	; 0028h PTR FAST_IO_DISPATCH

	; The following section describes the entry points to this particular
	; driver.  Note that the major function dispatch table must be the last
	; field in the object so that it remains extensible.

	DriverInit			PVOID		?	; 002Ch
	DriverStartIo		PVOID		?	; 0030h
	DriverUnload		PVOID		?	; 0034h
	MajorFunction		PVOID		(IRP_MJ_MAXIMUM_FUNCTION + 1) dup(?)	; 0038h

DRIVER_OBJECT ENDS
PDRIVER_OBJECT typedef PTR DRIVER_OBJECT

; The following structure is pointed to by the SectionObject pointer field
; of a file object, and is allocated by the various NT file systems.

SECTION_OBJECT_POINTERS STRUCT		; sizeof = 0Ch
	DataSectionObject	PVOID		?	; 0000h
	SharedCacheMap		PVOID		?	; 0004h
	ImageSectionObject	PVOID		?	; 0008h
SECTION_OBJECT_POINTERS ENDS
PSECTION_OBJECT_POINTERS typedef PTR SECTION_OBJECT_POINTERS

; Define the format of a completion message.

IO_COMPLETION_CONTEXT STRUCT		; sizeof = 08h
	Port	PVOID		?	; 0000h
	Key		PVOID		?	; 0004h
IO_COMPLETION_CONTEXT ENDS
PIO_COMPLETION_CONTEXT typedef PTR IO_COMPLETION_CONTEXT

; Define File Object (FO) flags

FO_FILE_OPEN                    equ 00000001h
FO_SYNCHRONOUS_IO               equ 00000002h
FO_ALERTABLE_IO                 equ 00000004h
FO_NO_INTERMEDIATE_BUFFERING    equ 00000008h
FO_WRITE_THROUGH                equ 00000010h
FO_SEQUENTIAL_ONLY              equ 00000020h
FO_CACHE_SUPPORTED              equ 00000040h
FO_NAMED_PIPE                   equ 00000080h
FO_STREAM_FILE                  equ 00000100h
FO_MAILSLOT                     equ 00000200h
FO_GENERATE_AUDIT_ON_CLOSE      equ 00000400h
FO_DIRECT_DEVICE_OPEN           equ 00000800h
FO_FILE_MODIFIED                equ 00001000h
FO_FILE_SIZE_CHANGED            equ 00002000h
FO_CLEANUP_COMPLETE             equ 00004000h
FO_TEMPORARY_FILE               equ 00008000h
FO_DELETE_ON_CLOSE              equ 00010000h
FO_OPENED_CASE_SENSITIVE        equ 00020000h
FO_HANDLE_CREATED               equ 00040000h
FO_FILE_FAST_IO_READ            equ 00080000h
FO_RANDOM_ACCESS                equ 00100000h
FO_FILE_OPEN_CANCELLED          equ 00200000h
FO_VOLUME_OPEN                  equ 00400000h

FILE_OBJECT STRUCT		; sizeof = 070h
	_Type					SWORD		?		; 0000h  IO_TYPE_FILE  (original field name Type)
	_Size					SWORD		?		; 0002h  (original name Size)
	DeviceObject			PVOID		?		; 0004h  PTR DEVICE_OBJECT
	Vpb						PVOID		?		; 0008h  PTR VPB
	FsContext				PVOID		?		; 000Ch
	FsContext2				PVOID		?		; 0010h
	SectionObjectPointer	PVOID		?		; 0014h  PTR SECTION_OBJECT_POINTERS
	PrivateCacheMap			PVOID		?		; 0018h
	FinalStatus				SDWORD		?		; 001Ch
	RelatedFileObject		PVOID		?		; 0020h  PTR FILE_OBJECT
	LockOperation			BYTE		?		; 0024h  BOOLEAN
	DeletePending			BYTE		?		; 0025h  BOOLEAN
	ReadAccess				BYTE		?		; 0026h  BOOLEAN
	WriteAccess				BYTE		?		; 0027h  BOOLEAN
	DeleteAccess			BYTE		?		; 0028h  BOOLEAN
	SharedRead				BYTE		?		; 0029h  BOOLEAN
	SharedWrite				BYTE		?		; 002Ah  BOOLEAN
	SharedDelete			BYTE		?		; 002Bh  BOOLEAN
	Flags					DWORD		?		; 002Ch
	FileName				UNICODE_STRING	<>	; 0030h
	CurrentByteOffset		LARGE_INTEGER	<>	; 0038h
	Waiters					DWORD		?		; 0040h
	Busy					DWORD		?		; 0044h
	LastLock				PVOID		?		; 0048h
	_Lock					KEVENT 		<>		; 004Ch  (original name Lock)
	Event					KEVENT 		<>		; 005Ch
	CompletionContext		PVOID		?		; 006Ch  PTR IO_COMPLETION_CONTEXT
FILE_OBJECT ENDS
PFILE_OBJECT typedef PTR FILE_OBJECT

; Define I/O Request Packet (IRP) flags

IRP_NOCACHE                     equ 00000001h
IRP_PAGING_IO                   equ 00000002h
IRP_MOUNT_COMPLETION            equ 00000002h
IRP_SYNCHRONOUS_API             equ 00000004h
IRP_ASSOCIATED_IRP              equ 00000008h
IRP_BUFFERED_IO                 equ 00000010h
IRP_DEALLOCATE_BUFFER           equ 00000020h
IRP_INPUT_OPERATION             equ 00000040h
IRP_SYNCHRONOUS_PAGING_IO       equ 00000040h
IRP_CREATE_OPERATION            equ 00000080h
IRP_READ_OPERATION              equ 00000100h
IRP_WRITE_OPERATION             equ 00000200h
IRP_CLOSE_OPERATION             equ 00000400h

IRP_DEFER_IO_COMPLETION         equ 00000800h
IRP_OB_QUERY_NAME               equ 00001000h
IRP_HOLD_DEVICE_QUEUE           equ 00002000h
IRP_RETRY_IO_COMPLETION         equ 00004000h

; Define I/O request packet (IRP) alternate flags for allocation control.

IRP_QUOTA_CHARGED               equ 01h
IRP_ALLOCATED_MUST_SUCCEED      equ 02h
IRP_ALLOCATED_FIXED_SIZE        equ 04h
IRP_LOOKASIDE_ALLOCATION        equ 08h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; I/O Request Packet (IRP) definition
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_IRP STRUCT				; sizeof = 70h
	_Type				WORD	?						; 00h  (original field name Type)
	_Size				WORD	?						; 02h  (original field name Size)

	; Define the common fields used to control the IRP.

	; Define a pointer to the Memory Descriptor List (MDL) for this I/O
	; request.  This field is only used if the I/O is "direct I/O".

	MdlAddress			PVOID		?					; 04h PMDL

	; Flags word - used to remember various flags.

	Flags				DWORD		?					; 08h

	; The following union is used for one of three purposes:
	;
	; 1. This IRP is an associated IRP.  The field is a pointer to a master IRP.
	;
	; 2. This is the master IRP.  The field is the count of the number of
	; IRPs which must complete (associated IRPs) before the master can
	; complete.
	;
	; 3. This operation is being buffered and the field is the address of
	; the system space buffer.

	UNION AssociatedIrp
		MasterIrp		PVOID		?					; 0Ch PIRP
		IrpCount		DWORD		?					; 0Ch 
		SystemBuffer	PVOID		?					; 0Ch 
	ENDS	; AssociatedIrp

	; Thread list entry - allows queueing the IRP to the thread pending I/O
	; request packet list.

	ThreadListEntry		LIST_ENTRY	<>					; 10h

	; I/O status - final status of operation.

	IoStatus			IO_STATUS_BLOCK	<>				; 18h

	; Requestor mode - mode of the original requestor of this operation.

	RequestorMode		BYTE		?					; 20h KPROCESSOR_MODE

	; Pending returned - TRUE if pending was initially returned as the
	; status for this packet.

	PendingReturned		BYTE		?					; 21h  BOOLEAN

	; Stack state information.

	StackCount			BYTE		?					; 22h
	CurrentLocation		BYTE		?					; 23h

	; Cancel - packet has been canceled.

	Cancel				BYTE		?					; 24h  BOOLEAN

	; Cancel Irql - Irql at which the cancel spinlock was acquired.

	CancelIrql			BYTE		?					; 25h  KIRQL

	; ApcEnvironment - Used to save the APC environment at the time that the
	; packet was initialized.

	ApcEnvironment		BYTE		?					; 26h

	; Allocation control flags.

	AllocationFlags		BYTE		?					; 27h

	; User parameters.

	UserIosb			PIO_STATUS_BLOCK	?			; 28h
	UserEvent			PKEVENT				?			; 2Ch
	UNION Overlay										; 30h
		STRUCT AsynchronousParameters					; 30h
			UserApcRoutine	PVOID	?					; 30h PIO_APC_ROUTINE
			UserApcContext	PVOID	?					; 34h
		ENDS	; AsynchronousParameters
		AllocationSize		LARGE_INTEGER	<>			; 30h
	ENDS	; Overlay

	; CancelRoutine - Used to contain the address of a cancel routine supplied
	; by a device driver when the IRP is in a cancelable state.

	CancelRoutine		PVOID		?					; 38h PDRIVER_CANCEL

	; Note that the UserBuffer parameter is outside of the stack so that I/O
	; completion can copy data back into the user's address space without
	; having to know exactly which service was being invoked.  The length
	; of the copy is stored in the second half of the I/O status block. If
	; the UserBuffer field is NULL, then no copy is performed.

	UserBuffer			PVOID		?					; 3Ch

	; Kernel structures
	;
	; The following section contains kernel structures which the IRP needs
	; in order to place various work information in kernel controller system
	; queues.  Because the size and alignment cannot be controlled, they are
	; placed here at the end so they just hang off and do not affect the
	; alignment of other fields in the IRP.

	UNION Tail											; 40h
		STRUCT Overlay									; 40h
			UNION
				; DeviceQueueEntry - The device queue entry field is used to
				; queue the IRP to the device driver device queue.

				DeviceQueueEntry	KDEVICE_QUEUE_ENTRY	<>		; 40h

				STRUCT
					; The following are available to the driver to use in
					; whatever manner is desired, while the driver owns the
					; packet.
					DriverContext	PVOID	4 dup(?)			; 40h
				ENDS

			ENDS

			; Thread - pointer to caller's Thread Control Block.

			Thread				PVOID	?				; 50h PETHREAD

			; Auxiliary buffer - pointer to any auxiliary buffer that is
			; required to pass information to a driver that is not contained
			; in a normal buffer.

			AuxiliaryBuffer		PCHAR	?				; 54h

			; The following unnamed structure must be exactly identical
			; to the unnamed structure used in the minipacket header used
			; for completion queue entries.

			STRUCT

				; List entry - used to queue the packet to completion queue, among
				; others.

				ListEntry		LIST_ENTRY	<>			; 58h

				UNION

					; Current stack location - contains a pointer to the current
					; IO_STACK_LOCATION structure in the IRP stack.  This field
					; should never be directly accessed by drivers.  They should
					; use the standard functions.

					CurrentStackLocation	PVOID	?	; 60h PTR IO_STACK_LOCATION

					; Minipacket type.

					PacketType				DWORD	?	; 60h
				ENDS
			ENDS

			; Original file object - pointer to the original file object
			; that was used to open the file.  This field is owned by the
			; I/O system and should not be used by any other drivers.

			OriginalFileObject	PFILE_OBJECT	?		; 64h

		ENDS	; Overlay

		; APC - This APC control block is used for the special kernel APC as
		; well as for the caller's APC, if one was specified in the original
		; argument list.  If so, then the APC is reused for the normal APC for
		; whatever mode the caller was in and the "special" routine that is
		; invoked before the APC gets control simply deallocates the IRP.

		Apc		KAPC	<>								; 40h

		; CompletionKey - This is the key that is used to distinguish
		; individual I/O operations initiated on a single file handle.

		CompletionKey	PVOID	?						; 40h

	ENDS	;Tail

_IRP ENDS
PIRP typedef PTR _IRP

; Define stack location control flags

SL_PENDING_RETURNED				equ 01h
SL_INVOKE_ON_CANCEL				equ 20h
SL_INVOKE_ON_SUCCESS			equ 40h
SL_INVOKE_ON_ERROR				equ 80h

; Define flags for various functions

;
; Create / Create Named Pipe
;
; The following flags must exactly match those in the IoCreateFile call's
; options.  The case sensitive flag is added in later, by the parse routine,
; and is not an actual option to open.  Rather, it is part of the object
; manager's attributes structure.
;

SL_FORCE_ACCESS_CHECK			equ 01
SL_OPEN_PAGING_FILE				equ 02
SL_OPEN_TARGET_DIRECTORY		equ 04

SL_CASE_SENSITIVE				equ 80h

; Read / Write

SL_KEY_SPECIFIED				equ 01
SL_OVERRIDE_VERIFY_VOLUME		equ 02
SL_WRITE_THROUGH				equ 04
SL_FT_SEQUENTIAL_WRITE			equ 08


; Device I/O Control
;
; Same SL_OVERRIDE_VERIFY_VOLUME as for read/write above.
;
; Lock

SL_FAIL_IMMEDIATELY				equ 01
SL_EXCLUSIVE_LOCK				equ 02

; QueryDirectory / QueryEa / QueryQuota

SL_RESTART_SCAN					equ 01
SL_RETURN_SINGLE_ENTRY			equ 02
SL_INDEX_SPECIFIED				equ 04

; NotifyDirectory

SL_WATCH_TREE                   equ 01

; FileSystemControl
;
;    minor: mount/verify volume

SL_ALLOW_RAW_MOUNT              equ 01

; Define PNP/POWER types required by IRP_MJ_PNP/IRP_MJ_POWER.

;typedef enum _DEVICE_RELATION_TYPE {
    BusRelations				equ 0
    EjectionRelations			equ 1
    PowerRelations				equ 2
    RemovalRelations			equ 3
    TargetDeviceRelation		equ 4

DEVICE_RELATIONS STRUCT				; sizeof = 08h
	Count	DWORD		?			; 0000h
	Objects	PVOID		1 dup(?)	; 0004h PDEVICE_OBJECT (variable length)
DEVICE_RELATIONS ENDS
PDEVICE_RELATIONS typedef ptr DEVICE_RELATIONS

;typedef enum _DEVICE_USAGE_NOTIFICATION_TYPE {
    DeviceUsageTypeUndefined	equ 0
    DeviceUsageTypePaging		equ 1
    DeviceUsageTypeHibernation	equ 2
    DeviceUsageTypeDumpFile		equ 3

INTERFACE STRUCT		; sizeof = 010h
	_Size					WORD		?	; 0000h
	Version					WORD		?	; 0002h
	Context					PVOID		?	; 0004h
	InterfaceReference		PVOID		?	; 0008h PINTERFACE_REFERENCE
	InterfaceDereference	PVOID		?	; 000Ch PINTERFACE_DEREFERENCE
	; interface specific entries go here
INTERFACE ENDS
PINTERFACE typedef ptr INTERFACE


; All fields prefixed with 'dcf' because of record fields should be unique :-(
DEVICE_CAPABILITIES_FLAGS RECORD \
	dcfReserved:14,				; bits 18-31
	dcfNoDisplayInUI:1,
	dcfWarmEjectSupported:1,
	dcfNonDynamic:1,
	dcfHardwareDisabled:1,
	dcfWakeFromD3:1,
	dcfWakeFromD2:1,
	dcfWakeFromD1:1,
	dcfWakeFromD0:1,
	dcfSurpriseRemovalOK:1,
	dcfRawDeviceOK:1,
	dcfSilentInstall:1,
	dcfUniqueID:1,
	dcfDockDevice:1,
	dcfRemovable:1,
	dcfEjectSupported:1,
	dcfLockSupported:1,
	dcfDeviceD2:1,
	dcfDeviceD1:1
	
DEVICE_CAPABILITIES STRUCT									; sizeof = 040h
	_Size				WORD						?		; 0000h
	Version				WORD						?		; 0002h (the version documented here is version 1)
	Flags				DEVICE_CAPABILITIES_FLAGS	<>		; 0004h not named in original ntddk.h
	Address				DWORD						?		; 0008h
	UINumber			DWORD						?		; 000Ch
	DeviceState			SDWORD	PowerSystemMaximum dup(?)	; 0010h DEVICE_POWER_STATE
	SystemWake			SDWORD						?		; 002Ch SYSTEM_POWER_STATE
	DeviceWake			SDWORD						?		; 0030h DEVICE_POWER_STATE
	D1Latency			DWORD						?		; 0034h
	D2Latency			DWORD						?		; 0038h
	D3Latency			DWORD						?		; 003Ch
DEVICE_CAPABILITIES ENDS
PDEVICE_CAPABILITIES typedef ptr DEVICE_CAPABILITIES

POWER_SEQUENCE STRUCT		; sizeof = 0Ch
	SequenceD1	DWORD		?	; 0000h
	SequenceD2	DWORD		?	; 0004h
	SequenceD3	DWORD		?	; 0008h
POWER_SEQUENCE ENDS
PPOWER_SEQUENCE typedef ptr POWER_SEQUENCE

;typedef enum {
    BusQueryDeviceID			equ 0	; <Enumerator>\<Enumerator-specific device id>
    BusQueryHardwareIDs			equ 1	; Hardware ids
    BusQueryCompatibleIDs		equ 2	; compatible device ids
    BusQueryInstanceID			equ 3	; persistent id for this instance of the device
    BusQueryDeviceSerialNumber	equ 4	; serial number for this device
;} BUS_QUERY_ID_TYPE, *PBUS_QUERY_ID_TYPE;

PNP_DEVICE_STATE	typedef DWORD
PPNP_DEVICE_STATE	typedef PTR DWORD

PNP_DEVICE_DISABLED                      equ 00000001
PNP_DEVICE_DONT_DISPLAY_IN_UI            equ 00000002
PNP_DEVICE_FAILED                        equ 00000004
PNP_DEVICE_REMOVED                       equ 00000008
PNP_DEVICE_RESOURCE_REQUIREMENTS_CHANGED equ 00000010h
PNP_DEVICE_NOT_DISABLEABLE               equ 00000020h

;typedef enum {
    DeviceTextDescription			equ 0	; DeviceDesc property
    DeviceTextLocationInformation	equ 1	; DeviceLocation property
;} DEVICE_TEXT_TYPE, *PDEVICE_TEXT_TYPE;


; Define I/O Request Packet (IRP) stack locations

IO_STACK_LOCATION STRUCT			; sizeof = 24h
	MajorFunction		BYTE	?	; 00h
	MinorFunction		BYTE	?	; 01h
	Flags				BYTE	?	; 02h
	Control				BYTE	?	; 03h

	; The following user parameters are based on the service that is being
	; invoked.  Drivers and file systems can determine which set to use based
	; on the above major and minor function codes.

	union Parameters				; 04h

		;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		; Some fields of this union are commented out because
		; it's takes too long for masm to deal with such complex structure :-(
		;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

		; System service parameters for:  NtCreateFile

		struct Create				; 04h
			SecurityContext		PVOID	?	; 04h PTR IO_SECURITY_CONTEXT
			Options				DWORD	?	; 08h
			; if _WIN64
			;	POINTER_ALIGNMENT DECLSPEC_ALIGN(8)
			; endif
			FileAttributes		WORD	?	; 0Ch POINTER_ALIGNMENT
			ShareAccess			WORD	?	; 0Eh
			EaLength			DWORD	?	; 10h POINTER_ALIGNMENT
		ends

		; System service parameters for:  NtReadFile

		struct Read							; 04h
			_Length				DWORD	?	; 04h
			Key					DWORD	?	; 08h POINTER_ALIGNMENT
			ByteOffset			LARGE_INTEGER	<>	; 0Ch
		ends

		; System service parameters for:  NtWriteFile

		struct Write						; 04h
			_Length				DWORD	?	; 04h
			Key					DWORD	?	; 08h POINTER_ALIGNMENT
			ByteOffset			LARGE_INTEGER	<>	; 0Ch
		ends

		; System service parameters for:  NtQueryInformationFile

		struct QueryFile
			_Length					DWORD	?	; 04h original field name Length
			FileInformationClass	DWORD	?	; 08h POINTER_ALIGNMENT
		ends

		; System service parameters for:  NtSetInformationFile

		struct SetFile
			_Length					DWORD	?
			FileInformationClass	DWORD	?	; FILE_INFORMATION_CLASS
			FileObject				PVOID	?	; PFILE_OBJECT
			union
				struct
					ReplaceIfExists	BOOLEAN	?
					AdvanceOnly		BOOLEAN	?
				ends
					ClusterCount	DWORD	?
					DeleteHandle	DWORD	?	; HANDLE
			ends
		ends

		; System service parameters for:  NtQueryVolumeInformationFile

		struct QueryVolume
			_Length					DWORD	?
			FsInformationClass		DWORD	?	; FS_INFORMATION_CLASS
		ends

		; System service parameters for:  NtFlushBuffersFile
		;
		; No extra user-supplied parameters.

		; System service parameters for:  NtDeviceIoControlFile
		; 
		; Note that the user's output buffer is stored in the UserBuffer field
		; and the user's input buffer is stored in the SystemBuffer field.

		struct DeviceIoControl		; 04h
			OutputBufferLength		DWORD	?	; 04h
			InputBufferLength		DWORD	?	; 08h POINTER_ALIGNMENT
			IoControlCode			DWORD	?	; 0Ch POINTER_ALIGNMENT
			Type3InputBuffer		PVOID	?	; 10h
		ends
comment ^
		; System service parameters for:  NtQuerySecurityObject

		struct QuerySecurity
			SecurityInformation		DWORD	?	; SECURITY_INFORMATION
			_Length					DWORD	?
		ends

		; System service parameters for:  NtSetSecurityObject

		struct SetSecurity
			SecurityInformation		DWORD	?	; SECURITY_INFORMATION
			SecurityDescriptor		PVOID	?	; PSECURITY_DESCRIPTOR
		ends

		; Non-system service parameters.
		;
		; Parameters for MountVolume

		struct MountVolume
			Vpb						PVOID	?	; PVPB
			DeviceObject			PVOID	?	; PDEVICE_OBJECT
		ends

		; Parameters for VerifyVolume

		struct VerifyVolume
			Vpb						PVOID	?	; PVPB
			DeviceObject			PVOID	?	; PDEVICE_OBJECT
		ends

		; Parameters for Scsi with internal device contorl.

		struct Scsi
			Srb						PVOID	?	; PSCSI_REQUEST_BLOCK
		ends
^
		; Parameters for IRP_MN_QUERY_DEVICE_RELATIONS

		struct QueryDeviceRelations
			_Type					DWORD	?	; DEVICE_RELATION_TYPE
		ends

		; Parameters for IRP_MN_QUERY_INTERFACE

		struct QueryInterface
			InterfaceType			DWORD	?	; PGUID
			_Size					WORD	?
			Version					WORD	?
			Interface				PVOID	?	; PINTERFACE
			InterfaceSpecificData	PVOID	?
		ends

		; Parameters for IRP_MN_QUERY_CAPABILITIES

		struct DeviceCapabilities
			Capabilities			PVOID	?	; PDEVICE_CAPABILITIES
		ends

		; Parameters for IRP_MN_FILTER_RESOURCE_REQUIREMENTS

		struct FilterResourceRequirements
			IoResourceRequirementList	PVOID	?	; PIO_RESOURCE_REQUIREMENTS_LIST
		ends

		; Parameters for IRP_MN_READ_CONFIG and IRP_MN_WRITE_CONFIG

		struct ReadWriteConfig			; 04h
			WhichSpace				DWORD	?	; 04h
			Buffer					PVOID	?	; 08h
			_Offset					DWORD	?	; 0Ch
			_Length					DWORD	?	; 10h POINTER_ALIGNMENT (original field name Length)
		ends

		; Parameters for IRP_MN_SET_LOCK

		struct SetLock					; 04h
			_Lock					BOOLEAN	?	; 04h (original name Lock)
									db 3 dup(?)	; padding ???
		ends

		; Parameters for IRP_MN_QUERY_ID

		struct QueryId
			IdType					DWORD	?
		ends

		; Parameters for IRP_MN_QUERY_DEVICE_TEXT

		struct QueryDeviceText
			DeviceTextType			DWORD	?	; DEVICE_TEXT_TYPE
			LocaleId				DWORD	?	; LCID
		ends

		; Parameters for IRP_MN_DEVICE_USAGE_NOTIFICATION

		struct UsageNotification
			InPath					BOOLEAN	?
			Reserved				BOOLEAN 3 dup(?)
			_Type					DWORD	?	; DEVICE_USAGE_NOTIFICATION_TYPE
		ends 
comment ^
		; Parameters for IRP_MN_WAIT_WAKE

		struct WaitWake
			PowerState				DWORD	?	; SYSTEM_POWER_STATE
		ends

		; Parameter for IRP_MN_POWER_SEQUENCE

		struct PowerSequence
			PowerSequence			PVOID	?	; PPOWER_SEQUENCE
		ends

		; Parameters for IRP_MN_SET_POWER and IRP_MN_QUERY_POWER

		struct Power
			SystemContext			DWORD	?
			_Type					DWORD	?	; POWER_STATE_TYPE
			State					POWER_STATE	<>
			ShutdownType			DWORD	?	; POWER_ACTION
		ends
^
		; Parameters for StartDevice

		struct StartDevice
            PCM_RESOURCE_LIST AllocatedResources;
            PCM_RESOURCE_LIST AllocatedResourcesTranslated;
		ends

		; Parameters for Cleanup
		;
		; No extra parameters supplied
comment ^
		; WMI Irps

		struct WMI
			ProviderId	DWORD			?	; 04h  ULONG_PTR
			DataPath	PVOID			?	; 08h
			BufferSize	DWORD			?	; 0Ch
			Buffer		PVOID			?	; 10h
		ends

		; Others - driver-specific

		struct Others
			Argument1	PVOID			?	; 04h
			Argument2	PVOID			?	; 08h
			Argument3	PVOID			?	; 0Ch
			Argument4	PVOID			?	; 10h
		ends
^
	ends	; Parameters

	; Save a pointer to this device driver's device object for this request
	; so it can be passed to the completion routine if needed.

	DeviceObject		PDEVICE_OBJECT	?	; 14h

	; The following location contains a pointer to the file object for this

	FileObject			PFILE_OBJECT	?	; 18h

	; The following routine is invoked depending on the flags in the above
	; flags field.

	CompletionRoutine	PVOID			?	; 1Ch PIO_COMPLETION_ROUTINE

	; The following is used to store the address of the context parameter
	; that should be passed to the CompletionRoutine.

	Context				PVOID			?	; 20h

IO_STACK_LOCATION ENDS
PIO_STACK_LOCATION typedef PTR IO_STACK_LOCATION

; +
; PIO_STACK_LOCATION IoGetCurrentIrpStackLocation( IN PIRP Irp )
;
; Macro Description:
;
;     This macro is invoked to return a pointer to the current stack location
;     in an I/O Request Packet (IRP).
;
; Arguments:
;
;     Irp - Pointer to the I/O Request Packet.
;
; Return Value:
;
;     The function value is a pointer to the current stack location in the
;     packet.
; -

IoGetCurrentIrpStackLocation MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	mov eax, (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation
ENDM

$IoGetCurrentIrpStackLocation MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	mov eax, (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation
	exitm <eax>
ENDM

; The following function is used to tell the caller how much stack is available

IoGetRemainingStackSize MACRO

;    IoGetStackLimits( &Bottom, &Top );
;    return((ULONG_PTR)(&Top) - Bottom );

	push eax
	mov ecx, esp
	push eax
	invoke IoGetStackLimits, ecx, esp
	pop eax
	pop ecx
	sub eax, ecx

ENDM

; +
;
; PIO_STACK_LOCATION
; IoGetNextIrpStackLocation( IN PIRP Irp )
;
; Macro Description:
;
;     This routine is invoked to return a pointer to the next stack location
;     in an I/O Request Packet (IRP).
;
; Arguments:
;
;     Irp - Pointer to the I/O Request Packet.
;
; Return Value:
;
;     The function value is a pointer to the next stack location in the packet.
;
; -

IoGetNextIrpStackLocation MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	mov eax, (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation
	sub eax, sizeof IO_STACK_LOCATION
ENDM

$IoGetNextIrpStackLocation MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	mov eax, (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation
	sub eax, sizeof IO_STACK_LOCATION
	exitm <eax>
ENDM

; +
;
; VOID
; IoMarkIrpPending(
;     IN OUT PIRP Irp
;     )
;
; Routine Description:
;
;     This routine marks the specified I/O Request Packet (IRP) to indicate
;     that an initial status of STATUS_PENDING was returned to the caller.
;     This is used so that I/O completion can determine whether or not to
;     fully complete the I/O operation requested by the packet.
;
; Arguments:
;
;     Irp - Pointer to the I/O Request Packet to be marked pending.
;
; Return Value:
;
;     None.
;
; -

;#define IoMarkIrpPending( Irp ) ( \
;    IoGetCurrentIrpStackLocation( (Irp) )->Control |= SL_PENDING_RETURNED )

IoMarkIrpPending MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	mov eax, (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation
	or (IO_STACK_LOCATION PTR [eax]).Control, SL_PENDING_RETURNED
ENDM

; +
;
; VOID
; IoSetCompletionRoutine(
;     IN PIRP Irp,
;     IN PIO_COMPLETION_ROUTINE CompletionRoutine,
;     IN PVOID Context,
;     IN BOOLEAN InvokeOnSuccess,
;     IN BOOLEAN InvokeOnError,
;     IN BOOLEAN InvokeOnCancel
;     )
;
; Routine Description:
;
;     This routine is invoked to set the address of a completion routine which
;     is to be invoked when an I/O packet has been completed by a lower-level
;     driver.
;
; Arguments:
;
;     Irp - Pointer to the I/O Request Packet itself.
;
;     CompletionRoutine - Address of the completion routine that is to be
;         invoked once the next level driver completes the packet.
;
;     Context - Specifies a context parameter to be passed to the completion
;         routine.
;
;     InvokeOnSuccess - Specifies that the completion routine is invoked when the
;         operation is successfully completed.
;
;     InvokeOnError - Specifies that the completion routine is invoked when the
;         operation completes with an error status.
;
;     InvokeOnCancel - Specifies that the completion routine is invoked when the
;         operation is being canceled.
;
; Return Value:
;
;     None.
;
; -

;#define IoSetCompletionRoutine( Irp, Routine, CompletionContext, Success, Error, Cancel ) { \
;    PIO_STACK_LOCATION irpSp;                                               \
;    ASSERT( (Success) | (Error) | (Cancel) ? (Routine) != NULL : TRUE );    \
;    irpSp = IoGetNextIrpStackLocation( (Irp) );                             \
;    irpSp->CompletionRoutine = (Routine);                                   \
;    irpSp->Context = (CompletionContext);                                   \
;    irpSp->Control = 0;                                                     \
;    if ((Success)) { irpSp->Control = SL_INVOKE_ON_SUCCESS; }               \
;    if ((Error)) { irpSp->Control |= SL_INVOKE_ON_ERROR; }                  \
;    if ((Cancel)) { irpSp->Control |= SL_INVOKE_ON_CANCEL; } }

IoSetCompletionRoutine MACRO pIrp:REQ, Routine:REQ, CompletionContext:REQ, Success:REQ, Error:REQ, Cancel:REQ
local _Control, line, adr
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	; IoGetNextIrpStackLocation
	mov eax, (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation
	sub eax, sizeof IO_STACK_LOCATION

;    irpSp->CompletionRoutine = (Routine);                                   \

	IF $IsAddr(Routine)
		adr SUBSTR <Routine>, 6
		IF (OPATTR (adr)) AND 00000001y
			;; References a code label
			mov (IO_STACK_LOCATION PTR [eax]).CompletionRoutine, adr
		ELSEIF (OPATTR (adr)) AND 00000010y
			;; is a memory variable or has a relocatable data label
			push adr
			pop (IO_STACK_LOCATION PTR [eax]).CompletionRoutine
		ELSE
			line TEXTEQU %@Line
			.ERR
			% ECHO @FileCur(line) : ERROR! Pointer to CompletionRoutine improperly specified.
		ENDIF
	ELSEIF (OPATTR (Routine)) AND 00000001y
		;; References a code label
		mov (IO_STACK_LOCATION PTR [eax]).CompletionRoutine, Routine
	ELSEIF (OPATTR (Routine)) AND 00010000y
		;; is a register value
		IFIDNI <Routine>, <eax>		;; is it eax
			line TEXTEQU %@Line
			.ERR
			% ECHO @FileCur(line) : ERROR! EAX register value overwritten by IoSetCompletionRoutine macro.
		ENDIF
		mov (IO_STACK_LOCATION PTR [eax]).CompletionRoutine, Routine
	ELSEIF (OPATTR (Routine)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push Routine
		pop (IO_STACK_LOCATION PTR [eax]).CompletionRoutine
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! Pointer to CompletionRoutine improperly specified.
	ENDIF
		
;    irpSp->Context = (CompletionContext);                                   \

	IF $IsAddr(CompletionContext)
		adr SUBSTR <CompletionContext>, 6
		IF (OPATTR (adr)) AND 01000000y		;; $IsStack(adr)
			;; relative to SS
			lea ecx, adr
		ELSE
			mov ecx, offset adr
		ENDIF
		mov (IO_STACK_LOCATION PTR [eax]).Context, ecx
	ELSEIF (OPATTR (CompletionContext)) AND 00010000y
		;; is a register value
		IFIDNI CompletionContext, <eax>		;; is it eax
			line TEXTEQU %@Line
			.ERR
			% ECHO @FileCur(line) : ERROR! EAX register value overwritten by IoSetCompletionRoutine macro.
		ENDIF
		mov (IO_STACK_LOCATION PTR [eax]).Context, CompletionContext
	ELSEIF (OPATTR (CompletionContext)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push CompletionContext
		pop (IO_STACK_LOCATION PTR [eax]).Context
	ELSEIF (OPATTR (CompletionContext)) AND 00000100y	;; $IsImm(CompletionContext)
		;; Is an immediate value
		IF CompletionContext EQ 0
			and (IO_STACK_LOCATION PTR [eax]).Context, 0	;; NULL
		ELSE
			mov (IO_STACK_LOCATION PTR [eax]).Context, CompletionContext
		ENDIF
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR! Context improperly specified.
	ENDIF
	
	_Control = 0
	
;    if ((Success)) { irpSp->Control = SL_INVOKE_ON_SUCCESS; }               \

	IF (OPATTR (Success)) AND 00000100y	;; ELSEIF $IsImm(Success)
		IF Success NE 0
			_Control = _Control or SL_INVOKE_ON_SUCCESS
		ENDIF
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR in IoSetCompletionRoutine macro! Success should be immediate value.
	ENDIF

;    if ((Error)) { irpSp->Control |= SL_INVOKE_ON_ERROR; }                  \

	IF (OPATTR (Error)) AND 00000100y		;; ELSEIF $IsImm(Success)
		IF Error NE 0
			_Control = _Control or SL_INVOKE_ON_ERROR
		ENDIF
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR in IoSetCompletionRoutine macro! Error should be immediate value.
	ENDIF

;    if ((Cancel)) { irpSp->Control |= SL_INVOKE_ON_CANCEL; } }

	IF (OPATTR (Cancel)) AND 00000100y		;; ELSEIF $IsImm(Success)
		IF Cancel NE 0
			_Control = _Control or SL_INVOKE_ON_CANCEL
		ENDIF
	ELSE
		line TEXTEQU %@Line
		.ERR
		% ECHO @FileCur(line) : ERROR in IoSetCompletionRoutine macro! Cancel should be immediate value.
	ENDIF

	IF _Control EQ 0
		and (IO_STACK_LOCATION PTR [eax]).Control, 0	;; NULL
	ELSE
		mov (IO_STACK_LOCATION PTR [eax]).Control, _Control
	ENDIF

ENDM




; +
;
; VOID
; IoSetNextIrpStackLocation ( IN OUT PIRP Irp )
;
; Macro Description:
;
;     This routine is invoked to set the current IRP stack location to
;     the next stack location, i.e. it "pushes" the stack.
;
; Arguments:
;
;     Irp - Pointer to the I/O Request Packet (IRP).
;
; Return Value:
;
;     None.
;
; -

IoSetNextIrpStackLocation MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	dec (_IRP PTR [eax]).CurrentLocation
	sub (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation, sizeof IO_STACK_LOCATION
ENDM

; +
;
; VOID
; IoCopyCurrentIrpStackLocationToNext( IN PIRP Irp )
;
; Macro Description:
;
;     This routine is invoked to copy the IRP stack arguments and file
;     pointer from the current IrpStackLocation to the next
;     in an I/O Request Packet (IRP).
;
;     If the caller wants to call IoCallDriver with a completion routine
;     but does not wish to change the arguments otherwise,
;     the caller first calls IoCopyCurrentIrpStackLocationToNext,
;     then IoSetCompletionRoutine, then IoCallDriver.
;
; Arguments:
;
;     Irp - Pointer to the I/O Request Packet.
;
; Return Value:
;
;     None.
;
; -

IoCopyCurrentIrpStackLocationToNext MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	push esi
	push edi
	; esi = IoGetCurrentIrpStackLocation(Irp);
	mov esi, (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation
	; ecx = FIELD_OFFSET(IO_STACK_LOCATION, CompletionRoutine));
	;mov ecx, IO_STACK_LOCATION.CompletionRoutine
	;db 0F3h, 0A4h				; repe movsb
	push 7
	pop ecx
	; eax = IoGetNextIrpStackLocation(Irp);
	lea edx, [esi][-sizeof IO_STACK_LOCATION]
	mov edi, edx
	rep movsd
	; [edx]->Control = 0;
	and (IO_STACK_LOCATION PTR [edx]).Control, 0
	pop edi
	pop esi
ENDM

;#define IoCopyCurrentIrpStackLocationToNext( Irp ) { \
;    PIO_STACK_LOCATION irpSp; \
;    PIO_STACK_LOCATION nextIrpSp; \
;    irpSp = IoGetCurrentIrpStackLocation( (Irp) ); \
;    nextIrpSp = IoGetNextIrpStackLocation( (Irp) ); \
;    RtlCopyMemory( nextIrpSp, irpSp, FIELD_OFFSET(IO_STACK_LOCATION, CompletionRoutine)); \
;    nextIrpSp->Control = 0; }

;++
;
; VOID IoSkipCurrentIrpStackLocation ( IN PIRP Irp )
;
; Macro Description:
;
;     This routine is invoked to increment the current stack location of
;     a given IRP.
;
;     If the caller wishes to call the next driver in a stack, and does not
;     wish to change the arguments, nor does he wish to set a completion
;     routine, then the caller first calls IoSkipCurrentIrpStackLocation
;     and the calls IoCallDriver.
;
; Arguments:
;
;     Irp - Pointer to the I/O Request Packet.
;
; Return Value:
;
;     None
;
;--

IoSkipCurrentIrpStackLocation MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	inc (_IRP PTR [eax]).CurrentLocation
	add (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation, sizeof IO_STACK_LOCATION
ENDM

IO_REMOVE_LOCK_COMMON_BLOCK STRUCT
	Removed		BOOLEAN	?
	Reserved	BOOLEAN	3 dup(?)
	IoCount		SDWORD	?
	RemoveEvent	KEVENT	<>
IO_REMOVE_LOCK_COMMON_BLOCK ENDS
PIO_REMOVE_LOCK_COMMON_BLOCK typedef ptr IO_REMOVE_LOCK_COMMON_BLOCK

IO_REMOVE_LOCK_DBG_BLOCK STRUCT		; Used only on checked build
	Signature		SDWORD		?
	HighWatermark	SDWORD		?
	MaxLockedTicks	QWORD		?
	AllocateTag		SDWORD		?
	LockList		LIST_ENTRY	<>
	Spin			KSPIN_LOCK	<>
	LowMemoryCount	SDWORD		?
	Reserved1		DWORD 4 dup(?)
	Reserved2		PVOID		?
	Blocks			PVOID		?	; PIO_REMOVE_LOCK_TRACKING_BLOCK
IO_REMOVE_LOCK_DBG_BLOCK ENDS
PIO_REMOVE_LOCK_DBG_BLOCK typedef ptr IO_REMOVE_LOCK_DBG_BLOCK

IO_REMOVE_LOCK STRUCT
	Common	IO_REMOVE_LOCK_COMMON_BLOCK	<>
IFDEF DBG
	Dbg		IO_REMOVE_LOCK_DBG_BLOCK	<>
ENDIF
IO_REMOVE_LOCK ENDS
PIO_REMOVE_LOCK typedef ptr IO_REMOVE_LOCK

PIO_WORKITEM typedef PTR	; typedef struct _IO_WORKITEM *PIO_WORKITEM;

; Define PnP Device Property for IoGetDeviceProperty

;typedef enum { _DEVICE_REGISTRY_PROPERTY
    DevicePropertyDeviceDescription				equ 0
    DevicePropertyHardwareID					equ 1
    DevicePropertyCompatibleIDs					equ 2
    DevicePropertyBootConfiguration				equ 3
    DevicePropertyBootConfigurationTranslated	equ 4
    DevicePropertyClassName						equ 5
    DevicePropertyClassGuid						equ 6
    DevicePropertyDriverKeyName					equ 7
    DevicePropertyManufacturer					equ 8
    DevicePropertyFriendlyName					equ 9
    DevicePropertyLocationInformation			equ 10
    DevicePropertyPhysicalDeviceObjectName		equ 11
    DevicePropertyBusTypeGuid					equ 12
    DevicePropertyLegacyBusType					equ 13
    DevicePropertyBusNumber						equ 14
    DevicePropertyEnumeratorName				equ 15
    DevicePropertyAddress						equ 16
    DevicePropertyUINumber						equ 17

ENDIF ; _NTDDK_

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; These macros are not a part of original ntddk.h file of course
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFNDEF @ArgRev
@ArgRev MACRO arglist
    LOCAL txt, arg
    txt TEXTEQU <>
%   FOR arg, <arglist>
        txt CATSTR <arg>, <!,>, txt
    ENDM
    txt SUBSTR  txt, 1, @SizeStr(%txt) - 1
    txt CATSTR  <!<>, txt, <!>>
    EXITM txt
ENDM
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; Added for supporting fastcall calling convention
;
; Usage Example:
;   	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT
;
; addr is not supported
; offset is not supported for first and second param
; May be next time
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fastcall MACRO api:REQ, p1, p2, px:VARARG

local arg, line, recx

	recx = 0		

	IFNB <px>
		% FOR arg, @ArgRev( <px> )
			push arg
		ENDM
	ENDIF

	IFNB <p1>
		IF (OPATTR (p1)) AND 00010000y	;; Is a register value
			IFDIFI <p1>, <ecx>			;; do not move ecx onto itself
				mov ecx, p1
				recx = 1				;; no more ecx
			ENDIF
		ELSEIF (OPATTR (p1)) AND 00000100y	;; Is an immediate value
			IF p1 EQ 0
				xor ecx, ecx
			ELSEIF p1 EQ 1
				xor ecx, ecx
				inc ecx
			ELSEIF p1 EQ -1
				or ecx, -1
				;;xor ecx, ecx
				;;dec ecx
			ELSE
				mov ecx, p1
				recx = 1				;; no more ecx
			ENDIF
		ELSE
			mov ecx, p1
			recx = 1					;; no more ecx
		ENDIF

		IFNB <p2>
			IF (OPATTR (p2)) AND 00010000y	;; Is a register value
				IFIDNI <p2>, <ecx>
					IF recx							;; if ecx was used report error
						line TEXTEQU %@Line
						% ECHO @FileCur(line) : ERROR! ECX register value overwritten by fastcall macro.
						.ERR
					ENDIF
				ENDIF

				IFDIFI <p2>, <edx>		;; do not move edx onto itself
					mov edx, p2
				ENDIF
			ELSEIF (OPATTR (p2)) AND 00000100y	;; Is an immediate value
				IF p2 EQ 0
					xor edx, edx
				ELSEIF p2 EQ 1
					xor edx, edx
					inc edx
				ELSEIF p2 EQ -1
					or edx, -1
					;;xor edx, edx
					;;dec edx
				ELSE
					mov edx, p2
				ENDIF
			ELSE
				mov edx, p2
			ENDIF
		ENDIF
	ENDIF
	call api
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; This helper macro is also not a part of original ntddk.h of course
; It helps me much never forget what i have to improve in source code of driver
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
