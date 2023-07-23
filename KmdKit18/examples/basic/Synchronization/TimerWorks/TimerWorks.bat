;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  TimerWorks - Creates, sets, waits for and cancels the timer.
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
include \masm32\include\w2k\hal.inc

includelib \masm32\lib\w2k\ntoskrnl.lib
includelib \masm32\lib\w2k\hal.lib

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 R E A D O N L Y    D A T A                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

CCOUNTED_UNICODE_STRING	"\\Device\\TimerWorks", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\TimerWorks", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_pkThread	PVOID	?	; PTR KTHREAD
g_fStop		BOOL	?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        ThreadProc                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ThreadProc proc Param:DWORD

local dwCounter:DWORD
local pkThread:PVOID			; PKTHREAD
local status:NTSTATUS
local kTimer:KTIMER
local liDueTime:LARGE_INTEGER

	and dwCounter, 0

	invoke DbgPrint, $CTA0("\nTimerWorks: Entering ThreadProc\n")

	;::::::::::::::::::::::::::::::
	; Just for educational purposes

	invoke KeGetCurrentIrql
	invoke DbgPrint, $CTA0("TimerWorks: IRQL = %d\n"), eax


	invoke KeGetCurrentThread
	mov pkThread, eax
	invoke KeQueryPriorityThread, eax
	push eax
	invoke DbgPrint, $CTA0("TimerWorks: Thread Priority = %d\n"), eax

	pop eax
	inc eax
	inc eax
	invoke KeSetPriorityThread, pkThread, eax
	
	; Bear in mind that threads running in kernel mode with priority in the range
	; LOW_REALTIME_PRIORITY-HIGH_PRIORITY are preemptible only by a thread with higher priority.

	invoke KeQueryPriorityThread, pkThread
	invoke DbgPrint, $CTA0("TimerWorks: Thread Priority = %d\n"), eax

	; Just for educational purposes
	;::::::::::::::::::::::::::::::

	invoke KeInitializeTimerEx, addr kTimer, SynchronizationTimer

	; relative time at which the timer expires
	; in 100-nanosecond intervalss = 5 secs
	
	or liDueTime.HighPart, -1
	mov liDueTime.LowPart, -50000000

	; period for the timer in milliseconds = 1 sec
	
	invoke KeSetTimerEx, addr kTimer, liDueTime.LowPart, liDueTime.HighPart, 1000, NULL

	invoke DbgPrint, $CTA0("TimerWorks: Timer is set. It starts counting in 5 seconds...\n")

	.while dwCounter < 10
		invoke KeWaitForSingleObject, addr kTimer, Executive, KernelMode, FALSE, NULL

		; Basically we don't need to check status because the only reason
		; the wait is satisfied is the timer gets signalled.

		inc dwCounter
		invoke DbgPrint, $CTA0("TimerWorks: Counter = %d\n"), dwCounter

		; If DriverUnload routine called it's time to break the loop

		.if g_fStop
			invoke DbgPrint, $CTA0("TimerWorks: Stop counting to let the driver to be uloaded\n")
			.break
		.endif

	.endw

	invoke KeCancelTimer, addr kTimer

	invoke DbgPrint, $CTA0("TimerWorks: Timer is canceled. Leaving ThreadProc\n")
	invoke DbgPrint, $CTA0("TimerWorks: Our thread is about to terminate\n")

	invoke PsTerminateSystemThread, STATUS_SUCCESS

	ret

ThreadProc endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

	invoke DbgPrint, $CTA0("\nTimerWorks: Entering DriverUnload\n")

	mov g_fStop, TRUE	; Break the timer loop if it's counting

	; We must not unload driver till our system thread is runing
	; because ThredProc resides in our driver's body. So we will wait.
	; It's not good, btw, because we block one of the system thread.
	; Another solution is zero out DRIVER_OBJECT.DriverUnload
	; to make the driver unloadable and later restore this field.

	invoke DbgPrint, $CTA0("TimerWorks: Wait for thread exits...\n")
		
	invoke KeWaitForSingleObject, g_pkThread, Executive, KernelMode, FALSE, NULL
	
	; Basically we don't need to check status because the only reason
	; the wait is satisfied is terminating the thread g_pkThread pointed to.

	invoke ObDereferenceObject, g_pkThread

	invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName

	mov eax, pDriverObject
	invoke IoDeleteDevice, (DRIVER_OBJECT PTR [eax]).DeviceObject

	invoke DbgPrint, $CTA0("TimerWorks: Leaving DriverUnload\n")

	ret

DriverUnload endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              D I S C A R D A B L E   C O D E                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code INIT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       StartThread                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

StartThread proc

local status:NTSTATUS
local oa:OBJECT_ATTRIBUTES
local hThread:HANDLE

	invoke DbgPrint, $CTA0("\nTimerWorks: Entering StartThread\n")

	invoke PsCreateSystemThread, addr hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc, NULL
	mov status, eax
	.if eax == STATUS_SUCCESS

		invoke ObReferenceObjectByHandle, hThread, THREAD_ALL_ACCESS, NULL, KernelMode, addr g_pkThread, NULL

		invoke ZwClose, hThread
		invoke DbgPrint, $CTA0("TimerWorks: Thread created\n")
	.else
		invoke DbgPrint, $CTA0("TimerWorks: Can't create Thread. Status: %08X\n"), eax
	.endif

	invoke DbgPrint, $CTA0("TimerWorks: Leaving StartThread\n")

	mov eax, status
	ret

StartThread endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local status:NTSTATUS
local pDeviceObject:PDEVICE_OBJECT

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, FILE_DEVICE_UNKNOWN, 0, TRUE, addr pDeviceObject
	.if eax == STATUS_SUCCESS
		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usDeviceName
		.if eax == STATUS_SUCCESS
			invoke StartThread
			.if eax == STATUS_SUCCESS
				and g_fStop, FALSE			; reset global flag
				mov eax, pDriverObject
				mov (DRIVER_OBJECT PTR [eax]).DriverUnload, offset DriverUnload
				mov status, STATUS_SUCCESS
			.else
				invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName
				invoke IoDeleteDevice, pDeviceObject
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

set drv=TimerWorks

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj

echo.
pause
