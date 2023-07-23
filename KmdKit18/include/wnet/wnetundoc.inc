comment ^

Module Name:
    w03undoc.inc

Abstract:
    This module defines some undocumented Windows 2003 Server structures and constants.

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
	; Exactly the same as in XP
	LimitLow		WORD	?				; 00h
	BaseLow			WORD	?				; 02h
	union _HighWord		; original HighWord
		struct Bytes		; __unnamed_17_
			BaseMid	BYTE	?
			Flags1	BYTE	?
			Flags2	BYTE	?
			BaseHi	BYTE	?
		ends
		; Damn! ...record field names must be unique...
		; kd displays it as __unnamed_18_, so i prepend each field name with 'u15'
		Bits RECORD \
			u18BaseHi:8,		; bits24-31 BaseHi
			u18Granularity:1,	; bits23-23 Granularity
			u18Default_Big:1,	; bits22-22 Default_Big
			u18Reserved_0:1,	; bits21-21 Reserved_0
			u18Sys:1,			; bits20-20 Sys
			u18LimitHi:4,		; bits16-19 LimitHi
			u18Pres:1,			; bits15-15 Pres
			u18Dpl:2,			; bits13-14 Dpl
			u18Type:5,			; bits8-12  Type
			u18BaseMid:8		; bits0-7   BaseMid
	ends ; HighWord
KGDTENTRY ENDS
PKGDTENTRY typedef PTR KGDTENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KIDTENTRY STRUCT			; sizeof = 8
	; Exactly the same as in XP
	_Offset			WORD	?	; 00h original name Offset
	Selector		WORD	?	; 02h
	Access			WORD	?	; 04h
	ExtendedOffset	WORD	?	; 06h
KIDTENTRY ENDS
PKIDTENTRY typedef PTR KIDTENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; a part of EPROCESS structure (first member)
; also known as Process Control Block, PCB
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KPROCESS STRUCT			; sizeof = 06Ch
	; Exactly the same as in XP
	Header				DISPATCHER_HEADER	<> ; DO_TYPE_PROCESS (0x1B)
	ProfileListHead		LIST_ENTRY			<>
	DirectoryTableBase	DWORD		2 dup(?)	; 018h Physical address of Page Data Directory (cr3 contains the same phys address)
	LdtDescriptor		KGDTENTRY			<>	; 020h
	Int21Descriptor		KIDTENTRY			<>	; 028h
	IopmOffset			WORD				?	; 030h
	Iopl				BYTE				?	; 032h
	Unused				BYTE				?	; 033h
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
	; Exactly the same as in XP
	union
		epl RECORD \
			eplShared:30,		; bits 2-31
			eplExclusive:1,		; bits 1
			eplWaiting:1		; bits 0
		Value			DWORD	?
		_Ptr			PVOID	?	; original field name "Ptr"
	ends
EX_PUSH_LOCK ENDS
PEX_PUSH_LOCK typedef PTR EX_PUSH_LOCK

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EX_RUNDOWN_REF STRUCT		; sizeof = 4h
	; Exactly the same as in XP
	union
		Count		DWORD	?
		_Ptr		PVOID	?	; original field name "Ptr"
	ends
EX_RUNDOWN_REF ENDS
PEX_RUNDOWN_REF typedef PTR EX_RUNDOWN_REF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EX_FAST_REF STRUCT		; sizeof = 4h
	; Exactly the same as in XP
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
	_reserved:1,		; bits11-11 reserved
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
	; Exactly the same as in XP
	ImageFileName	PVOID	?	; PTR OBJECT_NAME_INFORMATION
SE_AUDIT_PROCESS_CREATION_INFO ENDS
PSE_AUDIT_PROCESS_CREATION_INFO typedef PTR SE_AUDIT_PROCESS_CREATION_INFO

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MMSUPPORT_FLAGS RECORD \
	Available:14,				; bits 18-31
	AcquiredUnsafe:1,			; bits 17
	GrowWsleHash:1,				; bits 16
	MemoryPriority:8,			; bits 8-15
	Available0:1,				; bits 7
	MinimumWorkingSetHard:1,	; bits 6
	ForceTrim:1,				; bits 5
	MaximumWorkingSetHard:1,	; bits 4
	TrimHard:1,					; bits 3
	SessionLeader:1,			; bits 2
	BeingTrimmed:1,				; bits 1
	SessionSpace:1				; bits 0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KGUARDED_MUTEX STRUCT					; sizeof = 020h
	_Count				SDWORD		?	; 0000h
	Owner				PVOID		?	; 0004h PTR KTHREAD
	Contention			DWORD		?	; 0008h
	_Event				KEVENT 		<>	; 000Ch
	union
	 struct
	  KernelApcDisable	SWORD		?	; 001Ch
	  SpecialApcDisable	SWORD		?	; 001Eh
	 ends
	CombinedApcDisable	DWORD		?	; 001Ch
	ends
KGUARDED_MUTEX ENDS
PKGUARDED_MUTEX typedef PTR KGUARDED_MUTEX

MMSUPPORT STRUCT									; sizeof = 060h
	WorkingSetExpansionLinks	LIST_ENTRY <>		; 0000h
	LastTrimTime				LARGE_INTEGER <>	; 0008h
	Flags						MMSUPPORT_FLAGS <>	; 0010h
	PageFaultCount				DWORD		?		; 0014h
	PeakWorkingSetSize			DWORD		?		; 0018h
	GrowthSinceLastEstimate		DWORD		?		; 001Ch
	MinimumWorkingSetSize		DWORD		?		; 0020h
	MaximumWorkingSetSize		DWORD		?		; 0024h
	VmWorkingSetList			PVOID		?		; 0028h PTR MMWSL
	Claim						DWORD		?		; 002Ch
	NextEstimationSlot			DWORD		?		; 0030h
	NextAgingSlot				DWORD		?		; 0034h
	EstimatedAvailable			DWORD		?		; 0038h
	WorkingSetSize				DWORD		?		; 003Ch
	WorkingSetMutex				KGUARDED_MUTEX <>	; 0040h
MMSUPPORT ENDS
PMMSUPPORT typedef PTR MMSUPPORT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MMADDRESS_NODE STRUCT				; sizeof = 014h
	; Damn! ...record field names must be unique...
	; kd displays it as __unnamed_24_, so i prepend each field name with 'u24'
	union u1						; 0000h __unnamed_24_
		u24 RECORD \
			u24Undefined:29,		; bits 2-31
			u24Balance:2			; bits 0-1
		Parent		PVOID		?	; PTR MMADDRESS_NODE
	ends
	LeftChild		PVOID		?	; 0004h PTR MMADDRESS_NODE
	RightChild		PVOID		?	; 0008h PTR MMADDRESS_NODE
	StartingVpn		DWORD		?	; 000Ch
	EndingVpn		DWORD		?	; 0010h
MMADDRESS_NODE ENDS

MM_AVL_TABLE STRUCT									; sizeof = 020h
	BalancedRoot				MMADDRESS_NODE <>	; 0000h
	union
								DWORD			?
	mmat RECORD \									; 0014h
		NumberGenericTableElements:24,	; bits 8-31
		__Unused:3,						; bits 5-7
		DepthOfTree:5					; bits 0-4
	ends
	NodeHint					PVOID			?	; 0018h
	NodeFreeHint				PVOID			?	; 001Ch
MM_AVL_TABLE ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EPROCESS STRUCT						; sizeof = 0278h
	Pcb								KPROCESS		<>			; 0000h
	ProcessLock						EX_PUSH_LOCK	<>			; 006Ch
	CreateTime						LARGE_INTEGER	<>			; 0070h
	ExitTime						LARGE_INTEGER	<>			; 0078h
	RundownProtect					EX_RUNDOWN_REF	<>			; 0080h
	UniqueProcessId					PVOID			?			; 0084h
	ActiveProcessLinks				LIST_ENTRY		<>			; 0088h
	QuotaUsage						DWORD			3 dup(?)	; 0090h
	QuotaPeak						DWORD			3 dup(?)	; 009Ch
	CommitCharge					DWORD			?			; 00A8h
	PeakVirtualSize					DWORD			?			; 00ACh
	VirtualSize						DWORD			?			; 00B0h
	SessionProcessLinks				LIST_ENTRY		<>			; 00B4h
	DebugPort						PVOID			?			; 00BCh
	ExceptionPort					PVOID			?			; 00C0h
	ObjectTable						PVOID			?			; 00C4h PTR HANDLE_TABLE
	Token							EX_FAST_REF		<>			; 00C8h
	WorkingSetPage					DWORD			?			; 00CCh
	AddressCreationLock				KGUARDED_MUTEX	<>			; 00D0h
	HyperSpaceLock					DWORD			?			; 00F0h
	ForkInProgress					PVOID			?			; 00F4h PTR ETHREAD
	HardwareTrigger					DWORD			?			; 00F8h
	PhysicalVadRoot					PVOID			?			; 00FCh PTR MM_AVL_TABLE
	CloneRoot						PVOID			?			; 0100h
	NumberOfPrivatePages			DWORD			?			; 0104h
	NumberOfLockedPages				DWORD			?			; 0108h
	Win32Process					PVOID			?			; 010Ch
	Job								PVOID			?			; 0110h PTR EJOB
	SectionObject					PVOID			?			; 0114h
	SectionBaseAddress				PVOID			?			; 0118h
	QuotaBlock						PVOID			?			; 011Ch PTR EPROCESS_QUOTA_BLOCK
	WorkingSetWatch					PVOID			?			; 0120h PTR PAGEFAULT_HISTORY
	Win32WindowStation				PVOID			?			; 0124h
	InheritedFromUniqueProcessId	PVOID			?			; 0128h
	LdtInformation					PVOID			?			; 012Ch
	VadFreeHint						PVOID			?			; 0130h
	VdmObjects						PVOID			?			; 0134h
	DeviceMap						PVOID			?			; 0138h
	Spare0							PVOID			3 dup(?)	; 013Ch
	union
		PageDirectoryPte			HARDWARE_PTE	<>			; 0148h
		Filler						QWORD			?			; 0148h
	ends
	Session							PVOID			?			; 0150h
	ImageFileName					BYTE			16 dup(?)	; 0154h
	JobLinks						LIST_ENTRY		<>			; 0164h
	LockedPagesList					PVOID			?			; 016Ch
	ThreadListHead					LIST_ENTRY		<>			; 0170h
	SecurityPort					PVOID			?			; 0178h
	PaeTop							PVOID			?			; 017Ch
	ActiveThreads					DWORD			?			; 0180h
	GrantedAccess					DWORD			?			; 0184h
	DefaultHardErrorProcessing		DWORD			?			; 0188h
	LastThreadExitStatus			SDWORD			?			; 018Ch
	Peb								PVOID			?			; 0190h PTR PEB
	PrefetchTrace					EX_FAST_REF		<>			; 0194h
	ReadOperationCount				LARGE_INTEGER	<>			; 0198h
	WriteOperationCount				LARGE_INTEGER	<>			; 01A0h
	OtherOperationCount				LARGE_INTEGER	<>			; 01A8h
	ReadTransferCount				LARGE_INTEGER	<>			; 01B0h
	WriteTransferCount				LARGE_INTEGER	<>			; 01B8h
	OtherTransferCount				LARGE_INTEGER	<>			; 01C0h
	CommitChargeLimit				DWORD			?			; 01C8h
	CommitChargePeak				DWORD			?			; 01CCh
	AweInfo							PVOID			?			; 01D0h
	SeAuditProcessCreationInfo		SE_AUDIT_PROCESS_CREATION_INFO <>	; 01D4h
	Vm								MMSUPPORT		<>			; 01D8h
	MmProcessLinks					LIST_ENTRY		<>			; 0238h
	ModifiedPageCount				DWORD			?			; 0240h
	JobStatus						DWORD			?			; 0244h
	union
		Flags						DWORD			?			; 0248h
comment ^
		recFlags RECORD \
			Unused:7,						; bits 25-31
			VdmAllowed:1,					; bits 24
			PdeUpdateNeeded:1,				; bits 23
			ImageNotifyDone:1,				; bits 22
			VmTopDown:1,					; bits 21
			InjectInpageErrors:1,			; bits 20
			LaunchPrefetched:1,				; bits 19
			HasAddressSpace:1,				; bits 18
			OverrideAddressSpace:1,			; bits 17
			ProcessInSession:1,				; bits 16
			WriteWatch:1,					; bits 15
			SessionCreationUnderway:1,		; bits 14
			BreakOnTermination:1,			; bits 13
			SetTimerResolution:1,			; bits 12
			AddressSpaceInitialized:2,		; bits 10-11
			Wow64VaSpace4Gb:1,				; bits 9
			ForkFailed:1,					; bits 8
			Outswapped:1,					; bits 7
			OutswapEnabled:1,				; bits 6
			VmDeleted:1,					; bits 5
			Wow64SplitPages:1,				; bits 4
			ProcessDelete:1,				; bits 3
			ProcessExiting:1,				; bits 2
			NoDebugInherit:1,				; bits 1
			CreateReported:1				; bits 0
^
	ends
	ExitStatus						SDWORD			?			; 024Ch
	NextPageColor					WORD			?			; 0250h
	union
		struct
			SubSystemMinorVersion	BYTE			?			; 0252h
			SubSystemMajorVersion	BYTE			?			; 0253h
		ends
		SubSystemVersion			WORD			?			; 0252h
	ends
	PriorityClass					BYTE			?			; 0254h
									db 				3 dup(?)	; 0255 padding
	VadRoot							MM_AVL_TABLE	<>			; 0258h
EPROCESS ENDS
PEPROCESS typedef PTR EPROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KAPC_STATE STRUCT								; sizeof = 018h
	; Exactly the same as in XP
	ApcListHead			LIST_ENTRY	2 dup(<>)	; 0000h
	Process				PVOID		?			; 0010h PTR KPROCESS
	KernelApcInProgress	BYTE		?			; 0014h
	KernelApcPending	BYTE		?			; 0015h
	UserApcPending		BYTE		?			; 0016h
						BYTE		?			; 0017h  padding
KAPC_STATE ENDS

KTHREAD STRUCT												; sizeof = 01C8h
	Header					DISPATCHER_HEADER	<>			; 0000h
	MutantListHead			LIST_ENTRY			<>			; 0010h
	InitialStack			PVOID				?			; 0018h
	StackLimit				PVOID				?			; 001Ch
	KernelStack				PVOID				?			; 0020h
	ThreadLock				DWORD				?			; 0024h
	ContextSwitches			DWORD				?			; 0028h
	State					BYTE				?			; 002Ch
	NpxState				BYTE				?			; 002Dh
	WaitIrql				BYTE				?			; 002Eh
	WaitMode				BYTE				?			; 002Fh
	Teb						PVOID				?			; 0030h
	ApcState				KAPC_STATE			<>			; 0034h
	ApcQueueLock			DWORD				?			; 004Ch
	WaitStatus				SDWORD				?			; 0050h
	WaitBlockList			PVOID				?			; 0054h PTR KWAIT_BLOCK
	Alertable				BYTE				?			; 0058h
	WaitNext				BYTE				?			; 0059h
	WaitReason				BYTE				?			; 005Ah
	Priority				BYTE				?			; 005Bh
	EnableStackSwap			BYTE				?			; 005Ch
	SwapBusy				BYTE				?			; 005Dh
	Alerted					BYTE				2 dup(?)	; 005Eh
	union
		WaitListEntry		LIST_ENTRY 			<>			; 0060h
		struct
			SwapListEntry	SINGLE_LIST_ENTRY	<>			; 0060h
							db 					4 dup(?)	; padding
		ends
	ends
	Queue					PVOID				?			; 0068h PTR KQUEUE
	WaitTime				DWORD				?			; 006Ch
	union
		struct
			KernelApcDisable		SWORD		?			; 0070h
			SpecialApcDisable		SWORD		?			; 0072h
		ends
		struct
			CombinedApcDisable		DWORD		?			; 0070h
									db 			4 dup(?)	; padding
		ends
	ends
	Timer					KTIMER				<>			; 0078h
	WaitBlock				KWAIT_BLOCK			4 dup(<>)	; 00A0h
	QueueListEntry			LIST_ENTRY			<>			; 0100h
	ApcStateIndex			BYTE				?			; 0108h
	ApcQueueable			BYTE				?			; 0109h
	Preempted				BYTE				?			; 010Ah
	ProcessReadyQueue		BYTE				?			; 010Bh
	KernelStackResident		BYTE				?			; 010Ch
	Saturation				BYTE				?			; 010Dh
	IdealProcessor			BYTE				?			; 010Eh
	NextProcessor			BYTE				?			; 010Fh
	BasePriority			BYTE				?			; 0110h
	Spare4					BYTE				?			; 0111h
	PriorityDecrement		BYTE				?			; 0112h
	Quantum					BYTE				?			; 0113h
	SystemAffinityActive	BYTE				?			; 0114h
	PreviousMode			BYTE				?			; 0115h
	ResourceIndex			BYTE				?			; 0116h
	DisableBoost			BYTE				?			; 0117h
	UserAffinity			DWORD				?			; 0118h
	Process					PVOID				?			; 011Ch PTR KPROCESS
	Affinity				DWORD				?			; 0120h
	ServiceTable			PVOID				?			; 0124h
	ApcStatePointer			PVOID				2 dup(?)	; 0128h
	SavedApcState			KAPC_STATE			<>			; 0130h
	CallbackStack			PVOID				?			; 0148h
	Win32Thread				PVOID				?			; 014Ch
	TrapFrame				PVOID				?			; 0150h PTR KTRAP_FRAME
	KernelTime				DWORD				?			; 0154h
	UserTime				DWORD				?			; 0158h
	StackBase				PVOID				?			; 015Ch
	SuspendApc				KAPC				<>			; 0160h
	SuspendSemaphore		KSEMAPHORE			<>			; 0190h
	TlsArray				PVOID				?			; 01A4h
	LegoData				PVOID				?			; 01A8h
	ThreadListEntry			LIST_ENTRY			<>			; 01ACh
	LargeStack				BYTE				?			; 01B4h
	PowerState				BYTE				?			; 01B5h
	NpxIrql					BYTE				?			; 01B6h
	Spare5					BYTE				?			; 01B7h
	AutoAlignment			BYTE				?			; 01B8h
	Iopl					BYTE				?			; 01B9h
	FreezeCount				BYTE				?			; 01BAh
	SuspendCount			BYTE				?			; 01BBh
	Spare0					BYTE				1 dup(?)	; 01BCh
	UserIdealProcessor		BYTE				?			; 01BDh
	DeferredProcessor		BYTE				?			; 01BEh
	AdjustReason			BYTE				?			; 01BFh
	AdjustIncrement			BYTE				?			; 01C0h
	Spare2					BYTE				3 dup(?)	; 01C1h
							DWORD				?			; 01C4h  padding
KTHREAD ENDS
PKTHREAD typedef PTR KTHREAD

ETHREAD STRUCT											; sizeof = 0260h
	Tcb							KTHREAD			<>		; 0000h
	union
		CreateTime				LARGE_INTEGER	<>		; 01C8h
		struct
		ct RECORD \										; 01C8h
			ctUnused:29,								; bit 3-31
			ApcNeeded:1,								; bit 2
			NestedFaultCount:2							; bit 0-1
								db				7 dup(?); padding
		ends
	ends
	union
		ExitTime				LARGE_INTEGER	<>		; 01D0h
		LpcReplyChain			LIST_ENTRY		<>		; 01D0h
		KeyedWaitChain			LIST_ENTRY		<>		; 01D0h
	ends
	union
		ExitStatus				SDWORD			?		; 01D8h
		OfsChain				PVOID			?		; 01D8h
	ends
	PostBlockList				LIST_ENTRY		<>		; 01DCh
	union
		TerminationPort			PVOID			?		; 01E4h PTR TERMINATION_PORT
		ReaperLink				PVOID			?		; 01E4h PTR ETHREAD
		KeyedWaitValue			PVOID			?		; 01E4h
	ends
	ActiveTimerListLock			DWORD			?		; 01E8h
	ActiveTimerListHead			LIST_ENTRY		<>		; 01ECh
	Cid							CLIENT_ID		<>		; 01F4h
	union
		LpcReplySemaphore		KSEMAPHORE		<>		; 01FCh
		KeyedWaitSemaphore		KSEMAPHORE		<>		; 01FCh
	ends
	union
		LpcReplyMessage			PVOID			?		; 0210h
		LpcWaitingOnPort		PVOID			?		; 0210h
	ends
	ImpersonationInfo			PVOID			?		; 0214h PTR PS_IMPERSONATION_INFORMATION
	IrpList						LIST_ENTRY		<>		; 0218h
	TopLevelIrp					DWORD			?		; 0220h
	DeviceToVerify				PVOID			?		; 0224h PTR DEVICE_OBJECT
	ThreadsProcess				PVOID			?		; 0228h PTR EPROCESS
	StartAddress				PVOID			?		; 022Ch
	union
		Win32StartAddress		PVOID			?		; 0230h
		LpcReceivedMessageId	DWORD			?		; 0230h
	ends
	ThreadListEntry				LIST_ENTRY		<>		; 0234h
	RundownProtect				EX_RUNDOWN_REF	<>		; 023Ch
	ThreadLock					EX_PUSH_LOCK	<>		; 0240h
	LpcReplyMessageId			DWORD			?		; 0244h
	ReadClusterSize				DWORD			?		; 0248h
	GrantedAccess				DWORD			?		; 024Ch
	union
		CrossThreadFlags		DWORD			?		; 0250h
		struct
			ctFlags RECORD \							; 0250h
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
		SameThreadPassiveFlags	DWORD			?		; 0254h
		struct
			stpFlags RECORD \							; 0254h
				stpUnused:28,							; bit 4-31
				KeyedEventInUse:1,						; bit 3
				MemoryMaker:1,							; bit 2
				ExWorkerCanWaitUser:1,					; bit 1
				ActiveExWorker:1						; bit 0
								db				3 dup(?); padding
		ends
	ends
	union
		SameThreadApcFlags		DWORD			?		; 0258h
		struct
			stapcFlags RECORD \							; 0258h
				stapcUnused:29,							; bit 3-31
				AddressSpaceOwner:1,					; bit 2
				LpcExitThreadCalled:1,					; bit 1
				LpcReceivedMsgIdValid:1					; bit 0
								db				3 dup(?); padding
		ends
	ends
	ForwardClusterOnly			BYTE			?		; 025Ch
	DisablePageFaultClustering	BYTE			?		; 025Dh
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

PEB STRUCT									; sizeof = 0230h
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
	SparePtr1							PVOID		?		; 0020h
	SparePtr2							PVOID		?		; 0024h
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
										DWORD		?		; 006Ch padding
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
	LoaderLock							PVOID		?		; 00A0h PTR RTL_CRITICAL_SECTION
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
	ActivationContextData				PVOID		?		; 01F8h PTR ACTIVATION_CONTEXT_DATA
	ProcessAssemblyStorageMap			PVOID		?		; 01FCh PTR ASSEMBLY_STORAGE_MAP
	SystemDefaultActivationContextData	PVOID		?		; 0200h PTR ACTIVATION_CONTEXT_DATA
	SystemAssemblyStorageMap			PVOID		?		; 0204h PTR ASSEMBLY_STORAGE_MAP
	MinimumStackCommit					DWORD		?		; 0208h
	FlsCallback							PVOID		?		; 020Ch
	FlsListHead							LIST_ENTRY	<>		; 0210h
	FlsBitmap							PVOID		?		; 0218h
	FlsBitmapBits						DWORD	4 dup(?)	; 021Ch
	FlsHighIndex						DWORD		?		; 022Ch
PEB ENDS

