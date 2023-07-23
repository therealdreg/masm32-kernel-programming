comment ^

Module Name:
    w2kundoc.inc

Abstract:
    This module defines some undocumented W2000 structures and constants.

Author:
    Four-F (four-f@mail.ru)

Last Update:
	03-October-2003

IMPORTANT:
	Hand made -> Bugs are very possible :(
	Your bug report is very welcome.

Comments:
    Some fields name have been changed because of collision with masm reserved words.
    All such words are prefixed with underscore ('_') symbol.
^

include native.inc

IFNDEF KAFFINITY
	include ntddk.inc
ENDIF

;IFNDEF KPROCESSOR_MODE
;	KPROCESSOR_MODE typedef BYTE
;ENDIF

;IFNDEF ULARGE_INTEGER
;	include ntdef.inc
;ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KAPC_STATE STRUCT						; sizeof = 18h
	ApcListHead			LIST_ENTRY	2 dup(<?>)
	Process				PVOID		?	; 10h PTR KPROCESS
	KernelApcInProgress	BYTE		?	; 14h
	KernelApcPending	BYTE		?	; 15h
	UserApcPending		BYTE		?	; 16h
						db 			?
KAPC_STATE ENDS
PKAPC_STATE typedef PTR KAPC_STATE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KGDTENTRY STRUCT		; sizeof = 8
	LimitLow		WORD	?
	BaseLow			WORD	?
	union _HighWord		; original HighWord
		struct Bytes
			BaseMid	BYTE	?
			Flags1	BYTE	?
			Flags2	BYTE	?
			BaseHi	BYTE	?
		ends
		; Damn! ...record field names must be unique...
		; kd displays it as __unnamed11, so i prepend each field name with 'u11'
		Bits RECORD \
			u11BaseHi:8,		; bits24-31 BaseHi
			u11Granularity:1,	; bits23-23 Granularity
			u11Default_Big:1,	; bits22-22 Default_Big
			u11Reserved_0:1,	; bits21-21 Reserved_0
			u11Sys:1,			; bits20-20 Sys
			u11LimitHi:4,		; bits16-19 LimitHi
			u11Pres:1,			; bits15-15 Pres
			u11Dpl:2,			; bits13-14 Dpl
			u11Type:5,			; bits8-12  Type
			u11BaseMid:8		; bits0-7   BaseMid
	ends ; HighWord
KGDTENTRY ENDS
PKGDTENTRY typedef PTR KGDTENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KIDTENTRY STRUCT		; sizeof = 8
	_Offset			WORD	?	; original name Offset
	Selector		WORD	?
	Access			WORD	?
	ExtendedOffset	WORD	?
KIDTENTRY ENDS
PKIDTENTRY typedef PTR KIDTENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PS_IMPERSONATION_INFORMATION STRUCT			; sizeof = 0Ch
	Token				PVOID	?
	CopyOnOpen			BYTE	?	; 4h
	EffectiveOnly		BYTE	?	; 5h
						db 2 dup(?)	; padding
	ImpersonationLevel	UINT	?	; 8h
PS_IMPERSONATION_INFORMATION ENDS
PPS_IMPERSONATION_INFORMATION typedef PTR PS_IMPERSONATION_INFORMATION

WOW64_PROCESS STRUCT
	Wow64	PVOID	?
WOW64_PROCESS ENDS
PWOW64_PROCESS typedef PTR WOW64_PROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MMSUPPORT_FLAGS RECORD \
	Filler:25,			; bits7-31 Filler
	WriteWatch:1,		; bits6-6 WriteWatch
	WorkingSetHard:1,	; bits5-5 WorkingSetHard
	TrimHard:1,			; bits4-4 TrimHard
	SessionLeader:1,	; bits3-3 SessionLeader
	ProcessInSession:1,	; bits2-2 ProcessInSession
	BeingTrimmed:1,		; bits1-1 BeingTrimmed
	SessionSpace:1		; bits0-0 SessionSpace

MMSUPPORT STRUCT		; sizeof = 48h
	LastTrimTime				LARGE_INTEGER	<>
	LastTrimFaultCount			DWORD		?	; 08h
	PageFaultCount				DWORD		?	; 0Ch
	PeakWorkingSetSize			DWORD		?	; 10h
	WorkingSetSize				DWORD		?	; 14h
	MinimumWorkingSetSize		DWORD		?	; 18h
	MaximumWorkingSetSize		DWORD		?	; 1Ch
	VmWorkingSetList			PVOID		?	; 20h
	WorkingSetExpansionLinks	LIST_ENTRY	<>	; 24h
	AllowWorkingSetAdjustment	BOOLEAN		?	; 2Ch
	AddressSpaceBeingDeleted	BOOLEAN		?	; 2Dh
	ForegroundSwitchCount		BYTE		?	; 2Eh
	MemoryPriority				BYTE		?	; 2Fh
	union										; 30h
		LongFlags				DWORD		?
		Flags					MMSUPPORT_FLAGS	<>
	ends
	Claim						DWORD		?	; 34h
	NextEstimationSlot			DWORD		?	; 38h
	NextAgingSlot				DWORD		?	; 3Ch
	EstimatedAvailable			DWORD		?	; 40h
	GrowthSinceLastEstimate		DWORD		?	; 44h
MMSUPPORT ENDS
PMMSUPPORT typedef PTR MMSUPPORT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
comment ^
PDE_4M STRUCT		; page-directory entry (4-mB page)
	union
		dwPDE4k	DWORD	?			; packed
		btPDE4k RECORD \
			pde4kPFN:20,			; bits12-31
			pde4kReserved:3,		; bit9-11
			pde4kGlobal:1,			; bit8
			pde4kLargePage:1,		; bit7
			pde4kDirty:1,			; bit6 Reserved	???
			pde4kAccessed:1,		; bit5
			pde4kCacheDisabled:1,	; bit4
			pde4kWriteThrough:1,	; bit3
			pde4kOwner:1,			; bit2
			pde4kWrite:1,			; bit1
			pde4kValid:1			; bit0
		
	ends
PDE_4M ENDS

PDE_4K STRUCT		; page-directory entry (4-kB page)
	union
		dwPDE4k	DWORD	?			; packed
		btPDE4k RECORD \
			pde4kPFN:20,			; bits12-31
			pde4kReserved:3,		; bit9-11
			pde4kGlobal:1,			; bit8
			pde4kLargePage:1,		; bit7
			pde4kDirty:1,			; bit6 Reserved	???
			pde4kAccessed:1,		; bit5
			pde4kCacheDisabled:1,	; bit4
			pde4kWriteThrough:1,	; bit3
			pde4kOwner:1,			; bit2
			pde4kWrite:1,			; bit1
			pde4kValid:1			; bit0
		
	ends
PDE_4K ENDS


PTE_4K STRUCT
	union
		dwPTE4k	DWORD	?			; packed
		btPTE4k RECORD \
			pte4kPFN:20,			; bits12-31
			pte4kReserved2:3,		; bit9-11
			pte4kGlobal:1,			; bit8
			pte4kReserved:1,		; bit7
			pte4kDirty:1,			; bit6
			pte4kAccessed:1,		; bit5
			pte4kCacheDisabled:1,	; bit4
			pte4kWriteThrough:1,	; bit3
			pte4kOwner:1,			; bit2
			pte4kWrite:1,			; bit1
			pte4kValid:1			; bit0
		
	ends
PTE_4K ENDS
^
; page directory entry (4-mB page)
; I've prefixed all fields with "pde4m"
HARDWARE_PDE4M RECORD \
	pde4mPageFrameNumber:10,	; bits22-31 PageFrameNumber
	pde4m_reserved:13,			; bits9-21 reserved
	pde4mGlobal:1,				; bit8 Global
	pde4mLargePage:1,			; bit7 LargePage
	pde4mDirty:1,				; bit6 Dirty
	pde4mAccessed:1,			; bit5 Accessed
	pde4mCacheDisable:1,		; bit4 CacheDisable
	pde4mWriteThrough:1,		; bit3 WriteThrough
	pde4mOwner:1, 				; bit2 Owner
	pde4mWrite:1,				; bit1 Write
	pde4mValid:1				; bit0 Valid

; page directory entry (4-kB page)
; I've prefixed all fields with "pde4k"
HARDWARE_PDE4K RECORD \
	pde4kPageFrameNumber:20,; bits12-31
	pde4k_reserved:3,		; bit9-11
	pde4kGlobal:1,			; bit8
	pde4kLargePage:1,		; bit7
	pde4kDirty:1,			; bit6 Reserved	???
	pde4kAccessed:1,		; bit5
	pde4kCacheDisable:1,	; bit4
	pde4kWriteThrough:1,	; bit3
	pde4kOwner:1,			; bit2
	pde4kWrite:1,			; bit1
	pde4kValid:1			; bit0

; page table entry
; I've prefixed all fields with "pte"
HARDWARE_PTE RECORD \
	ptePageFrameNumber:20,	; bits12-31 PageFrameNumber
	pte_reserved:1,			; bit11 reserved
	ptePrototype:1,			; bit10 Prototype
	pteCopyOnWrite:1,		; bit9 CopyOnWrite
	pteGlobal:1,			; bit8 Global
	pteLargePage:1,			; bit7 LargePage
	pteDirty:1,				; bit6 Dirty
	pteAccessed:1,			; bit5 Accessed
	pteCacheDisable:1,		; bit4 CacheDisable
	pteWriteThrough:1,		; bit3 WriteThrough
	pteOwner:1, 			; bit2 Owner
	pteWrite:1,				; bit1 Write
	pteValid:1				; bit0 Valid

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;HANDLE_TABLE_ENTRY STRUCT		; sizeof = 8
;	Object					PVOID	?
;	ObAttributes			DWORD	?
;	GrantedAccess			DWORD	?
;	GrantedAccessIndex		DWORD	?
;	CreatorBackTraceIndex	DWORD	?
;	NextFreeTableEntry		DWORD	?


HANDLE_TABLE STRUCT			; sizeof = 6Ch
	Flags					DWORD		?
	HandleCount				SDWORD		?	; 04h
	Table					PVOID		?	; 08h PTR PTR PTR HANDLE_TABLE_ENTRY
	QuotaProcess			PVOID		?	; 0Ch PTR EPROCESS
	UniqueProcessId			PVOID		?	; 10h
	FirstFreeTableEntry		SDWORD		?	; 14h
	NextIndexNeedingPool	SDWORD		?	; 18h
	HandleTableLock			ERESOURCE	<>	; 1Ch
	HandleTableList			LIST_ENTRY	<>	; 54h
	HandleContentionEvent	KEVENT		<>	; 5Ch
HANDLE_TABLE ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PAGEFAULT_HISTORY STRUCT		; sizeof = 18h
	CurrentIndex	DWORD	?
	MaxIndex		DWORD	?	; 04h
	SpinLock		DWORD	?	; 08h
	Reserved		PVOID	?	; 0Ch
	WatchInfo		PROCESS_WS_WATCH_INFORMATION	<>	; 10h
PAGEFAULT_HISTORY ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EPROCESS_QUOTA_BLOCK STRUCT			; sizeof = 2Ch
	QuotaLock				DWORD		?
	ReferenceCount			DWORD		?	; 04h
	QuotaPeakPoolUsage		DWORD 2 dup(?)	; 08h
	QuotaPoolUsage			DWORD 2 dup(?)	; 10h
	QuotaPoolLimit			DWORD 2 dup(?)	; 18h
	PeakPagefileUsage		DWORD		?	; 20h
	PagefileUsage			DWORD		?	; 24h
	PagefileLimit			DWORD		?	; 28h
EPROCESS_QUOTA_BLOCK ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EJOB STRUCT						; sizeof = 170h
	Event						KEVENT			<>
	JobLinks					LIST_ENTRY		<>	; 010h
	ProcessListHead				LIST_ENTRY		<>	; 018h
	JobLock						ERESOURCE		<>	; 020h
	TotalUserTime				LARGE_INTEGER	<>	; 058h
	TotalKernelTime				LARGE_INTEGER	<>	; 060h
	ThisPeriodTotalUserTime		LARGE_INTEGER	<>	; 068h
	ThisPeriodTotalKernelTime	LARGE_INTEGER	<>	; 070h
	TotalPageFaultCount			DWORD			?	; 078h
	TotalProcesses				DWORD			?	; 07Ch
	ActiveProcesses				DWORD			?	; 080h
	TotalTerminatedProcesses	DWORD			?	; 084h
	PerProcessUserTimeLimit		LARGE_INTEGER	<>	; 088h
	PerJobUserTimeLimit			LARGE_INTEGER	<>	; 090h
	LimitFlags					DWORD			?	; 098h
	MinimumWorkingSetSize		DWORD			?	; 09Ch
	MaximumWorkingSetSize		DWORD			?	; 0A0h
	ActiveProcessLimit			DWORD			?	; 0A4h
	Affinity					DWORD			?	; 0A8h
	PriorityClass				BYTE			?	; 0ACh
								db 	3 dup(?)		; padding
	UIRestrictionsClass			DWORD			?	; 0B0h
	SecurityLimitFlags			DWORD			?	; 0B4h
	Token						PVOID			?	; 0B8h
	Filter						PVOID			?	; 0BCh PTR PS_JOB_TOKEN_FILTER
	EndOfJobTimeAction			DWORD			?	; 0C0h
	CompletionPort				PVOID			?	; 0C4h
	CompletionKey				PVOID			?	; 0C8h
	SessionId					DWORD			?	; 0CCh
	SchedulingClass				DWORD			?	; 0D0h
								dd				?	; padding
	ReadOperationCount			QWORD			?	; 0D8h
	WriteOperationCount			QWORD			?	; 0E0h
	OtherOperationCount			QWORD			?	; 0E8h
	ReadTransferCount			QWORD			?	; 0F0h
	WriteTransferCount			QWORD			?	; 0F8h
	OtherTransferCount			QWORD			?	; 100h
	IoInfo						IO_COUNTERS		<>	; 108h
	ProcessMemoryLimit			DWORD			?	; 138h
	JobMemoryLimit				DWORD			?	; 13Ch
	PeakProcessMemoryUsed		DWORD			?	; 140h
	PeakJobMemoryUsed			DWORD			?	; 144h
	CurrentJobMemoryUsed		DWORD			?	; 148h
	MemoryLimitsLock			FAST_MUTEX		<>	; 14Ch
								DWORD			?	; padding ???
EJOB ENDS
PEJOB typedef PTR EJOB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; a part of EPROCESS structure (first member)
; also known as Process Control Block, PCB

KPROCESS STRUCT			; sizeof = 06Ch
	Header				DISPATCHER_HEADER	<>	; 000h DO_TYPE_PROCESS (0x1B)
	ProfileListHead		LIST_ENTRY			<>	; 010h
	DirectoryTableBase	DWORD				?	; 018h
	PageTableBase		DWORD				?	; 01Ch
	LdtDescriptor		KGDTENTRY			<>	; 020h
	Int21Descriptor		KIDTENTRY			<>	; 028h
	IopmOffset			WORD				?	; 030h
	Iopl				BYTE				?	; 032h
	VdmFlag				BOOLEAN				?	; 033h
	ActiveProcessors	DWORD				?	; 034h
	KernelTime			DWORD				?	; 038h ticks
	UserTime			DWORD				?	; 03Ch ticks
	ReadyListHead		LIST_ENTRY			<>	; 040h
	SwapListEntry		LIST_ENTRY			<>	; 048h
	ThreadListHead		LIST_ENTRY			<>	; 050h KTHREAD.ThreadListEntry
	ProcessLock			PVOID				?	; 058h
	Affinity			KAFFINITY			?	; 05Ch
	StackCount			WORD				?	; 060h
	BasePriority		BYTE				?	; 062h
	ThreadQuantum		BYTE				?	; 063h
	AutoAlignment		BOOLEAN				?	; 064h
	State				BYTE				?	; 065h
	ThreadSeed			BYTE				?	; 066h
	DisableBoost		BOOLEAN				?	; 067h
	PowerState			BYTE				?	; 068h
	DisableQuantum		BOOLEAN				?	; 069h
	Spare				BYTE		  2 dup(?)	; 06Ah
KPROCESS ENDS
PKPROCESS typedef PTR KPROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EPROCESS STRUCT											; sizeof = 288h
	Pcb								KPROCESS		<>
	ExitStatus						DWORD			?	; 06Ch
	LockEvent						KEVENT			<>	; 070h
	LockCount						DWORD			?	; 080h
									DWORD			?	; 084h padding
	CreateTime						LARGE_INTEGER	<>	; 088h
	ExitTime						LARGE_INTEGER	<>	; 090h
	LockOwner						PVOID			?	; 098h PTR KTHREAD
	UniqueProcessId					DWORD			?	; 09Ch
	ActiveProcessLinks				LIST_ENTRY		<>	; 0A0h
	QuotaPeakPoolUsage				DWORD	2 dup(?)	; 0A8h  NP, P
	QuotaPoolUsage					DWORD	2 dup(?)	; 0B0h  NP, P
	PagefileUsage					DWORD			?	; 0B8h
	CommitCharge					DWORD			?	; 0BCh
	PeakPagefileUsage				DWORD			?	; 0C0h
	PeakVirtualSize					DWORD			?	; 0C4h
	VirtualSize						LARGE_INTEGER	<>	; 0C8h
	Vm								MMSUPPORT		<>	; 0D0h
	SessionProcessLinks				LIST_ENTRY		<>	; 118h
	DebugPort						PVOID			?	; 120h
	ExceptionPort					PVOID			?	; 124h
	ObjectTable						PVOID			?	; 128h PTR HANDLE_TABLE
	Token							PVOID			?	; 12Ch
	WorkingSetLock					FAST_MUTEX		<>	; 130h
	WorkingSetPage					DWORD			?	; 150h
	ProcessOutswapEnabled			BYTE			?	; 154h
	ProcessOutswapped				BYTE			?	; 155h
	AddressSpaceInitialized			BYTE			?	; 156h
	AddressSpaceDeleted				BYTE			?	; 157h
	AddressCreationLock				FAST_MUTEX		<>	; 158h
	HyperSpaceLock					DWORD			?	; 178h
	ForkInProgress					PVOID			?	; 17Ch PTR ETHREAD
	VmOperation						WORD			?	; 180h
	ForkWasSuccessful				BYTE			?	; 182h
	MmAgressiveWsTrimMask			BYTE			?	; 183h
	VmOperationEvent				PVOID			?	; 184h PTR KEVENT
	PaeTop							PVOID			?	; 188h
	LastFaultCount					DWORD			?	; 18Ch
	ModifiedPageCount				DWORD			?	; 190h
	VadRoot							PVOID			?	; 194h
	VadHint							PVOID			?	; 198h
	CloneRoot						PVOID			?	; 19Ch
	NumberOfPrivatePages			DWORD			?	; 1A0h
	NumberOfLockedPages				DWORD			?	; 1A4h
	NextPageColor					WORD			?	; 1A8h
	ExitProcessCalled				BYTE			?	; 1AAh
	CreateProcessReported			BYTE			?	; 1ABh
	SectionHandle					PVOID			?	; 1ACh
	Peb								PVOID			?	; 1B0h PTR PEB
	SectionBaseAddress				PVOID			?	; 1B4h
	QuotaBlock						PVOID			?	; 1B8h PTR EPROCESS_QUOTA_BLOCK
	LastThreadExitStatus			DWORD			?	; 1BCh
	WorkingSetWatch					PVOID			?	; 1C0h PTR PAGEFAULT_HISTORY
	Win32WindowStation				PVOID			?	; 1C4h
	InheritedFromUniqueProcessId	PVOID			?	; 1C8h
	GrantedAccess					DWORD			?	; 1CCh
	DefaultHardErrorProcessing		DWORD			?	; 1D0h
	LdtInformation					PVOID			?	; 1D4h
	VadFreeHint						PVOID			?	; 1D8h
	VdmObjects						PVOID			?	; 1DCh
	DeviceMap						PVOID			?	; 1E0h
	SessionId						DWORD			?	; 1E4h
	PhysicalVadList					LIST_ENTRY		<>	; 1E8h
	union
		PageDirectoryPte			HARDWARE_PTE	<>	; 1F0h
		Filler						QWORD			?	; 1F0h
	ends
	PaePageDirectoryPage			DWORD			?	; 1F8h
	ImageFileName					BYTE	16 dup(?)	; 1FCh
	VmTrimFaultValue				DWORD			?	; 20Ch
	SetTimerResolution				BYTE			?	; 210h
	PriorityClass					BYTE			?	; 211h
	union
		struct
			SubSystemMinorVersion	BYTE			?	; 212h
			SubSystemMajorVersion	BYTE			?	; 213h
		ends
		SubSystemVersion			WORD			?	; 212h
	ends
	Win32Process					PVOID			?	; 214h
	Job								PVOID			?	; 218h PTR EJOB
	JobStatus						DWORD			?	; 21Ch
	JobLinks						LIST_ENTRY		<>	; 220h
	LockedPagesList					PVOID			?	; 228h
	SecurityPort					PVOID			?	; 22Ch
	Wow64Process					PVOID			?	; 230h PTR WOW64_PROCESS
									DWORD			?	; 234h ???
	ReadOperationCount				LARGE_INTEGER	<>	; 238h
	WriteOperationCount				LARGE_INTEGER	<>	; 240h
	OtherOperationCount				LARGE_INTEGER	<>	; 248h
	ReadTransferCount				LARGE_INTEGER	<>	; 250h
	WriteTransferCount				LARGE_INTEGER	<>	; 258h
	OtherTransferCount				LARGE_INTEGER	<>	; 260h
	CommitChargeLimit				DWORD			?	; 268h
	CommitChargePeak				DWORD			?	; 26Ch
	ThreadListHead					LIST_ENTRY		<>	; 270h
	VadPhysicalPagesBitMap			PVOID			?	; 278h PTR RTL_BITMAP
	VadPhysicalPages				DWORD			?	; 27Ch
	AweLock							DWORD			?	; 280h
									DWORD			?	; 284h padding
EPROCESS ENDS
PEPROCESS typedef PTR EPROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; a part of ETHREAD structure (first member)
; also known as Thread Control Block, TCB

KTHREAD STRUCT										; sizeof = 1B0h
	Header					DISPATCHER_HEADER	<>	; 000h DO_TYPE_THREAD (0x6C)
	MutantListHead			LIST_ENTRY			<>	; 010h
	InitialStack			PVOID				?	; 018h
	StackLimit				PVOID				?	; 01Ch
	Teb						PVOID				?	; 020h PTR TEB
	TlsArray				PVOID				?	; 024h
	KernelStack				PVOID				?	; 028h
	DebugActive				BYTE				?	; 02Ch
	State					BYTE				?	; 02Dh THREAD_STATE_*
	Alerted					BYTE		2 dup(?)	; 02Eh
	Iopl					BYTE				?	; 030h
	NpxState				BYTE				?	; 031h
	Saturation				BYTE				?	; 032h
	Priority				BYTE				?	; 033h
	ApcState				KAPC_STATE			<>	; 034h
	ContextSwitches			DWORD				?	; 04Ch
	WaitStatus				DWORD				?	; 050h
	WaitIrql				BYTE				?	; 054h
	WaitMode				BYTE				?	; 055h
	WaitNext				BYTE				?	; 056h
	WaitReason				BYTE				?	; 057h
	WaitBlockList			PVOID				?	; 058h PTR KWAIT_BLOCK
	WaitListEntry			LIST_ENTRY			<>	; 05Ch
	WaitTime				DWORD				?	; 064h
	BasePriority			BYTE				?	; 068h
	DecrementCount			BYTE				?	; 069h
	PriorityDecrement		BYTE				?	; 06Ah
	Quantum					BYTE				?	; 06Bh
	WaitBlock				KWAIT_BLOCK	4 dup(<>)	; 06Ch
	LegoData				DWORD				?	; 0CCh
	KernelApcDisable		DWORD				? 	; 0D0h
	UserAffinity			KAFFINITY			?	; 0D4h
	SystemAffinityActive	BYTE				?	; 0D8h
	PowerState				BYTE				?	; 0D9h
	NpxIrql					BYTE				?	; 0DAh
	Pad						BYTE				?	; 0DBh
	ServiceTable			PVOID				?	; 0DCh PTR SERVICE_DESCRIPTOR_TABLE
	Queue					PVOID				?	; 0E0h PTR KQUEUE
	ApcQueueLock			DWORD				?	; 0E4h
	Timer					KTIMER				<>	; 0E8h
	QueueListEntry			LIST_ENTRY			<>	; 110h
	Affinity				KAFFINITY			?	; 118h
	Preempted				BYTE				?	; 11Ch
	ProcessReadyQueue		BYTE				?	; 11Dh
	KernelStackResident		BYTE				?	; 11Eh
	NextProcessor			BYTE				?	; 11Fh
	CallbackStack			PVOID				?	; 120h
	Win32Thread				PVOID				?	; 124h PTR WIN32_THREAD ???
	TrapFrame				PVOID				?	; 128h PTR KTRAP_FRAME
	ApcStatePointer			PVOID		2 dup(?)	; 12Ch PTR KAPC_STATE
	PreviousMode			KPROCESSOR_MODE		?	; 134h
	EnableStackSwap			BOOLEAN				?	; 135h
	LargeStack				BOOLEAN				?	; 136h
	ResourceIndex			BYTE				?	; 137h
	KernelTime				DWORD				?	; 138h ticks
	UserTime				DWORD				?	; 13Ch ticks
	SavedApcState			KAPC_STATE			<>	; 140h
	Alertable				BOOLEAN				?	; 158h
	ApcStateIndex			BYTE				?	; 159h
	ApcQueueable			BOOLEAN				?	; 15Ah
	AutoAlignment			BOOLEAN				?	; 15Bh
	StackBase				PVOID				?	; 15Ch
	SuspendApc				KAPC				<>	; 160h
	SuspendSemaphore		KSEMAPHORE			<>	; 190h
	ThreadListEntry			LIST_ENTRY			<>	; 1A4h see KPROCESS
	FreezeCount				BYTE				?	; 1ACh
	SuspendCount			BYTE				?	; 1ADh
	IdealProcessor			BYTE				?	; 1AEh
	DisableBoost			BYTE				?	; 1AFh
KTHREAD ENDS
PKTHREAD typedef PTR KTHREAD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; ...record field names must be unique...
; I named it as ETHREAD_BITS
ETHREAD_BITS RECORD \
			EthreadBitsReserved:29,	; bits 3-31
			ApcNeeded:1,			; bits 2-2
			NestedFaultCount:2		; bits 0-1

ETHREAD STRUCT										; sizeof = 248h
	Tcb							KTHREAD			<>	; 000h
	union
		CreateTime					LARGE_INTEGER	<>	; 1B0h
		EthreadBits					ETHREAD_BITS	<>	; 1B0h
	ends
	union
		ExitTime				LARGE_INTEGER	<>	; 1B8h
		LpcReplyChain			LIST_ENTRY		<>	; 1B8h
	ends
	union
		ExitStatus				DWORD		?	; 1C0h
		OfsChain				PVOID		?	; 1C0h
	ends
	PostBlockList				LIST_ENTRY	<>	; 1C4h
	TerminationPortList			LIST_ENTRY	<>	; 1CCh
	ActiveTimerListLock			DWORD		?	; 1D4h
	ActiveTimerListHead			LIST_ENTRY	<>	; 1D8h
	Cid							CLIENT_ID	<>	; 1E0h
	LpcReplySemaphore			KSEMAPHORE	<>	; 1E8h
	LpcReplyMessage				PVOID		?	; 1FCh
	LpcReplyMessageId			DWORD		?	; 200h
	PerformanceCountLow			DWORD		?	; 204h
	ImpersonationInfo			PVOID		?	; 208h PTR PS_IMPERSONATION_INFORMATION
	IrpList						LIST_ENTRY	<>	; 20Ch
	TopLevelIrp					DWORD		?	; 214h
	DeviceToVerify				PVOID		?	; 218h PTR DEVICE_OBJECT
	ReadClusterSize				UINT		?	; 21Ch
	ForwardClusterOnly			BYTE		?	; 220h
	DisablePageFaultClustering	BYTE		?	; 221h
	DeadThread					BYTE		?	; 222h
	HideFromDebugger			BYTE		?	; 223h
	HasTerminated				DWORD		?	; 224h
	GrantedAccess				DWORD		?	; 228h
	ThreadsProcess				PVOID		?	; 22Ch PTR EPROCESS
	StartAddress				PVOID		?	; 230h
	union
		Win32StartAddress		PVOID		?	; 234h
		LpcReceivedMessageId	DWORD		?	; 234h
	ends
	LpcExitThreadCalled			BYTE		?	; 238h
	HardErrorsAreDisabled		BYTE		?	; 239h
	LpcReceivedMsgIdValid		BYTE		?	; 23Ah
	ActiveImpersonationInfo		BYTE		?	; 23Bh
	PerformanceCountHigh		SDWORD		?	; 23Ch
	ThreadListEntry				LIST_ENTRY	<>	; 240h
ETHREAD ENDS
PETHREAD typedef PTR ETHREAD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KQUEUE STRUCT		 ;sizeof = 28h
	Header			DISPATCHER_HEADER	<>
	EntryListHead	LIST_ENTRY			<>	; 10h
	CurrentCount	DWORD				?	; 18h
	MaximumCount	DWORD				?	; 1Ch
	ThreadListHead	LIST_ENTRY			<>	; 20h
KQUEUE ENDS
PKQUEUE typedef PTR KQUEUE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PEB_LDR_DATA STRUCT					; sizeof = 24h
	_Length							DWORD		?	; original name Length
	Initialized						BYTE		?	; 04h
									db 	3 dup(?)	; padding
	SsHandle						PVOID		?	; 08h
	InLoadOrderModuleList			LIST_ENTRY	<>	; 0Ch
	InMemoryOrderModuleList			LIST_ENTRY	<>	; 14h
	InInitializationOrderModuleList	LIST_ENTRY	<>	; 1Ch
PEB_LDR_DATA ENDS
PPEB_LDR_DATA typedef PTR PEB_LDR_DATA

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RTL_DRIVE_LETTER_CURDIR STRUCT	; sizeof = 10h
	Flags		WORD		?	; 00h
	_Length		WORD		?	; 02h original name Length
	TimeStamp	DWORD		?	; 04h
	DosPath		_STRING		<>	; 08h
RTL_DRIVE_LETTER_CURDIR ENDS
PRTL_DRIVE_LETTER_CURDIR typedef PTR RTL_DRIVE_LETTER_CURDIR

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CURDIR STRUCT		; sizeof = 0Ch
	DosPath		UNICODE_STRING	<>	; 0
	Handle		PVOID			?	; 8
CURDIR ENDS
PCURDIR typedef ptr CURDIR

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PEB_FREE_BLOCK STRUCT	; sizeof = 8
	Next		PVOID	?	; PTR PEB_FREE_BLOCK
	_Size		DWORD	?	; original name Size
PEB_FREE_BLOCK ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RTL_USER_PROCESS_PARAMETERS STRUCT	; sizeof = 290h
	MaximumLength		DWORD			?	; 000h
	_Length			DWORD			?	; 004h original name Length
	Flags				DWORD			?	; 008h
	DebugFlags			DWORD			?	; 00Ch
	ConsoleHandle		PVOID			?	; 010h
	ConsoleFlags		DWORD			?	; 014h
	StandardInput		PVOID			?	; 018h
	StandardOutput		PVOID			?	; 01Ch
	StandardError		PVOID			?	; 020h
	CurrentDirectory	CURDIR			<>	; 024h
	DllPath				UNICODE_STRING	<>	; 030h
	ImagePathName		UNICODE_STRING	<>	; 038h
	CommandLine			UNICODE_STRING	<>	; 040h
	Environment			PVOID			?	; 048h
	StartingX			DWORD			?	; 04Ch
	StartingY			DWORD			?	; 050h
	CountX				DWORD			?	; 054h
	CountY				DWORD			?	; 058h
	CountCharsX			DWORD			?	; 05Ch
	CountCharsY			DWORD			?	; 060h
	FillAttribute		DWORD			?	; 064h
	WindowFlags			DWORD			?	; 068h
	ShowWindowFlags		DWORD			?	; 06Ch
	WindowTitle			UNICODE_STRING	<>	; 070h
	DesktopInfo			UNICODE_STRING	<>	; 078h
	ShellInfo			UNICODE_STRING	<>	; 080h
	RuntimeData			UNICODE_STRING	<>	; 088h
	CurrentDirectores	RTL_DRIVE_LETTER_CURDIR 32 dup(<>)	; 090h
RTL_USER_PROCESS_PARAMETERS ENDS
PRTL_USER_PROCESS_PARAMETERS typedef ptr RTL_USER_PROCESS_PARAMETERS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Process Environment Block (PEB)
; located at 7FFDF000h (pointed by fs:[30] in user mode)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PEB STRUCT							; sizeof = 1E8h
	InheritedAddressSpace			BYTE		?		; 0000h
	ReadImageFileExecOptions		BYTE		?		; 0001h
	BeingDebugged					BYTE		?		; 0002h
	SpareBool						BYTE		?		; 0003h
	Mutant							PVOID		?		; 0004h
	ImageBaseAddress				PVOID		?		; 0008h
	Ldr								PVOID		?		; 000Ch PTR PEB_LDR_DATA
	ProcessParameters				PVOID		?		; 0010h PTR RTL_USER_PROCESS_PARAMETERS
	SubSystemData					PVOID		?		; 0014h
	ProcessHeap						PVOID		?		; 0018h
	FastPebLock						PVOID		?		; 001Ch
	FastPebLockRoutine				PVOID		?		; 0020h
	FastPebUnlockRoutine			PVOID		?		; 0024h
	EnvironmentUpdateCount			DWORD		?		; 0028h
	KernelCallbackTable				PVOID		?		; 002Ch
	SystemReserved					DWORD	2 dup(?)	; 0030h
	FreeList						PVOID		?		; 0038h PTR PEB_FREE_BLOCK
	TlsExpansionCounter				DWORD		?		; 003Ch
	TlsBitmap						PVOID		?		; 0040h
	TlsBitmapBits					DWORD	2 dup(?)	; 0044h
	ReadOnlySharedMemoryBase		PVOID		?		; 004Ch
	ReadOnlySharedMemoryHeap		PVOID		?		; 0050h
	ReadOnlyStaticServerData		PVOID		?		; 0054h
	AnsiCodePageData				PVOID		?		; 0058h
	OemCodePageData					PVOID		?		; 005Ch
	UnicodeCaseTableData			PVOID		?		; 0060h
	NumberOfProcessors				DWORD		?		; 0064h
	NtGlobalFlag					DWORD		?		; 0068h
									DWORD		?		; 006Ch
	CriticalSectionTimeout			LARGE_INTEGER	<>	; 0070h
	HeapSegmentReserve				DWORD		?		; 0078h
	HeapSegmentCommit				DWORD		?		; 007Ch
	HeapDeCommitTotalFreeThreshold	DWORD		?		; 0080h
	HeapDeCommitFreeBlockThreshold	DWORD		?		; 0084h
	NumberOfHeaps					DWORD		?		; 0088h
	MaximumNumberOfHeaps			DWORD		?		; 008Ch
	ProcessHeaps					PVOID		?		; 0090h
	GdiSharedHandleTable			PVOID		?		; 0094h
	ProcessStarterHelper			PVOID		?		; 0098h
	GdiDCAttributeList				DWORD		?		; 009Ch
	LoaderLock						PVOID		?		; 00A0h
	OSMajorVersion					DWORD		?		; 00A4h
	OSMinorVersion					DWORD		?		; 00A8h
	OSBuildNumber					WORD		?		; 00ACh
	OSCSDVersion					WORD		?		; 00AEh
	OSPlatformId					DWORD		?		; 00B0h
	ImageSubsystem					DWORD		?		; 00B4h
	ImageSubsystemMajorVersion		DWORD		?		; 00B8h
	ImageSubsystemMinorVersion		DWORD		?		; 00BCh
	ImageProcessAffinityMask		DWORD		?		; 00C0h
	GdiHandleBuffer					DWORD	34 dup(?)	; 00C4h
	PostProcessInitRoutine			DWORD		?		; 014Ch
	TlsExpansionBitmap				PVOID		?		; 0150h
	TlsExpansionBitmapBits			DWORD	32 dup(?)	; 0154h
	SessionId						DWORD		?		; 01D4h
	AppCompatInfo					PVOID		?		; 01D8h
	CSDVersion						UNICODE_STRING	<>	; 01DCh
									DWORD		?		; 01E4h
PEB ENDS
PPEB typedef PTR PEB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Thread Environment Block (TEB)
; First Teb located at 7FFDE000h (pointed by fs:[18] in user mode)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; This structure is not present even in PDB files. I found it somewhere.

TEB STRUCT
	Tib								NT_TIB		<>		; 000h
	EnvironmentPointer				PVOID		?		; 01Ch
	Cid								CLIENT_ID	<>		; 020h
	ActiveRpcInfo					PVOID		?		; 028h
	ThreadLocalStoragePointer		PVOID		?		; 02Ch
	Peb								PVOID		?		; 030h PTR PEB
	LastErrorValue					DWORD		?		; 034h
	CountOfOwnedCriticalSections	DWORD		?		; 038h
	CsrClientThread					PVOID		?		; 03Ch
	Win32ThreadInfo					PVOID		?		; 040h
	Win32ClientInfo					DWORD	1Fh dup(?)	; 044h
	WOW32Reserved					PVOID		?		; 0C0h
	CurrentLocale					DWORD		?		; 0C4
	FpSoftwareStatusRegister		DWORD		?		; 0C8
	SystemReserved1					PVOID	36h dup(?)	; 0CC
	Spare1							PVOID		?		; 1A4
	ExceptionCode					DWORD		?		; 1A8
	SpareBytes1						DWORD	28h dup(?)	; 1AC
	SystemReserved2					PVOID	0Ah dup(?)	; 24C
	GdiRgn							DWORD		?		; 274
	GdiPen							DWORD		?		; 278
	GdiBrush						DWORD		?		; 27C
	RealClientId					CLIENT_ID	<>		; 6B4h
	GdiCachedProcessHandle			PVOID		?		; 
	GdiClientPID					DWORD		?		; 
	GdiClientTID					DWORD		?		; 
	GdiThreadLocaleInfo				PVOID		?		; 
	UserReserved					PVOID	5 dup(?)	; 
	GlDispatchTable					PVOID	118h dup(?)	; 
	GlReserved1						DWORD	1Ah dup(?)	; 
	GlReserved2						PVOID		?		; 
	GlSectionInfo					PVOID		?		; 
	GlSection						PVOID		?		; 
	GlTable							PVOID		?		; 
	GlCurrentRC						PVOID		?		; 
	GlContext						PVOID		?		; 
	LastStatusValue					DWORD		?		; BF4h NTSTATUS  !!! 100%
	StaticUnicodeString				UNICODE_STRING	<>	; 
	StaticUnicodeBuffer				WORD	105h dup(?)	; WCHAR
	DeallocationStack				PVOID		?		; 
	TlsSlots						PVOID	40h dup(?)	; 
	TlsLinks						LIST_ENTRY	<>		; 0
	Vdm								PVOID		?		; 
	ReservedForNtRpc				PVOID		?		; 
	DbgSsReserved					PVOID	2 dup(?)	; 
	HardErrorDisabled				DWORD		?		; F28h HardErrorsMode 100%
	Instrumentation					PVOID	10h dup(?)	; 
	WinSockData						PVOID		?		; 
	GdiBatchCount					DWORD		?		; 
	Spare2							DWORD		?		; 
	Spare3							DWORD		?		; 
	Spare4							DWORD		?		; 
	ReservedForOle					PVOID		?		; 
	WaitingOnLoaderLock				DWORD		?		; 
	StackCommit						PVOID		?		; 
	StackCommitMax					PVOID		?		; 
	StackReserved					PVOID		?		; 
TEB ENDS
PTEB typedef PTR TEB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Task Segment State
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KiIoAccessMap STRUCT		; sizeof= 2024h
	DirectionMap	BYTE 32 dup(?)
	IoMap			BYTE 8196 dup(?)	; 020h
KiIoAccessMap ENDS

KTSS STRUCT			; sizeof = 20ACh
	Backlink		WORD		?	; 0000
	Reserved0		WORD		?	; 0000
	Esp0			DWORD		?	; 0004h
	Ss0				WORD		?	; 0008h
	Reserved1		WORD		?	; 000Ah
	NotUsed1		DWORD 4 dup(?)	; 000Ch
	rCR3			DWORD		?	; 001Ch original field name CR3
	Eip				DWORD		?	; 0020h
	NotUsed2		DWORD 9 dup(?)	; 0024h
	rEs				WORD		?	; 0048h original field name Es
	Reserved2		WORD		?	; 004Ah
	rCs				WORD		?	; 004Ch original field name Cs
	Reserved3		WORD		?	; 004Eh
	rSs				WORD		?	; 0050h original field name Ss
	Reserved4		WORD		?	; 0052h
	rDs				WORD		?	; 0054h original field name Ds
	Reserved5		WORD		?	; 0056h
	rFs				WORD		?	; 0058h original field name Fs
	Reserved6		WORD		?	; 005Ah
	rGs				WORD		?	; 005Ch original field name Gs
	Reserved7		WORD		?	; 005Eh
	LDT				WORD		?	; 0060h
	Reserved8		WORD		?	; 0062h
	Flags			WORD		?	; 0064h
	IoMapBase		WORD		?	; 0066h
	IoMaps			KiIoAccessMap	<>	; 0068h
	IntDirectionMap	BYTE 32 dup(?)	; 208Ch
KTSS ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OBJECT_NAME STRUCT				; sizeof = 10h	 (by Nebbett)
	Directory	PVOID			?	; 0000h  PTR OBJECT_DIRECTORY
	_Name		UNICODE_STRING	<>	; 0004h
				DWORD			?	; 000Ch Reserved
OBJECT_NAME ENDS

OBJECT_TYPE_INITIALIZER STRUCT		; sizeof = 04Ch
	_Length						WORD		?	; 0000h  (org name Length)
	UseDefaultObject			BYTE		?	; 0002h
	Reserved					BYTE		?	; 0003h
	InvalidAttributes			DWORD		?	; 0004h
	GenericMapping				GENERIC_MAPPING <>	; 0008h
	ValidAccessMask				DWORD		?	; 0018h
	SecurityRequired			BYTE		?	; 001Ch
	MaintainHandleCount			BYTE		?	; 001Dh
	MaintainTypeList			BYTE		?	; 001Eh
								db 1 dup(?)	; padding
	PoolType					SDWORD		?	; 0020h
	DefaultPagedPoolCharge		DWORD		?	; 0024h
	DefaultNonPagedPoolCharge	DWORD		?	; 0028h
	DumpProcedure				PVOID		?	; 002Ch
	OpenProcedure				PVOID		?	; 0030h
	CloseProcedure				PVOID		?	; 0034h
	DeleteProcedure				PVOID		?	; 0038h
	ParseProcedure				PVOID		?	; 003Ch
	SecurityProcedure			PVOID		?	; 0040h
	QueryNameProcedure			PVOID		?	; 0044h
	OkayToCloseProcedure		PVOID		?	; 0048h
OBJECT_TYPE_INITIALIZER ENDS

OBJECT_TYPE STRUCT				; sizeof = 0B0h
	Mutex						ERESOURCE 		<>	; 0000h
	TypeList					LIST_ENTRY 		<>	; 0038h
	_Name						UNICODE_STRING 	<>	; 0040h  (org name Name)
	DefaultObject				PVOID			?	; 0048h
	Index						DWORD			?	; 004Ch
	TotalNumberOfObjects		DWORD			?	; 0050h
	TotalNumberOfHandles		DWORD			?	; 0054h
	HighWaterNumberOfObjects	DWORD			?	; 0058h
	HighWaterNumberOfHandles	DWORD			?	; 005Ch
	TypeInfo					OBJECT_TYPE_INITIALIZER <>	; 0060h
	Key							DWORD			?	; 00ACh
OBJECT_TYPE ENDS

OBJECT_CREATE_INFORMATION STRUCT			; sizeof = 030h
	Attributes					DWORD		?	; 0000h
	RootDirectory				PVOID		?	; 0004h
	ParseContext				PVOID		?	; 0008h
	ProbeMode					BYTE		?	; 000Ch
								db 	3 dup(?)	; padding
	PagedPoolCharge				DWORD		?	; 0010h
	NonPagedPoolCharge			DWORD		?	; 0014h
	SecurityDescriptorCharge	DWORD		?	; 0018h
	SecurityDescriptor			PVOID		?	; 001Ch
	SecurityQos					PVOID		?	; 0020h PTR SECURITY_QUALITY_OF_SERVICE
	SecurityQualityOfService	SECURITY_QUALITY_OF_SERVICE <>	; 0024h
OBJECT_CREATE_INFORMATION ENDS

OBJECT_HEADER STRUCT						; sizeof = 018h
	PointerCount			SDWORD		?	; 0000h
	union
		HandleCount			SDWORD		?	; 0004h
		SEntry				PVOID		?	; 0004h PTR SINGLE_LIST_ENTRY
	ends
	_Type					PVOID		?	; 0008h PTR OBJECT_TYPE  (original name Type)
	NameInfoOffset			BYTE		?	; 000Ch
	HandleInfoOffset		BYTE		?	; 000Dh
	QuotaInfoOffset			BYTE		?	; 000Eh
	Flags					BYTE		?	; 000Fh
	union
		ObjectCreateInfo	PVOID		?	; 0010h PTR OBJECT_CREATE_INFORMATION
		QuotaBlockCharged	PVOID		?	; 0010h
	ends
	SecurityDescriptor		PVOID		?	; 0014h
;	Body					QUAD 		<>	; 0018h
OBJECT_HEADER ENDS

; Processor Control Block (PRCB)

PRCB_MINOR_VERSION		equ 1
PRCB_MAJOR_VERSION		equ 1
PRCB_BUILD_DEBUG		equ 1
PRCB_BUILD_UNIPROCESSOR	equ 2

; KPRCB base address is 0FFDFF120h

KPRCB STRUCT		; sizeof = XXX

	; Major and minor version numbers of the PCR.

	MinorVersion	WORD	?
	MajorVersion	WORD	?

	; Start of the architecturally defined section of the PRCB. This section
	; may be directly addressed by vendor/platform specific HAL code and will
	; not change from version to version of NT.

	CurrentThread	PVOID		? ; 04h PTR KTHREAD
	NextThread		PVOID		? ; 08h PTR KTHREAD
	IdleThread		PVOID		? ; 0Ch PTR KTHREAD
	Number			CHAR 		? ; 10h
	Reserved		CHAR 		? ; 11h
	BuildType		WORD		? ; 12h
	SetMember		KAFFINITY	? ; 14h

	; End of the architecturally defined section of the PRCB. This section
	; may be directly addressed by vendor/platform specific HAL code and will
	; not change from version to version of NT.

	; To be continued...

KPRCB ENDS
PKPRCB typedef PTR KPRCB

VAD STRUCT
	StartingPageAddress		PVOID	?	; 0000h in pages
	EndingPageAddress		PVOID	?	; 0004h in pages
	ParentLink				PVOID	?	; 0008h PTR VAD
	LeftLink				PVOID	?	; 000Ch PTR VAD
	RightLink				PVOID	?	; 0010h PTR VAD
	Commit					WORD	?	; 0014h number of pages
	Flags					WORD	?	; 0016h
	ControlArea				PVOID	?	; 0018h PTR CONTROL_AREA
	ThePtes					PVOID	?	; 001C; PTR _MMPTE  SEGMENT.ThePtes[1]
VAD ENDS
PVAD typedef ptr VAD


SUBSECTION STRUCT
	ControlArea				PVOID	?	; 0000h PTR CONTROL_AREA
	union
	Flags					DWORD	?	; 0004h
	ssBits RECORD \
		ssfUnknown2:24,		; bits 8-31
		Protection:4,		; bits 4-7
		ssfUnknown1:4		; bits 0-3
	ends
	StartingSector			DWORD	?	; 0008h
	NumberOfSectors			DWORD	?	; 000Ch
	BasePte					DWORD	?	; 0010h
	UnusedPtes				DWORD	?	; 0014h
	PtesInSubsect			DWORD	?	; 0018h
	Next					PVOID	?	; 001Ch PTR SUBSECTION if any NULL if last
SUBSECTION ENDS
PSUBSECTION typedef ptr SUBSECTION
