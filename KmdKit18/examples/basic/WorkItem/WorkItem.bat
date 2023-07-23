;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; WorkItem - How to use a work item.
;
; Written by Four-F (four-f@mail.ru)
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

IOCTL_WORK equ CTL_CODE(FILE_DEVICE_UNKNOWN, 800h, METHOD_BUFFERED, 0)

WORK STRUCT
	pIoWorkItem		PVOID	?	; PIO_WORKITEM
	nWorkNumber		DWORD	?
WORK ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
CCOUNTED_UNICODE_STRING	"\\Device\\WorkItem", g_usDeviceName, 4
CCOUNTED_UNICODE_STRING	"\\DosDevices\\WorkItem", g_usSymbolicLinkName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

g_fTimerStarted		BOOL			?
g_nWorkToDo			DWORD			?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                            N O N D I S C A R D A B L E   C O D E                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      WorkItemRoutine                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

WorkItemRoutine proc uses esi pDeviceObject:PDEVICE_OBJECT, pContext:PVOID

	mov esi, pContext
	assume esi:ptr WORK

	invoke DbgPrint, $CTA0("WorkItem: Work #%d is done\n"), [esi].nWorkNumber

	invoke IoFreeWorkItem, [esi].pIoWorkItem
			
	assume esi:nothing

	invoke ExFreePool, esi

	ret

WorkItemRoutine endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       TimerRoutine                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TimerRoutine proc uses esi pDeviceObject:PDEVICE_OBJECT, pContext:PVOID

; This routine is called at IRQL DISPATCH_LEVEL !

local pIoWorkItem:PVOID	; PIO_WORKITEM

	.if g_nWorkToDo != 0
	
		invoke IoAllocateWorkItem, pDeviceObject
		.if eax != NULL
			mov pIoWorkItem, eax

			; A caller executing at DISPATCH_LEVEL must specify a NonPagedXxx value for PoolType.
	
			invoke ExAllocatePool, NonPagedPool, sizeof WORK
			.if eax != NULL
			
				mov esi, eax
				assume esi:ptr WORK
				mov eax, pIoWorkItem
				mov [esi].pIoWorkItem, eax
				mov eax, g_nWorkToDo
				mov [esi].nWorkNumber, eax
				assume esi:nothing
				
				invoke IoQueueWorkItem, pIoWorkItem, offset WorkItemRoutine, DelayedWorkQueue, esi
				dec g_nWorkToDo
				
			.endif

		.endif

	.else
		invoke IoStopTimer, pDeviceObject
		and g_fTimerStarted, FALSE
	.endif

	ret

TimerRoutine endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverUnload                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverUnload proc pDriverObject:PDRIVER_OBJECT

	invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName

	.if g_fTimerStarted
		mov eax, pDriverObject
		invoke IoStopTimer, (DRIVER_OBJECT PTR [eax]).DeviceObject
		and g_fTimerStarted, FALSE
		invoke DbgPrint, $CTA0("WorkItem: Timer stopped\n")
	.endif

	mov eax, pDriverObject
	invoke IoDeleteDevice, (DRIVER_OBJECT PTR [eax]).DeviceObject

	ret

DriverUnload endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              D I S C A R D A B L E   C O D E                                      
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code INIT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

local status:NTSTATUS
local pDeviceObject:PDEVICE_OBJECT

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR

	; Explicity initialize global variables

	and g_fTimerStarted, FALSE
	and g_nWorkToDo, 0

	; Create exclusive device

	invoke IoCreateDevice, pDriverObject, 0, addr g_usDeviceName, FILE_DEVICE_UNKNOWN, 0, TRUE, addr pDeviceObject
	.if eax == STATUS_SUCCESS
		invoke IoCreateSymbolicLink, addr g_usSymbolicLinkName, addr g_usDeviceName
		.if eax == STATUS_SUCCESS
			mov eax, pDriverObject
			assume eax:ptr DRIVER_OBJECT
			mov [eax].DriverUnload, offset DriverUnload
			assume eax:nothing

			invoke IoInitializeTimer, pDeviceObject, TimerRoutine, NULL
			.if eax == STATUS_SUCCESS

				mov g_nWorkToDo, 5			; Number of jobs to do

				; Our TimerRoutine routine will be called once per second.

				invoke IoStartTimer, pDeviceObject
				mov g_fTimerStarted, TRUE

				invoke DbgPrint, $CTA0("WorkItem: Timer started\n")

				mov status, STATUS_SUCCESS

			.else
				invoke DbgPrint, $CTA0("WorkItem: Couldn't initialize timer. Status: %08X\n"), eax
				invoke IoDeleteSymbolicLink, addr g_usSymbolicLinkName
				invoke IoDeleteDevice, pDeviceObject
			.endif
		.else
			invoke DbgPrint, $CTA0("WorkItem: Couldn't create device. Status: %08X\n"), eax
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

set drv=WorkItem

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj

echo.
pause
