;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  MutualExclusion - "mutual exclusion" functionality
;
;  Written by Four-F (four-f@mail.ru)
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.386
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

NUM_THREADS	equ 5		; must not exceed MAXIMUM_WAIT_OBJECTS (64)	- Maximum number of wait objects
NUM_WORKS	equ 10

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       M A C R O S                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include Mutex.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 R E A D O N L Y    D A T A                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

CCOUNTED_UNICODE_STRING	"\\Device\\MutualExclusion", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\MutualExclusion", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_pkWaitBlock		PKWAIT_BLOCK	?
g_apkThreads		DWORD NUM_THREADS dup(?)	; Array of PKTHREAD
g_dwCountThreads	DWORD	?
g_kMutex			KMUTEX	<>
g_dwWorkElement		DWORD	?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        ThreadProc                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ThreadProc proc uses ebx Param:DWORD

local liDelayTime:LARGE_INTEGER
local pkThread:DWORD		; PKTHREAD
local dwWorkElement:DWORD

	invoke PsGetCurrentThread
	mov pkThread, eax
	invoke DbgPrint, $CTA0("MutualExclusion: Thread %08X is entering ThreadProc\n"), pkThread

	xor ebx, ebx
	.while ebx < NUM_WORKS

		invoke DbgPrint, $CTA0("MutualExclusion: Thread %08X is working on #%d\n"), pkThread, ebx

		MUTEX_WAIT addr g_kMutex

		; Read the resource shared across the threads

		push g_dwWorkElement
		pop dwWorkElement

		; Simulate working with the shared across the threads resource

		invoke rand				; Generates pseudo-random number 0 - 07FFFh
		shl eax, 4				; * 16
		neg eax					; delay = 0 - ~50 ms
		or liDelayTime.HighPart, -1
		mov liDelayTime.LowPart, eax
		invoke KeDelayExecutionThread, KernelMode, FALSE, addr liDelayTime

		; Write the resource shared across the threads back

		inc dwWorkElement

		push dwWorkElement
		pop g_dwWorkElement

		MUTEX_RELEASE addr g_kMutex

		mov eax, liDelayTime.LowPart
		neg eax
		mov edx, 3518437209		; Magic number
		mul edx
		shr edx, 13
		invoke DbgPrint, $CTA0("MutualExclusion: Thread %08X work #%d is done (%02dms)\n"), \
							pkThread, ebx, edx

		inc ebx					; Do next work

	.endw

	invoke DbgPrint, $CTA0("MutualExclusion: Thread %08X is about to terminate\n"), pkThread

	invoke PsTerminateSystemThread, STATUS_SUCCESS

	ret

ThreadProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          CleanUp                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CleanUp proc pDriverObject:PDRIVER_OBJECT

	invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName

	mov eax, pDriverObject
	invoke IoDeleteDevice, (DRIVER_OBJECT PTR [eax]).DeviceObject

	.if g_pkWaitBlock != NULL
		invoke ExFreePool, g_pkWaitBlock
		and g_pkWaitBlock, NULL
	.endif

	ret

CleanUp endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

	invoke DbgPrint, $CTA0("MutualExclusion: Entering DriverUnload\n")
	invoke DbgPrint, $CTA0("MutualExclusion: Wait for threads exit...\n")

	; We must not unload driver till even one of our system threads is runing
	; because ThreadProc resides in our driver's body. So we will wait.

	.if g_dwCountThreads > 0

		; No Timeout is supplied - wait infinite.

		invoke KeWaitForMultipleObjects, g_dwCountThreads, addr g_apkThreads, WaitAll, \
					Executive, KernelMode, FALSE, NULL, g_pkWaitBlock

		; Here all of the dispatcher objects satisfied the wait.
		; If the wait can be satisfied immediately an appropriate value 
		; STATUS_WAIT_1, STATUS_WAIT_2,...) is returned.
		; So, do not check return value against STATUS_SUCCESS.

		; Dereference all thread objects

		.while g_dwCountThreads
			dec g_dwCountThreads
			mov eax, g_dwCountThreads	; zero-based
			fastcall ObfDereferenceObject, g_apkThreads[eax * type g_apkThreads]
		.endw

	.endif

	invoke CleanUp, pDriverObject

	; Print result. g_dwWorkElement should be equal to NUM_THREADS * NUM_WORKS

	invoke DbgPrint, $CTA0("MutualExclusion: WorkElement = %d\n"), g_dwWorkElement

	invoke DbgPrint, $CTA0("MutualExclusion: Leaving DriverUnload\n")

	ret

DriverUnload endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              D I S C A R D A B L E   C O D E                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code INIT

; Compile-time check for shure.
; NUM_THREADS must not exceed MAXIMUM_WAIT_OBJECTS (64)	- Maximum number of wait objects.

; If number of wait objects > MAXIMUM_WAIT_OBJECTS,
; the system issues Bug Check 0xC (MAXIMUM_WAIT_OBJECTS_EXCEEDED).

IF NUM_THREADS GT MAXIMUM_WAIT_OBJECTS
	.ERR Maximum number of wait objects exceeded!
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       StartThread                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

StartThreads proc uses ebx

local hThread:HANDLE
local i:DWORD
	
	and i, 0							; Init counter of threads to run
	xor ebx, ebx						; ebx holds number of actually running threads
	.while i < NUM_THREADS				; Start NUM_THREADS threads
	
		; Drivers for Windows 2000 must only call PsCreateSystemThread from the system process context.
		; I'm not sure is it correct. Anyway we are in system process context here.

		invoke PsCreateSystemThread, addr hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc, 0
		.if eax == STATUS_SUCCESS

			; We do not need thread's handle returned by PsCreateSystemThread.
			; But we do need its pointer. So we reference thread object and close its handle.

			invoke ObReferenceObjectByHandle, hThread, THREAD_ALL_ACCESS, NULL, KernelMode, \
									addr g_apkThreads[ebx * type g_apkThreads], NULL

			invoke ZwClose, hThread
			invoke DbgPrint, $CTA0("MutualExclusion: System thread created. Thread Object: %08X\n"), \
									g_apkThreads[ebx * type g_apkThreads]
			inc ebx
		.else
			invoke DbgPrint, $CTA0("MutualExclusion: Can't create system thread. Status: %08X\n"), eax
		.endif
		inc i
	.endw

	mov g_dwCountThreads, ebx
	.if ebx != 0
		mov eax, STATUS_SUCCESS				; Indicates that at least one thread is running
	.else
		mov eax, STATUS_UNSUCCESSFUL		; Couldn't start any thread
	.endif

	ret

StartThreads endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local status:NTSTATUS
local pDeviceObject:PDEVICE_OBJECT
local liTickCount:LARGE_INTEGER

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, \
								FILE_DEVICE_UNKNOWN, 0, FALSE, addr pDeviceObject
	.if eax == STATUS_SUCCESS
		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usDeviceName
		.if eax == STATUS_SUCCESS

			; We must not unload driver till even one of our thread is runing
			; because ThreadProc resides in our driver's body. We will wait for threads exit.
			; For this purpose we need some memory. We must allocate it here because
			; if we will fail to do this in DriverUnload there is no way
			; to prevent driver from unloading.

			; Each thread object has a built-in array of wait blocks that can be used to wait
			; on several objects concurrently. Whenever possible, the built-in array of wait blocks
			; should be used in a wait-multiple operation because no additional wait block storage
			; needs to be allocated and later deallocated. However, if the number of objects
			; that must be waited on concurrently is greater than the number of built-in wait blocks,
			; use the WaitBlockArray parameter to specify an alternate set of wait blocks
			; to be used in the wait operation.
		
			; Our NUM_THREADS is larger than THREAD_WAIT_OBJECTS. So we have to use Wait Block.

			mov eax, NUM_THREADS
			mov ecx, sizeof KWAIT_BLOCK
			xor edx, edx
			mul ecx

			and g_pkWaitBlock, NULL
			invoke ExAllocatePool, NonPagedPool, eax
			.if eax != NULL
				mov g_pkWaitBlock, eax

				; Initialize the mutex. It is initialized with an initial state of signaled.

				MUTEX_INIT addr g_kMutex

				; For better performance, use the Ex..FastMutex routines instead of the Ke..Mutex.
				; However, a fast mutex cannot be acquired recursively, as a kernel mutex can.
				; Another drawback is that ExAcquireFastMutex sets the IRQL to APC_LEVEL,
				; and the caller continues to run at APC_LEVEL after ExAcquireFastMutex returns.

				invoke KeQueryTickCount, addr liTickCount

				; Initialize seed. It's global kernel variable, by the way, but never
				; (at least on my box) used.

				invoke srand, liTickCount.LowPart

				and g_dwWorkElement, 0

				invoke StartThreads
				.if eax == STATUS_SUCCESS
					mov eax, pDriverObject
					mov (DRIVER_OBJECT PTR [eax]).DriverUnload, offset DriverUnload
					mov status, STATUS_SUCCESS
				.else
					invoke CleanUp, pDriverObject
				.endif
			.else
				invoke CleanUp, pDriverObject
				invoke DbgPrint, $CTA0("MutualExclusion: Couldn't allocate memory for Wait Block\n")
			.endif
		.else
			invoke IoDeleteDevice, pDeviceObject
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

set drv=MutualExclusion

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj

echo.
pause
