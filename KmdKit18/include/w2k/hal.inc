
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:           This file was created on 16 Jan 2005 with Protogen v1.0 by Four-F.                   ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                            STDCALL                                             ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

HalAcquireDisplayOwnership     proto stdcall :DWORD
HalAdjustResourceList          proto stdcall :DWORD
HalAllProcessorsStarted        proto stdcall
HalAllocateAdapterChannel      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
HalAllocateCommonBuffer        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
HalAllocateCrashDumpRegisters  proto stdcall :DWORD, :DWORD
HalAssignSlotResources         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HalBeginSystemInterrupt        proto stdcall :DWORD, :DWORD, :DWORD
HalCalibratePerformanceCounter proto stdcall :DWORD, :DWORD, :DWORD
HalDisableSystemInterrupt      proto stdcall :DWORD, :DWORD
HalDisplayString               proto stdcall :DWORD
HalEnableSystemInterrupt       proto stdcall :DWORD, :DWORD, :DWORD
HalEndSystemInterrupt          proto stdcall :DWORD, :DWORD
HalFlushCommonBuffer           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HalFreeCommonBuffer            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HalGetAdapter                  proto stdcall :DWORD, :DWORD
HalGetBusData                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HalGetBusDataByOffset          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HalGetEnvironmentVariable      proto stdcall :DWORD, :DWORD, :DWORD
HalGetInterruptVector          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HalHandleNMI                   proto stdcall :DWORD
HalInitSystem                  proto stdcall :DWORD, :DWORD
HalInitializeProcessor         proto stdcall :DWORD, :DWORD
HalMakeBeep                    proto stdcall :DWORD
HalProcessorIdle               proto stdcall
HalQueryDisplayParameters      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
HalQueryRealTimeClock          proto stdcall :DWORD
HalReadDmaCounter              proto stdcall :DWORD
HalReportResourceUsage         proto stdcall
HalRequestIpi                  proto stdcall :DWORD
HalReturnToFirmware            proto stdcall :DWORD
HalSetBusData                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HalSetBusDataByOffset          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HalSetDisplayParameters        proto stdcall :DWORD, :DWORD
HalSetEnvironmentVariable      proto stdcall :DWORD, :DWORD
HalSetProfileInterval          proto stdcall :DWORD
HalSetRealTimeClock            proto stdcall :DWORD
HalSetTimeIncrement            proto stdcall :DWORD
HalStartNextProcessor          proto stdcall :DWORD, :DWORD
HalStartProfileInterrupt       proto stdcall :DWORD
HalStopProfileInterrupt        proto stdcall :DWORD
HalTranslateBusAddress         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoAssignDriveLetters           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoFlushAdapterBuffers          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoFreeAdapterChannel           proto stdcall :DWORD
IoFreeMapRegisters             proto stdcall :DWORD, :DWORD, :DWORD
IoMapTransfer                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoReadPartitionTable           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoSetPartitionInformation      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoWritePartitionTable          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KdPortGetByte                  proto stdcall :DWORD
KdPortInitialize               proto stdcall :DWORD, :DWORD, :DWORD
KdPortPollByte                 proto stdcall :DWORD
KdPortPutByte                  proto stdcall :DWORD
KdPortRestore                  proto stdcall
KdPortSave                     proto stdcall
KeAcquireSpinLock              proto stdcall :DWORD, :DWORD
KeFlushWriteBuffer             proto stdcall
KeGetCurrentIrql               proto stdcall
KeLowerIrql                    proto stdcall :DWORD
KeQueryPerformanceCounter      proto stdcall :DWORD
KeRaiseIrql                    proto stdcall :DWORD, :DWORD
KeRaiseIrqlToDpcLevel          proto stdcall
KeRaiseIrqlToSynchLevel        proto stdcall
KeReleaseSpinLock              proto stdcall :DWORD, :DWORD
KeStallExecutionProcessor      proto stdcall :DWORD
READ_PORT_BUFFER_UCHAR         proto stdcall :DWORD, :DWORD, :DWORD
READ_PORT_BUFFER_ULONG         proto stdcall :DWORD, :DWORD, :DWORD
READ_PORT_BUFFER_USHORT        proto stdcall :DWORD, :DWORD, :DWORD
READ_PORT_UCHAR                proto stdcall :DWORD
READ_PORT_ULONG                proto stdcall :DWORD
READ_PORT_USHORT               proto stdcall :DWORD
WRITE_PORT_BUFFER_UCHAR        proto stdcall :DWORD, :DWORD, :DWORD
WRITE_PORT_BUFFER_ULONG        proto stdcall :DWORD, :DWORD, :DWORD
WRITE_PORT_BUFFER_USHORT       proto stdcall :DWORD, :DWORD, :DWORD
WRITE_PORT_UCHAR               proto stdcall :DWORD, :DWORD
WRITE_PORT_ULONG               proto stdcall :DWORD, :DWORD
WRITE_PORT_USHORT              proto stdcall :DWORD, :DWORD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                             CDECL                                              ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KdComPortInUse proto c :VARARG	; possible exported variable

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                           FASTCALL                                             ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

externdef syscall @ExAcquireFastMutex@4:proc
ExAcquireFastMutex textequ <@ExAcquireFastMutex@4>

externdef syscall @ExReleaseFastMutex@4:proc
ExReleaseFastMutex textequ <@ExReleaseFastMutex@4>

externdef syscall @ExTryToAcquireFastMutex@4:proc
ExTryToAcquireFastMutex textequ <@ExTryToAcquireFastMutex@4>

externdef syscall @HalClearSoftwareInterrupt@4:proc
HalClearSoftwareInterrupt textequ <@HalClearSoftwareInterrupt@4>

externdef syscall @HalRequestSoftwareInterrupt@4:proc
HalRequestSoftwareInterrupt textequ <@HalRequestSoftwareInterrupt@4>

externdef syscall @HalSystemVectorDispatchEntry@12:proc
HalSystemVectorDispatchEntry textequ <@HalSystemVectorDispatchEntry@12>

externdef syscall @KeAcquireQueuedSpinLock@4:proc
KeAcquireQueuedSpinLock textequ <@KeAcquireQueuedSpinLock@4>

externdef syscall @KeAcquireQueuedSpinLockRaiseToSynch@4:proc
KeAcquireQueuedSpinLockRaiseToSynch textequ <@KeAcquireQueuedSpinLockRaiseToSynch@4>

externdef syscall @KeAcquireSpinLockRaiseToSynch@4:proc
KeAcquireSpinLockRaiseToSynch textequ <@KeAcquireSpinLockRaiseToSynch@4>

externdef syscall @KeReleaseQueuedSpinLock@8:proc
KeReleaseQueuedSpinLock textequ <@KeReleaseQueuedSpinLock@8>

externdef syscall @KeTryToAcquireQueuedSpinLock@8:proc
KeTryToAcquireQueuedSpinLock textequ <@KeTryToAcquireQueuedSpinLock@8>

externdef syscall @KeTryToAcquireQueuedSpinLockRaiseToSynch@8:proc
KeTryToAcquireQueuedSpinLockRaiseToSynch textequ <@KeTryToAcquireQueuedSpinLockRaiseToSynch@8>

externdef syscall @KfAcquireSpinLock@4:proc
KfAcquireSpinLock textequ <@KfAcquireSpinLock@4>

externdef syscall @KfLowerIrql@4:proc
KfLowerIrql textequ <@KfLowerIrql@4>

externdef syscall @KfRaiseIrql@4:proc
KfRaiseIrql textequ <@KfRaiseIrql@4>

externdef syscall @KfReleaseSpinLock@8:proc
KfReleaseSpinLock textequ <@KfReleaseSpinLock@8>
