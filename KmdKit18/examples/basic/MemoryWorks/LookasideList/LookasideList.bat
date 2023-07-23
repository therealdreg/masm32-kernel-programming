;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  LookasideList - Merely allocates and releases some fixed-size blocks of memory.
;
;  If you know beforehand that you will need some memory blocks of fixed size
;    but you not quite sure how much of such blocks you will need the lookaside list is for you
;
;  Please remember that InitializeListHead, InsertHeadList, InsertTailList, RemoveHeadList, 
;    RemoveTailList and RemoveEntryList are not fonctions. They are macros (see ntddk.inc).
;
;  Written by Four-F (four-f@mail.ru)
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.486
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\w2k\ntstatus.inc
include \masm32\include\w2k\ntddk.inc
include \masm32\include\w2k\ntoskrnl.inc

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     S T R U C T U R E S                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SOME_STRUCTURE STRUCT
	SomeField1	DWORD		?
	SomeField2	DWORD		?
	; . . .						; Any other fields come here

	ListEntry	LIST_ENTRY	<>	; For tracking memory blocks.
								; It can be the first member but
								; to place it into is more common solution

	; . . .						; Any other fields come here
	SomeFieldX	DWORD		?
SOME_STRUCTURE ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_pPagedLookasideList	PPAGED_LOOKASIDE_LIST	?
g_ListHead				LIST_ENTRY				<>
g_dwIndex				DWORD					?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         AddEntry                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

AddEntry proc uses esi

	; We need new entry.
	; Allocate a memory block from lookaside list

	invoke ExAllocateFromPagedLookasideList, g_pPagedLookasideList
	.if eax != NULL
		mov esi, eax

		invoke DbgPrint, $CTA0("LookasideList: + Memory block allocated from lookaside list at address %08X\n"), esi

		; Zero out allocated memory block.

		invoke memset, esi, 0, sizeof SOME_STRUCTURE

		assume esi:ptr SOME_STRUCTURE

		; It's up to you how to add entries: to head or to tail of the list.
		; I add to head here.

		lea ecx, [esi].ListEntry
		InsertHeadList addr g_ListHead, ecx

		; Use SomeField1 to hold index. Only just we can see it works.

		inc g_dwIndex
		mov eax, g_dwIndex
		mov [esi].SomeField1, eax

		invoke DbgPrint, $CTA0("LookasideList: + Entry #%d added\n"), [esi].SomeField1

		assume esi:nothing

	.else
		invoke DbgPrint, $CTA0("LookasideList: Very bad. Couldn't allocate from lookaside list\n")
	.endif

	ret

AddEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       RemoveEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RemoveEntry proc uses esi

	IsListEmpty addr g_ListHead
	.if eax != TRUE				; Is there something to remove?

		; It's up to you how to remove entries: from head (RemoveHeadList),
		; from tail (RemoveTailList) or from the middle (RemoveEntryList) of the list.
		; I remove from head here.

		RemoveHeadList addr g_ListHead

		; Here eax -> SOME_STRUCTURE.ListEntry
		; We need to get pointer to structure containing this ListEntry.
		; CONTAINING_RECORD macro does this job but I do not implemented it.
		; So, we do it manually. It's easy, by the way.
		; We just need to substract relative offset to ListEntry field

		sub eax, SOME_STRUCTURE.ListEntry

		; Here eax -> SOME_STRUCTURE

		mov esi, eax				; esi -> SOME_STRUCTURE

		invoke DbgPrint, $CTA0("LookasideList: - Entry #%d removed\n"), \
							(SOME_STRUCTURE PTR [esi]).SomeField1

		; Put a block back onto lookaside list

		invoke ExFreeToPagedLookasideList, g_pPagedLookasideList, esi

		invoke DbgPrint, \
			$CTA0("LookasideList: - Memory block at address %08X returned to lookaside list\n"), esi
	.else
		invoke DbgPrint, \
			$CTA0("LookasideList: - An attempt was made to remove entry from empty lookaside list\n")
	.endif

	ret

RemoveEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc uses ebx pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

	invoke DbgPrint, $CTA0("\nLookasideList: Entering DriverEntry\n")

	; Always allocate nonpaged memory for PAGED_LOOKASIDE_LIST

	invoke ExAllocatePool, NonPagedPool, sizeof PAGED_LOOKASIDE_LIST
	.if eax != NULL

		mov g_pPagedLookasideList, eax
		
		invoke DbgPrint, \
		$CTA0("LookasideList: Nonpaged memory for lookaside list allocated at address %08X\n"), \
		g_pPagedLookasideList

		; Mark with any 4-char tag. I use 'Bla '

		invoke ExInitializePagedLookasideList, g_pPagedLookasideList, NULL, NULL, 0, sizeof SOME_STRUCTURE, ' alB', 0

		invoke DbgPrint, $CTA0("LookasideList: Lookaside list initialized\n")

		; We need somehow to track memory blocks we will allocate/free.
		; Doubly linked list is good solution.

		InitializeListHead addr g_ListHead

		invoke DbgPrint, $CTA0("LookasideList: Doubly linked list head initialized\n")

		; Suppose somewhere in your code you need to allocate
		; some arbitrary number of your SOME_STRUCTURE to collect some info.
		; Also you don't know exactly when and how many structures will be removed.
		; You just know you will need to add X structures and remove Y structures in arbitrary order.
		; This circle imitate such situation.

		invoke DbgPrint, $CTA0("\nLookasideList: Start to allocate/free from/to lookaside list\n")

		and g_dwIndex, 0				; Explicity initialize index.

		xor ebx, ebx
		.while ebx < 5					; Do some add/remove actions.

			invoke AddEntry
			invoke AddEntry

			invoke RemoveEntry

			inc ebx
		.endw

		; It's time to empty our list.
		; We don't know how many entries we have.
		; We just remove any untill list is empty.

		invoke DbgPrint, $CTA0("\nLookasideList: Free the rest to lookaside list\n")

		.while TRUE

			invoke RemoveEntry

			IsListEmpty addr g_ListHead
			.if eax == TRUE
				invoke DbgPrint, $CTA0("LookasideList: List is empty\n\n")
				.break
			.endif

		.endw

		; Here lookaside list is empty. Destroy a lookaside list.

		invoke ExDeletePagedLookasideList, g_pPagedLookasideList

		invoke DbgPrint, $CTA0("LookasideList: Lookaside list deleted\n")



		invoke ExFreePool, g_pPagedLookasideList

		invoke DbgPrint, \
		$CTA0("LookasideList: Nonpaged memory for lookaside list at address %08X released\n"), \
		g_pPagedLookasideList

	.else
		invoke DbgPrint, \
		$CTA0("LookasideList: Couldn't allocate memory for lookaside list from nonpaged memory\n")
	.endif

	invoke DbgPrint, $CTA0("LookasideList: Leaving DriverEntry\n")

	; Remove driver from the memory.

	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=LookasideList

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
