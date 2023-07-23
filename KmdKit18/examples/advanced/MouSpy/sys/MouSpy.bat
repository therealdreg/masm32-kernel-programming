;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  MouSpy - This is an example of a simple legacy mouse filter driver
;
;  WARNING: You will fail to attach to USB-mouse stack
;
;  We create two device objects. The first one is a control device. It provides
;  the interface to our user-mode client. The second device is a filter device.
;  It intercepts mouse data being passed from the mouse class driver.
;  So, it is an upper filter.  The intercepted data is collected in the list.
;  Upon timer triggering we signal shared event object to notify our user-mode
;  client about there is something interesting for it.  The user-mode client
;  issues control request and we copy all info into the buffer.
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
include \masm32\include\w2k\ntddmou.inc
include \masm32\include\w2k\hal.inc

includelib \masm32\lib\w2k\ntoskrnl.lib
includelib \masm32\lib\w2k\hal.lib

include \masm32\Macros\Strings.mac

include ..\common.inc

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       M A C R O S                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; spin lock macros

LOCK_ACQUIRE MACRO lck:REQ
	; Returns old IRQL in al
	IF (OPATTR (lck)) AND 00010000y
		;; Is a register value
		IFDIFI <lck>, <ecx>	;; don't move ecx onto itself
			mov ecx, lck
		ENDIF
	ELSEIF (OPATTR (lck)) AND 01000000y
		;; relative to SS
		lea ecx, lck
	ELSE
		mov ecx, offset lck
	ENDIF
	fastcall KfAcquireSpinLock, ecx
ENDM

LOCK_RELEASE MACRO lck:REQ, NewIrql:REQ
	IF (OPATTR (lck)) AND 00010000y
		;; Is a register value
		IFDIFI <lck>, <ecx>	;; don't move ecx onto itself
			mov ecx, lck
		ENDIF
	ELSEIF (OPATTR (lck)) AND 01000000y
		;; relative to SS
		lea ecx, lck
	ELSE
		mov ecx, offset lck
	ENDIF

	IFDIFI <NewIrql>, <dl>	;; don't move dl onto itself
		mov dl, NewIrql
	ENDIF

	.if dl == DISPATCH_LEVEL
		fastcall KefReleaseSpinLockFromDpcLevel, ecx
	.else
		and edx, 0FFh		;; for shure (KIRQL is BYTE)
		fastcall KfReleaseSpinLock, ecx, edx
	.endif
ENDM

; mutex macros

MUTEX_INIT MACRO mtx:REQ
	IF (OPATTR (mtx)) AND 00010000y
		;; Is a register value
		invoke KeInitializeMutex, mtx, 0
	ELSEIF (OPATTR (mtx)) AND 01000000y
		;; relative to SS
		invoke KeInitializeMutex, addr mtx, 0
	ELSE
		invoke KeInitializeMutex, offset mtx, 0
	ENDIF
ENDM

MUTEX_ACQUIRE MACRO mtx:REQ
	IF (OPATTR (mtx)) AND 00010000y
		;; Is a register value
		invoke KeWaitForMutexObject, mtx, Executive, KernelMode, FALSE, NULL
	ELSEIF (OPATTR (mtx)) AND 01000000y
		;; relative to SS
		invoke KeWaitForMutexObject, addr mtx, Executive, KernelMode, FALSE, NULL
	ELSE
		invoke KeWaitForMutexObject, offset mtx, Executive, KernelMode, FALSE, NULL
	ENDIF
ENDM

MUTEX_RELEASE MACRO mtx:REQ
	IF (OPATTR (mtx)) AND 00010000y
		;; Is a register value
		invoke KeReleaseMutex, mtx, FALSE
	ELSEIF (OPATTR (mtx)) AND 01000000y
		;; relative to SS
		invoke KeReleaseMutex, addr mtx, FALSE
	ELSE
		invoke KeReleaseMutex, offset mtx, FALSE
	ENDIF
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     S T R U C T U R E S                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MOUSE_DATA_ENTRY STRUCT
	ListEntry	LIST_ENTRY	<>		; For memory blocks tracking.
	MouseData	MOUSE_DATA	<>
MOUSE_DATA_ENTRY ENDS

FiDO_DEVICE_EXTENSION STRUCT

	; The top of the stack before this filter was added

	pNextLowerDeviceObject	PDEVICE_OBJECT	?

	; The referenced pointer to file object that represents
	; the corresponding device object.  This pointer we get
	; from IoGetDeviceObjectPointer and must dereference
	; while detaching.

	pTargetFileObject	PFILE_OBJECT	?

FiDO_DEVICE_EXTENSION ENDS
PFiDO_DEVICE_EXTENSION typedef ptr FiDO_DEVICE_EXTENSION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 R E A D O N L Y    D A T A                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

CCOUNTED_UNICODE_STRING	"\\Device\\MouSpy", g_usControlDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\MouSpy", g_usSymbolicLinkName, 4

CCOUNTED_UNICODE_STRING	"\\Device\\PointerClass0", g_usTargetDeviceName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_pDriverObject					PDRIVER_OBJECT	?

g_pControlDeviceObject			PDEVICE_OBJECT	?	; Control Device Object pointer
g_pFilterDeviceObject			PDEVICE_OBJECT	?	; Filter Device Object pointer

g_pEventObject					PKEVENT			?
; This spin-lock let us be sure that no one will dereference event object pointer
; while we compare it agaist NULL and then call KeSetEvent in our completion routine
g_EventSpinLock					KSPIN_LOCK		?	; locks mouse data list

g_fCDO_Opened					BOOL			?
g_fFiDO_Attached				BOOL			?
g_fSpy							BOOL			?

g_dwPendingRequests				DWORD			?

align 4
g_pMouseDataLookaside			PNPAGED_LOOKASIDE_LIST	?

align 4
g_MouseDataListHead				LIST_ENTRY		<>	; accessed under lock

; Holds number of MOUSE_DATA_ENTRYs in list. Should not exceed MAX_MOUSE_DATA_ENTRIES.
g_cMouseDataEntries				SDWORD			?	; accessed under lock

; This spin-lock let us be sure that only one thread is working with mouse data at a time
g_MouseDataSpinLock				KSPIN_LOCK		?

; This mutex let us be sure no one will try to do some unpredictable things.
; For example: no one can try to attach while we in the middle of the detaching.
align 4
g_mtxCDO_State					KMUTEX		<>

g_fInvertButtons				BOOL		?
g_fInvertMovement				BOOL		?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              _ExAllocateFromNPagedLookasideList                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_ExAllocateFromNPagedLookasideList proc uses esi Lookaside:PNPAGED_LOOKASIDE_LIST

;; Function Description:
;;    This function removes (pops) the first entry from the specified
;;    nonpaged lookaside list.
;;
;; Arguments:
;;    Lookaside - Supplies a pointer to a nonpaged lookaside list structure.
;;
;; Return Value:
;;    If an entry is removed from the specified lookaside list, then the
;;    address of the entry is returned as the function value. Otherwise,
;;    NULL is returned.

	mov esi, Lookaside
	assume esi:ptr NPAGED_LOOKASIDE_LIST
	mov ecx, esi
	inc [esi].L.TotalAllocates
	lea edx, [esi]._Lock
	fastcall ExInterlockedPopEntrySList, ecx, edx
	.if eax == NULL
		push [esi].L.Tag
		inc [esi].L.AllocateMisses
		push [esi].L._Size
		push [esi].L._Type
		call [esi].L.Allocate
	.endif
	assume esi:nothing

	ret

_ExAllocateFromNPagedLookasideList endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  _ExFreeToNPagedLookasideList                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_ExFreeToNPagedLookasideList proc Lookaside:PNPAGED_LOOKASIDE_LIST, Entry:PVOID

;; Function Description:
;;    This function inserts (pushes) the specified entry into the specified
;;    nonpaged lookaside list.
;;
;; Arguments:
;;    Lookaside - Supplies a pointer to a nonpaged lookaside list structure.
;;    Entry - Supples a pointer to the entry that is inserted in the lookaside list.
;;
;; Return Value:
;;    None.

	mov ecx, Lookaside
	assume ecx:ptr NPAGED_LOOKASIDE_LIST
	inc [ecx].L.TotalFrees
	mov ax, [ecx].L.ListHead.Depth
	.if ax >= [ecx].L.Depth
		push Entry
		inc [ecx].L.FreeMisses
		call [ecx].L.Free
	.else
		mov edx, Entry
		lea eax, [ecx]._Lock
		fastcall ExInterlockedPushEntrySList, ecx, edx, eax
	.endif
	assume ecx:nothing

	ret

_ExFreeToNPagedLookasideList endp
		
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         AddEntry                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

AddEntry proc uses ebx pMouseData:PMOUSE_DATA

; We have to access g_cMouseDataEntries and g_MouseDataListHead
; under lock protection. Since mouse movement occur relatively rare
; we simply protect whole code.  So, this proc may be optimized a little.

	LOCK_ACQUIRE g_MouseDataSpinLock
	mov bl, al			; old IRQL

	.if g_cMouseDataEntries < MAX_MOUSE_DATA_ENTRIES

		; Allocate new entry from lookaside list
	
		invoke _ExAllocateFromNPagedLookasideList, g_pMouseDataLookaside
		.if eax != NULL

			mov edx, eax
			assume edx:ptr MOUSE_DATA_ENTRY

			mov ecx, pMouseData
			assume ecx:ptr MOUSE_DATA

			mov eax, [ecx].LastX
			mov [edx].MouseData.LastX, eax

			mov eax, [ecx].LastY
			mov [edx].MouseData.LastY, eax

			mov eax, [ecx].Buttons
			mov [edx].MouseData.Buttons, eax

			assume ecx:nothing

			; Add to head

			lea ecx, [edx].ListEntry
			InsertHeadList addr g_MouseDataListHead, ecx

			assume edx:nothing

			inc g_cMouseDataEntries

		.endif
	.endif

	LOCK_RELEASE g_MouseDataSpinLock, bl

	ret

AddEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       RemoveEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RemoveEntry proc uses ebx pBuffer:PVOID

local cbCopied:DWORD

	and cbCopied, 0
	
	; We have to access g_cMouseDataEntries and g_MouseDataListHead
	; under lock protection. Since mouse movements occur relatively rare
	; we simply protect whole code.  So, this proc may be optimized a little.
	
	LOCK_ACQUIRE g_MouseDataSpinLock
	mov bl, al							; old IRQL

	IsListEmpty addr g_MouseDataListHead
	.if eax != TRUE							; Is there something to remove?
			
		; Remove from tail

		RemoveTailList addr g_MouseDataListHead

		mov edx, eax						; edx -> MOUSE_DATA_ENTRY.ListEntry
		sub edx, MOUSE_DATA_ENTRY.ListEntry	; edx -> MOUSE_DATA_ENTRY

		assume edx:ptr MOUSE_DATA_ENTRY

		mov ecx, pBuffer
		assume ecx:ptr MOUSE_DATA

		mov eax, [edx].MouseData.LastX
		mov [ecx].LastX, eax

		mov eax, [edx].MouseData.LastY
		mov [ecx].LastY, eax

		mov eax, [edx].MouseData.Buttons
		mov [ecx].Buttons, eax

		mov cbCopied, sizeof MOUSE_DATA

		assume ecx:nothing
		assume edx:nothing

		; Put a block back onto lookaside list

		invoke _ExFreeToNPagedLookasideList, g_pMouseDataLookaside, edx

		dec g_cMouseDataEntries

	.endif

	LOCK_RELEASE g_MouseDataSpinLock, bl

	mov eax, cbCopied
	ret

RemoveEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       MouseAttach                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MouseAttach proc

local status:NTSTATUS
local pTargetDeviceObject:PDEVICE_OBJECT
local pTargetFileObject:PFILE_OBJECT

	invoke DbgPrint, $CTA0("MouSpy: Entering MouseAttach\n")

	mov status, STATUS_UNSUCCESSFUL

	.if ( g_pFilterDeviceObject != NULL )

		; Filter device object exist and should be attached

		mov status, STATUS_SUCCESS

	.else

		; Let's attach to mouse device stack
		;
		; Create unnamed device because filter device objects should never be named.
		; We are going to attach it to existing mouse device stack. So no one may
		; directly open filter device by name.
		
		mov eax, g_pControlDeviceObject
		mov ecx, (DEVICE_OBJECT PTR [eax]).DriverObject

		invoke IoCreateDevice, ecx, sizeof FiDO_DEVICE_EXTENSION, NULL, \
					FILE_DEVICE_UNKNOWN, 0, FALSE, addr g_pFilterDeviceObject
		.if eax == STATUS_SUCCESS
	
			; Supply a name for any device object in the stack we are about to attach to.
			; IoGetDeviceObjectPointer returns the pointer to upper most device object in the stack.

			invoke IoGetDeviceObjectPointer, addr g_usTargetDeviceName, FILE_READ_DATA, \
										addr pTargetFileObject, addr pTargetDeviceObject
			.if eax == STATUS_SUCCESS
    
				; Here we have two pointers: pointer to the topmost device in the mouse stack
				; and pointer to the corresponding file object.  IoGetDeviceObjectPointer
				; references file object but not the device object.
				;
				; We are just one line from attaching to our target.  We must prevent
				; our driver from unloading while it intercepts mouse IRPs.
				; We could use RemoveLock, but the easiest solution is to remove pointer
				; to DriverUnload routine from driver object. OK, let's do it.

				mov eax, g_pDriverObject
				and (DRIVER_OBJECT PTR [eax]).DriverUnload, NULL
			
				; Now our driver is not unloadable

				invoke IoAttachDeviceToDeviceStack, g_pFilterDeviceObject, pTargetDeviceObject
				.if eax != NULL

					mov edx, eax

					; Fill filter device object extension

					mov ecx, g_pFilterDeviceObject
					mov eax, (DEVICE_OBJECT ptr [ecx]).DeviceExtension
					assume eax:ptr FiDO_DEVICE_EXTENSION
					mov [eax].pNextLowerDeviceObject, edx
					push pTargetFileObject
					pop [eax].pTargetFileObject
					assume eax:nothing

					; We need to copy DeviceType and Characteristics from the target device object
					; underneath us to our filter device object.  We also need to copy DO_DIRECT_IO,
					; DO_BUFFERED_IO, and DO_POWER_PAGABLE flags.  This guarantees that the filter
					; device object looks the same as the target device object.

					assume edx:ptr DEVICE_OBJECT
					assume ecx:ptr DEVICE_OBJECT

					mov eax, [edx].DeviceType
					mov [ecx].DeviceType, eax

					mov eax, [edx].Flags
					and eax, DO_DIRECT_IO + DO_BUFFERED_IO + DO_POWER_PAGABLE
					or [ecx].Flags, eax

					; IoCreateDevice sets the DO_DEVICE_INITIALIZING flag in the device object.
					; While this flag is set, the I/O Manager will refuse to attach other device
					; objects to us or to open a handle to our device.  So we have to clear
					; this flag because now we are ready to filter.
					;
					; Note: It is not necessary to clear the DO_DEVICE_INITIALIZING flag on device
					; objects that are created in DriverEntry, because this is done automatically
					; by the I/O Manager.

					and [ecx].Flags, not DO_DEVICE_INITIALIZING

					assume edx:nothing
					assume ecx:nothing

					mov status, STATUS_SUCCESS

				.else		; IoAttachDeviceToDeviceStack failed

					; We have failed to attach

					invoke ObDereferenceObject, pTargetFileObject
				
					invoke IoDeleteDevice, g_pFilterDeviceObject
					and g_pFilterDeviceObject, NULL

					; Let the driver to be unloaded
		
					mov eax, g_pDriverObject
					mov (DRIVER_OBJECT PTR [eax]).DriverUnload, offset DriverUnload

					invoke DbgPrint, $CTA0("MouSpy: Couldn't attach to target device\n")

					mov status, STATUS_NO_SUCH_DEVICE

				.endif
						
			.else		; IoGetDeviceObjectPointer failed

				invoke IoDeleteDevice, g_pFilterDeviceObject
				and g_pFilterDeviceObject, NULL
					
				invoke DbgPrint, $CTA0("MouSpy: Couldn't get target device object pointer\n")
			.endif

		.else
			invoke DbgPrint, $CTA0("MouSpy: Couldn't create filter device\n")
		.endif

	.endif

	mov eax, status
	ret

MouseAttach endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       MouseDetach                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MouseDetach proc

local status:NTSTATUS
local pTopmostDeviceObject:PDEVICE_OBJECT

	invoke DbgPrint, $CTA0("MouSpy: Entering MouseDetach\n")		

	mov status, STATUS_UNSUCCESSFUL

	.if g_pFilterDeviceObject != NULL

		; Lets see if there is someone above us.
		; Temporary set the DO_DEVICE_INITIALIZING flag in filter device object.
		; So no one can attach while we check the stack.

		mov eax, g_pFilterDeviceObject
		or (DEVICE_OBJECT ptr [eax]).Flags, DO_DEVICE_INITIALIZING

		invoke IoGetAttachedDeviceReference, g_pFilterDeviceObject
		mov pTopmostDeviceObject, eax

		.if eax != g_pFilterDeviceObject

			; Someone sits on the top of us. Do nothing except restoring
			; Flags field in the filter device object

			mov eax, g_pFilterDeviceObject
			and (DEVICE_OBJECT ptr [eax]).Flags, not DO_DEVICE_INITIALIZING
						
			invoke DbgPrint, $CTA0("MouSpy: Couldn't detach. Someone sits over\n")
			invoke DbgPrint, $CTA0("MouSpy: Filter device is still attached\n")

		.else			

			mov eax, g_pFilterDeviceObject
			mov eax, (DEVICE_OBJECT ptr [eax]).DeviceExtension
			mov ecx, (FiDO_DEVICE_EXTENSION ptr [eax]).pTargetFileObject

			fastcall ObfDereferenceObject, ecx

			mov eax, g_pFilterDeviceObject
			mov eax, (DEVICE_OBJECT ptr [eax]).DeviceExtension
			mov eax, (FiDO_DEVICE_EXTENSION ptr [eax]).pNextLowerDeviceObject

			invoke IoDetachDevice, eax
			
			mov status, STATUS_SUCCESS

			invoke DbgPrint, $CTA0("MouSpy: Filter device detached\n")

			; Destroy filter device.

			mov eax, g_pFilterDeviceObject
			and g_pFilterDeviceObject, NULL
			invoke IoDeleteDevice, eax

			; Our driver is still not unloadable because we might have outstanding IRPs

		.endif

		; Dereference the device object pointer returned by IoGetAttachedDeviceReference

		invoke ObDereferenceObject, pTopmostDeviceObject

	.endif

	mov eax, status
	ret

MouseDetach endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  FiDO_DispatchPassThrough                                         
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FiDO_DispatchPassThrough proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

; The default dispatch routine. Our driver should send down all IRPs it deal not with

   	IoSkipCurrentIrpStackLocation pIrp

	mov eax, pDeviceObject
	mov eax, (DEVICE_OBJECT ptr [eax]).DeviceExtension
	mov eax, (FiDO_DEVICE_EXTENSION ptr [eax]).pNextLowerDeviceObject

	invoke IoCallDriver, eax, pIrp
	ret

FiDO_DispatchPassThrough endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    CDO_DispatchCreate                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CDO_DispatchCreate proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

local status:NTSTATUS
local MouseData:MOUSE_DATA

	invoke DbgPrint, $CTA0("MouSpy: Entering CDO_DispatchCreate\n")

	; Drain g_MouseDataListHead.  If someone have ran MouSpy previously
	; but have failed to unload the driver because of not moving
	; the mouse as recommended, we have at least one pending IRP.  When
	; someone move the mouse this pending IRP is completed and our
	; completion routine will add one entry into g_MouseDataListHead.
	; So if it's not a first time we are being created we may have
	; some entr(ies)y in g_MouseDataListHead from previous sessions.
	; So lets throw them away.

	.while TRUE

		invoke RemoveEntry, addr MouseData
		.break .if eax == 0

	.endw

	MUTEX_ACQUIRE g_mtxCDO_State

	.if g_fCDO_Opened

		; Only one client at a time is allowed

		mov status, STATUS_DEVICE_BUSY

	.else

		; No one else may open control device
	
		mov g_fCDO_Opened, TRUE
		
		mov status, STATUS_SUCCESS

	.endif

	MUTEX_RELEASE g_mtxCDO_State

	mov ecx, pIrp
	and (_IRP PTR [ecx]).IoStatus.Information, 0
	mov eax, status
	mov (_IRP PTR [ecx]).IoStatus.Status, eax

	fastcall IofCompleteRequest, ecx, IO_NO_INCREMENT

	mov eax, status
	ret

CDO_DispatchCreate endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    CDO_DispatchClose                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CDO_DispatchClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	invoke DbgPrint, $CTA0("MouSpy: Entering CDO_DispatchClose\n")

	; Do not set completion routine any more
	
	and g_fSpy, FALSE

	and g_fInvertButtons, FALSE
	and g_fInvertMovement, FALSE
				
	MUTEX_ACQUIRE g_mtxCDO_State
				
	.if ( g_pFilterDeviceObject == NULL )

		.if g_dwPendingRequests == 0

			; If we have datached from the mouse stack, and there is
			; no outstanding IRPs it's safe to unload.

			mov eax, g_pDriverObject
			mov (DRIVER_OBJECT PTR [eax]).DriverUnload, offset DriverUnload

		.endif

	.endif

	; Someone else may open control device

	and g_fCDO_Opened, FALSE	

	MUTEX_RELEASE g_mtxCDO_State

	mov eax, STATUS_SUCCESS
		
	mov ecx, pIrp
	and (_IRP PTR [ecx]).IoStatus.Information, 0
	mov (_IRP PTR [ecx]).IoStatus.Status, eax

	push eax

	fastcall IofCompleteRequest, ecx, IO_NO_INCREMENT

	pop eax
	ret

CDO_DispatchClose endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     FillMouseData                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FillMouseData proc uses edi ebx pBuffer:PVOID, cbBuffer:DWORD

local MouseData:MOUSE_DATA
local cbReturned:DWORD

	and cbReturned, 0

	; Lets see how many MOUSE_DATAs will fit into passed in buffer

	mov eax, cbBuffer
	mov ecx, sizeof MOUSE_DATA
	xor edx, edx
	div ecx
	mov ebx, eax

	mov edi, pBuffer

	.while ebx

		invoke RemoveEntry, edi
		
		.break .if eax == 0

		add cbReturned,  eax

		dec ebx
		add edi, sizeof MOUSE_DATA

	.endw

	mov eax, cbReturned
	ret

FillMouseData endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                CDO_DispatchDeviceControl                                          
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CDO_DispatchDeviceControl proc uses esi edi ebx pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

local status:NTSTATUS
local pEventObject:PKEVENT
local liDelayTime:LARGE_INTEGER
local MouseData:MOUSE_DATA

	mov status, STATUS_UNSUCCESSFUL

	mov esi, pIrp
	assume esi:ptr _IRP

	mov [esi].IoStatus.Status, STATUS_UNSUCCESSFUL
	and [esi].IoStatus.Information, 0

	IoGetCurrentIrpStackLocation esi
	mov edi, eax
	assume edi:ptr IO_STACK_LOCATION

	.if [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_MOUSE_ATTACH
		.if [edi].Parameters.DeviceIoControl.InputBufferLength == sizeof HANDLE

			MUTEX_ACQUIRE g_mtxCDO_State

			; The user-mode client wants us attach to the mouse device stack

			mov edx, [esi].AssociatedIrp.SystemBuffer
			mov edx, [edx]			; event handle from user-mode

			mov ecx, ExEventObjectType
			mov ecx, [ecx]
			mov ecx, [ecx]			; PTR OBJECT_TYPE
	
			invoke ObReferenceObjectByHandle, edx, EVENT_MODIFY_STATE, ecx, \
										UserMode, addr pEventObject, NULL
			.if eax == STATUS_SUCCESS

				; If passed event handle is valid, attach to mouse
					
				.if !g_fFiDO_Attached

					invoke MouseAttach
					mov [esi].IoStatus.Status, eax

					.if eax == STATUS_SUCCESS

						mov eax, pEventObject
						mov g_pEventObject, eax			; No need to lock, since mov is atomic

						mov g_fFiDO_Attached, TRUE
						mov g_fSpy, TRUE				; Set completion routine.
		
					.else
						; Failed to attach
						invoke ObDereferenceObject, pEventObject
					.endif

				.else
					; We was attached

					LOCK_ACQUIRE g_EventSpinLock
					mov bl, al			; old IRQL

					mov eax, g_pEventObject
					.if eax != NULL
						and g_pEventObject, NULL
						invoke ObDereferenceObject, eax
					.endif

					mov eax, pEventObject
					mov g_pEventObject, eax
					
					LOCK_RELEASE g_EventSpinLock, bl

					mov g_fSpy, TRUE				; Set completion routine.

					mov [esi].IoStatus.Status, STATUS_SUCCESS
				.endif

			.else
				mov [esi].IoStatus.Status, STATUS_INVALID_PARAMETER
			.endif

			MUTEX_RELEASE g_mtxCDO_State
	
		.else
			mov [esi].IoStatus.Status, STATUS_INFO_LENGTH_MISMATCH
		.endif

	.elseif [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_MOUSE_DETACH

		MUTEX_ACQUIRE g_mtxCDO_State
	
		; The user-mode client wants us to detach from the mouse device stack

		.if g_fFiDO_Attached

			; Do not set completion routine any more no matter will we detach or not
	
			and g_fSpy, FALSE

			and g_fInvertButtons, FALSE
			and g_fInvertMovement, FALSE

			invoke MouseDetach
			mov [esi].IoStatus.Status, eax

			.if eax == STATUS_SUCCESS
				mov g_fFiDO_Attached, FALSE
			.endif

			LOCK_ACQUIRE g_EventSpinLock
			mov bl, al			; old IRQL

			mov eax, g_pEventObject
			.if eax != NULL
				and g_pEventObject, NULL
				invoke ObDereferenceObject, eax
			.endif

			LOCK_RELEASE g_EventSpinLock, bl

		.endif

		MUTEX_RELEASE g_mtxCDO_State
	
	.elseif [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_GET_MOUSE_DATA
		.if [edi].Parameters.DeviceIoControl.OutputBufferLength >= sizeof MOUSE_DATA

			invoke FillMouseData, [esi].AssociatedIrp.SystemBuffer, \
						[edi].Parameters.DeviceIoControl.OutputBufferLength

			mov [esi].IoStatus.Information, eax
			mov [esi].IoStatus.Status, STATUS_SUCCESS

		.else
			mov [esi].IoStatus.Status, STATUS_BUFFER_TOO_SMALL
		.endif


	.elseif [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_INVERT_BUTTONS
		.if [edi].Parameters.DeviceIoControl.InputBufferLength == sizeof BOOL

			mov eax, [esi].AssociatedIrp.SystemBuffer
			mov eax, [eax]
			mov g_fInvertButtons, eax

			and [esi].IoStatus.Information, 0
			mov [esi].IoStatus.Status, STATUS_SUCCESS

		.else
			mov [esi].IoStatus.Status, STATUS_INFO_LENGTH_MISMATCH
		.endif


	.elseif [edi].Parameters.DeviceIoControl.IoControlCode == IOCTL_INVERT_MOVEMENT
		.if [edi].Parameters.DeviceIoControl.InputBufferLength == sizeof BOOL

			mov eax, [esi].AssociatedIrp.SystemBuffer
			mov eax, [eax]
			mov g_fInvertMovement, eax

			and [esi].IoStatus.Information, 0
			mov [esi].IoStatus.Status, STATUS_SUCCESS

		.else
			mov [esi].IoStatus.Status, STATUS_INFO_LENGTH_MISMATCH
		.endif


	.else
		mov [esi].IoStatus.Status, STATUS_INVALID_DEVICE_REQUEST
	.endif

	mov eax, [esi].IoStatus.Status
	mov status, eax

	assume esi:nothing
	assume edi:nothing

	fastcall IofCompleteRequest, esi, IO_NO_INCREMENT

	mov eax, status
	ret

CDO_DispatchDeviceControl endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

local MouseData:MOUSE_DATA

	invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName

	; Empty and destroy list

	.while TRUE

		invoke RemoveEntry, addr MouseData
		.break .if eax == 0

	.endw

	invoke ExDeleteNPagedLookasideList, g_pMouseDataLookaside
	invoke ExFreePool, g_pMouseDataLookaside

	mov eax, pDriverObject
	invoke IoDeleteDevice, (DRIVER_OBJECT PTR [eax]).DeviceObject

	ret

DriverUnload endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      ReadComplete                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ReadComplete proc uses esi edi ebx pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP, pContext:PVOID

local MouseData:MOUSE_DATA
local cEntriesLogged:DWORD

	; This routine is to be called when the IRP is completed.
	; It is running at IRQL <= DISPATCH_LEVEL and in an arbitrary thread context.

    mov esi, pIrp
    assume esi:ptr _IRP

	; Probably better to use NT_SUCCESS-like behaviour, but it works anyway

	.if [esi].IoStatus.Status == STATUS_SUCCESS
	
		; At least one MOUSE_INPUT_DATA structure was transferred.

		; The AssociatedIrp.SystemBuffer member points to the output buffer 
		; that is allocated by the Win32 subsystem to output the requested  
		; number of MOUSE_INPUT_DATA structures.
		
		mov edi, [esi].AssociatedIrp.SystemBuffer
		assume edi:ptr MOUSE_INPUT_DATA
		
        ; The Information member specifies the number of bytes       
        ; that are transferred to the Win32 subsystem output buffer. 
        
        mov ebx, [esi].IoStatus.Information

		and cEntriesLogged, 0
		.while sdword ptr ebx >= sizeof MOUSE_INPUT_DATA
			
			mov eax, [edi].LastX
			mov MouseData.LastX, eax

			mov eax, [edi].LastY
			mov MouseData.LastY, eax

			mov eax, [edi].Buttons
			mov MouseData.Buttons, eax

			invoke AddEntry, addr MouseData
				
			inc cEntriesLogged

			; Now lets have fun
			
			.if g_fInvertButtons

				.if [edi].ButtonFlags & MOUSE_LEFT_BUTTON_DOWN
					and [edi].ButtonFlags, not MOUSE_LEFT_BUTTON_DOWN
					or [edi].ButtonFlags, MOUSE_RIGHT_BUTTON_DOWN
				.elseif [edi].ButtonFlags & MOUSE_RIGHT_BUTTON_DOWN
					and [edi].ButtonFlags, not MOUSE_RIGHT_BUTTON_DOWN
					or [edi].ButtonFlags, MOUSE_LEFT_BUTTON_DOWN
				.endif

				.if [edi].ButtonFlags & MOUSE_LEFT_BUTTON_UP
					and [edi].ButtonFlags, not MOUSE_LEFT_BUTTON_UP
					or [edi].ButtonFlags, MOUSE_RIGHT_BUTTON_UP
				.elseif [edi].ButtonFlags & MOUSE_RIGHT_BUTTON_UP
					and [edi].ButtonFlags, not MOUSE_RIGHT_BUTTON_UP
					or [edi].ButtonFlags, MOUSE_LEFT_BUTTON_UP
				.endif
			
			.endif

			.if g_fInvertMovement

				movzx eax, [edi].Flags
				and eax, MOUSE_MOVE_RELATIVE
				.if eax == MOUSE_MOVE_RELATIVE

					; Only for relative movement

					.if [edi].LastX != 0
						xor eax, eax
						sub eax, [edi].LastX
						mov [edi].LastX, eax
					.endif

					.if [edi].LastY != 0
						xor eax, eax
						sub eax, [edi].LastY
						mov [edi].LastY, eax
					.endif

				.endif
			.endif

			add edi, sizeof MOUSE_INPUT_DATA
			sub ebx, sizeof MOUSE_INPUT_DATA
		.endw

		assume edi:nothing

		; Notify user-mode client.

		.if ( cEntriesLogged != 0 )

			LOCK_ACQUIRE g_EventSpinLock
			mov bl, al			; old IRQL

			.if ( g_pEventObject != NULL ) 	; EventObject may go away
				invoke KeSetEvent, g_pEventObject, 0, FALSE
			.endif
			
			LOCK_RELEASE g_EventSpinLock, bl
						
		.endif
	
	.endif

	; Any driver that returns STATUS_SUCCESS from IoCompletion routine should check the
	; IRP->PendingReturned flag in the IoCompletion routine.  If the flag is set,
	; the IoCompletion routine must call IoMarkIrpPending with the IRP.
	
	.if [esi].PendingReturned
		IoMarkIrpPending esi
	.endif

 	assume esi:nothing

	lock dec g_dwPendingRequests

	mov eax, STATUS_SUCCESS
	ret

ReadComplete endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     FiDO_DispatchRead                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FiDO_DispatchRead proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; The IRP_MJ_READ request transfers zero or more MOUSE_INPUT_DATA structures 
	; from Mouclass's internal data queue to the Win32 subsystem buffer.

	.if g_fSpy

		lock inc g_dwPendingRequests

		; We pass the same parameters to lower driver copying our stack location to the next-lower one.

		IoCopyCurrentIrpStackLocationToNext pIrp

		; To find out how the IRP will be completed we install completion routine.
		; It will be called when the next-lower-level driver has completed IRP.

		IoSetCompletionRoutine pIrp, ReadComplete, NULL, TRUE, TRUE, TRUE

	.else

		; No need to know what will happen with IRP. So just pass it down and forget.
		; Bacause we do not need to set completion routine use IoSkipCurrentIrpStackLocation
		; instead of IoCopyCurrentIrpStackLocationToNext. It's faster.

    	IoSkipCurrentIrpStackLocation pIrp

	.endif

	; It's time to send an IRP to next-lower-level driver.

	mov eax, pDeviceObject
	mov eax, (DEVICE_OBJECT ptr [eax]).DeviceExtension
	mov eax, (FiDO_DEVICE_EXTENSION ptr [eax]).pNextLowerDeviceObject

	invoke IoCallDriver, eax, pIrp

	; We must return exactly the same value IoCallDriver has returned.

	ret

FiDO_DispatchRead endp


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     FiDO_DispatchPower                                            
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FiDO_DispatchPower proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	invoke PoStartNextPowerIrp, pIrp

   	IoSkipCurrentIrpStackLocation pIrp
	
	mov eax, pDeviceObject
	mov eax, (DEVICE_OBJECT ptr [eax]).DeviceExtension
	mov eax, (FiDO_DEVICE_EXTENSION ptr [eax]).pNextLowerDeviceObject

	invoke PoCallDriver, eax, pIrp

	; We must return exactly the same value PoCallDriver has returned.

	ret

FiDO_DispatchPower endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      DriverDispatch                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverDispatch proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

local status:NTSTATUS
local dwMajorFunction:DWORD

	IoGetCurrentIrpStackLocation pIrp

	movzx eax, (IO_STACK_LOCATION PTR [eax]).MajorFunction
	mov dwMajorFunction, eax

	mov eax, pDeviceObject
	.if eax == g_pFilterDeviceObject

		mov eax, dwMajorFunction
		.if eax == IRP_MJ_READ
			invoke FiDO_DispatchRead, pDeviceObject, pIrp
			mov status, eax
		.elseif eax == IRP_MJ_POWER
			invoke FiDO_DispatchPower, pDeviceObject, pIrp
			mov status, eax
		.else
			invoke FiDO_DispatchPassThrough, pDeviceObject, pIrp
			mov status, eax
		.endif

	.elseif eax == g_pControlDeviceObject

		; Request is to our CDO. Let' see what our client want us do
	
		mov eax, dwMajorFunction
		.if eax == IRP_MJ_CREATE
			invoke CDO_DispatchCreate, pDeviceObject, pIrp
			mov status, eax
		.elseif eax == IRP_MJ_CLOSE
			invoke CDO_DispatchClose, pDeviceObject, pIrp
			mov status, eax
		.elseif eax == IRP_MJ_DEVICE_CONTROL
			invoke CDO_DispatchDeviceControl, pDeviceObject, pIrp
			mov status, eax
		.else

			mov ecx, pIrp
			mov (_IRP PTR [ecx]).IoStatus.Status, STATUS_INVALID_DEVICE_REQUEST
			and (_IRP PTR [ecx]).IoStatus.Information, 0

			fastcall IofCompleteRequest, ecx, IO_NO_INCREMENT

			mov status, STATUS_INVALID_DEVICE_REQUEST
	
		.endif
	
	.else

		; Strange, we have recieved IRP for the device we do not know about.
		; This should never happen. Just complete IRP as invalid.

		mov ecx, pIrp
		mov (_IRP PTR [ecx]).IoStatus.Status, STATUS_INVALID_DEVICE_REQUEST
		and (_IRP PTR [ecx]).IoStatus.Information, 0

		fastcall IofCompleteRequest, ecx, IO_NO_INCREMENT

		mov status, STATUS_INVALID_DEVICE_REQUEST

	.endif

	mov eax, status
	ret

DriverDispatch endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              D I S C A R D A B L E   C O D E                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code INIT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local status:NTSTATUS

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	; Create a Control Device Object (CDO). The purpose of the CDO is to allow
	; our user-mode client to communicate with us, even before the filter is attached
	; to its target
	;
	; We store the CDO pointer into g_pControlDeviceObject, a globally defined variable.
	; This way we can identify the control device object in dispatch routines by comparing
	; the passed in device pointer against our CDO pointer
	;
	; CDO is exclusive one. It ensures that only one process opens the device at a time.
	; DDK stands it is reserved for system use and drivers set this parameter to FALSE.
	; Anyway we set it to TRUE and to force single-client logic mantain global variable
	; g_fCDOOpened which we will set/reset in CDO_DispatchCreate/CDO_DispatchClose

	invoke IoCreateDevice, pDriverObject, 0, addr g_usControlDeviceName, \
							FILE_DEVICE_UNKNOWN, 0, TRUE, addr g_pControlDeviceObject
	.if eax == STATUS_SUCCESS

		;mov eax, g_pControlDeviceObject
		;mov eax, (DEVICE_OBJECT ptr [eax]).DeviceExtension
		;and (CDO_DEVICE_EXTENSION ptr [eax]).fOpened, 0

		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usControlDeviceName
		.if eax == STATUS_SUCCESS

			; Allocate memory for lookaside list

			invoke ExAllocatePool, NonPagedPool, sizeof NPAGED_LOOKASIDE_LIST
			.if eax != NULL

				mov g_pMouseDataLookaside, eax

				invoke ExInitializeNPagedLookasideList, g_pMouseDataLookaside, \
										NULL, NULL, 0, sizeof MOUSE_DATA_ENTRY, 'ypSM', 0

				; Use doubly linked list to track memory blocks
				; we will allocate/free from/to lookaside list

				InitializeListHead addr g_MouseDataListHead

				and g_cMouseDataEntries, 0

				; Init spin lock guarding common driver routines
				
				invoke KeInitializeSpinLock, addr g_MouseDataSpinLock

				; Init spin lock guarding event pointer
				
				invoke KeInitializeSpinLock, addr g_EventSpinLock

				; Init CDO state mutex
				
				MUTEX_INIT g_mtxCDO_State
			
				; I know they all are zero by default, but...

				and g_fCDO_Opened, FALSE
				and g_fFiDO_Attached, FALSE
				and g_pFilterDeviceObject, NULL
				and g_fSpy, FALSE
				and g_dwPendingRequests, 0
				and g_fInvertButtons, FALSE
				and g_fInvertMovement, FALSE

				mov eax, pDriverObject
				assume eax:ptr DRIVER_OBJECT

				mov ecx, IRP_MJ_MAXIMUM_FUNCTION + 1
				.while ecx
					dec ecx
					mov [eax].MajorFunction[ecx*(sizeof PVOID)], offset DriverDispatch
				.endw

				mov [eax].DriverUnload,	offset DriverUnload
				assume eax:nothing

				mov eax, pDriverObject
				mov g_pDriverObject, eax

				mov status, STATUS_SUCCESS

			.else	; ExAllocatePool failed
				invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName
				invoke IoDeleteDevice, g_pControlDeviceObject
			.endif

		.else		; IoCreateSymbolicLink failed
			invoke IoDeleteDevice, g_pControlDeviceObject
		.endif

	.endif

	mov eax, status
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=MouSpy

if exist ..\%drv%.sys del ..\%drv%.sys

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj
move %drv%.sys ..

echo.
pause
