comment ^

Module Name:
    wxpundoc.inc

Abstract:
    This module defines some undocumented Windows XP structures and constants.

Author:
    Four-F (four-f@mail.ru)

Last Update:
	08-Jul-2003

Comments:
	Hand made -> Bugs are very possible ;(
	Your bug report is very welcome.

^
IFNDEF KAFFINITY
	include \masm32\include\w2k\ntddk.inc
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KGDTENTRY STRUCT		; sizeof = 8
	LimitLow		WORD	?
	BaseLow			WORD	?
	union _HighWord		; original HighWord
		struct Bytes		; __unnamed_26_
			BaseMid	BYTE	?
			Flags1	BYTE	?
			Flags2	BYTE	?
			BaseHi	BYTE	?
		ends
		; Damn! ...record field names must be unique...
		; kd displays it as __unnamed15, so i prepend each field name with 'u15'
		Bits RECORD \
			u15BaseHi:8,		; bits24-31 BaseHi
			u15Granularity:1,	; bits23-23 Granularity
			u15Default_Big:1,	; bits22-22 Default_Big
			u15Reserved_0:1,	; bits21-21 Reserved_0
			u15Sys:1,			; bits20-20 Sys
			u15LimitHi:4,		; bits16-19 LimitHi
			u15Pres:1,			; bits15-15 Pres
			u15Dpl:2,			; bits13-14 Dpl
			u15Type:5,			; bits8-12  Type
			u15BaseMid:8		; bits0-7   BaseMid
	ends ; HighWord
KGDTENTRY ENDS
PKGDTENTRY typedef PTR KGDTENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KIDTENTRY STRUCT		; sizeof = 8
	_Offset		WORD	?	; original name Offset
	Selector		WORD	?
	Access			WORD	?
	ExtendedOffset	WORD	?
KIDTENTRY ENDS
PKIDTENTRY typedef PTR KIDTENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; a part of EPROCESS structure (first member)
; also known as Process Control Block, PCB
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KPROCESS STRUCT			; sizeof = 06Ch
	Header				DISPATCHER_HEADER	<> ; DO_TYPE_PROCESS (0x1B)
	ProfileListHead		LIST_ENTRY			<>
	DirectoryTableBase	DWORD		2 dup(?)	; 018h Physical address of Page Data Directory (cr3 contains the same phys address)
	LdtDescriptor		KGDTENTRY			<>	; 020h
	Int21Descriptor		KIDTENTRY			<>	; 028h
	IopmOffset			WORD				?	; 030h
	Iopl				BYTE				?	; 032h
	Unused				BOOLEAN				?	; 033h
	ActiveProcessors	DWORD				?	; 034h
	KernelTime			DWORD				?	; ticks
	UserTime			DWORD				?	; ticks
	ReadyListHead		LIST_ENTRY			<>	; 040h
	SwapListEntry		SINGLE_LIST_ENTRY	<>	; 048h
	VdmTrapcHandler		PVOID				?	; 04Ch
	ThreadListHead		LIST_ENTRY			<>	; KTHREAD.ThreadListEntry
	ProcessLock			PVOID				?	; 058h KSPIN_LOCK
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
	IdealNode			BYTE				?	; 06Ah
	Spare				BYTE				?	; 06Bh
KPROCESS ENDS
PKPROCESS typedef PTR KPROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EX_PUSH_LOCK STRUCT		; sizeof = 4h
	union
		epl RECORD \
			eplShared:30,	; bits 2-31
			eplExclusive:1,	; bits 1
			eplWaiting:1	; bits 0
		Value			DWORD	?
		_Ptr			PVOID	?	; original field name "Ptr"
	ends
EX_PUSH_LOCK ENDS
PEX_PUSH_LOCK typedef PTR EX_PUSH_LOCK

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EX_RUNDOWN_REF STRUCT		; sizeof = 4h
	union
		Count		DWORD	?
		_Ptr		PVOID	?	; original field name "Ptr"
	ends
EX_RUNDOWN_REF ENDS
PEX_RUNDOWN_REF typedef PTR EX_RUNDOWN_REF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EX_FAST_REF STRUCT		; sizeof = 4h
	union
		Object			PVOID	?
		efr RECORD \
			efrUndefined:28,; bits 3-31
			efrRefCnt:3		; bits 0-2
		Value			DWORD	?	; 00h
	ends
EX_FAST_REF ENDS
PEX_FAST_REF typedef PTR EX_FAST_REF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

HARDWARE_PTE RECORD \
	PageFrameNumber:20,	; bits12-31 PageFrameNumber
	_reserved:1,			; bits11-11 reserved
	Prototype:1,		; bits10-10 Prototype
	CopyOnWrite:1,		; bits9-9 CopyOnWrite
	Global:1,			; bits8-8 Global
	LargePage:1,		; bits7-7 LargePage
	Dirty:1,			; bits6-6 Dirty
	Accessed:1,			; bits5-5 Accessed
	CacheDisable:1,		; bits4-4 CacheDisable
	WriteThrough:1,		; bits3-3 WriteThrough
	Owner:1, 			; bits2-2 Owner
	Write:1,			; bits1-1 Write
	Valid:1				; bits0-0 Valid

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SE_AUDIT_PROCESS_CREATION_INFO STRUCT
	ImageFileName	PVOID	?	; PTR OBJECT_NAME_INFORMATION
SE_AUDIT_PROCESS_CREATION_INFO ENDS
PSE_AUDIT_PROCESS_CREATION_INFO typedef PTR SE_AUDIT_PROCESS_CREATION_INFO

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MMSUPPORT_FLAGS RECORD \
	MemoryPriority:8,			; bits 24-31
	AllowWorkingSetAdjustment:8,; bits 16-23
	Available:10,				; bits 6-15
	AddressSpaceBeingDeleted:1,	; bits 5
	WorkingSetHard:1,			; bits 4
	TrimHard:1,					; bits 3
	SessionLeader:1,			; bits 2
	BeingTrimmed:1,				; bits 1
	SessionSpace:1				; bits 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MMSUPPORT STRUCT				; sizeof = 40h
	LastTrimTime				LARGE_INTEGER			<>	; 00h
	Flags						MMSUPPORT_FLAGS			<>	; 08h
	PageFaultCount				DWORD		?	; 00ch
	PeakWorkingSetSize			DWORD		?	; 010h
	WorkingSetSize				DWORD		?	; 014h
	MinimumWorkingSetSize		DWORD		?	; 018h
	MaximumWorkingSetSize		DWORD		?	; 01ch
	VmWorkingSetList			PVOID		?	; 020h PTR MMWSL
	WorkingSetExpansionLinks	LIST_ENTRY	<>	; 024h
	Claim						DWORD		?	; 02ch
	NextEstimationSlot			DWORD		?	; 030h
	NextAgingSlot				DWORD		?	; 034h
	EstimatedAvailable			DWORD		?	; 038h
	GrowthSinceLastEstimate		DWORD		?	; 03ch
MMSUPPORT ENDS
PMMSUPPORT typedef PTR MMSUPPORT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EPROCESS STRUCT						; sizeof = 258h
	Pcb								KPROCESS		<>	; 0000h
	ProcessLock						EX_PUSH_LOCK	<>	; 006ch
	CreateTime						LARGE_INTEGER	<>	; 0070h
	ExitTime						LARGE_INTEGER	<>	; 0078h
	RundownProtect					EX_RUNDOWN_REF	<>	; 0080h
	UniqueProcessId					DWORD			?	; 0084h
	ActiveProcessLinks				LIST_ENTRY		<>	; 0088h
	QuotaUsage						DWORD	3 dup(?)	; 0090h
	QuotaPeak						DWORD	3 dup(?)	; 009ch
	CommitCharge					DWORD			?	; 00a8h
	PeakVirtualSize					DWORD			?	; 00ach
	VirtualSize						DWORD			?	; 00b0h
	SessionProcessLinks				LIST_ENTRY		<>	; 00b4h
	DebugPort						PVOID			?	; 00bch
	ExceptionPort					PVOID			?	; 00c0h
	ObjectTable						PVOID			?	; 00c4h PTR HANDLE_TABLE
	Token							EX_FAST_REF		<>	; 00c8h
	WorkingSetLock					FAST_MUTEX		<>	; 00cch
	WorkingSetPage					DWORD			?	; 00ech
	AddressCreationLock				FAST_MUTEX		<>	; 00f0h
	HyperSpaceLock					DWORD			?	; 0110h
	ForkInProgress					PVOID			?	; 0114h PTR ETHREAD
	HardwareTrigger					DWORD			?	; 0118h
	VadRoot							PVOID			?	; 011ch
	VadHint							PVOID			?	; 0120h
	CloneRoot						PVOID			?	; 0124h
	NumberOfPrivatePages			DWORD			?	; 0128h
	NumberOfLockedPages				DWORD			?	; 012ch
	Win32Process					PVOID			?	; 0130h
	Job								PVOID			?	; 0134h PTR EJOB
	SectionObject					PVOID			?	; 0138h
	SectionBaseAddress				PVOID			?	; 013ch
	QuotaBlock						PVOID			?	; 0140h PTR EPROCESS_QUOTA_BLOCK
	WorkingSetWatch					PVOID			?	; 0144h PTR PAGEFAULT_HISTORY
	Win32WindowStation				PVOID			?	; 0148h
	InheritedFromUniqueProcessId	PVOID			?	; 014ch
	LdtInformation					PVOID			?	; 0150h
	VadFreeHint						PVOID			?	; 0154h
	VdmObjects						PVOID			?	; 0158h
	DeviceMap						PVOID			?	; 015ch
	PhysicalVadList					LIST_ENTRY		<>	; 0160h
	union
		PageDirectoryPte			HARDWARE_PTE	<>	; 0168h
		Filler						QWORD			?	; 0168h
	ends
	Session							PVOID			?	; 0170h
	ImageFileName					BYTE	16 dup(?)	; 0174h
	JobLinks						LIST_ENTRY		<>	; 0184h
	LockedPagesList					PVOID			?	; 018ch
	ThreadListHead					LIST_ENTRY		<>	; 0190h
	SecurityPort					PVOID			?	; 0198h
	PaeTop							PVOID			?	; 019ch
	ActiveThreads					DWORD			?	; 01a0h
	GrantedAccess					DWORD			?	; 01a4h
	DefaultHardErrorProcessing		DWORD			?	; 01a8h
	LastThreadExitStatus			SDWORD			?	; 01ach
	Peb								PVOID			?	; 01b0h PTR PEB
	PrefetchTrace					EX_FAST_REF		<>	; 01b4h
	ReadOperationCount				LARGE_INTEGER	<>	; 01b8h
	WriteOperationCount				LARGE_INTEGER	<>	; 01c0h
	OtherOperationCount				LARGE_INTEGER	<>	; 01c8h
	ReadTransferCount				LARGE_INTEGER	<>	; 01d0h
	WriteTransferCount				LARGE_INTEGER	<>	; 01d8h
	OtherTransferCount				LARGE_INTEGER	<>	; 01e0h
	CommitChargeLimit				DWORD			?	; 01e8h
	CommitChargePeak				DWORD			?	; 01ech
	AweInfo							PVOID			?	; 01f0h
	SeAuditProcessCreationInfo		SE_AUDIT_PROCESS_CREATION_INFO	<>	; 01f4h
	Vm								MMSUPPORT		<>	; 01f8h
	LastFaultCount					DWORD			?	; 0238h
	ModifiedPageCount				DWORD			?	; 023ch
	NumberOfVads					DWORD			?	; 0240h
	JobStatus						DWORD			?	; 0244h
	union
		Flags						DWORD			?	; 0248h
comment ^
		recFlags RECORD \
			Unused:11,					; bits 21-31
			InjectInpageErrors:1,		; bits 20
			LaunchPrefetched:1,			; bits 19
			HasAddressSpace:1,			; bits 18
			OverrideAddressSpace:1,		; bits 17
			ProcessInSession:1,			; bits 16
			WriteWatch:1,				; bits 15
			SessionCreationUnderway:1,	; bits 14
			BreakOnTermination:1,		; bits 13
			SetTimerResolution:1,		; bits 12
			AddressSpaceInitialized:2,	; bits 10-11
			HasPhysicalVad:1,			; bits 9
			ForkFailed:1,				; bits 8
			Outswapped:1,				; bits 7
			OutswapEnabled:1,			; bits 6
			VmDeleted:1,				; bits 5
			Wow64SplitPages:1,			; bits 4
			ProcessDelete:1,			; bits 3
			ProcessExiting:1,			; bits 2
			NoDebugInherit:1,			; bits 1
			CreateReported:1			; bits 0
^
	ends
   	ExitStatus						SDWORD			?	; 024ch
   	NextPageColor					WORD			?	; 0250h
   	SubSystemMinorVersion			BYTE			?	; 0252h
   	SubSystemMajorVersion			BYTE			?	; 0253h
   	SubSystemVersion				WORD			?	; 0252h
   	PriorityClass					BYTE			?	; 0254h
   	WorkingSetAcquiredUnsafe		BYTE			?	; 0255h
EPROCESS ENDS
PEPROCESS typedef PTR EPROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KAPC_STATE STRUCT		; sizeof = 018h
	ApcListHead			LIST_ENTRY	2 dup(<>)	; 0000h
	Process				PVOID		?	; 0010h PTR KPROCESS
	KernelApcInProgress	BYTE		?	; 0014h
	KernelApcPending	BYTE		?	; 0015h
	UserApcPending		BYTE		?	; 0016h
						BYTE		?	; 0017h  padding
KAPC_STATE ENDS

KTHREAD STRUCT										; sizeof = 01C0h
	Header					DISPATCHER_HEADER	<>	; 0000h
	MutantListHead			LIST_ENTRY			<>		; 0010h
	InitialStack			PVOID				?		; 0018h
	StackLimit				PVOID				?		; 001Ch
	Teb						PVOID				?		; 0020h
	TlsArray				PVOID				?		; 0024h
	KernelStack				PVOID				?	; 0028h
	DebugActive				BYTE				?	; 002Ch
	State					BYTE				?	; 002Dh
	Alerted					BYTE				2 dup(?)	; 002Eh
	Iopl					BYTE				?	; 0030h
	NpxState				BYTE				?	; 0031h
	Saturation				BYTE				?	; 0032h
	Priority				BYTE				?	; 0033h
	ApcState				KAPC_STATE			<>	; 0034h
	ContextSwitches			DWORD				?	; 004Ch
	IdleSwapBlock			BYTE				?	; 0050h
	Spare0					BYTE				3 dup(?)	; 0051h
	WaitStatus				SDWORD				?	; 0054h
	WaitIrql				BYTE				?	; 0058h
	WaitMode				BYTE				?	; 0059h
	WaitNext				BYTE				?	; 005Ah
	WaitReason				BYTE				?	; 005Bh
	WaitBlockList			PVOID				?	; 005Ch PTR KWAIT_BLOCK
	union
		WaitListEntry		LIST_ENTRY			<>	; 0060h
		struct
			SwapListEntry	SINGLE_LIST_ENTRY	<>	; 0060h
							db 					4 dup(?)	; padding
		ends
	ends
	WaitTime				DWORD				?	; 0068h
	BasePriority			BYTE				?	; 006Ch
	DecrementCount			BYTE				?	; 006Dh
	PriorityDecrement		BYTE				?	; 006Eh
	Quantum					BYTE				?	; 006Fh
	WaitBlock				KWAIT_BLOCK			4 dup(<>)	; 0070h
	LegoData				PVOID				?	; 00D0h
	KernelApcDisable		DWORD				?	; 00D4h
	UserAffinity			DWORD				?	; 00D8h
	SystemAffinityActive	BYTE				?	; 00DCh
	PowerState				BYTE				?	; 00DDh
	NpxIrql					BYTE				?	; 00DEh
	InitialNode				BYTE				?	; 00DFh
	ServiceTable			PVOID				?	; 00E0h
	Queue					PVOID				?	; 00E4h PTR KQUEUE
	ApcQueueLock			DWORD				?	; 00E8h
							db					4 dup(?)	; padding
	Timer					KTIMER 				<>	; 00F0h
	QueueListEntry			LIST_ENTRY			<>	; 0118h
	SoftAffinity			DWORD				?	; 0120h
	Affinity				DWORD				?	; 0124h
	Preempted				BYTE				?	; 0128h
	ProcessReadyQueue		BYTE				?	; 0129h
	KernelStackResident		BYTE				?	; 012Ah
	NextProcessor			BYTE				?	; 012Bh
	CallbackStack			PVOID				?	; 012Ch
	Win32Thread				PVOID				?	; 0130h
	TrapFrame				PVOID				?	; 0134h PTR KTRAP_FRAME
	ApcStatePointer			PVOID				2 dup(?)	; 0138h
	PreviousMode			BYTE				?	; 0140h
	EnableStackSwap			BYTE				?	; 0141h
	LargeStack				BYTE				?	; 0142h
	ResourceIndex			BYTE				?	; 0143h
	KernelTime				DWORD				?	; 0144h
	UserTime				DWORD				?	; 0148h
	SavedApcState			KAPC_STATE			<>	; 014Ch
	Alertable				BYTE				?	; 0164h
	ApcStateIndex			BYTE				?	; 0165h
	ApcQueueable			BYTE				?	; 0166h
	AutoAlignment			BYTE				?	; 0167h
	StackBase				PVOID				?	; 0168h
	SuspendApc				KAPC 				<>	; 016Ch
	SuspendSemaphore		KSEMAPHORE			<>	; 019Ch
	ThreadListEntry			LIST_ENTRY			<>	; 01B0h
	FreezeCount				BYTE				?	; 01B8h
	SuspendCount			BYTE				?	; 01B9h
	IdealProcessor			BYTE				?	; 01BAh
	DisableBoost			BYTE				?	; 01BBh
							DWORD				?	; 01BCh  padding
KTHREAD ENDS
PKTHREAD typedef PTR KTHREAD

ETHREAD STRUCT					; sizeof = 0258h
	Tcb							KTHREAD			<>		; 0000h
	union
		CreateTime				LARGE_INTEGER	<>		; 01C0h
		struct
		ct RECORD \										; 01C0h
			ctUnused:29,								; bit 3-31
			ApcNeeded:1,								; bit 2
			NestedFaultCount:2							; bit 0-1
								db				7 dup(?); padding
		ends
	ends
	union
		ExitTime				LARGE_INTEGER	<>		; 01C8h
		LpcReplyChain			LIST_ENTRY		<>		; 01C8h
		KeyedWaitChain			LIST_ENTRY		<>		; 01C8h
	ends
	union
		ExitStatus				SDWORD			?		; 01D0h
		OfsChain				PVOID			?		; 01D0h
	ends
	PostBlockList				LIST_ENTRY		<>		; 01D4h
	union
		TerminationPort			PVOID			?		; 01DCh PTR TERMINATION_PORT
		ReaperLink				PVOID			?		; 01DCh PTR ETHREAD
		KeyedWaitValue			PVOID			?		; 01DCh
	ends
	ActiveTimerListLock			DWORD			?		; 01E0h
	ActiveTimerListHead			LIST_ENTRY		<>		; 01E4h
	Cid							CLIENT_ID		<>		; 01ECh
	union
		LpcReplySemaphore		KSEMAPHORE		<>		; 01F4h
		KeyedWaitSemaphore		KSEMAPHORE		<>		; 01F4h
	ends
	union
		LpcReplyMessage			PVOID			?		; 0208h
		LpcWaitingOnPort		PVOID			?		; 0208h
	ends
	ImpersonationInfo			PVOID			?		; 020Ch PTR PS_IMPERSONATION_INFORMATION
	IrpList						LIST_ENTRY		<>		; 0210h
	TopLevelIrp					DWORD			?		; 0218h
	DeviceToVerify				PVOID			?		; 021Ch PTR DEVICE_OBJECT
	ThreadsProcess				PVOID			?		; 0220h PTR EPROCESS
	StartAddress				PVOID			?		; 0224h
	union
		Win32StartAddress		PVOID			?		; 0228h
		LpcReceivedMessageId	DWORD			?		; 0228h
	ends
	ThreadListEntry				LIST_ENTRY		<>		; 022Ch
	RundownProtect				EX_RUNDOWN_REF	<>		; 0234h
	ThreadLock					EX_PUSH_LOCK	<>		; 0238h
	LpcReplyMessageId			DWORD			?		; 023Ch
	ReadClusterSize				DWORD			?		; 0240h
	GrantedAccess				DWORD			?		; 0244h
	union
		CrossThreadFlags		DWORD			?		; 0248h
		struct
			ctFlags RECORD \							; 0248h
				SkipTerminationMsg:1,					; bit 8
				SkipCreationMsg:1,						; bit 7
				BreakOnTermination:1,					; bit 6
				HardErrorsAreDisabled:1,				; bit 5
				SystemThread:1,							; bit 4
				ActiveImpersonationInfo:1,				; bit 3
				HideFromDebugger:1,						; bit 2
				DeadThread:1,							; bit 1
				Terminated:1							; bit 0
								db				3 dup(?); padding
		ends
	ends
	union
		SameThreadPassiveFlags	DWORD			?		; 024Ch
		struct
			stpFlags RECORD \							; 024Ch
				stpUnused:29,							; bit 3-31
				MemoryMaker:1,							; bit 2
				ExWorkerCanWaitUser:1,					; bit 1
				ActiveExWorker:1						; bit 0
								db				3 dup(?); padding
		ends
	ends
	union
		SameThreadApcFlags		DWORD			?		; 0250h
		struct
			stapcFlags RECORD \							; 0250h
				stapcUnused:29,							; bit 3-31
				AddressSpaceOwner:1,					; bit 2
				LpcExitThreadCalled:1,					; bit 1
				LpcReceivedMsgIdValid:1					; bit 0
								db				3 dup(?); padding
		ends
	ends
	ForwardClusterOnly			BYTE			?		; 0254h
	DisablePageFaultClustering	BYTE			?		; 0255h
								db				2 dup(?); padding
ETHREAD ENDS
PETHREAD typedef PTR ETHREAD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Process Environment Block (PEB)
; located at 7FFDF000h (pointed by fs:[30] in user mode)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; ...record field names must be unique...
; I named it as PEB_BITS
PEB_BITS RECORD \
			SpareBits:30,			; bits 2-31
			ExecuteOptions:2		; bits 0-1

PEB STRUCT													; sizeof = 0210h
	InheritedAddressSpace				BYTE		?		; 0000h
	ReadImageFileExecOptions			BYTE		?		; 0001h
	BeingDebugged						BYTE		?		; 0002h
	SpareBool							BYTE		?		; 0003h
	Mutant								PVOID		?		; 0004h
	ImageBaseAddress					PVOID		?		; 0008h
	Ldr									PVOID		?		; 000Ch PTR PEB_LDR_DATA
	ProcessParameters					PVOID		?		; 0010h PTR RTL_USER_PROCESS_PARAMETERS
	SubSystemData						PVOID		?		; 0014h
	ProcessHeap							PVOID		?		; 0018h
	FastPebLock							PVOID		?		; 001Ch PTR RTL_CRITICAL_SECTION
	FastPebLockRoutine					PVOID		?		; 0020h
	FastPebUnlockRoutine				PVOID		?		; 0024h
	EnvironmentUpdateCount				DWORD		?		; 0028h
	KernelCallbackTable					PVOID		?		; 002Ch
	SystemReserved						DWORD	1 dup(?)	; 0030h
	PebBits								PEB_BITS	<>		; 0034h named by me
	FreeList							PVOID		?		; 0038h PTR PEB_FREE_BLOCK
	TlsExpansionCounter					DWORD		?		; 003Ch
	TlsBitmap							PVOID		?		; 0040h
	TlsBitmapBits						DWORD	2 dup(?)	; 0044h
	ReadOnlySharedMemoryBase			PVOID		?		; 004Ch
	ReadOnlySharedMemoryHeap			PVOID		?		; 0050h
	ReadOnlyStaticServerData			PVOID		?		; 0054h
	AnsiCodePageData					PVOID		?		; 0058h
	OemCodePageData						PVOID		?		; 005Ch
	UnicodeCaseTableData				PVOID		?		; 0060h
	NumberOfProcessors					DWORD		?		; 0064h
	NtGlobalFlag						DWORD		?		; 0068h
										DWORD		?		; 0064h padding
	CriticalSectionTimeout				LARGE_INTEGER <>	; 0070h
	HeapSegmentReserve					DWORD		?		; 0078h
	HeapSegmentCommit					DWORD		?		; 007Ch
	HeapDeCommitTotalFreeThreshold		DWORD		?		; 0080h
	HeapDeCommitFreeBlockThreshold		DWORD		?		; 0084h
	NumberOfHeaps						DWORD		?		; 0088h
	MaximumNumberOfHeaps				DWORD		?		; 008Ch
	ProcessHeaps						PVOID		?		; 0090h
	GdiSharedHandleTable				PVOID		?		; 0094h
	ProcessStarterHelper				PVOID		?		; 0098h
	GdiDCAttributeList					DWORD		?		; 009Ch
	LoaderLock							PVOID		?		; 00A0h
	OSMajorVersion						DWORD		?		; 00A4h
	OSMinorVersion						DWORD		?		; 00A8h
	OSBuildNumber						WORD		?		; 00ACh
	OSCSDVersion						WORD		?		; 00AEh
	OSPlatformId						DWORD		?		; 00B0h
	ImageSubsystem						DWORD		?		; 00B4h
	ImageSubsystemMajorVersion			DWORD		?		; 00B8h
	ImageSubsystemMinorVersion			DWORD		?		; 00BCh
	ImageProcessAffinityMask			DWORD		?		; 00C0h
	GdiHandleBuffer						DWORD	34 dup(?)	; 00C4h
	PostProcessInitRoutine				PVOID		?		; 014Ch
	TlsExpansionBitmap					PVOID		?		; 0150h
	TlsExpansionBitmapBits				DWORD	32 dup(?)	; 0154h
	SessionId							DWORD		?		; 01D4h
	AppCompatFlags						ULARGE_INTEGER <>	; 01D8h
	AppCompatFlagsUser					ULARGE_INTEGER <>	; 01E0h
	pShimData							PVOID		?		; 01E8h
	AppCompatInfo						PVOID		?		; 01ECh
	CSDVersion							UNICODE_STRING <>	; 01F0h
	ActivationContextData				PVOID		?		; 01F8h
	ProcessAssemblyStorageMap			PVOID		?		; 01FCh
	SystemDefaultActivationContextData	PVOID		?		; 0200h
	SystemAssemblyStorageMap			PVOID		?		; 0204h
	MinimumStackCommit					DWORD		?		; 0208h
										DWORD		?		; 020Ch padding
PEB ENDS

