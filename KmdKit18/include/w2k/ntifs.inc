comment ^
Module Name:
    ntifs.inc

Abstract:
    This module defines the NT types, constants, and functions that are
    exposed to file system drivers.

Author:
    Four-F (four-f@mail.ru)

Last update:
	06-July-2004

IMPORTANT:
	Hand made -> Bugs are very possible :(
	Your bug report is very welcome.

Comments:
    Some fields name have been changed because of collision with masm reserved words.
    All such words are prefixed with underscore ('_') symbol.
^

IFNDEF _NTIFS_
_NTIFS_ equ 1
comment ^
#define NT_INCLUDED
#define _NTMSV1_0_
#define _CTYPE_DISABLE_MACROS

#include <excpt.h>
^
include ntdef.inc

; end_winnt

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

; begin_ntndis

; Processor modes.

KPROCESSOR_MODE typedef BYTE

;typedef enum _MODE {
	KernelMode	equ 0
	UserMode	equ 1
	MaximumMode	equ 2

; end_ntndis

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

; begin_ntndis

; +
; Define DPC importance.
;
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

MDL_MAPPED_TO_SYSTEM_VA     equ 0001
MDL_PAGES_LOCKED            equ 0002
MDL_SOURCE_IS_NONPAGED_POOL equ 0004
MDL_ALLOCATED_FIXED_SIZE    equ 0008
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

; end_ntndis


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

; begin_ntndis begin_winnt

KSPIN_LOCK	typedef DWORD	; ULONG_PTR
PKSPIN_LOCK	typedef PTR DWORD

; end_ntndis end_winnt

;ifndef _PO_DDK_
;define _PO_DDK_

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

; begin_winnt

ES_SYSTEM_REQUIRED	equ 00000001
ES_DISPLAY_REQUIRED	equ 00000002
ES_USER_PRESENT		equ 00000004
ES_CONTINUOUS		equ 80000000h

EXECUTION_STATE typedef DWORD

;typedef enum _LATENCY_TIME {
    LT_DONT_CARE 		equ 0
    LT_LOWEST_LATENCY	equ 1

;endif // !_PO_DDK_

; Define the various device type values.  Note that values used by Microsoft
; Corporation are in the range 0-32767, and 32768-65535 are reserved for use
; by customers.

DEVICE_TYPE	typedef	DWORD

FILE_DEVICE_BEEP                equ 00000001
FILE_DEVICE_CD_ROM              equ 00000002
FILE_DEVICE_CD_ROM_FILE_SYSTEM  equ 00000003
FILE_DEVICE_CONTROLLER          equ 00000004
FILE_DEVICE_DATALINK            equ 00000005
FILE_DEVICE_DFS                 equ 00000006
FILE_DEVICE_DISK                equ 00000007
FILE_DEVICE_DISK_FILE_SYSTEM    equ 00000008
FILE_DEVICE_FILE_SYSTEM         equ 00000009
FILE_DEVICE_INPORT_PORT         equ 0000000ah
FILE_DEVICE_KEYBOARD            equ 0000000bh
FILE_DEVICE_MAILSLOT            equ 0000000ch
FILE_DEVICE_MIDI_IN             equ 0000000dh
FILE_DEVICE_MIDI_OUT            equ 0000000eh
FILE_DEVICE_MOUSE               equ 0000000fh
FILE_DEVICE_MULTI_UNC_PROVIDER  equ 00000010h
FILE_DEVICE_NAMED_PIPE          equ 00000011h
FILE_DEVICE_NETWORK             equ 00000012h
FILE_DEVICE_NETWORK_BROWSER     equ 00000013h
FILE_DEVICE_NETWORK_FILE_SYSTEM equ 00000014h
FILE_DEVICE_NULL                equ 00000015h
FILE_DEVICE_PARALLEL_PORT       equ 00000016h
FILE_DEVICE_PHYSICAL_NETCARD    equ 00000017h
FILE_DEVICE_PRINTER             equ 00000018h
FILE_DEVICE_SCANNER             equ 00000019h
FILE_DEVICE_SERIAL_MOUSE_PORT   equ 0000001ah
FILE_DEVICE_SERIAL_PORT         equ 0000001bh
FILE_DEVICE_SCREEN              equ 0000001ch
FILE_DEVICE_SOUND               equ 0000001dh
FILE_DEVICE_STREAMS             equ 0000001eh
FILE_DEVICE_TAPE                equ 0000001fh
FILE_DEVICE_TAPE_FILE_SYSTEM    equ 00000020h
FILE_DEVICE_TRANSPORT           equ 00000021h
FILE_DEVICE_UNKNOWN             equ 00000022h
FILE_DEVICE_VIDEO               equ 00000023h
FILE_DEVICE_VIRTUAL_DISK        equ 00000024h
FILE_DEVICE_WAVE_IN             equ 00000025h
FILE_DEVICE_WAVE_OUT            equ 00000026h
FILE_DEVICE_8042_PORT           equ 00000027h
FILE_DEVICE_NETWORK_REDIRECTOR  equ 00000028h
FILE_DEVICE_BATTERY             equ 00000029h
FILE_DEVICE_BUS_EXTENDER        equ 0000002ah
FILE_DEVICE_MODEM               equ 0000002bh
FILE_DEVICE_VDM                 equ 0000002ch
FILE_DEVICE_MASS_STORAGE        equ 0000002dh
FILE_DEVICE_SMB                 equ 0000002eh
FILE_DEVICE_KS                  equ 0000002fh
FILE_DEVICE_CHANGER             equ 00000030h
FILE_DEVICE_SMARTCARD           equ 00000031h
FILE_DEVICE_ACPI                equ 00000032h
FILE_DEVICE_DVD                 equ 00000033h
FILE_DEVICE_FULLSCREEN_VIDEO    equ 00000034h
FILE_DEVICE_DFS_FILE_SYSTEM     equ 00000035h
FILE_DEVICE_DFS_VOLUME          equ 00000036h
FILE_DEVICE_SERENUM             equ 00000037h
FILE_DEVICE_TERMSRV             equ 00000038h
FILE_DEVICE_KSEC                equ 00000039h

; Macro definition for defining IOCTL and FSCTL function control codes.  Note
; that function codes 0-2047 are reserved for Microsoft Corporation, and
; 2048-4095 are reserved for customers.

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

; Macro to extract device type out of the device io control code

IFNDEF DEVICE_TYPE_FROM_CTL_CODE
DEVICE_TYPE_FROM_CTL_CODE MACRO ctrlCode:REQ
	EXITM %(((ctrlCode) AND 0ffff0000h) SHR 16)
ENDM
ENDIF

; Define the method codes for how buffers are passed for I/O and FS controls

METHOD_BUFFERED                 equ 0
METHOD_IN_DIRECT                equ 1
METHOD_OUT_DIRECT               equ 2
METHOD_NEITHER                  equ 3

; +
; Define the access check value for any access
;
; The FILE_READ_ACCESS and FILE_WRITE_ACCESS constants are also defined in
; ntioapi.h as FILE_READ_DATA and FILE_WRITE_DATA. The values for these
; constants *MUST* always be in sync.
;
; FILE_SPECIAL_ACCESS is checked by the NT I/O system the same as FILE_ANY_ACCESS.
; The file systems, however, may add additional access checks for I/O and FS controls
; that use this value.
; -

FILE_ANY_ACCESS			equ 0
FILE_SPECIAL_ACCESS		equ FILE_ANY_ACCESS
FILE_READ_ACCESS		equ 1	; file & pipe
FILE_WRITE_ACCESS		equ 2	; file & pipe

; begin_winnt

; Define access rights to files and directories

; The FILE_READ_DATA and FILE_WRITE_DATA constants are also defined in
; devioctl.h as FILE_READ_ACCESS and FILE_WRITE_ACCESS. The values for these
; constants *MUST* always be in sync.
; The values are redefined in devioctl.h because they must be available to
; both DOS and NT.

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

; end_winnt


; Define share access rights to files and directories

FILE_SHARE_READ                 equ 1	; winnt
FILE_SHARE_WRITE                equ 2	; winnt
FILE_SHARE_DELETE               equ 4	; winnt
FILE_SHARE_VALID_FLAGS          equ 7

; +
; Define the file attributes values
;
; Note:  0x00000008 is reserved for use for the old DOS VOLID (volume ID)
;        and is therefore not considered valid in NT.
;
; Note:  0x00000010 is reserved for use for the old DOS SUBDIRECTORY flag
;        and is therefore not considered valid in NT.  This flag has
;        been disassociated with file attributes since the other flags are
;        protected with READ_ and WRITE_ATTRIBUTES access to the file.
;
; Note:  Note also that the order of these flags is set to allow both the
;        FAT and the Pinball File Systems to directly set the attributes
;        flags in attributes words without having to pick each flag out
;        individually.  The order of these flags should not be changed!
; -

FILE_ATTRIBUTE_READONLY             equ 00000001h  ; winnt
FILE_ATTRIBUTE_HIDDEN               equ 00000002h  ; winnt
FILE_ATTRIBUTE_SYSTEM               equ 00000004h  ; winnt
;OLD DOS VOLID                             0x00000008

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

; end_ntddk end_wdm end_nthal

; Define the I/O status information return values for requests for oplocks
; via NtFsControlFile

FILE_OPLOCK_BROKEN_TO_LEVEL_2   equ 00000007
FILE_OPLOCK_BROKEN_TO_NONE      equ 00000008

; Define the I/O status information return values for NtCreateFile/NtOpenFile
; when the sharing access fails but a batch oplock break is in progress

FILE_OPBATCH_BREAK_UNDERWAY     equ 00000009

; Define the filter flags for NtNotifyChangeDirectoryFile

FILE_NOTIFY_CHANGE_FILE_NAME    equ 00000001   ; winnt
FILE_NOTIFY_CHANGE_DIR_NAME     equ 00000002   ; winnt
FILE_NOTIFY_CHANGE_NAME         equ 00000003
FILE_NOTIFY_CHANGE_ATTRIBUTES   equ 00000004   ; winnt
FILE_NOTIFY_CHANGE_SIZE         equ 00000008   ; winnt
FILE_NOTIFY_CHANGE_LAST_WRITE   equ 00000010h  ; winnt
FILE_NOTIFY_CHANGE_LAST_ACCESS  equ 00000020h  ; winnt
FILE_NOTIFY_CHANGE_CREATION     equ 00000040h  ; winnt
FILE_NOTIFY_CHANGE_EA           equ 00000080h
FILE_NOTIFY_CHANGE_SECURITY     equ 00000100h  ; winnt
FILE_NOTIFY_CHANGE_STREAM_NAME  equ 00000200h
FILE_NOTIFY_CHANGE_STREAM_SIZE  equ 00000400h
FILE_NOTIFY_CHANGE_STREAM_WRITE equ 00000800h
FILE_NOTIFY_VALID_MASK          equ 00000fffh

; Define the file action type codes for NtNotifyChangeDirectoryFile

FILE_ACTION_ADDED                   equ 00000001   ; winnt
FILE_ACTION_REMOVED                 equ 00000002   ; winnt
FILE_ACTION_MODIFIED                equ 00000003   ; winnt
FILE_ACTION_RENAMED_OLD_NAME        equ 00000004   ; winnt
FILE_ACTION_RENAMED_NEW_NAME        equ 00000005   ; winnt
FILE_ACTION_ADDED_STREAM            equ 00000006
FILE_ACTION_REMOVED_STREAM          equ 00000007
FILE_ACTION_MODIFIED_STREAM         equ 00000008
FILE_ACTION_REMOVED_BY_DELETE       equ 00000009
FILE_ACTION_ID_NOT_TUNNELLED        equ 0000000Ah
FILE_ACTION_TUNNELLED_ID_COLLISION  equ 0000000Bh

; Define the NamedPipeType flags for NtCreateNamedPipeFile

FILE_PIPE_BYTE_STREAM_TYPE      equ 00000000
FILE_PIPE_MESSAGE_TYPE          equ 00000001

; Define the CompletionMode flags for NtCreateNamedPipeFile

FILE_PIPE_QUEUE_OPERATION       equ 00000000
FILE_PIPE_COMPLETE_OPERATION    equ 00000001

; Define the ReadMode flags for NtCreateNamedPipeFile

FILE_PIPE_BYTE_STREAM_MODE      equ 00000000
FILE_PIPE_CONNECTED_STATE       equ 00000003  

; Define special ByteOffset parameters for read and write operations

FILE_WRITE_TO_END_OF_FILE       equ 0ffffffffh
FILE_USE_FILE_POINTER_POSITION  equ 0fffffffeh

; Define alignment requirement values

FILE_BYTE_ALIGNMENT             equ 00000000
FILE_WORD_ALIGNMENT             equ 00000001
FILE_LONG_ALIGNMENT             equ 00000003
FILE_QUAD_ALIGNMENT             equ 00000007
FILE_OCTA_ALIGNMENT             equ 0000000fh
FILE_32_BYTE_ALIGNMENT          equ 0000001fh
FILE_64_BYTE_ALIGNMENT          equ 0000003fh
FILE_128_BYTE_ALIGNMENT         equ 0000007fh
FILE_256_BYTE_ALIGNMENT         equ 000000ffh
FILE_512_BYTE_ALIGNMENT         equ 000001ffh

; Define the maximum length of a filename string

MAXIMUM_FILENAME_LENGTH         equ 256

; end_ntddk end_wdm end_nthal

; Define the file system attributes flags

FILE_CASE_SENSITIVE_SEARCH      equ 00000001   ; winnt
FILE_CASE_PRESERVED_NAMES       equ 00000002   ; winnt
FILE_UNICODE_ON_DISK            equ 00000004   ; winnt
FILE_PERSISTENT_ACLS            equ 00000008   ; winnt
FILE_FILE_COMPRESSION           equ 00000010h  ; winnt
FILE_VOLUME_QUOTAS              equ 00000020h  ; winnt
FILE_SUPPORTS_SPARSE_FILES      equ 00000040h  ; winnt
FILE_SUPPORTS_REPARSE_POINTS    equ 00000080h  ; winnt
FILE_SUPPORTS_REMOTE_STORAGE    equ 00000100h  ; winnt
FILE_VOLUME_IS_COMPRESSED       equ 00008000h  ; winnt
FILE_SUPPORTS_OBJECT_IDS        equ 00010000h  ; winnt
FILE_SUPPORTS_ENCRYPTION        equ 00020000h  ; winnt
FILE_NAMED_STREAMS              equ 00040000h  ; winnt

; Define the flags for NtSet(Query)EaFile service structure entries

FILE_NEED_EA                    equ 00000080h

; Define EA type values

FILE_EA_TYPE_BINARY             equ 0fffeh
FILE_EA_TYPE_ASCII              equ 0fffdh
FILE_EA_TYPE_BITMAP             equ 0fffbh
FILE_EA_TYPE_METAFILE           equ 0fffah
FILE_EA_TYPE_ICON               equ 0fff9h
FILE_EA_TYPE_EA                 equ 0ffeeh
FILE_EA_TYPE_MVMT               equ 0ffdfh
FILE_EA_TYPE_MVST               equ 0ffdeh
FILE_EA_TYPE_ASN1               equ 0ffddh
FILE_EA_TYPE_FAMILY_IDS         equ 0ff01h

; begin_ntddk begin_wdm begin_nthal

; Define the various device characteristics flags

FILE_REMOVABLE_MEDIA            equ 00000001
FILE_READ_ONLY_DEVICE           equ 00000002
FILE_FLOPPY_DISKETTE            equ 00000004
FILE_WRITE_ONCE_MEDIA           equ 00000008
FILE_REMOTE_DEVICE              equ 00000010h
FILE_DEVICE_IS_MOUNTED          equ 00000020h
FILE_VIRTUAL_VOLUME             equ 00000040h
FILE_AUTOGENERATED_DEVICE_NAME  equ 00000080h
FILE_DEVICE_SECURE_OPEN         equ 00000100h

; end_wdm

; flags specified here will be propagated up and down a device stack
; after FDO and all filter devices are added, but before the device
; stack is started

FILE_CHARACTERISTICS_PROPAGATED equ (FILE_REMOVABLE_MEDIA or FILE_READ_ONLY_DEVICE or FILE_FLOPPY_DISKETTE or FILE_WRITE_ONCE_MEDIA or FILE_DEVICE_SECURE_OPEN)

; end_ntddk end_nthal

; begin_ntddk begin_wdm begin_nthal

; Define the base asynchronous I/O argument types

IO_STATUS_BLOCK STRUCT		; sizeof = 08h
	union
	Status		SDWORD		?	; 0000h  NTSTATUS
	Pointer		PVOID		?
	ends
	Information	DWORD		?	; 0004h ULONG_PTR
IO_STATUS_BLOCK ENDS
PIO_STATUS_BLOCK typedef PTR IO_STATUS_BLOCK

; end_ntddk end_wdm end_nthal

; Define the file information class values
;
; WARNING:  The order of the following values are assumed by the I/O system.
;           Any changes made here should be reflected there as well.

;typedef enum _FILE_INFORMATION_CLASS {
; end_wdm
    FileDirectoryInformation        equ 1
    FileFullDirectoryInformation    equ 2
    FileBothDirectoryInformation    equ 3
    FileBasicInformation            equ 4  ; wdm
    FileStandardInformation         equ 5  ; wdm
    FileInternalInformation         equ 6
    FileEaInformation               equ 7
    FileAccessInformation           equ 8
    FileNameInformation             equ 9
    FileRenameInformation           equ 10
    FileLinkInformation             equ 11
    FileNamesInformation            equ 12
    FileDispositionInformation      equ 13
    FilePositionInformation         equ 14 ; wdm
    FileFullEaInformation           equ 15
    FileModeInformation             equ 16
    FileAlignmentInformation        equ 17
    FileAllInformation              equ 18
    FileAllocationInformation       equ 19
    FileEndOfFileInformation        equ 20 ; wdm
    FileAlternateNameInformation    equ 21
    FileStreamInformation           equ 22
    FilePipeInformation             equ 23
    FilePipeLocalInformation        equ 24
    FilePipeRemoteInformation       equ 25
    FileMailslotQueryInformation    equ 26
    FileMailslotSetInformation      equ 27
    FileCompressionInformation      equ 28
    FileObjectIdInformation         equ 29
    FileCompletionInformation       equ 30
    FileMoveClusterInformation      equ 31
    FileQuotaInformation            equ 32
    FileReparsePointInformation     equ 33
    FileNetworkOpenInformation      equ 34
    FileAttributeTagInformation     equ 35
    FileTrackingInformation         equ 36
    FileIdBothDirectoryInformation  equ 37
    FileIdFullDirectoryInformation  equ 38
    FileMaximumInformation  		equ 39
; begin_wdm


; Define the various structures which are returned on query operations

; end_ntddk end_wdm end_nthal

; +
; NtQueryDirectoryFile return types:
;
;      FILE_DIRECTORY_INFORMATION
;      FILE_FULL_DIR_INFORMATION
;      FILE_ID_FULL_DIR_INFORMATION
;      FILE_BOTH_DIR_INFORMATION
;      FILE_ID_BOTH_DIR_INFORMATION
;      FILE_NAMES_INFORMATION
;      FILE_OBJECTID_INFORMATION
; -

FILE_DIRECTORY_INFORMATION STRUCT		; sizeof = 48h
	NextEntryOffset		DWORD	?
	FileIndex			DWORD	?
	CreationTime		LARGE_INTEGER	<>
	LastAccessTime		LARGE_INTEGER	<>
	LastWriteTime		LARGE_INTEGER	<>
	ChangeTime			LARGE_INTEGER	<>
	EndOfFile			LARGE_INTEGER	<>
	AllocationSize		LARGE_INTEGER	<>
	FileAttributes		DWORD	?
	FileNameLength		DWORD	?
	FileName			WORD 1 dup(?)	; WCHAR
						WORD 3 dup(?)	; padding
FILE_DIRECTORY_INFORMATION ENDS
PFILE_DIRECTORY_INFORMATION typedef ptr FILE_DIRECTORY_INFORMATION

FILE_FULL_DIR_INFORMATION STRUCT		; sizeof = 48h
	NextEntryOffset		DWORD	?
	FileIndex			DWORD	?
	CreationTime		LARGE_INTEGER	<>
	LastAccessTime		LARGE_INTEGER	<>
	LastWriteTime		LARGE_INTEGER	<>
	ChangeTime			LARGE_INTEGER	<>
	EndOfFile			LARGE_INTEGER	<>
	AllocationSize		LARGE_INTEGER	<>
	FileAttributes		DWORD	?
	FileNameLength		DWORD	?
	EaSize				DWORD	?
	FileName			WORD 1 dup(?)	; WCHAR
						WORD	?		; padding
FILE_FULL_DIR_INFORMATION ENDS
PFILE_FULL_DIR_INFORMATION  typedef ptr FILE_FULL_DIR_INFORMATION

FILE_ID_FULL_DIR_INFORMATION STRUCT		; sizeof = 58h
	NextEntryOffset		DWORD	?
	FileIndex			DWORD	?
	CreationTime		LARGE_INTEGER	<>
	LastAccessTime		LARGE_INTEGER	<>
	LastWriteTime		LARGE_INTEGER	<>
	ChangeTime			LARGE_INTEGER	<>
	EndOfFile			LARGE_INTEGER	<>
	AllocationSize		LARGE_INTEGER	<>
	FileAttributes		DWORD	?
	FileNameLength		DWORD	?
	EaSize				DWORD	?			; 040h
						DWORD	?			; 044h
	FileId				LARGE_INTEGER	<>	; 048h
	FileName			WORD 1 dup(?)		; 050h WCHAR
						WORD 3 dup(?)		; padding
FILE_ID_FULL_DIR_INFORMATION ENDS
PFILE_ID_FULL_DIR_INFORMATION typedef ptr FILE_ID_FULL_DIR_INFORMATION

FILE_BOTH_DIR_INFORMATION STRUCT		; sizeof = 60h
	NextEntryOffset		DWORD	?
	FileIndex			DWORD	?
	CreationTime		LARGE_INTEGER	<>
	LastAccessTime		LARGE_INTEGER	<>
	LastWriteTime		LARGE_INTEGER	<>
	ChangeTime			LARGE_INTEGER	<>
	EndOfFile			LARGE_INTEGER	<>
	AllocationSize		LARGE_INTEGER	<>
	FileAttributes		DWORD	?
	FileNameLength		DWORD	?
	EaSize				DWORD	?			; 040h
	ShortNameLength		BYTE	?			; 044h CCHAR
						BYTE	?
	ShortName			WORD 12 dup(?)		; 046h WCHAR
	FileName			WORD 1 dup(?)		; 05Eh WCHAR
FILE_BOTH_DIR_INFORMATION ENDS
PFILE_BOTH_DIR_INFORMATION typedef ptr FILE_BOTH_DIR_INFORMATION

FILE_ID_BOTH_DIR_INFORMATION STRUCT		; sizeof = 70h
	NextEntryOffset		DWORD	?
	FileIndex			DWORD	?
	CreationTime		LARGE_INTEGER	<>
	LastAccessTime		LARGE_INTEGER	<>
	LastWriteTime		LARGE_INTEGER	<>
	ChangeTime			LARGE_INTEGER	<>
	EndOfFile			LARGE_INTEGER	<>
	AllocationSize		LARGE_INTEGER	<>
	FileAttributes		DWORD	?
	FileNameLength		DWORD	?
	EaSize				DWORD	?			; 040h
	ShortNameLength		BYTE	?			; 044h CCHAR
						BYTE	?
	ShortName			WORD 12 dup(?)		; 046h WCHAR
						WORD	?
	FileId				LARGE_INTEGER	<>	; 060h
	FileName			WORD 1 dup(?)		; 068 WCHAR
						WORD 3 dup(?)		; padding
FILE_ID_BOTH_DIR_INFORMATION ENDS
PFILE_ID_BOTH_DIR_INFORMATION typedef ptr FILE_ID_BOTH_DIR_INFORMATION

FILE_NAMES_INFORMATION STRUCT		; sizeof = 10h
	NextEntryOffset		DWORD	?
	FileIndex			DWORD	?
	FileNameLength		DWORD	?
	FileName			WORD 1 dup(?)		; 068 WCHAR
						WORD	?			; padding
FILE_NAMES_INFORMATION ENDS
PFILE_NAMES_INFORMATION typedef ptr FILE_NAMES_INFORMATION

FILE_OBJECTID_INFORMATION STRUCT		; sizeof = 48h
	FileReference			QWORD	?
	ObjectId				BYTE 16 dup(?) 	; UCHAR
    union
        struct
			BirthVolumeId	BYTE 16 dup(?) 	; 018h UCHAR
			BirthObjectId	BYTE 16 dup(?) 	; 028h UCHAR
			DomainId		BYTE 16 dup(?) 	; 038h UCHAR
		ends
		ExtendedInfo		BYTE 48 dup(?) 	; 018h UCHAR
	ends
FILE_OBJECTID_INFORMATION ENDS
PFILE_OBJECTID_INFORMATION typedef ptr FILE_OBJECTID_INFORMATION

; +
; NtQuery(Set)InformationFile return types:
;
;      FILE_BASIC_INFORMATION
;      FILE_STANDARD_INFORMATION
;      FILE_INTERNAL_INFORMATION
;      FILE_EA_INFORMATION
;      FILE_ACCESS_INFORMATION
;      FILE_POSITION_INFORMATION
;      FILE_MODE_INFORMATION
;      FILE_ALIGNMENT_INFORMATION
;      FILE_NAME_INFORMATION
;      FILE_ALL_INFORMATION
;
;      FILE_NETWORK_OPEN_INFORMATION
;
;      FILE_ALLOCATION_INFORMATION
;      FILE_COMPRESSION_INFORMATION
;      FILE_DISPOSITION_INFORMATION
;      FILE_END_OF_FILE_INFORMATION
;      FILE_LINK_INFORMATION
;      FILE_MOVE_CLUSTER_INFORMATION
;      FILE_RENAME_INFORMATION
;      FILE_STREAM_INFORMATION
;      FILE_COMPLETION_INFORMATION
;
;      FILE_PIPE_INFORMATION
;      FILE_PIPE_LOCAL_INFORMATION
;      FILE_PIPE_REMOTE_INFORMATION
;
;      FILE_MAILSLOT_QUERY_INFORMATION
;      FILE_MAILSLOT_SET_INFORMATION
;      FILE_REPARSE_POINT_INFORMATION
; -

FILE_BASIC_INFORMATION STRUCT				; sizeof = 028h
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

FILE_INTERNAL_INFORMATION STRUCT
	IndexNumber			LARGE_INTEGER	<>
FILE_INTERNAL_INFORMATION ENDS
PFILE_INTERNAL_INFORMATION typedef ptr FILE_INTERNAL_INFORMATION

FILE_EA_INFORMATION STRUCT
	EaSize				DWORD	?
FILE_EA_INFORMATION ENDS
PFILE_EA_INFORMATION typedef ptr FILE_EA_INFORMATION

FILE_ACCESS_INFORMATION STRUCT
	AccessFlags			DWORD	?	; ACCESS_MASK
FILE_ACCESS_INFORMATION ENDS
PFILE_ACCESS_INFORMATION typedef ptr FILE_ACCESS_INFORMATION

FILE_POSITION_INFORMATION STRUCT
	CurrentByteOffset	LARGE_INTEGER	<>
FILE_POSITION_INFORMATION ENDS
PFILE_POSITION_INFORMATION typedef ptr FILE_POSITION_INFORMATION

FILE_MODE_INFORMATION STRUCT
	Mode				DWORD	?
FILE_MODE_INFORMATION ENDS
PFILE_MODE_INFORMATION typedef ptr FILE_MODE_INFORMATION

FILE_ALIGNMENT_INFORMATION  STRUCT
	AlignmentRequirement	DWORD	?
FILE_ALIGNMENT_INFORMATION ENDS
PFILE_ALIGNMENT_INFORMATION typedef ptr FILE_ALIGNMENT_INFORMATION

FILE_NAME_INFORMATION  STRUCT
	FileNameLength	DWORD	?
	FileName		WCHAR	1 dup(?)
FILE_NAME_INFORMATION ENDS
PFILE_NAME_INFORMATION typedef ptr FILE_NAME_INFORMATION

FILE_ALL_INFORMATION STRUCT		; sizeof = 68h
	BasicInformation		FILE_BASIC_INFORMATION		<>
	StandardInformation		FILE_STANDARD_INFORMATION	<>
	InternalInformation		FILE_INTERNAL_INFORMATION	<>
	EaInformation			FILE_EA_INFORMATION			<>
	AccessInformation		FILE_ACCESS_INFORMATION		<>
	PositionInformation		FILE_POSITION_INFORMATION	<>
	ModeInformation			FILE_MODE_INFORMATION		<>
	AlignmentInformation	FILE_ALIGNMENT_INFORMATION	<>
	NameInformation			FILE_NAME_INFORMATION		<>
FILE_ALL_INFORMATION ENDS
PFILE_ALL_INFORMATION typedef ptr FILE_ALL_INFORMATION

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

FILE_ALLOCATION_INFORMATION STRUCT
	AllocationSize	LARGE_INTEGER	<>
FILE_ALLOCATION_INFORMATION ENDS
PFILE_ALLOCATION_INFORMATION typedef ptr FILE_ALLOCATION_INFORMATION

FILE_DISPOSITION_INFORMATION STRUCT
	DeleteFile		BOOLEAN	?
FILE_DISPOSITION_INFORMATION ENDS
PFILE_DISPOSITION_INFORMATION typedef PTR FILE_DISPOSITION_INFORMATION

FILE_END_OF_FILE_INFORMATION STRUCT			; sizeof=8
	EndOfFile		LARGE_INTEGER	<>
FILE_END_OF_FILE_INFORMATION ENDS
PFILE_END_OF_FILE_INFORMATION typedef PTR FILE_END_OF_FILE_INFORMATION

FILE_LINK_INFORMATION STRUCT			; sizeof = 10h
	ReplaceIfExists			BOOLEAN	?
							db 3 dup(?)
	RootDirectory			DWORD	?	; HANDLE
	FileNameLength			DWORD	?
	FileName				WORD 1 dup(?)	; WCHAR
							WORD	?
FILE_LINK_INFORMATION ENDS
PFILE_LINK_INFORMATION typedef ptr FILE_LINK_INFORMATION

FILE_RENAME_INFORMATION STRUCT			; sizeof = 10h
	ReplaceIfExists			BOOLEAN	?
							db 3 dup(?)
	RootDirectory			DWORD	?	; HANDLE
	FileNameLength			DWORD	?
	FileName				WORD 1 dup(?)	; WCHAR
							WORD	?
FILE_RENAME_INFORMATION ENDS
PFILE_RENAME_INFORMATION typedef ptr FILE_RENAME_INFORMATION

FILE_STREAM_INFORMATION STRUCT			; sizeof = 20h
	NextEntryOffset			DWORD			?
	StreamNameLength		DWORD			?
	StreamSize				LARGE_INTEGER	<>
	StreamAllocationSize	LARGE_INTEGER	<>
	StreamName				WORD 1 dup(?)	; WCHAR
							WORD 3 dup(?)
FILE_STREAM_INFORMATION ENDS
PFILE_STREAM_INFORMATION typedef ptr FILE_STREAM_INFORMATION

FILE_TRACKING_INFORMATION STRUCT			; sizeof = 0Ch
	DestinationFile			DWORD	?		; HANDLE
	ObjectInformationLength	DWORD	?
	ObjectInformation		BYTE 1 dup(?)	; CHAR
							db 3 dup(?)
FILE_TRACKING_INFORMATION ENDS
PFILE_TRACKING_INFORMATION typedef ptr FILE_TRACKING_INFORMATION

FILE_COMPLETION_INFORMATION STRUCT		; sizeof = 8
	Port		DWORD	?	; HANDLE
	Key			PVOID	?
FILE_COMPLETION_INFORMATION ENDS
PFILE_COMPLETION_INFORMATION typedef ptr FILE_COMPLETION_INFORMATION

FILE_PIPE_INFORMATION STRUCT			; sizeof = 8
	ReadMode			DWORD	?
	CompletionMode		DWORD	?
FILE_PIPE_INFORMATION ENDS
PFILE_PIPE_INFORMATION typedef ptr FILE_PIPE_INFORMATION

FILE_PIPE_LOCAL_INFORMATION STRUCT			; sizeof = 28h
	NamedPipeType				DWORD	?
	NamedPipeConfiguration				DWORD	?
	MaximumInstances				DWORD	?
	CurrentInstances				DWORD	?
	InboundQuota				DWORD	?
	ReadDataAvailable				DWORD	?
	OutboundQuota				DWORD	?
	WriteQuotaAvailable				DWORD	?
	NamedPipeState				DWORD	?
	NamedPipeEnd				DWORD	?
FILE_PIPE_LOCAL_INFORMATION ENDS
PFILE_PIPE_LOCAL_INFORMATION typedef ptr FILE_PIPE_LOCAL_INFORMATION

FILE_PIPE_REMOTE_INFORMATION STRUCT			; sizeof = 10h
	CollectDataTime			LARGE_INTEGER	<>
	MaximumCollectionCount	DWORD	?
							DWORD	?
FILE_PIPE_REMOTE_INFORMATION ENDS
PFILE_PIPE_REMOTE_INFORMATION typedef ptr FILE_PIPE_REMOTE_INFORMATION

; begin_winioctl
IFNDEF _FILESYSTEMFSCTL_
_FILESYSTEMFSCTL_ equ 1

; The following is a list of the native file system fsctls followed by
; additional network file system fsctls.  Some values have been
; decommissioned.

FSCTL_REQUEST_OPLOCK_LEVEL_1    equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  0, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_REQUEST_OPLOCK_LEVEL_2    equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  1, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_REQUEST_BATCH_OPLOCK      equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  2, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_OPLOCK_BREAK_ACKNOWLEDGE  equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  3, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_OPBATCH_ACK_CLOSE_PENDING equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  4, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_OPLOCK_BREAK_NOTIFY       equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  5, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_LOCK_VOLUME               equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  6, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_UNLOCK_VOLUME             equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  7, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_DISMOUNT_VOLUME           equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  8, METHOD_BUFFERED, FILE_ANY_ACCESS)
; decommissioned fsctl value                                              9
FSCTL_IS_VOLUME_MOUNTED         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_IS_PATHNAME_VALID         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 11, METHOD_BUFFERED, FILE_ANY_ACCESS) ; PATHNAME_BUFFER,
FSCTL_MARK_VOLUME_DIRTY         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 12, METHOD_BUFFERED, FILE_ANY_ACCESS)
; decommissioned fsctl value                                             13
FSCTL_QUERY_RETRIEVAL_POINTERS  equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 14, METHOD_NEITHER, FILE_ANY_ACCESS)
FSCTL_GET_COMPRESSION           equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_SET_COMPRESSION           equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 16, METHOD_BUFFERED, FILE_READ_DATA or FILE_WRITE_DATA)
; decommissioned fsctl value                                             17
; decommissioned fsctl value                                             18
FSCTL_MARK_AS_SYSTEM_HIVE       equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 19, METHOD_NEITHER, FILE_ANY_ACCESS)
FSCTL_OPLOCK_BREAK_ACK_NO_2     equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 20, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_INVALIDATE_VOLUMES        equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_QUERY_FAT_BPB             equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 22, METHOD_BUFFERED, FILE_ANY_ACCESS) ; FSCTL_QUERY_FAT_BPB_BUFFER
FSCTL_REQUEST_FILTER_OPLOCK     equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 23, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_FILESYSTEM_GET_STATISTICS equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 24, METHOD_BUFFERED, FILE_ANY_ACCESS) ; FILESYSTEM_STATISTICS
;#if(_WIN32_WINNT >= 0x0400)
FSCTL_GET_NTFS_VOLUME_DATA      equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 25, METHOD_BUFFERED, FILE_ANY_ACCESS) ; NTFS_VOLUME_DATA_BUFFER
FSCTL_GET_NTFS_FILE_RECORD      equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 26, METHOD_BUFFERED, FILE_ANY_ACCESS) ; NTFS_FILE_RECORD_INPUT_BUFFER, NTFS_FILE_RECORD_OUTPUT_BUFFER
FSCTL_GET_VOLUME_BITMAP         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 27, METHOD_NEITHER, FILE_ANY_ACCESS) ; STARTING_LCN_INPUT_BUFFER, VOLUME_BITMAP_BUFFER
FSCTL_GET_RETRIEVAL_POINTERS    equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 28, METHOD_NEITHER, FILE_ANY_ACCESS) ; STARTING_VCN_INPUT_BUFFER, RETRIEVAL_POINTERS_BUFFER
FSCTL_MOVE_FILE                 equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 29, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) ; MOVE_FILE_DATA,
FSCTL_IS_VOLUME_DIRTY           equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 30, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_GET_HFS_INFORMATION       equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 31, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_ALLOW_EXTENDED_DASD_IO    equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 32, METHOD_NEITHER,  FILE_ANY_ACCESS)
;#endif /* _WIN32_WINNT >= 0x0400 */

;#if(_WIN32_WINNT >= 0x0500)
FSCTL_READ_PROPERTY_DATA        equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 33, METHOD_NEITHER, FILE_ANY_ACCESS)
FSCTL_WRITE_PROPERTY_DATA       equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 34, METHOD_NEITHER, FILE_ANY_ACCESS)
FSCTL_FIND_FILES_BY_SID         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 35, METHOD_NEITHER, FILE_ANY_ACCESS)
; decommissioned fsctl value                                             36
FSCTL_DUMP_PROPERTY_DATA        equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 37, METHOD_NEITHER, FILE_ANY_ACCESS)
FSCTL_SET_OBJECT_ID             equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 38, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) ; FILE_OBJECTID_BUFFER
FSCTL_GET_OBJECT_ID             equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 39, METHOD_BUFFERED, FILE_ANY_ACCESS) ; FILE_OBJECTID_BUFFER
FSCTL_DELETE_OBJECT_ID          equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 40, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
FSCTL_SET_REPARSE_POINT         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 41, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) ; REPARSE_DATA_BUFFER,
FSCTL_GET_REPARSE_POINT         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 42, METHOD_BUFFERED, FILE_ANY_ACCESS) ; REPARSE_DATA_BUFFER
FSCTL_DELETE_REPARSE_POINT      equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 43, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) ; REPARSE_DATA_BUFFER,
FSCTL_ENUM_USN_DATA             equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 44, METHOD_NEITHER, FILE_ANY_ACCESS) ; MFT_ENUM_DATA,
FSCTL_SECURITY_ID_CHECK         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 45, METHOD_NEITHER, FILE_READ_DATA)  ; BULK_SECURITY_TEST_DATA,
FSCTL_READ_USN_JOURNAL          equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 46, METHOD_NEITHER, FILE_ANY_ACCESS) ; READ_USN_JOURNAL_DATA, USN
FSCTL_SET_OBJECT_ID_EXTENDED    equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 47, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
FSCTL_CREATE_OR_GET_OBJECT_ID   equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 48, METHOD_BUFFERED, FILE_ANY_ACCESS) ; FILE_OBJECTID_BUFFER
FSCTL_SET_SPARSE                equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 49, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
FSCTL_SET_ZERO_DATA             equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 50, METHOD_BUFFERED, FILE_WRITE_DATA) ; FILE_ZERO_DATA_INFORMATION,
FSCTL_QUERY_ALLOCATED_RANGES    equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 51, METHOD_NEITHER, FILE_READ_DATA)  ; FILE_ALLOCATED_RANGE_BUFFER, FILE_ALLOCATED_RANGE_BUFFER
FSCTL_ENABLE_UPGRADE            equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 52, METHOD_BUFFERED, FILE_WRITE_DATA)
FSCTL_SET_ENCRYPTION            equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 53, METHOD_NEITHER, FILE_ANY_ACCESS) ; ENCRYPTION_BUFFER, DECRYPTION_STATUS_BUFFER
FSCTL_ENCRYPTION_FSCTL_IO       equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 54, METHOD_NEITHER, FILE_ANY_ACCESS)
FSCTL_WRITE_RAW_ENCRYPTED       equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 55, METHOD_NEITHER, FILE_SPECIAL_ACCESS) ; ENCRYPTED_DATA_INFO,
FSCTL_READ_RAW_ENCRYPTED        equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 56, METHOD_NEITHER, FILE_SPECIAL_ACCESS) ; REQUEST_RAW_ENCRYPTED_DATA, ENCRYPTED_DATA_INFO
FSCTL_CREATE_USN_JOURNAL        equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 57, METHOD_NEITHER, FILE_ANY_ACCESS) ; CREATE_USN_JOURNAL_DATA,
FSCTL_READ_FILE_USN_DATA        equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 58, METHOD_NEITHER, FILE_ANY_ACCESS) ; Read the Usn Record for a file
FSCTL_WRITE_USN_CLOSE_RECORD    equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 59, METHOD_NEITHER, FILE_ANY_ACCESS) ; Generate Close Usn Record
FSCTL_EXTEND_VOLUME             equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 60, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_QUERY_USN_JOURNAL         equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_DELETE_USN_JOURNAL        equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 62, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_MARK_HANDLE               equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 63, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_SIS_COPYFILE              equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 64, METHOD_BUFFERED, FILE_ANY_ACCESS)
FSCTL_SIS_LINK_FILES            equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 65, METHOD_BUFFERED, FILE_READ_DATA or FILE_WRITE_DATA)
FSCTL_HSM_MSG                   equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 66, METHOD_BUFFERED, FILE_READ_DATA or FILE_WRITE_DATA)
FSCTL_NSS_CONTROL               equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 67, METHOD_BUFFERED, FILE_WRITE_DATA)
FSCTL_HSM_DATA                  equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 68, METHOD_NEITHER, FILE_READ_DATA or FILE_WRITE_DATA)
FSCTL_RECALL_FILE               equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 69, METHOD_NEITHER, FILE_ANY_ACCESS)
FSCTL_NSS_RCONTROL              equ CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 70, METHOD_BUFFERED, FILE_READ_DATA)
;#endif /* _WIN32_WINNT >= 0x0500 */

ENDIF ; _FILESYSTEMFSCTL_

IO_REPARSE_TAG_MOUNT_POINT               equ 0A0000003h
IO_REPARSE_TAG_HSM                       equ 0C0000004h
IO_REPARSE_TAG_SIS                       equ 080000007h

FSCTL_PIPE_PEEK                 equ CTL_CODE(FILE_DEVICE_NAMED_PIPE, 3, METHOD_BUFFERED, FILE_READ_DATA)
; Control structure for FSCTL_PIPE_PEEK


PROCESS_ALL_ACCESS		equ (STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 0FFFh)

THREAD_ALL_ACCESS		equ (STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 3FFh)

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

; begin_winnt

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

NtCurrentProcess	equ -1
NtCurrentThread		equ -2

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

; Priority increment for completing mailslot I/O.  This is used by the mail-
; slot file system driver when completing an IRP (IoCompleteRequest).

IO_MAILSLOT_INCREMENT	equ 2

; Priority increment for completing named pipe I/O.  This is used by the
; named pipe file system driver when completing an IRP (IoCompleteRequest).

IO_NAMED_PIPE_INCREMENT	equ 2

; Priority increment for completing network I/O.  This is used by network
; device and network file system drivers when completing an IRP
; (IoCompleteRequest).

IO_NETWORK_INCREMENT	equ 2

; Priority increment used when satisfying a wait on an executive semaphore
; (NtReleaseSemaphore)

SEMAPHORE_INCREMENT		equ 1


; Section Information Structures.

;typedef enum _SECTION_INHERIT {
    ViewShare	equ 1
    ViewUnmap	equ 2

; Section Access Rights.

; begin_winnt
SECTION_QUERY       equ 0001h
SECTION_MAP_WRITE   equ 0002h
SECTION_MAP_READ    equ 0004h
SECTION_MAP_EXECUTE equ 0008h
SECTION_EXTEND_SIZE equ 0010h

SECTION_ALL_ACCESS	equ (STANDARD_RIGHTS_REQUIRED or SECTION_QUERY or SECTION_MAP_WRITE or SECTION_MAP_READ or SECTION_MAP_EXECUTE or SECTION_EXTEND_SIZE)

; end_winnt

SEGMENT_ALL_ACCESS	equ SECTION_ALL_ACCESS

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

; Define system time structure.

KSYSTEM_TIME STRUCT				; sizeof = 0Ch
	LowPart		DWORD		?	; 0000h
	High1Time	SDWORD		?	; 0004h
	High2Time	SDWORD		?	; 0008h
KSYSTEM_TIME ENDS
PKSYSTEM_TIME typedef PTR KSYSTEM_TIME

; begin_ntddk begin_wdm begin_nthal begin_ntndis


; Common dispatcher object header
;
; N.B. The size field contains the number of dwords in the structure.


DISPATCHER_HEADER STRUCT		; sizeof = 010h
	_Type			BYTE		?	; 0000h  DISP_TYPE_* (original name Type)
	Absolute		BYTE		?	; 0001h
	_Size			BYTE		?	; 0002h  (original name Size)
	Inserted		BYTE		?	; 0003h
	SignalState		SDWORD		?	; 0004h
	WaitListHead	LIST_ENTRY <>	; 0008h
DISPATCHER_HEADER ENDS

KWAIT_BLOCK STRUCT		; sizeof = 018h
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

KDEVICE_QUEUE STRUCT		; sizeof = 014h
	_Type			SWORD		?	; 0000h  (original name Type)
	_Size			SWORD		?	; 0002h  (original name Size)
	DeviceListHead	LIST_ENTRY <>	; 0004h
	slLock			DWORD		?	; 000Ch  KSPIN_LOCK  (original name Lock)
	Busy			BYTE		?	; 0010h  BOOLEAN
					db			3 dup(?) ; padding
KDEVICE_QUEUE ENDS
PKDEVICE_QUEUE	typedef PTR KDEVICE_QUEUE
PRKDEVICE_QUEUE typedef PTR KDEVICE_QUEUE	; *RESTRICTED_POINTER

KDEVICE_QUEUE_ENTRY STRUCT		; sizeof = 010h
	DeviceListEntry	LIST_ENTRY <>	; 0000h
	SortKey			DWORD		?	; 0008h
	Inserted		BYTE		?	; 000Ch
					db			3 dup(?) ; padding
KDEVICE_QUEUE_ENTRY ENDS
PKDEVICE_QUEUE_ENTRY  typedef PTR KDEVICE_QUEUE_ENTRY
PRKDEVICE_QUEUE_ENTRY typedef PTR KDEVICE_QUEUE_ENTRY	; *RESTRICTED_POINTER

; begin_ntndis

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

; end_ntddk end_wdm


;typedef enum _POOL_TYPE {
    NonPagedPool							equ 0
    PagedPool								equ 1
    NonPagedPoolMustSucceed					equ 2
    DontUseThisType							equ 3
    NonPagedPoolCacheAligned				equ 4
    PagedPoolCacheAligned					equ 5
    NonPagedPoolCacheAlignedMustS			equ 6
    MaxPoolType								equ 7

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

;  begin_wdm

; Define Wait Context Block (WCB)

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

; end_wdm

CONTROLLER_OBJECT STRUCT
	_Type				SWORD			?	; 000h   (original name Type)
	_Size				SWORD			?	; 002h   (original name Size)
	ControllerExtension	PVOID			?	; 004h
	DeviceWaitQueue		KDEVICE_QUEUE	<>	; 008h

	Spare1				DWORD			?	; 01Ch
	_Spare2				LARGE_INTEGER	<>	; 020h  (original name Spare2)

CONTROLLER_OBJECT ENDS
PCONTROLLER_OBJECT typedef PTR CONTROLLER_OBJECT

; begin_wdm

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

FO_FILE_OPEN                    equ 00000001
FO_SYNCHRONOUS_IO               equ 00000002
FO_ALERTABLE_IO                 equ 00000004
FO_NO_INTERMEDIATE_BUFFERING    equ 00000008
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
FO_REMOTE_ORIGIN                equ 01000000h


FILE_OBJECT STRUCT		; sizeof = 070h
	_Type					SWORD		?		; 0000h  IO_TYPE_FILE  (original field name Type)
	_Size					SWORD		?		; 0002h  (original name Size)
	DeviceObject			PVOID		?		; 0004h  PTR DEVICE_OBJECT
	Vpb						PVOID		?		; 0008h  PTR VPB
	FsContext				PVOID		?		; 000Ch
	FsContext2				PVOID		?		; 0010h
	SectionObjectPointer	PVOID		?		; 0014h  PTR SECTION_OBJECT_POINTERS
	PrivateCacheMap			PVOID		?		; 0018h
	FinalStatus				SDWORD		?		; 001Ch  NTSTATUS
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
	_Lock					KEVENT 		<>		; 004Ch  (org name Lock)
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

; end_wdm

IRP_DEFER_IO_COMPLETION         equ 00000800h
IRP_OB_QUERY_NAME               equ 00001000h
IRP_HOLD_DEVICE_QUEUE           equ 00002000h
IRP_RETRY_IO_COMPLETION         equ 00004000h

; begin_wdm

; Define I/O request packet (IRP) alternate flags for allocation control.

IRP_QUOTA_CHARGED               equ 01h
IRP_ALLOCATED_MUST_SUCCEED      equ 02h
IRP_ALLOCATED_FIXED_SIZE        equ 04h
IRP_LOOKASIDE_ALLOCATION        equ 08h

; I/O Request Packet (IRP) definition

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

; Object Manager types

OBJECT_HANDLE_INFORMATION STRUCT
	HandleAttributes	DWORD	?
	GrantedAccess		DWORD	?	; ACCESS_MASK
OBJECT_HANDLE_INFORMATION ENDS
POBJECT_HANDLE_INFORMATION typedef ptr OBJECT_HANDLE_INFORMATION

IOCTL_REDIR_QUERY_PATH	equ CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 99, METHOD_NEITHER, FILE_ANY_ACCESS)

ENDIF ; _NTIFS_


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
