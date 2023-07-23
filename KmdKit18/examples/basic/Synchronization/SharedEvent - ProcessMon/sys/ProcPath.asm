
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    S T R U C T U R E S                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; These structures are the same on 2000, XP and 2003

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

_SEGMENT STRUCT								; sizeof = 40h
	ControlArea				PVOID		?	; 000 PTR CONTROL_AREA
	SegmentBaseAddress		PVOID		?	; 004
	TotalNumberOfPtes		DWORD		?	; 008
	NonExtendedPtes			DWORD		?	; 00C
	SizeOfSegment			QWORD		?	; 010 ULONG64
	ImageCommitment			DWORD		?	; 018
	ImageInformation		PVOID		?	; 01C PTR SECTION_IMAGE_INFORMATION
	SystemImageBase			PVOID		?	; 020
	NumberOfCommittedPages	DWORD		?	; 024
	SegmentPteTemplate		DWORD		?	; 028 MMPTE
	BasedAddress			PVOID		?	; 02C
	ExtendInfo				PVOID		?	; 030 PTR MMEXTEND_INFO
	PrototypePte			PVOID		?	; 034 PTR MMPTE
	ThePtes					DWORD 1 dup(?)	; 038 array of MMPTE
_SEGMENT ENDS

CONTROL_AREA STRUCT								; sizeof = 38h
	_Segment					PVOID		?	; 000 PTR _SEGMENT
	DereferenceList				LIST_ENTRY	<>	; 004
	NumberOfSectionReferences	DWORD		?	; 00C
	NumberOfPfnReferences		DWORD		?	; 010
	NumberOfMappedViews			DWORD		?	; 014
	NumberOfSubsections			WORD		?	; 018
	FlushInProgressCount		WORD		?	; 01A
	NumberOfUserReferences		DWORD		?	; 01C
	union u
		LongFlags				DWORD		?	; 020
		Flags					DWORD		?	; 020 MMSECTION_FLAGS
	ends
	FilePointer					PVOID		?	; 024 PTR FILE_OBJECT
	WaitingForDeletion			PVOID		?	; 028 PTR EVENT_COUNTER
	ModifiedWriteCount			WORD		?	; 02C
	NumberOfSystemCacheViews	WORD		?	; 02E
	PagedPoolUsage				DWORD		?	; 030
	NonPagedPoolUsage			DWORD		?	; 034
CONTROL_AREA ENDS

MMADDRESS_NODE STRUCT							; sizeof = 14h
	StartingVpn					DWORD		?	; 00 ULONG_PTR
	EndingVpn					DWORD		?	; 04 ULONG_PTR
	Parent						PVOID		?	; 08 PTR MMADDRESS_NODE
	LeftChild					PVOID		?	; 0C PTR MMADDRESS_NODE
	RightChild					PVOID		?	; 10 PTR MMADDRESS_NODE
MMADDRESS_NODE ENDS
PMMADDRESS_NODE typedef ptr MMADDRESS_NODE

COMMENT ^
It's allmost the same as SECTION but we need SECTION.
SECTION_OBJECT STRUCT			; sizeof = 18h
	StartingVa		PVOID	?	; 00
	EndingVa		PVOID	?	; 04
	Parent			PVOID	?	; 08
	LeftChild		PVOID	?	; 0C
	RightChild		PVOID	?	; 10
	_Segment		PVOID	?	; 14 PTR _SEGMENT ( not SEGMENT_OBJECT as defined in PDB!)
SECTION_OBJECT ENDS
^

SECTION STRUCT										; sizeof = 28h
	Address						MMADDRESS_NODE	<>	; 00
	_Segment					PVOID			?	; 14 PTR _SEGMENT
	SizeOfSection				LARGE_INTEGER	<>	; 18
	union u
		LongFlags				DWORD			?	; 20
		Flags					DWORD			?	; 20 MMSECTION_FLAGS
	ends
	InitialPageProtection		DWORD			?	; 24
SECTION ENDS
PSECTION typedef ptr SECTION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      E Q U A T E S                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

WINVER_UNINITIALIZED	equ -1
WINVER_2K				equ 0
WINVER_XP_OR_HIGHER		equ 1

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                               I N I T I A L I Z E D  D A T A                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data

g_dwWinVer	DWORD	WINVER_UNINITIALIZED

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  IsAddressInPoolRanges                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IsAddressInPoolRanges proc uses ebx pAddress:PVOID

; The purpose of this routine is check to see
; if specified address is in the range of system pools.
; (80000000 < pAddress < A0000000) || (E1000000 < pAddress < FFBE0000)
		
comment ^
+- MmSystemRangeStart = 80000000 -----------+
|                                           | sizeof = 20000000 MB
|    System code (ntoskrnl and hal)         |
|      and initial nonpaged pool            |
|                                           |
+- MiSystemViewStart  = A0000000 -----------+
|                                           |
. . .                                   . . .
|                                           |
+- MmPagedPoolStart   = E1000000 -----------+
|                                           | sizeof = 1EBE0000 MB
|    Paged pool                             |
|                                           |
|    System page table entries (PTEs)       |
|                                           |
|    Expanded nonpaged pool                 |
|                                           |
+- MmPagedPoolEnd (calculated at boot time) +

Note:
Above system space layout is for system
with 2Gb system space (non-PAE and no /3GB boot.ini option)

MiSystemViewStart  = A0000000 without Terminal Services
MiSystemViewStart  = A3000000 with Terminal Services

MmPagedPoolEnd is less then MmNonPagedPoolEnd  = FFBE0000
and is less then Crash Dump structures also starting at FFBE0000
^

local fOk:BOOL

	and fOk, FALSE

	mov eax, MmSystemRangeStart
	mov eax, [eax]
	mov eax, [eax]

	.if eax == 80000000h
	
		; OK. 2Gb system space
		
		mov ebx, pAddress

		xor ecx, ecx		; LowerRange flag
		xor edx, edx		; UpperRange flag

		.if ( ebx > 80000000h ) && ( ebx < 0A0000000h )
			inc ecx			; In LowerRange
		.endif

		.if ( ebx > 0E1000000h ) && ( ebx < 0FFBE0000h )
			inc edx			; In UpperRange
		.endif

		or ecx, edx
		.if !ZERO?
			mov fOk, TRUE	; OK
		.endif

	.endif

	mov eax, fOk
	ret

IsAddressInPoolRanges endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     IsLikeObjectPointer                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IsLikeObjectPointer proc uses esi pObject:PVOID

;
; Determines whether pointer seems to be a valid object pointer.
; If pointer is in system pools range & at least 8 bytes aligned
; & points to valid memory & (pObject - sizeof OBJECT_HEADER) is also valid
; we make reasonable decision about pObjectis probably points to some object.
;
; You MUST NOT make any assumption that it is really some object pointer!!!
;
; You are only guaranteed (if call to this function is successful)
; that your subsequent call to ObReferenceObjectByPointer at IRQL <= DISPATCH_LEVEL
; doesn't bugcheck.  And if and only if ObReferenceObjectByPointer,,,UserMode
; returns STATUS_SUCCESS you are 100% shure that pObject is particular object pointer.
; See comments in GetImageFilePath about UserMode parameter.
;

local fOk:BOOL

	and fOk, FALSE

	mov esi, pObject

	invoke IsAddressInPoolRanges, esi
	.if eax == TRUE

		;
		; Check alignment.
		;
		; Object body immediately follows the object header in memory.  Optionaly
		; four structures immediately precede object header. They are:
		; OBJECT_HEADER_QUOTA_INFO, OBJECT_HEADER_HANDLE_INFO, OBJECT_HEADER_NAME_INFO
		; and OBJECT_HEADER_CREATOR_INFO. The size of each structure and OBJECT_HEADER
		; is divisible by 8.
		;
		; The object body with all accompanion structures is part of a single memory
		; allocation, and memory allocations of less than PAGE_SIZE are aligned
		; on an 8-byte boundary. So objects body is always at least 8 aligned.
		;

		mov eax, esi
		and eax, (8 - 1)
		.if eax == 0

			; Object body should resides in valid memory
				
			invoke MmIsAddressValid, esi
			.if al

				; Object header also must be valid

				mov eax, esi
				and eax, (PAGE_SIZE-1)
				.if eax < sizeof OBJECT_HEADER
				
					; Object header crosses a page boundary 
					; We have to call MmIsAddressValid again
					; against object header

					sub esi, sizeof OBJECT_HEADER
	
					invoke MmIsAddressValid, esi
					.if al

						mov fOk, TRUE

					.endif

				.else
				
					; Object header is in the same page as its body
					; So no need to call MmIsAddressValid once more
					
					mov fOk, TRUE

				.endif
			.endif
		.endif
	.endif

	mov eax, fOk
	ret

IsLikeObjectPointer endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     GetImageFilePath                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetImageFilePath proc uses ebx esi edi peProcess:PVOID, pusImageFilePath:PUNICODE_STRING

;
; This routine returns a fool path to image file of the process.
; Caller of this routine must be running at IRQL = PASSIVE_LEVEL.
; If siccessful the caller of this routine must call ExFreePool
; on pusImageFilePath->Buffer when it is no longer needed.
; peProcess must be valid pointer to EPROCESS
;

local status:NTSTATUS
local pSection:PVOID			; PTR SECTION
local usDosName:UNICODE_STRING

	mov status, STATUS_UNSUCCESSFUL

	; Check object type and reference it for shure

	;
	; In ObReferenceObjectByPointer description DDK stands that ObjectType can be
	; either [IoFileObjectType] or [ExEventObjectType].  It's not true! 
	; It can be valid pointer to any ObjectType: [PsProcessType], [PsJobType],
	; [MmSectionObjectType], [ExWindowStationObjectType] etc...
	;
	; BUT !
	;
	; If KernelMode specified in AccessMode parameter, ObReferenceObjectByPointer doesn't
	; check object type at all !!!
	;
	; So, calling it this way, for example, will be successful !!!
	;  (with only one exception for not exported ObpSymbolicLinkObjectType)
	;
	; mov ecx, ExWindowStationObjectType
	; mov ecx, [ecx]
	; mov ecx, [ecx]
	; invoke ObReferenceObjectByPointer, peProcess, 12345678h, ecx, KernelMode
	;
	; DesiredAccess is also doesn't matter, by the way !!! It can be any value :)
	;
	; To make it really check the object type we must specify UserMode.
	;
	; The above note is applicable to all ObReferenceObjectByPointer calls in this source code.
	;
	; Also bear in mind that existence of AccessMode parameter for ObReferenceObjectByPointer
	; is odd (in my humble opinion). It's OK for ObReferenceObjectByHandle because we may deal
	; with handle passed from user mode, but if we already have a pointer to object it means we
	; have managed to get it somehow from kernel. So no need to check access type.
	; I've tested this driver on 2000, XP & 2003 and it seems to be workable.
	; But I'm afraid that behaviour of ObReferenceObjectByPointer may change in the future.
	; 

	; Although DesiredAccess doesn't matter we will always pass valid access mask.

	PROCESS_QUERY_INFORMATION equ 400h	; winnt.inc

	mov ecx, PsProcessType
	mov ecx, [ecx]
	mov ecx, [ecx]						; PTR OBJECT_TYPE

	invoke ObReferenceObjectByPointer, peProcess, PROCESS_QUERY_INFORMATION, ecx, UserMode
	.if eax == STATUS_SUCCESS

		.if g_dwWinVer == WINVER_UNINITIALIZED
		
			; We are first time here
			; What Windows we are running on?

			invoke IoIsWdmVersionAvailable, 1, 20h
			.if al
				; If WDM 1.20 is supported, this is Windows XP or better
				mov g_dwWinVer, WINVER_XP_OR_HIGHER
			.else
				; If not, this is Windows 2000
				mov g_dwWinVer, WINVER_2K
			.endif

		.endif

		.if g_dwWinVer == WINVER_XP_OR_HIGHER

			;
			; This is Windows XP or better
			; So we should find EPROCESS.SectionObject
			; XP:   EPROCESS.SectionObject at 0138h
			; 2003: EPROCESS.SectionObject at 0114h
			; We could hardcode offset but better try to find it
			;
			; I hope to find section object pointer
			; in the range 80h - 200h from beginning of EPROCESS
			;

			mov esi, peProcess
			mov ebx, 80h			; Start at offset 80h
			.while ebx < 204h

				; Filter unreasonable candidates

				mov edi, [esi][ebx]
				invoke IsLikeObjectPointer, edi
				.if eax == TRUE

					; Additional check. At the moment of process creation/destruction
					; base section object PointerCount equal to 3/2 and
					; HandleCount equal to 1/0. This is true under XP+.
					; Assume that PointerCount may grow up to 4.
					; This check let us filter the rest.

					mov eax, edi
					sub eax, sizeof OBJECT_HEADER

					.if ([OBJECT_HEADER PTR [eax]].PointerCount <= 4) && ([OBJECT_HEADER PTR [eax]].HandleCount <= 1)

						; Very high chances that edi holds base section object pointer.

						mov ecx, MmSectionObjectType
						mov ecx, [ecx]
						mov ecx, [ecx]	; PTR OBJECT_TYPE

						invoke ObReferenceObjectByPointer, edi, SECTION_QUERY, ecx, UserMode
						.if eax == STATUS_SUCCESS

							; edi seems really to be a pointer to base section object

							mov status, eax
							mov pSection, edi

							;invoke DbgPrint, \
							;	$CTA0("ProcessMon: Section object pointer found at offset %X\n"), \
							;	ebx

							.break
						.endif
					.endif
				.endif

				add ebx, 4			; Pointer must be DWORD aligned
									; So lets try next DWORD
			.endw

		.else

			;
			; We are under Windows 2000. On this system the section handle that
			; process image file mapped into is stored in EPROCESS.SectionHandle
			; and is always (with one exception) equal to 4 because it's very
			; first object created in the process.  Handle tables are implemented
			; as a three-level arrays, similar to the way that the x86 memory
			; management unit implements virtual to physical address translation.
			; The object manager treats the low 24 bits of an object handle's value
			; as three 8-bit fields that index into each of the three levels
			; in the handle table.  The arrays at each level consist of 256 entries.
			; Each entry is 4 bytes long because it contains pointer to the object.
			; The last entry in the subhandle table is initialized with a value of -1.
			;
			; So when a process is created, the object manager starts to fill subhandle
			; tables from the beginning of the subhandle table. (The 0 handle index
			; is reserved, first handle index is 4, the second 8, and so on).
			; So we can just reference handle 4 to get section object pointer.
			;
			; On w2k+sp4 if the process is started from command line (cmd.exe)
			; or bat file the section handle is not value of 4!  Don't know why
			; but in this particular case the object manager fills subhandle tables
			; in reverse order (from top to bottom).  So the first index it uses
			; is not 4 but 254 (255 initialized with a value of -1.)
			; 254*sizeof(pointer) = 03F8h
			;
			; I can't be shure it's so on any sp4 box, but it appears to be so
			; at least on 5-6 test machines.  So my first workaround
			; is to try reference handle 3F8h.
			;

			; If it still fails our last try just to reference whatever value
			; in EPROCESS.SectionHandle.

			xor ebx, ebx		; counter of tries
			mov edi, 4			; First try to reference handle 4 (most common).
			.while ebx < 3

				invoke IoGetCurrentProcess
				.if eax == peProcess
					
					; The same process context
					
					mov ecx, MmSectionObjectType
					mov ecx, [ecx]
					mov ecx, [ecx]	; PTR OBJECT_TYPE
				
					invoke ObReferenceObjectByHandle, edi, SECTION_QUERY, ecx, KernelMode, addr pSection, NULL
					mov status, eax

				.else

					; Different process. Since handles are process specific switch to target.

					invoke KeAttachProcess, peProcess

					mov ecx, MmSectionObjectType
					mov ecx, [ecx]
					mov ecx, [ecx]	; PTR OBJECT_TYPE

					invoke ObReferenceObjectByHandle, edi, SECTION_QUERY, ecx, KernelMode, addr pSection, NULL
					mov status, eax

					invoke KeDetachProcess

				.endif

				; If section referenced successefuly break.

				.break .if status == STATUS_SUCCESS

				; It seams we are under SP4 and process started from command line.
				; Handle invalid or object we probably tried to reference is not a section object
				; (it can be while process destruction because process still has many handles)
				; or access denied. Whatever value it can be try to workaround anyway.
					
				.if ebx == 0

					mov edi, 03F8h	; Try 03F8h handle.
						
				.elseif ebx == 1
					
					; Last chance.
			
					mov eax, peProcess
					add eax, 01ACh			; + SectionHandle field offset
					mov eax, [eax]			; [EPROCESS.SectionHandle]
					mov edi, eax

					; The handle value is multiple of 4.  And the section handle
					; must have some reasonable value. If not, better go away.

					and eax, (4 - 1)
					.break .if ( eax != 0 ) || ( edi >= 800h )
						
				.endif
				
				inc ebx						; Next workaround.
			.endw

			;invoke DbgPrint, $CTA0("ProcessMon: Reference section. status: %08X\n"), status

		.endif

		; If status != STATUS_SUCCESS we failed to get section object pointer
		; Very bad. No section no image file name :(

		.if status == STATUS_SUCCESS

			; OK. We have section pointer in pSection and it is referenced

			mov status, STATUS_UNSUCCESSFUL

			mov ebx, pSection
			mov ebx, (SECTION PTR [ebx])._Segment				; -> _SEGMENT

			invoke IsAddressInPoolRanges, ebx
			push eax
			invoke MmIsAddressValid, ebx
			pop ecx
			.if al && ( ecx == TRUE )

				mov esi, ebx									; save PTR _SEGMENT

				mov ebx, (_SEGMENT PTR [ebx]).ControlArea		; -> CONTROL_AREA

				invoke IsAddressInPoolRanges, ebx
				push eax
				invoke MmIsAddressValid, ebx
				pop ecx
				.if al && ( ecx == TRUE ) && ([CONTROL_AREA PTR [ebx]]._Segment == esi )	; check for shure

					mov ebx, (CONTROL_AREA PTR [ebx]).FilePointer	; -> FILE_OBJECT

					invoke IsLikeObjectPointer, ebx
					.if eax == TRUE

						; Check object type and reference it for sure

						mov ecx, IoFileObjectType
						mov ecx, [ecx]
						mov ecx, [ecx]			; PTR OBJECT_TYPE

						invoke ObReferenceObjectByPointer, ebx, FILE_READ_ATTRIBUTES, ecx, UserMode
						.if eax == STATUS_SUCCESS

							; Allocate memory for full image file path

							invoke ExAllocatePool, PagedPool, (IMAGE_FILE_PATH_LEN+1) * sizeof WCHAR
							.if eax != NULL

								mov edi, pusImageFilePath
								assume edi:ptr UNICODE_STRING

								mov [edi].Buffer, eax

								invoke memset, eax, 0, (IMAGE_FILE_PATH_LEN+1) * sizeof WCHAR	; Zero out

								; MaximumLength is one char less than allocated/zeroed
								; because I want to have zero char for shure

								mov [edi].MaximumLength,	IMAGE_FILE_PATH_LEN * sizeof WCHAR
								and [edi]._Length,			0

								; Get dos name for volume. DDK stands that drivers written for
								; Windows XP and later must use IoVolumeDeviceToDosName instead of
								; RtlVolumeDeviceToDosName. But on XP and later both functions
								; have the same entry point. So it's OK to call RtlVolumeDeviceToDosName
								; under any.

								invoke RtlVolumeDeviceToDosName, \
										(FILE_OBJECT PTR [ebx]).DeviceObject, addr usDosName
								.if eax == STATUS_SUCCESS

									; Copy drive letter

									invoke RtlCopyUnicodeString, edi, addr usDosName

									; Free memory allocated by DeviceToDosName

									invoke ExFreePool, usDosName.Buffer

								.else
									; If we fail to get drive letter we could query device name instead.
									; So instead of
									; "\WINNT\system32\notepad.exe"
									; we would get
									; "\Device\HarddiskVolume1\system32\notepad.exe"
									; But I do nothing to simplify the things
								.endif

								; Append relative file path

								; We could use ObQueryNameString to obtain file name.
								; I just get it directly from file object. It's much more faster.

								invoke RtlAppendUnicodeStringToString, edi, \
												addr (FILE_OBJECT PTR [ebx]).FileName

								;invoke DbgPrint, $CTA0("ProcessMon: %ws\n"), [edi].Buffer

								assume edi:nothing

								mov status, STATUS_SUCCESS

							.endif

							invoke ObDereferenceObject, ebx		; FILE_OBJECT
						.endif
					.endif
				.endif
			.endif

			invoke ObDereferenceObject, pSection
		.endif

		invoke ObDereferenceObject, peProcess
	.endif

	mov eax, status
	ret

GetImageFilePath endp
