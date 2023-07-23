
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:           This file was created on 16 Jan 2005 with Protogen v1.0 by Four-F.                   ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; __cdecls was fixed by hand !!!

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                            STDCALL                                             ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CcCanIWrite                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CcCopyRead                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcCopyWrite                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcDeferWrite                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcFastCopyRead                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcFastCopyWrite                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CcFlushCache                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CcGetDirtyPages                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CcGetFileObjectFromBcb                       proto stdcall :DWORD
CcGetFileObjectFromSectionPtrs               proto stdcall :DWORD
CcGetFlushedValidData                        proto stdcall :DWORD, :DWORD
CcGetLsnForFileObject                        proto stdcall :DWORD, :DWORD
CcInitializeCacheMap                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcIsThereDirtyData                           proto stdcall :DWORD
CcMapData                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcMdlRead                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcMdlReadComplete                            proto stdcall :DWORD, :DWORD
CcMdlWriteComplete                           proto stdcall :DWORD, :DWORD, :DWORD
CcPinMappedData                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcPinRead                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcPrepareMdlWrite                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcPreparePinWrite                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CcPurgeCacheSection                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CcRemapBcb                                   proto stdcall :DWORD
CcRepinBcb                                   proto stdcall :DWORD
CcScheduleReadAhead                          proto stdcall :DWORD, :DWORD, :DWORD
CcSetAdditionalCacheAttributes               proto stdcall :DWORD, :DWORD, :DWORD
CcSetBcbOwnerPointer                         proto stdcall :DWORD, :DWORD
CcSetDirtyPageThreshold                      proto stdcall :DWORD, :DWORD
CcSetDirtyPinnedData                         proto stdcall :DWORD, :DWORD
CcSetFileSizes                               proto stdcall :DWORD, :DWORD
CcSetLogHandleForFile                        proto stdcall :DWORD, :DWORD, :DWORD
CcSetReadAheadGranularity                    proto stdcall :DWORD, :DWORD
CcUninitializeCacheMap                       proto stdcall :DWORD, :DWORD, :DWORD
CcUnpinData                                  proto stdcall :DWORD
CcUnpinDataForThread                         proto stdcall :DWORD, :DWORD
CcUnpinRepinnedBcb                           proto stdcall :DWORD, :DWORD, :DWORD
CcWaitForCurrentLazyWriterActivity           proto stdcall
CcZeroData                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
DbgBreakPoint                                proto stdcall
DbgBreakPointWithStatus                      proto stdcall :DWORD
DbgLoadImageSymbols                          proto stdcall :DWORD, :DWORD, :DWORD
DbgPrompt                                    proto stdcall :DWORD, :DWORD, :DWORD
ExAcquireResourceExclusive                   proto stdcall :DWORD, :DWORD
ExAcquireResourceExclusiveLite               proto stdcall :DWORD, :DWORD
ExAcquireResourceSharedLite                  proto stdcall :DWORD, :DWORD
ExAcquireSharedStarveExclusive               proto stdcall :DWORD, :DWORD
ExAcquireSharedWaitForExclusive              proto stdcall :DWORD, :DWORD
ExAllocateFromPagedLookasideList             proto stdcall :DWORD
ExAllocatePool                               proto stdcall :DWORD, :DWORD
ExAllocatePoolWithQuota                      proto stdcall :DWORD, :DWORD
ExAllocatePoolWithQuotaTag                   proto stdcall :DWORD, :DWORD, :DWORD
ExAllocatePoolWithTag                        proto stdcall :DWORD, :DWORD, :DWORD
ExAllocatePoolWithTagPriority                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ExConvertExclusiveToSharedLite               proto stdcall :DWORD
ExCreateCallback                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ExDeleteNPagedLookasideList                  proto stdcall :DWORD
ExDeletePagedLookasideList                   proto stdcall :DWORD
ExDeleteResource                             proto stdcall :DWORD
ExDeleteResourceLite                         proto stdcall :DWORD
ExDisableResourceBoostLite                   proto stdcall :DWORD
ExEnumHandleTable                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ExExtendZone                                 proto stdcall :DWORD, :DWORD, :DWORD
ExFreePool                                   proto stdcall :DWORD
ExFreePoolWithTag                            proto stdcall :DWORD, :DWORD
ExFreeToPagedLookasideList                   proto stdcall :DWORD, :DWORD
ExGetExclusiveWaiterCount                    proto stdcall :DWORD
ExGetPreviousMode                            proto stdcall
ExGetSharedWaiterCount                       proto stdcall :DWORD
ExInitializeNPagedLookasideList              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ExInitializePagedLookasideList               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ExInitializeResource                         proto stdcall :DWORD
ExInitializeResourceLite                     proto stdcall :DWORD
ExInitializeZone                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ExInterlockedAddLargeInteger                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ExInterlockedAddUlong                        proto stdcall :DWORD, :DWORD, :DWORD
ExInterlockedDecrementLong                   proto stdcall :DWORD, :DWORD
ExInterlockedExchangeUlong                   proto stdcall :DWORD, :DWORD, :DWORD
ExInterlockedExtendZone                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ExInterlockedIncrementLong                   proto stdcall :DWORD, :DWORD
ExInterlockedInsertHeadList                  proto stdcall :DWORD, :DWORD, :DWORD
ExInterlockedInsertTailList                  proto stdcall :DWORD, :DWORD, :DWORD
ExInterlockedPopEntryList                    proto stdcall :DWORD, :DWORD
ExInterlockedPushEntryList                   proto stdcall :DWORD, :DWORD, :DWORD
ExInterlockedRemoveHeadList                  proto stdcall :DWORD, :DWORD
ExIsProcessorFeaturePresent                  proto stdcall :DWORD
ExIsResourceAcquiredExclusiveLite            proto stdcall :DWORD
ExIsResourceAcquiredSharedLite               proto stdcall :DWORD
ExLocalTimeToSystemTime                      proto stdcall :DWORD, :DWORD
ExNotifyCallback                             proto stdcall :DWORD, :DWORD, :DWORD
ExQueryPoolBlockSize                         proto stdcall :DWORD, :DWORD
ExQueueWorkItem                              proto stdcall :DWORD, :DWORD
ExRaiseAccessViolation                       proto stdcall
ExRaiseDatatypeMisalignment                  proto stdcall
ExRaiseException                             proto stdcall :DWORD
ExRaiseHardError                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ExRaiseStatus                                proto stdcall :DWORD
ExRegisterCallback                           proto stdcall :DWORD, :DWORD, :DWORD
ExReinitializeResourceLite                   proto stdcall :DWORD
ExReleaseResourceForThread                   proto stdcall :DWORD, :DWORD
ExReleaseResourceForThreadLite               proto stdcall :DWORD, :DWORD
ExSetResourceOwnerPointer                    proto stdcall :DWORD, :DWORD
ExSetTimerResolution                         proto stdcall :DWORD, :DWORD
ExSystemExceptionFilter                      proto stdcall
ExSystemTimeToLocalTime                      proto stdcall :DWORD, :DWORD
ExUnregisterCallback                         proto stdcall :DWORD
ExUuidCreate                                 proto stdcall :DWORD
Exi386InterlockedDecrementLong               proto stdcall :DWORD
Exi386InterlockedExchangeUlong               proto stdcall :DWORD, :DWORD
Exi386InterlockedIncrementLong               proto stdcall :DWORD
FsRtlAcquireFileExclusive                    proto stdcall :DWORD
FsRtlAddLargeMcbEntry                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlAddMcbEntry                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FsRtlAddToTunnelCache                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlAllocateFileLock                        proto stdcall :DWORD, :DWORD
FsRtlAllocatePool                            proto stdcall :DWORD, :DWORD
FsRtlAllocatePoolWithQuota                   proto stdcall :DWORD, :DWORD
FsRtlAllocatePoolWithQuotaTag                proto stdcall :DWORD, :DWORD, :DWORD
FsRtlAllocatePoolWithTag                     proto stdcall :DWORD, :DWORD, :DWORD
FsRtlAllocateResource                        proto stdcall
FsRtlAreNamesEqual                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FsRtlBalanceReads                            proto stdcall :DWORD
FsRtlCheckLockForReadAccess                  proto stdcall :DWORD, :DWORD
FsRtlCheckLockForWriteAccess                 proto stdcall :DWORD, :DWORD
FsRtlCheckOplock                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlCopyRead                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlCopyWrite                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlCurrentBatchOplock                      proto stdcall :DWORD
FsRtlDeleteKeyFromTunnelCache                proto stdcall :DWORD, :DWORD, :DWORD
FsRtlDeleteTunnelCache                       proto stdcall :DWORD
FsRtlDeregisterUncProvider                   proto stdcall :DWORD
FsRtlDissectDbcs                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FsRtlDissectName                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FsRtlDoesDbcsContainWildCards                proto stdcall :DWORD
FsRtlDoesNameContainWildCards                proto stdcall :DWORD
FsRtlFastCheckLockForRead                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlFastCheckLockForWrite                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlFastUnlockAll                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FsRtlFastUnlockAllByKey                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlFastUnlockSingle                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlFindInTunnelCache                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlFreeFileLock                            proto stdcall :DWORD
FsRtlGetFileSize                             proto stdcall :DWORD, :DWORD
FsRtlGetNextFileLock                         proto stdcall :DWORD, :DWORD
FsRtlGetNextLargeMcbEntry                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlGetNextMcbEntry                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlInitializeFileLock                      proto stdcall :DWORD, :DWORD, :DWORD
FsRtlInitializeLargeMcb                      proto stdcall :DWORD, :DWORD
FsRtlInitializeMcb                           proto stdcall :DWORD, :DWORD
FsRtlInitializeOplock                        proto stdcall :DWORD
FsRtlInitializeTunnelCache                   proto stdcall :DWORD
FsRtlInsertFilterContext                     proto stdcall :DWORD, :DWORD
FsRtlIsDbcsInExpression                      proto stdcall :DWORD, :DWORD
FsRtlIsFatDbcsLegal                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlIsHpfsDbcsLegal                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlIsNameInExpression                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FsRtlIsNtstatusExpected                      proto stdcall :DWORD
FsRtlIsTotalDeviceFailure                    proto stdcall :DWORD
FsRtlLookupFilterContextInternal             proto stdcall :DWORD, :DWORD, :DWORD
FsRtlLookupLargeMcbEntry                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlLookupLastLargeMcbEntry                 proto stdcall :DWORD, :DWORD, :DWORD
FsRtlLookupLastLargeMcbEntryAndIndex         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FsRtlLookupLastMcbEntry                      proto stdcall :DWORD, :DWORD, :DWORD
FsRtlLookupMcbEntry                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlMdlRead                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlMdlReadComplete                         proto stdcall :DWORD, :DWORD
FsRtlMdlReadCompleteDev                      proto stdcall :DWORD, :DWORD, :DWORD
FsRtlMdlReadDev                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlMdlWriteComplete                        proto stdcall :DWORD, :DWORD, :DWORD
FsRtlMdlWriteCompleteDev                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FsRtlNormalizeNtstatus                       proto stdcall :DWORD, :DWORD
FsRtlNotifyChangeDirectory                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlNotifyCleanup                           proto stdcall :DWORD, :DWORD, :DWORD
FsRtlNotifyFullChangeDirectory               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlNotifyFullReportChange                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlNotifyInitializeSync                    proto stdcall :DWORD
FsRtlNotifyReportChange                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlNotifyUninitializeSync                  proto stdcall :DWORD
FsRtlNotifyVolumeEvent                       proto stdcall :DWORD, :DWORD
FsRtlNumberOfRunsInLargeMcb                  proto stdcall :DWORD
FsRtlNumberOfRunsInMcb                       proto stdcall :DWORD
FsRtlOplockFsctrl                            proto stdcall :DWORD, :DWORD, :DWORD
FsRtlOplockIsFastIoPossible                  proto stdcall :DWORD
FsRtlPostPagingFileStackOverflow             proto stdcall :DWORD, :DWORD, :DWORD
FsRtlPostStackOverflow                       proto stdcall :DWORD, :DWORD, :DWORD
FsRtlPrepareMdlWrite                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlPrepareMdlWriteDev                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlPrivateLock                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlProcessFileLock                         proto stdcall :DWORD, :DWORD, :DWORD
FsRtlRegisterUncProvider                     proto stdcall :DWORD, :DWORD, :DWORD
FsRtlReleaseFile                             proto stdcall :DWORD
FsRtlRemoveFilterContext                     proto stdcall :DWORD, :DWORD, :DWORD
FsRtlRemoveLargeMcbEntry                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlRemoveMcbEntry                          proto stdcall :DWORD, :DWORD, :DWORD
FsRtlResetLargeMcb                           proto stdcall :DWORD, :DWORD
FsRtlSplitLargeMcb                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FsRtlSyncVolumes                             proto stdcall :DWORD, :DWORD, :DWORD
FsRtlTeardownFilterContexts                  proto stdcall :DWORD
FsRtlTruncateLargeMcb                        proto stdcall :DWORD, :DWORD, :DWORD
FsRtlTruncateMcb                             proto stdcall :DWORD, :DWORD
FsRtlUninitializeFileLock                    proto stdcall :DWORD
FsRtlUninitializeLargeMcb                    proto stdcall :DWORD
FsRtlUninitializeMcb                         proto stdcall :DWORD
FsRtlUninitializeOplock                      proto stdcall :DWORD
InbvAcquireDisplayOwnership                  proto stdcall
InbvCheckDisplayOwnership                    proto stdcall
InbvDisplayString                            proto stdcall :DWORD
InbvEnableBootDriver                         proto stdcall :DWORD
InbvEnableDisplayString                      proto stdcall :DWORD
InbvInstallDisplayStringFilter               proto stdcall :DWORD
InbvIsBootDriverInstalled                    proto stdcall
InbvNotifyDisplayOwnershipLost               proto stdcall :DWORD
InbvResetDisplay                             proto stdcall
InbvSetScrollRegion                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
InbvSetTextColor                             proto stdcall :DWORD
InbvSolidColorFill                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoAcquireCancelSpinLock                      proto stdcall :DWORD
IoAcquireRemoveLockEx                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoAcquireVpbSpinLock                         proto stdcall :DWORD
IoAllocateAdapterChannel                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoAllocateController                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoAllocateDriverObjectExtension              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoAllocateErrorLogEntry                      proto stdcall :DWORD, :DWORD
IoAllocateIrp                                proto stdcall :DWORD, :DWORD
IoAllocateMdl                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoAllocateWorkItem                           proto stdcall :DWORD
IoAssignResources                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoAttachDevice                               proto stdcall :DWORD, :DWORD, :DWORD
IoAttachDeviceByPointer                      proto stdcall :DWORD, :DWORD
IoAttachDeviceToDeviceStack                  proto stdcall :DWORD, :DWORD
IoBuildAsynchronousFsdRequest                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoBuildDeviceIoControlRequest                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoBuildPartialMdl                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoBuildSynchronousFsdRequest                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoCallDriver                                 proto stdcall :DWORD, :DWORD
IoCancelFileOpen                             proto stdcall :DWORD, :DWORD
IoCancelIrp                                  proto stdcall :DWORD
IoCheckDesiredAccess                         proto stdcall :DWORD, :DWORD
IoCheckEaBufferValidity                      proto stdcall :DWORD, :DWORD, :DWORD
IoCheckFunctionAccess                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoCheckQuerySetFileInformation               proto stdcall :DWORD, :DWORD, :DWORD
IoCheckQuerySetVolumeInformation             proto stdcall :DWORD, :DWORD, :DWORD
IoCheckQuotaBufferValidity                   proto stdcall :DWORD, :DWORD, :DWORD
IoCheckShareAccess                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoCompleteRequest                            proto stdcall :DWORD, :DWORD
IoConnectInterrupt                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoCreateController                           proto stdcall :DWORD
IoCreateDevice                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoCreateDriver                               proto stdcall :DWORD, :DWORD
IoCreateFile                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoCreateNotificationEvent                    proto stdcall :DWORD, :DWORD
IoCreateStreamFileObject                     proto stdcall :DWORD, :DWORD
IoCreateStreamFileObjectLite                 proto stdcall :DWORD, :DWORD
IoCreateSymbolicLink                         proto stdcall :DWORD, :DWORD
IoCreateSynchronizationEvent                 proto stdcall :DWORD, :DWORD
IoCreateUnprotectedSymbolicLink              proto stdcall :DWORD, :DWORD
IoDeleteController                           proto stdcall :DWORD
IoDeleteDevice                               proto stdcall :DWORD
IoDeleteDriver                               proto stdcall :DWORD
IoDeleteSymbolicLink                         proto stdcall :DWORD
IoDetachDevice                               proto stdcall :DWORD
IoDisconnectInterrupt                        proto stdcall :DWORD
IoEnqueueIrp                                 proto stdcall :DWORD
IoFastQueryNetworkAttributes                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoFreeController                             proto stdcall :DWORD
IoFreeIrp                                    proto stdcall :DWORD
IoFreeMdl                                    proto stdcall :DWORD
IoFreeWorkItem                               proto stdcall :DWORD
IoGetAttachedDevice                          proto stdcall :DWORD
IoGetAttachedDeviceReference                 proto stdcall :DWORD
IoGetBaseFileSystemDeviceObject              proto stdcall :DWORD
IoGetBootDiskInformation                     proto stdcall :DWORD, :DWORD
IoGetConfigurationInformation                proto stdcall
IoGetCurrentProcess                          proto stdcall
IoGetDeviceInterfaceAlias                    proto stdcall :DWORD, :DWORD, :DWORD
IoGetDeviceInterfaces                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoGetDeviceObjectPointer                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoGetDeviceProperty                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoGetDeviceToVerify                          proto stdcall :DWORD
IoGetDmaAdapter                              proto stdcall :DWORD, :DWORD, :DWORD
IoGetDriverObjectExtension                   proto stdcall :DWORD, :DWORD
IoGetFileObjectGenericMapping                proto stdcall
IoGetInitialStack                            proto stdcall
IoGetRelatedDeviceObject                     proto stdcall :DWORD
IoGetRequestorProcess                        proto stdcall :DWORD
IoGetRequestorProcessId                      proto stdcall :DWORD
IoGetRequestorSessionId                      proto stdcall :DWORD, :DWORD
IoGetStackLimits                             proto stdcall :DWORD, :DWORD
IoGetTopLevelIrp                             proto stdcall
IoInitializeIrp                              proto stdcall :DWORD, :DWORD, :DWORD
IoInitializeRemoveLockEx                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoInitializeTimer                            proto stdcall :DWORD, :DWORD, :DWORD
IoInvalidateDeviceRelations                  proto stdcall :DWORD, :DWORD
IoInvalidateDeviceState                      proto stdcall :DWORD
IoIsOperationSynchronous                     proto stdcall :DWORD
IoIsSystemThread                             proto stdcall :DWORD
IoIsValidNameGraftingBuffer                  proto stdcall :DWORD, :DWORD
IoIsWdmVersionAvailable                      proto stdcall :DWORD, :DWORD
IoMakeAssociatedIrp                          proto stdcall :DWORD, :DWORD
IoOpenDeviceInterfaceRegistryKey             proto stdcall :DWORD, :DWORD, :DWORD
IoOpenDeviceRegistryKey                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoPageRead                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoPnPDeliverServicePowerNotification         proto stdcall :DWORD, :DWORD
IoQueryDeviceDescription                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoQueryFileInformation                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoQueryVolumeInformation                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoQueueThreadIrp                             proto stdcall :DWORD
IoQueueWorkItem                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoRaiseHardError                             proto stdcall :DWORD, :DWORD, :DWORD
IoRaiseInformationalHardError                proto stdcall :DWORD, :DWORD, :DWORD
IoRegisterBootDriverReinitialization         proto stdcall :DWORD, :DWORD, :DWORD
IoRegisterDeviceInterface                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoRegisterDriverReinitialization             proto stdcall :DWORD, :DWORD, :DWORD
IoRegisterFileSystem                         proto stdcall :DWORD
IoRegisterFsRegistrationChange               proto stdcall :DWORD, :DWORD
IoRegisterLastChanceShutdownNotification     proto stdcall :DWORD
IoRegisterPlugPlayNotification               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoRegisterShutdownNotification               proto stdcall :DWORD
IoReleaseCancelSpinLock                      proto stdcall :DWORD
IoReleaseRemoveLockAndWaitEx                 proto stdcall :DWORD, :DWORD, :DWORD
IoReleaseRemoveLockEx                        proto stdcall :DWORD, :DWORD, :DWORD
IoReleaseVpbSpinLock                         proto stdcall :DWORD
IoRemoveShareAccess                          proto stdcall :DWORD, :DWORD
IoReportDetectedDevice                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoReportHalResourceUsage                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoReportResourceForDetection                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoReportResourceUsage                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoReportTargetDeviceChange                   proto stdcall :DWORD, :DWORD
IoReportTargetDeviceChangeAsynchronous       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoRequestDeviceEject                         proto stdcall :DWORD
IoReuseIrp                                   proto stdcall :DWORD, :DWORD
IoSetDeviceInterfaceState                    proto stdcall :DWORD, :DWORD
IoSetDeviceToVerify                          proto stdcall :DWORD, :DWORD
IoSetHardErrorOrVerifyDevice                 proto stdcall :DWORD, :DWORD
IoSetInformation                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoSetIoCompletion                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoSetShareAccess                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoSetThreadHardErrorMode                     proto stdcall :DWORD
IoSetTopLevelIrp                             proto stdcall :DWORD
IoStartNextPacket                            proto stdcall :DWORD, :DWORD
IoStartNextPacketByKey                       proto stdcall :DWORD, :DWORD, :DWORD
IoStartPacket                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoStartTimer                                 proto stdcall :DWORD
IoStopTimer                                  proto stdcall :DWORD
IoSynchronousInvalidateDeviceRelations       proto stdcall :DWORD, :DWORD
IoSynchronousPageWrite                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IoThreadToProcess                            proto stdcall :DWORD
IoUnregisterFileSystem                       proto stdcall :DWORD
IoUnregisterFsRegistrationChange             proto stdcall :DWORD, :DWORD
IoUnregisterPlugPlayNotification             proto stdcall :DWORD
IoUnregisterShutdownNotification             proto stdcall :DWORD
IoUpdateShareAccess                          proto stdcall :DWORD, :DWORD
IoVerifyVolume                               proto stdcall :DWORD, :DWORD
IoWMIAllocateInstanceIds                     proto stdcall :DWORD, :DWORD, :DWORD
IoWMIRegistrationControl                     proto stdcall :DWORD, :DWORD
IoWMISuggestInstanceName                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IoWMIWriteEvent                              proto stdcall :DWORD
IoWriteErrorLogEntry                         proto stdcall :DWORD
KdDisableDebugger                            proto stdcall
KdEnableDebugger                             proto stdcall
KdPollBreakIn                                proto stdcall
Ke386CallBios                                proto stdcall :DWORD, :DWORD
Ke386IoSetAccessProcess                      proto stdcall :DWORD, :DWORD
Ke386QueryIoAccessMap                        proto stdcall :DWORD, :DWORD
Ke386SetIoAccessMap                          proto stdcall :DWORD, :DWORD
KeAcquireSpinLockAtDpcLevel                  proto stdcall :DWORD
KeAddSystemServiceTable                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeAttachProcess                              proto stdcall :DWORD
KeBugCheck                                   proto stdcall :DWORD
KeBugCheckEx                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeCancelTimer                                proto stdcall :DWORD
KeClearEvent                                 proto stdcall :DWORD
KeConnectInterrupt                           proto stdcall :DWORD
KeDelayExecutionThread                       proto stdcall :DWORD, :DWORD, :DWORD
KeDeregisterBugCheckCallback                 proto stdcall :DWORD
KeDetachProcess                              proto stdcall
KeDisconnectInterrupt                        proto stdcall :DWORD
KeEnterCriticalRegion                        proto stdcall
KeEnterKernelDebugger                        proto stdcall
KeFindConfigurationEntry                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
KeFindConfigurationNextEntry                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeFlushEntireTb                              proto stdcall :DWORD, :DWORD
KeGetCurrentThread                           proto stdcall
KeGetPreviousMode                            proto stdcall
KeI386AbiosCall                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
KeI386AllocateGdtSelectors                   proto stdcall :DWORD, :DWORD
KeI386Call16BitCStyleFunction                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
KeI386Call16BitFunction                      proto stdcall :DWORD
KeI386FlatToGdtSelector                      proto stdcall :DWORD, :DWORD, :DWORD
KeI386GetLid                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeI386ReleaseGdtSelectors                    proto stdcall :DWORD, :DWORD
KeI386ReleaseLid                             proto stdcall :DWORD, :DWORD
KeI386SetGdtSelector                         proto stdcall :DWORD, :DWORD
KeInitializeApc                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeInitializeDeviceQueue                      proto stdcall :DWORD
KeInitializeDpc                              proto stdcall :DWORD, :DWORD, :DWORD
KeInitializeEvent                            proto stdcall :DWORD, :DWORD, :DWORD
KeInitializeInterrupt                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeInitializeMutant                           proto stdcall :DWORD, :DWORD
KeInitializeMutex                            proto stdcall :DWORD, :DWORD
KeInitializeQueue                            proto stdcall :DWORD, :DWORD
KeInitializeSemaphore                        proto stdcall :DWORD, :DWORD, :DWORD
KeInitializeSpinLock                         proto stdcall :DWORD
KeInitializeTimer                            proto stdcall :DWORD
KeInitializeTimerEx                          proto stdcall :DWORD, :DWORD
KeInsertByKeyDeviceQueue                     proto stdcall :DWORD, :DWORD, :DWORD
KeInsertDeviceQueue                          proto stdcall :DWORD, :DWORD
KeInsertHeadQueue                            proto stdcall :DWORD, :DWORD
KeInsertQueue                                proto stdcall :DWORD, :DWORD
KeInsertQueueApc                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
KeInsertQueueDpc                             proto stdcall :DWORD, :DWORD, :DWORD
KeIsExecutingDpc                             proto stdcall
KeLeaveCriticalRegion                        proto stdcall
KeProfileInterrupt                           proto stdcall :DWORD
KeProfileInterruptWithSource                 proto stdcall :DWORD, :DWORD
KePulseEvent                                 proto stdcall :DWORD, :DWORD, :DWORD
KeQueryActiveProcessors                      proto stdcall
KeQueryInterruptTime                         proto stdcall
KeQueryPriorityThread                        proto stdcall :DWORD
KeQuerySystemTime                            proto stdcall :DWORD
KeQueryTickCount                             proto stdcall :DWORD
KeQueryTimeIncrement                         proto stdcall
KeRaiseUserException                         proto stdcall :DWORD
KeReadStateEvent                             proto stdcall :DWORD
KeReadStateMutant                            proto stdcall :DWORD
KeReadStateMutex                             proto stdcall :DWORD
KeReadStateQueue                             proto stdcall :DWORD
KeReadStateSemaphore                         proto stdcall :DWORD
KeReadStateTimer                             proto stdcall :DWORD
KeRegisterBugCheckCallback                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeReleaseMutant                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
KeReleaseMutex                               proto stdcall :DWORD, :DWORD
KeReleaseSemaphore                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
KeReleaseSpinLockFromDpcLevel                proto stdcall :DWORD
KeRemoveByKeyDeviceQueue                     proto stdcall :DWORD, :DWORD
KeRemoveDeviceQueue                          proto stdcall :DWORD
KeRemoveEntryDeviceQueue                     proto stdcall :DWORD, :DWORD
KeRemoveQueue                                proto stdcall :DWORD, :DWORD, :DWORD
KeRemoveQueueDpc                             proto stdcall :DWORD
KeResetEvent                                 proto stdcall :DWORD
KeRestoreFloatingPointState                  proto stdcall :DWORD
KeRevertToUserAffinityThread                 proto stdcall
KeRundownQueue                               proto stdcall :DWORD
KeSaveFloatingPointState                     proto stdcall :DWORD
KeSetAffinityThread                          proto stdcall :DWORD, :DWORD
KeSetBasePriorityThread                      proto stdcall :DWORD, :DWORD
KeSetDmaIoCoherency                          proto stdcall :DWORD
KeSetEvent                                   proto stdcall :DWORD, :DWORD, :DWORD
KeSetEventBoostPriority                      proto stdcall :DWORD, :DWORD
KeSetIdealProcessorThread                    proto stdcall :DWORD, :DWORD
KeSetImportanceDpc                           proto stdcall :DWORD, :DWORD
KeSetKernelStackSwapEnable                   proto stdcall :DWORD
KeSetPriorityThread                          proto stdcall :DWORD, :DWORD
KeSetProfileIrql                             proto stdcall :DWORD
KeSetSystemAffinityThread                    proto stdcall :DWORD
KeSetTargetProcessorDpc                      proto stdcall :DWORD, :DWORD
KeSetTimeIncrement                           proto stdcall :DWORD, :DWORD
KeSetTimer                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
KeSetTimerEx                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeStackAttachProcess                         proto stdcall :DWORD, :DWORD
KeSynchronizeExecution                       proto stdcall :DWORD, :DWORD, :DWORD
KeTerminateThread                            proto stdcall :DWORD
KeUnstackDetachProcess                       proto stdcall :DWORD
KeUpdateRunTime                              proto stdcall :DWORD
KeUpdateSystemTime                           proto stdcall
KeUserModeCallback                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeWaitForMultipleObjects                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeWaitForMutexObject                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KeWaitForSingleObject                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KiCoprocessorError                           proto stdcall
KiDeliverApc                                 proto stdcall :DWORD, :DWORD, :DWORD
KiDispatchInterrupt                          proto stdcall
KiIpiServiceRoutine                          proto stdcall :DWORD, :DWORD
LdrAccessResource                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrEnumResources                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
LdrFindResourceDirectory_U                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrFindResource_U                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LpcRequestPort                               proto stdcall :DWORD, :DWORD
LpcRequestWaitReplyPort                      proto stdcall :DWORD, :DWORD, :DWORD
LsaCallAuthenticationPackage                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
LsaDeregisterLogonProcess                    proto stdcall :DWORD
LsaFreeReturnBuffer                          proto stdcall :DWORD
LsaLogonUser                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
LsaLookupAuthenticationPackage               proto stdcall :DWORD, :DWORD, :DWORD
LsaRegisterLogonProcess                      proto stdcall :DWORD, :DWORD, :DWORD
MmAddPhysicalMemory                          proto stdcall :DWORD, :DWORD
MmAdjustWorkingSetSize                       proto stdcall :DWORD, :DWORD, :DWORD
MmAllocateContiguousMemory                   proto stdcall :DWORD, :DWORD, :DWORD
MmAllocateContiguousMemorySpecifyCache       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
MmAllocateNonCachedMemory                    proto stdcall :DWORD
MmAllocatePagesForMdl                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
MmBuildMdlForNonPagedPool                    proto stdcall :DWORD
MmCanFileBeTruncated                         proto stdcall :DWORD, :DWORD
MmCreateMdl                                  proto stdcall :DWORD, :DWORD, :DWORD
MmCreateSection                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
MmDisableModifiedWriteOfSection              proto stdcall :DWORD
MmFlushImageSection                          proto stdcall :DWORD, :DWORD
MmForceSectionClosed                         proto stdcall :DWORD, :DWORD
MmFreeContiguousMemory                       proto stdcall :DWORD
MmFreeContiguousMemorySpecifyCache           proto stdcall :DWORD, :DWORD, :DWORD
MmFreeNonCachedMemory                        proto stdcall :DWORD, :DWORD
MmFreePagesFromMdl                           proto stdcall :DWORD
MmGetPhysicalAddress                         proto stdcall :DWORD
MmGetPhysicalMemoryRanges                    proto stdcall
MmGetSystemRoutineAddress                    proto stdcall :DWORD
MmGetVirtualForPhysical                      proto stdcall :DWORD, :DWORD
MmGrowKernelStack                            proto stdcall :DWORD
MmIsAddressValid                             proto stdcall :DWORD
MmIsDriverVerifying                          proto stdcall :DWORD
MmIsNonPagedSystemAddressValid               proto stdcall :DWORD
MmIsRecursiveIoFault                         proto stdcall
MmIsThisAnNtAsSystem                         proto stdcall
MmLockPagableDataSection                     proto stdcall :DWORD
MmLockPagableImageSection                    proto stdcall :DWORD
MmLockPagableSectionByHandle                 proto stdcall :DWORD
MmMapIoSpace                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
MmMapLockedPages                             proto stdcall :DWORD, :DWORD
MmMapLockedPagesSpecifyCache                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
MmMapMemoryDumpMdl                           proto stdcall :DWORD
MmMapUserAddressesToPage                     proto stdcall :DWORD, :DWORD, :DWORD
MmMapVideoDisplay                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
MmMapViewInSessionSpace                      proto stdcall :DWORD, :DWORD, :DWORD
MmMapViewInSystemSpace                       proto stdcall :DWORD, :DWORD, :DWORD
MmMapViewOfSection                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
MmPageEntireDriver                           proto stdcall :DWORD
MmProbeAndLockPages                          proto stdcall :DWORD, :DWORD, :DWORD
MmProbeAndLockProcessPages                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
MmProbeAndLockSelectedPages                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
MmQuerySystemSize                            proto stdcall
MmRemovePhysicalMemory                       proto stdcall :DWORD, :DWORD
MmResetDriverPaging                          proto stdcall :DWORD
MmSecureVirtualMemory                        proto stdcall :DWORD, :DWORD, :DWORD
MmSetAddressRangeModified                    proto stdcall :DWORD, :DWORD
MmSetBankedSection                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
MmSizeOfMdl                                  proto stdcall :DWORD, :DWORD
MmTrimAllSystemPagableMemory                 proto stdcall :DWORD
MmUnlockPagableImageSection                  proto stdcall :DWORD
MmUnlockPages                                proto stdcall :DWORD
MmUnmapIoSpace                               proto stdcall :DWORD, :DWORD
MmUnmapLockedPages                           proto stdcall :DWORD, :DWORD
MmUnmapVideoDisplay                          proto stdcall :DWORD, :DWORD
MmUnmapViewInSessionSpace                    proto stdcall :DWORD
MmUnmapViewInSystemSpace                     proto stdcall :DWORD
MmUnmapViewOfSection                         proto stdcall :DWORD, :DWORD
MmUnsecureVirtualMemory                      proto stdcall :DWORD
NtAddAtom                                    proto stdcall :DWORD, :DWORD, :DWORD
NtAdjustPrivilegesToken                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAllocateLocallyUniqueId                    proto stdcall :DWORD
NtAllocateUuids                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtAllocateVirtualMemory                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtClose                                      proto stdcall :DWORD
NtConnectPort                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateEvent                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateFile                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateSection                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtDeleteAtom                                 proto stdcall :DWORD
NtDeleteFile                                 proto stdcall :DWORD
NtDeviceIoControlFile                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtDuplicateObject                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtDuplicateToken                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtFindAtom                                   proto stdcall :DWORD, :DWORD, :DWORD
NtFreeVirtualMemory                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtFsControlFile                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtLockFile                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtMapViewOfSection                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtNotifyChangeDirectoryFile                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtOpenFile                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtOpenProcess                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtOpenProcessToken                           proto stdcall :DWORD, :DWORD, :DWORD
NtQueryDirectoryFile                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryEaFile                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationAtom                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationFile                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationProcess                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationToken                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryQuotaInformationFile                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQuerySecurityObject                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQuerySystemInformation                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtQueryVolumeInformationFile                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtReadFile                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtRequestPort                                proto stdcall :DWORD, :DWORD
NtRequestWaitReplyPort                       proto stdcall :DWORD, :DWORD, :DWORD
NtSetEaFile                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetEvent                                   proto stdcall :DWORD, :DWORD
NtSetInformationFile                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtSetInformationProcess                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetInformationThread                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetQuotaInformationFile                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetSecurityObject                          proto stdcall :DWORD, :DWORD, :DWORD
NtSetVolumeInformationFile                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtUnlockFile                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtVdmControl                                 proto stdcall :DWORD, :DWORD
NtWaitForSingleObject                        proto stdcall :DWORD, :DWORD, :DWORD
NtWriteFile                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObAssignSecurity                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ObCheckCreateObjectAccess                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObCheckObjectAccess                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObCreateObject                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObDereferenceObject                          proto stdcall :DWORD
ObFindHandleForObject                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObGetObjectPointerCount                      proto stdcall :DWORD
ObGetObjectSecurity                          proto stdcall :DWORD, :DWORD, :DWORD
ObInsertObject                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObMakeTemporaryObject                        proto stdcall :DWORD
ObOpenObjectByName                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObOpenObjectByPointer                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObQueryNameString                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ObQueryObjectAuditingByHandle                proto stdcall :DWORD, :DWORD
ObReferenceObjectByHandle                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObReferenceObjectByName                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ObReferenceObjectByPointer                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ObReleaseObjectSecurity                      proto stdcall :DWORD, :DWORD
ObSetSecurityDescriptorInfo                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PfxFindPrefix                                proto stdcall :DWORD, :DWORD
PfxInitialize                                proto stdcall :DWORD
PfxInsertPrefix                              proto stdcall :DWORD, :DWORD, :DWORD
PfxRemovePrefix                              proto stdcall :DWORD, :DWORD
PoCallDriver                                 proto stdcall :DWORD, :DWORD
PoCancelDeviceNotify                         proto stdcall :DWORD
PoRegisterDeviceForIdleDetection             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
PoRegisterDeviceNotify                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PoRegisterSystemState                        proto stdcall :DWORD, :DWORD
PoRequestPowerIrp                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PoSetHiberRange                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PoSetPowerState                              proto stdcall :DWORD, :DWORD, :DWORD
PoSetSystemState                             proto stdcall :DWORD
PoShutdownBugCheck                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PoStartNextPowerIrp                          proto stdcall :DWORD
PoUnregisterSystemState                      proto stdcall :DWORD
ProbeForRead                                 proto stdcall :DWORD, :DWORD, :DWORD
ProbeForWrite                                proto stdcall :DWORD, :DWORD, :DWORD
PsAssignImpersonationToken                   proto stdcall :DWORD, :DWORD
PsChargePoolQuota                            proto stdcall :DWORD, :DWORD, :DWORD
PsCreateSystemProcess                        proto stdcall :DWORD, :DWORD, :DWORD
PsCreateSystemThread                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PsDisableImpersonation                       proto stdcall :DWORD, :DWORD
PsEstablishWin32Callouts                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PsGetCurrentProcessId                        proto stdcall
PsGetCurrentThreadId                         proto stdcall
PsGetProcessExitTime                         proto stdcall
PsGetVersion                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
PsImpersonateClient                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PsIsThreadTerminating                        proto stdcall :DWORD
PsLookupProcessByProcessId                   proto stdcall :DWORD, :DWORD
PsLookupProcessThreadByCid                   proto stdcall :DWORD, :DWORD, :DWORD
PsLookupThreadByThreadId                     proto stdcall :DWORD, :DWORD
PsReferenceImpersonationToken                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
PsReferencePrimaryToken                      proto stdcall :DWORD
PsRestoreImpersonation                       proto stdcall :DWORD, :DWORD
PsReturnPoolQuota                            proto stdcall :DWORD, :DWORD, :DWORD
PsRevertToSelf                               proto stdcall
PsSetCreateProcessNotifyRoutine              proto stdcall :DWORD, :DWORD
PsSetCreateThreadNotifyRoutine               proto stdcall :DWORD
PsSetLegoNotifyRoutine                       proto stdcall :DWORD
PsSetLoadImageNotifyRoutine                  proto stdcall :DWORD
PsSetProcessPriorityByClass                  proto stdcall :DWORD, :DWORD
PsTerminateSystemThread                      proto stdcall :DWORD
READ_REGISTER_BUFFER_UCHAR                   proto stdcall :DWORD, :DWORD, :DWORD
READ_REGISTER_BUFFER_ULONG                   proto stdcall :DWORD, :DWORD, :DWORD
READ_REGISTER_BUFFER_USHORT                  proto stdcall :DWORD, :DWORD, :DWORD
READ_REGISTER_UCHAR                          proto stdcall :DWORD
READ_REGISTER_ULONG                          proto stdcall :DWORD
READ_REGISTER_USHORT                         proto stdcall :DWORD
RtlAbsoluteToSelfRelativeSD                  proto stdcall :DWORD, :DWORD, :DWORD
RtlAddAccessAllowedAce                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAce                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAtomToAtomTable                        proto stdcall :DWORD, :DWORD, :DWORD
RtlAddRange                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAllocateHeap                              proto stdcall :DWORD, :DWORD, :DWORD
RtlAnsiCharToUnicodeChar                     proto stdcall :DWORD
RtlAnsiStringToUnicodeSize                   proto stdcall :DWORD
RtlAnsiStringToUnicodeString                 proto stdcall :DWORD, :DWORD, :DWORD
RtlAppendAsciizToString                      proto stdcall :DWORD, :DWORD
RtlAppendStringToString                      proto stdcall :DWORD, :DWORD
RtlAppendUnicodeStringToString               proto stdcall :DWORD, :DWORD
RtlAppendUnicodeToString                     proto stdcall :DWORD, :DWORD
RtlAreAllAccessesGranted                     proto stdcall :DWORD, :DWORD
RtlAreAnyAccessesGranted                     proto stdcall :DWORD, :DWORD
RtlAreBitsClear                              proto stdcall :DWORD, :DWORD, :DWORD
RtlAreBitsSet                                proto stdcall :DWORD, :DWORD, :DWORD
RtlAssert                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlCaptureContext                            proto stdcall :DWORD
RtlCaptureStackBackTrace                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlCharToInteger                             proto stdcall :DWORD, :DWORD, :DWORD
RtlCheckRegistryKey                          proto stdcall :DWORD, :DWORD
RtlClearAllBits                              proto stdcall :DWORD
RtlClearBits                                 proto stdcall :DWORD, :DWORD, :DWORD
RtlCompareMemory                             proto stdcall :DWORD, :DWORD, :DWORD
RtlCompareMemoryUlong                        proto stdcall :DWORD, :DWORD, :DWORD
RtlCompareString                             proto stdcall :DWORD, :DWORD, :DWORD
RtlCompareUnicodeString                      proto stdcall :DWORD, :DWORD, :DWORD
RtlCompressBuffer                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCompressChunks                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlConvertLongToLargeInteger                 proto stdcall :DWORD
RtlConvertSidToUnicodeString                 proto stdcall :DWORD, :DWORD, :DWORD
RtlConvertUlongToLargeInteger                proto stdcall :DWORD
RtlCopyLuid                                  proto stdcall :DWORD, :DWORD
RtlCopyRangeList                             proto stdcall :DWORD, :DWORD
RtlCopySid                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlCopyString                                proto stdcall :DWORD, :DWORD
RtlCopyUnicodeString                         proto stdcall :DWORD, :DWORD
RtlCreateAcl                                 proto stdcall :DWORD, :DWORD, :DWORD
RtlCreateAtomTable                           proto stdcall :DWORD, :DWORD
RtlCreateHeap                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateRegistryKey                         proto stdcall :DWORD, :DWORD
RtlCreateSecurityDescriptor                  proto stdcall :DWORD, :DWORD
RtlCreateUnicodeString                       proto stdcall :DWORD, :DWORD
RtlCustomCPToUnicodeN                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDecompressBuffer                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDecompressChunks                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDecompressFragment                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDelete                                    proto stdcall :DWORD
RtlDeleteAce                                 proto stdcall :DWORD, :DWORD
RtlDeleteAtomFromAtomTable                   proto stdcall :DWORD, :DWORD
RtlDeleteElementGenericTable                 proto stdcall :DWORD, :DWORD
RtlDeleteNoSplay                             proto stdcall :DWORD, :DWORD
RtlDeleteOwnersRanges                        proto stdcall :DWORD, :DWORD
RtlDeleteRange                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDeleteRegistryValue                       proto stdcall :DWORD, :DWORD, :DWORD
RtlDescribeChunk                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDestroyAtomTable                          proto stdcall :DWORD
RtlDestroyHeap                               proto stdcall :DWORD
RtlDowncaseUnicodeString                     proto stdcall :DWORD, :DWORD, :DWORD
RtlEmptyAtomTable                            proto stdcall :DWORD, :DWORD
RtlEnlargedIntegerMultiply                   proto stdcall :DWORD, :DWORD
RtlEnlargedUnsignedDivide                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlEnlargedUnsignedMultiply                  proto stdcall :DWORD, :DWORD
RtlEnumerateGenericTable                     proto stdcall :DWORD, :DWORD
RtlEnumerateGenericTableWithoutSplaying      proto stdcall :DWORD, :DWORD
RtlEqualLuid                                 proto stdcall :DWORD, :DWORD
RtlEqualSid                                  proto stdcall :DWORD, :DWORD
RtlEqualString                               proto stdcall :DWORD, :DWORD, :DWORD
RtlEqualUnicodeString                        proto stdcall :DWORD, :DWORD, :DWORD
RtlExtendedIntegerMultiply                   proto stdcall :DWORD, :DWORD, :DWORD
RtlExtendedLargeIntegerDivide                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlExtendedMagicDivide                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlFillMemory                                proto stdcall :DWORD, :DWORD, :DWORD
RtlFillMemoryUlong                           proto stdcall :DWORD, :DWORD, :DWORD
RtlFindClearBits                             proto stdcall :DWORD, :DWORD, :DWORD
RtlFindClearBitsAndSet                       proto stdcall :DWORD, :DWORD, :DWORD
RtlFindClearRuns                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlFindFirstRunClear                         proto stdcall :DWORD, :DWORD
RtlFindLastBackwardRunClear                  proto stdcall :DWORD, :DWORD, :DWORD
RtlFindLeastSignificantBit                   proto stdcall :DWORD, :DWORD
RtlFindLongestRunClear                       proto stdcall :DWORD, :DWORD
RtlFindMessage                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlFindMostSignificantBit                    proto stdcall :DWORD, :DWORD
RtlFindNextForwardRunClear                   proto stdcall :DWORD, :DWORD, :DWORD
RtlFindRange                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlFindSetBits                               proto stdcall :DWORD, :DWORD, :DWORD
RtlFindSetBitsAndClear                       proto stdcall :DWORD, :DWORD, :DWORD
RtlFindUnicodePrefix                         proto stdcall :DWORD, :DWORD, :DWORD
RtlFormatCurrentUserKeyPath                  proto stdcall :DWORD
RtlFreeAnsiString                            proto stdcall :DWORD
RtlFreeHeap                                  proto stdcall :DWORD, :DWORD, :DWORD
RtlFreeOemString                             proto stdcall :DWORD
RtlFreeRangeList                             proto stdcall :DWORD
RtlFreeUnicodeString                         proto stdcall :DWORD
RtlGUIDFromString                            proto stdcall :DWORD, :DWORD
RtlGenerate8dot3Name                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlGetAce                                    proto stdcall :DWORD, :DWORD, :DWORD
RtlGetCallersAddress                         proto stdcall :DWORD, :DWORD
RtlGetCompressionWorkSpaceSize               proto stdcall :DWORD, :DWORD, :DWORD
RtlGetDaclSecurityDescriptor                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlGetDefaultCodePage                        proto stdcall :DWORD, :DWORD
RtlGetElementGenericTable                    proto stdcall :DWORD, :DWORD
RtlGetFirstRange                             proto stdcall :DWORD, :DWORD, :DWORD
RtlGetGroupSecurityDescriptor                proto stdcall :DWORD, :DWORD, :DWORD
RtlGetNextRange                              proto stdcall :DWORD, :DWORD, :DWORD
RtlGetNtGlobalFlags                          proto stdcall
RtlGetOwnerSecurityDescriptor                proto stdcall :DWORD, :DWORD, :DWORD
RtlGetSaclSecurityDescriptor                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlImageDirectoryEntryToData                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlImageNtHeader                             proto stdcall :DWORD
RtlInitAnsiString                            proto stdcall :DWORD, :DWORD
RtlInitCodePageTable                         proto stdcall :DWORD, :DWORD
RtlInitString                                proto stdcall :DWORD, :DWORD
RtlInitUnicodeString                         proto stdcall :DWORD, :DWORD
RtlInitializeBitMap                          proto stdcall :DWORD, :DWORD, :DWORD
RtlInitializeGenericTable                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlInitializeRangeList                       proto stdcall :DWORD
RtlInitializeSid                             proto stdcall :DWORD, :DWORD, :DWORD
RtlInitializeUnicodePrefix                   proto stdcall :DWORD
RtlInsertElementGenericTable                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlInsertElementGenericTableFull             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlInsertUnicodePrefix                       proto stdcall :DWORD, :DWORD, :DWORD
RtlIntegerToChar                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlIntegerToUnicodeString                    proto stdcall :DWORD, :DWORD, :DWORD
RtlInvertRangeList                           proto stdcall :DWORD, :DWORD
RtlIsGenericTableEmpty                       proto stdcall :DWORD
RtlIsNameLegalDOS8Dot3                       proto stdcall :DWORD, :DWORD, :DWORD
RtlIsRangeAvailable                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlIsValidOemCharacter                       proto stdcall :DWORD
RtlLargeIntegerAdd                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlLargeIntegerArithmeticShift               proto stdcall :DWORD, :DWORD, :DWORD
RtlLargeIntegerDivide                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlLargeIntegerNegate                        proto stdcall :DWORD, :DWORD
RtlLargeIntegerShiftLeft                     proto stdcall :DWORD, :DWORD, :DWORD
RtlLargeIntegerShiftRight                    proto stdcall :DWORD, :DWORD, :DWORD
RtlLargeIntegerSubtract                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlLengthRequiredSid                         proto stdcall :DWORD
RtlLengthSecurityDescriptor                  proto stdcall :DWORD
RtlLengthSid                                 proto stdcall :DWORD
RtlLookupAtomInAtomTable                     proto stdcall :DWORD, :DWORD, :DWORD
RtlLookupElementGenericTable                 proto stdcall :DWORD, :DWORD
RtlLookupElementGenericTableFull             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlMapGenericMask                            proto stdcall :DWORD, :DWORD
RtlMergeRangeLists                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlMoveMemory                                proto stdcall :DWORD, :DWORD, :DWORD
RtlMultiByteToUnicodeN                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlMultiByteToUnicodeSize                    proto stdcall :DWORD, :DWORD, :DWORD
RtlNextUnicodePrefix                         proto stdcall :DWORD, :DWORD
RtlNtStatusToDosError                        proto stdcall :DWORD
RtlNtStatusToDosErrorNoTeb                   proto stdcall :DWORD
RtlNumberGenericTableElements                proto stdcall :DWORD
RtlNumberOfClearBits                         proto stdcall :DWORD
RtlNumberOfSetBits                           proto stdcall :DWORD
RtlOemStringToCountedUnicodeString           proto stdcall :DWORD, :DWORD, :DWORD
RtlOemStringToUnicodeSize                    proto stdcall :DWORD
RtlOemStringToUnicodeString                  proto stdcall :DWORD, :DWORD, :DWORD
RtlOemToUnicodeN                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlPinAtomInAtomTable                        proto stdcall :DWORD, :DWORD
RtlPrefixString                              proto stdcall :DWORD, :DWORD, :DWORD
RtlPrefixUnicodeString                       proto stdcall :DWORD, :DWORD, :DWORD
RtlQueryAtomInAtomTable                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlQueryRegistryValues                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlQueryTimeZoneInformation                  proto stdcall :DWORD
RtlRaiseException                            proto stdcall :DWORD
RtlRandom                                    proto stdcall :DWORD
RtlRealPredecessor                           proto stdcall :DWORD
RtlRealSuccessor                             proto stdcall :DWORD
RtlRemoveUnicodePrefix                       proto stdcall :DWORD, :DWORD
RtlReserveChunk                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlSecondsSince1970ToTime                    proto stdcall :DWORD, :DWORD
RtlSecondsSince1980ToTime                    proto stdcall :DWORD, :DWORD
RtlSelfRelativeToAbsoluteSD2                 proto stdcall :DWORD, :DWORD
RtlSelfRelativeToAbsoluteSD                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlSetAllBits                                proto stdcall :DWORD
RtlSetBits                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlSetDaclSecurityDescriptor                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlSetGroupSecurityDescriptor                proto stdcall :DWORD, :DWORD, :DWORD
RtlSetOwnerSecurityDescriptor                proto stdcall :DWORD, :DWORD, :DWORD
RtlSetSaclSecurityDescriptor                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlSetTimeZoneInformation                    proto stdcall :DWORD
RtlSizeHeap                                  proto stdcall :DWORD, :DWORD, :DWORD
RtlSplay                                     proto stdcall :DWORD
RtlStringFromGUID                            proto stdcall :DWORD, :DWORD
RtlSubAuthorityCountSid                      proto stdcall :DWORD
RtlSubAuthoritySid                           proto stdcall :DWORD, :DWORD
RtlSubtreePredecessor                        proto stdcall :DWORD
RtlSubtreeSuccessor                          proto stdcall :DWORD
RtlTimeFieldsToTime                          proto stdcall :DWORD, :DWORD
RtlTimeToSecondsSince1970                    proto stdcall :DWORD, :DWORD
RtlTimeToSecondsSince1980                    proto stdcall :DWORD, :DWORD
RtlTimeToTimeFields                          proto stdcall :DWORD, :DWORD
RtlTraceDatabaseAdd                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlTraceDatabaseCreate                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlTraceDatabaseDestroy                      proto stdcall :DWORD
RtlTraceDatabaseEnumerate                    proto stdcall :DWORD, :DWORD, :DWORD
RtlTraceDatabaseFind                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlTraceDatabaseLock                         proto stdcall :DWORD
RtlTraceDatabaseUnlock                       proto stdcall :DWORD
RtlTraceDatabaseValidate                     proto stdcall :DWORD
RtlUnicodeStringToAnsiSize                   proto stdcall :DWORD
RtlUnicodeStringToAnsiString                 proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeStringToCountedOemString           proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeStringToInteger                    proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeStringToOemSize                    proto stdcall :DWORD
RtlUnicodeStringToOemString                  proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeToCustomCPN                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUnicodeToMultiByteN                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUnicodeToMultiByteSize                    proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeToOemN                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUnwind                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeChar                         proto stdcall :DWORD
RtlUpcaseUnicodeString                       proto stdcall :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeStringToAnsiString           proto stdcall :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeStringToCountedOemString     proto stdcall :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeStringToOemString            proto stdcall :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeToCustomCPN                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeToMultiByteN                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeToOemN                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUpperChar                                 proto stdcall :DWORD
RtlUpperString                               proto stdcall :DWORD, :DWORD
RtlValidRelativeSecurityDescriptor           proto stdcall :DWORD, :DWORD, :DWORD
RtlValidSecurityDescriptor                   proto stdcall :DWORD
RtlValidSid                                  proto stdcall :DWORD
RtlVolumeDeviceToDosName                     proto stdcall :DWORD, :DWORD
RtlWalkFrameChain                            proto stdcall :DWORD, :DWORD, :DWORD
RtlWriteRegistryValue                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlZeroHeap                                  proto stdcall :DWORD, :DWORD
RtlZeroMemory                                proto stdcall :DWORD, :DWORD
RtlxAnsiStringToUnicodeSize                  proto stdcall :DWORD
RtlxOemStringToUnicodeSize                   proto stdcall :DWORD
RtlxUnicodeStringToAnsiSize                  proto stdcall :DWORD
RtlxUnicodeStringToOemSize                   proto stdcall :DWORD
SeAccessCheck                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SeAppendPrivileges                           proto stdcall :DWORD, :DWORD
SeAssignSecurity                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SeAssignSecurityEx                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SeAuditingFileEvents                         proto stdcall :DWORD, :DWORD
SeAuditingFileOrGlobalEvents                 proto stdcall :DWORD, :DWORD, :DWORD
SeCaptureSecurityDescriptor                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SeCaptureSubjectContext                      proto stdcall :DWORD
SeCloseObjectAuditAlarm                      proto stdcall :DWORD, :DWORD, :DWORD
SeCreateAccessState                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
SeCreateClientSecurity                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
SeCreateClientSecurityFromSubjectContext     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
SeDeassignSecurity                           proto stdcall :DWORD
SeDeleteAccessState                          proto stdcall :DWORD
SeDeleteObjectAuditAlarm                     proto stdcall :DWORD, :DWORD
SeFreePrivileges                             proto stdcall :DWORD
SeImpersonateClient                          proto stdcall :DWORD, :DWORD
SeImpersonateClientEx                        proto stdcall :DWORD, :DWORD
SeLockSubjectContext                         proto stdcall :DWORD
SeMarkLogonSessionForTerminationNotification proto stdcall :DWORD
SeOpenObjectAuditAlarm                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SeOpenObjectForDeleteAuditAlarm              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SePrivilegeCheck                             proto stdcall :DWORD, :DWORD, :DWORD
SePrivilegeObjectAuditAlarm                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SeQueryAuthenticationIdToken                 proto stdcall :DWORD, :DWORD
SeQueryInformationToken                      proto stdcall :DWORD, :DWORD, :DWORD
SeQuerySecurityDescriptorInfo                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
SeQuerySessionIdToken                        proto stdcall :DWORD, :DWORD
SeRegisterLogonSessionTerminatedRoutine      proto stdcall :DWORD
SeReleaseSecurityDescriptor                  proto stdcall :DWORD, :DWORD, :DWORD
SeReleaseSubjectContext                      proto stdcall :DWORD
SeSetAccessStateGenericMapping               proto stdcall :DWORD, :DWORD
SeSetSecurityDescriptorInfo                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SeSetSecurityDescriptorInfoEx                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SeSinglePrivilegeCheck                       proto stdcall :DWORD, :DWORD, :DWORD
SeTokenImpersonationLevel                    proto stdcall :DWORD
SeTokenIsAdmin                               proto stdcall :DWORD
SeTokenIsRestricted                          proto stdcall :DWORD
SeTokenType                                  proto stdcall :DWORD
SeUnlockSubjectContext                       proto stdcall :DWORD
SeUnregisterLogonSessionTerminatedRoutine    proto stdcall :DWORD
SeValidSecurityDescriptor                    proto stdcall :DWORD, :DWORD
WRITE_REGISTER_BUFFER_UCHAR                  proto stdcall :DWORD, :DWORD, :DWORD
WRITE_REGISTER_BUFFER_ULONG                  proto stdcall :DWORD, :DWORD, :DWORD
WRITE_REGISTER_BUFFER_USHORT                 proto stdcall :DWORD, :DWORD, :DWORD
WRITE_REGISTER_UCHAR                         proto stdcall :DWORD, :DWORD
WRITE_REGISTER_ULONG                         proto stdcall :DWORD, :DWORD
WRITE_REGISTER_USHORT                        proto stdcall :DWORD, :DWORD
ZwAccessCheckAndAuditAlarm                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAdjustPrivilegesToken                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAlertThread                                proto stdcall :DWORD
ZwAllocateVirtualMemory                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCancelIoFile                               proto stdcall :DWORD, :DWORD
ZwCancelTimer                                proto stdcall :DWORD, :DWORD
ZwClearEvent                                 proto stdcall :DWORD
ZwClose                                      proto stdcall :DWORD
ZwCloseObjectAuditAlarm                      proto stdcall :DWORD, :DWORD, :DWORD
ZwConnectPort                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateDirectoryObject                      proto stdcall :DWORD, :DWORD, :DWORD
ZwCreateEvent                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateFile                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateKey                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateSection                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateSymbolicLinkObject                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateTimer                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwDeleteFile                                 proto stdcall :DWORD
ZwDeleteKey                                  proto stdcall :DWORD
ZwDeleteValueKey                             proto stdcall :DWORD, :DWORD
ZwDeviceIoControlFile                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwDisplayString                              proto stdcall :DWORD
ZwDuplicateObject                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwDuplicateToken                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwEnumerateKey                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwEnumerateValueKey                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwFlushInstructionCache                      proto stdcall :DWORD, :DWORD, :DWORD
ZwFlushKey                                   proto stdcall :DWORD
ZwFlushVirtualMemory                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwFreeVirtualMemory                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwFsControlFile                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwInitiatePowerAction                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwLoadDriver                                 proto stdcall :DWORD
ZwLoadKey                                    proto stdcall :DWORD, :DWORD
ZwMakeTemporaryObject                        proto stdcall :DWORD
ZwMapViewOfSection                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwNotifyChangeKey                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenDirectoryObject                        proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenEvent                                  proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenFile                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenKey                                    proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenProcess                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenProcessToken                           proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenSection                                proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenSymbolicLinkObject                     proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenThread                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenThreadToken                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenTimer                                  proto stdcall :DWORD, :DWORD, :DWORD
ZwPowerInformation                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwPulseEvent                                 proto stdcall :DWORD, :DWORD
ZwQueryDefaultLocale                         proto stdcall :DWORD, :DWORD
ZwQueryDefaultUILanguage                     proto stdcall :DWORD
ZwQueryDirectoryFile                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryDirectoryObject                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryEaFile                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationFile                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationProcess                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationToken                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInstallUILanguage                     proto stdcall :DWORD
ZwQueryKey                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryObject                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySection                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySecurityObject                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySymbolicLinkObject                    proto stdcall :DWORD, :DWORD, :DWORD
ZwQuerySystemInformation                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryValueKey                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryVolumeInformationFile                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwReadFile                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwReplaceKey                                 proto stdcall :DWORD, :DWORD, :DWORD
ZwRequestWaitReplyPort                       proto stdcall :DWORD, :DWORD, :DWORD
ZwResetEvent                                 proto stdcall :DWORD, :DWORD
ZwRestoreKey                                 proto stdcall :DWORD, :DWORD, :DWORD
ZwSaveKey                                    proto stdcall :DWORD, :DWORD
ZwSetDefaultLocale                           proto stdcall :DWORD, :DWORD
ZwSetDefaultUILanguage                       proto stdcall :DWORD
ZwSetEaFile                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetEvent                                   proto stdcall :DWORD, :DWORD
ZwSetInformationFile                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationObject                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationProcess                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationThread                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetSecurityObject                          proto stdcall :DWORD, :DWORD, :DWORD
ZwSetSystemInformation                       proto stdcall :DWORD, :DWORD, :DWORD
ZwSetSystemTime                              proto stdcall :DWORD, :DWORD
ZwSetTimer                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSetValueKey                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSetVolumeInformationFile                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwTerminateProcess                           proto stdcall :DWORD, :DWORD
ZwUnloadDriver                               proto stdcall :DWORD
ZwUnloadKey                                  proto stdcall :DWORD
ZwUnmapViewOfSection                         proto stdcall :DWORD, :DWORD
ZwWaitForMultipleObjects                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwWaitForSingleObject                        proto stdcall :DWORD, :DWORD, :DWORD
ZwWriteFile                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwYieldExecution                             proto stdcall

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                             CDECL                                              ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_abnormal_termination PROTO C
_alldiv PROTO C :DWORD, :DWORD, :DWORD, :DWORD	; ???!!!
_allmul PROTO C :DWORD, :DWORD, :DWORD, :DWORD	; ???!!!
_allrem PROTO C :DWORD, :DWORD, :DWORD, :DWORD	; ???!!!
_allshl PROTO C	; ???!!!
_allshr PROTO C	; ???!!!
_aulldiv PROTO C :DWORD, :DWORD, :DWORD, :DWORD	; ???!!!
_aullrem PROTO C :DWORD, :DWORD, :DWORD, :DWORD	; ???!!!
_aullshr PROTO C	; ???!!!
;_except_handler2 PROTO C
;_except_handler3 PROTO C
;_global_unwind2 PROTO C :DWORD
_itoa PROTO C :DWORD, :DWORD, :DWORD
_itow PROTO C :DWORD, :DWORD, :DWORD
;_local_unwind2 PROTO C :DWORD, :DWORD
_purecall PROTO C
_snprintf PROTO C :DWORD, :DWORD, :DWORD, :VARARG
_snwprintf PROTO C :DWORD, :DWORD, :DWORD, :VARARG
_stricmp PROTO C :DWORD, :DWORD
_strlwr PROTO C :DWORD
_strnicmp PROTO C :DWORD, :DWORD, :DWORD
_strnset PROTO C :DWORD, :DWORD, :DWORD
_strrev PROTO C :DWORD
_strset PROTO C :DWORD, :DWORD
_strupr PROTO C :DWORD
_vsnprintf PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_wcsicmp PROTO C :DWORD, :DWORD
_wcslwr PROTO C :DWORD
_wcsnicmp PROTO C :DWORD, :DWORD, :DWORD	
_wcsnset PROTO C :DWORD, :DWORD, :DWORD	
_wcsrev PROTO C :DWORD
_wcsupr PROTO C :DWORD
atoi PROTO C :DWORD
atol PROTO C :DWORD
isdigit PROTO C :DWORD
islower PROTO C :DWORD
isprint PROTO C :DWORD
isspace PROTO C :DWORD
isupper PROTO C :DWORD
isxdigit PROTO C :DWORD
mbstowcs PROTO C :DWORD, :DWORD, :DWORD
mbtowc PROTO C :DWORD, :DWORD, :DWORD
memchr PROTO C :DWORD, :DWORD, :DWORD
memcpy PROTO C :DWORD, :DWORD, :DWORD
memmove PROTO C :DWORD, :DWORD, :DWORD
memset PROTO C :DWORD, :DWORD, :DWORD
qsort PROTO C :DWORD, :DWORD, :DWORD, :DWORD
rand PROTO C
sprintf PROTO C :DWORD, :DWORD, :VARARG
srand PROTO C :DWORD
strcat PROTO C :DWORD, :DWORD
strchr PROTO C :DWORD, :DWORD
strcmp PROTO C :DWORD, :DWORD
strcpy PROTO C :DWORD, :DWORD
strlen PROTO C :DWORD
strncat PROTO C :DWORD, :DWORD, :DWORD	
strncmp PROTO C :DWORD, :DWORD, :DWORD	
strncpy PROTO C :DWORD, :DWORD, :DWORD	
strrchr PROTO C :DWORD, :DWORD
strspn PROTO C :DWORD, :DWORD
strstr PROTO C :DWORD, :DWORD
swprintf PROTO C :DWORD, :DWORD, :VARARG
tolower PROTO C :DWORD
toupper PROTO C :DWORD
towlower PROTO C :DWORD
towupper PROTO C :DWORD
vsprintf PROTO C :DWORD, :DWORD, :DWORD
wcscat PROTO C :DWORD, :DWORD
wcschr PROTO C :DWORD, :DWORD
wcscmp PROTO C :DWORD, :DWORD
wcscpy PROTO C :DWORD, :DWORD
wcscspn PROTO C :DWORD, :DWORD
wcslen PROTO C :DWORD
wcsncat PROTO C :DWORD, :DWORD, :DWORD
wcsncmp PROTO C :DWORD, :DWORD, :DWORD
wcsncpy PROTO C :DWORD, :DWORD, :DWORD
wcsrchr PROTO C :DWORD, :DWORD
wcsspn PROTO C :DWORD, :DWORD
wcsstr PROTO C :DWORD, :DWORD
wcstombs PROTO C :DWORD, :DWORD, :DWORD
wctomb PROTO C :DWORD, :DWORD

; Exported variables
SeSystemDefaultDacl PROTO C
SePublicDefaultDacl PROTO C
PsThreadType PROTO C
PsProcessType PROTO C
PsJobType PROTO C
PsInitialSystemProcess PROTO C
NtGlobalFlag PROTO C
NtBuildNumber PROTO C
NlsOemLeadByteInfo PROTO C
NlsOemCodePage PROTO C
NlsMbOemCodePageTag PROTO C
NlsMbCodePageTag PROTO C
NlsLeadByteInfo PROTO C
NlsAnsiCodePage PROTO C
MmUserProbeAddress PROTO C
MmSystemRangeStart PROTO C
MmSectionObjectType PROTO C
MmHighestUserAddress PROTO C
Mm64BitPhysicalAddress PROTO C
LpcPortObjectType PROTO C
KiUnexpectedInterrupt PROTO C
KiEnableTimerWatchdog PROTO C
KiBugCheckData PROTO C
KeTickCount PROTO C
KeServiceDescriptorTable PROTO C
KeSaveStateForHibernate PROTO C
KeNumberProcessors PROTO C
KeLoaderBlock PROTO C
KeIcacheFlushCount PROTO C
KeI386MachineType PROTO C
KeDcacheFlushCount PROTO C
KdEnteredDebugger PROTO C
KdDebuggerNotPresent PROTO C
KdDebuggerEnabled PROTO C
IoWriteTransferCount PROTO C
IoWriteOperationCount PROTO C
IoStatisticsLock PROTO C
IoReadTransferCount PROTO C
IoReadOperationCount PROTO C
IoFileObjectType PROTO C
IoDriverObjectType PROTO C
IoDeviceObjectType PROTO C
IoDeviceHandlerObjectType PROTO C
IoDeviceHandlerObjectSize PROTO C
IoAdapterObjectType PROTO C
InitSafeBootMode PROTO C
HalPrivateDispatchTable PROTO C
HalDispatchTable PROTO C
FsRtlLegalAnsiCharacterArray PROTO C
ExWindowStationObjectType PROTO C
ExSemaphoreObjectType PROTO C
ExEventObjectType PROTO C
ExDesktopObjectType PROTO C
DbgPrintReturnControlC PROTO C
DbgPrint PROTO C :DWORD, :VARARG
CcFastReadWait PROTO C
CcFastReadNotPossible PROTO C
CcFastMdlReadWait PROTO C
_except_list PROTO C

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                           FASTCALL                                             ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

externdef syscall @ExAcquireFastMutexUnsafe@4:proc
ExAcquireFastMutexUnsafe textequ <@ExAcquireFastMutexUnsafe@4>

externdef syscall @ExInterlockedAddLargeStatistic@8:proc
ExInterlockedAddLargeStatistic textequ <@ExInterlockedAddLargeStatistic@8>

externdef syscall @ExInterlockedCompareExchange64@16:proc
ExInterlockedCompareExchange64 textequ <@ExInterlockedCompareExchange64@16>

externdef syscall @ExInterlockedFlushSList@4:proc
ExInterlockedFlushSList textequ <@ExInterlockedFlushSList@4>

externdef syscall @ExInterlockedPopEntrySList@8:proc
ExInterlockedPopEntrySList textequ <@ExInterlockedPopEntrySList@8>

externdef syscall @ExInterlockedPushEntrySList@12:proc
ExInterlockedPushEntrySList textequ <@ExInterlockedPushEntrySList@12>

externdef syscall @ExReleaseFastMutexUnsafe@4:proc
ExReleaseFastMutexUnsafe textequ <@ExReleaseFastMutexUnsafe@4>

externdef syscall @ExReleaseResourceLite@4:proc
ExReleaseResourceLite textequ <@ExReleaseResourceLite@4>

externdef syscall @ExfInterlockedAddUlong@12:proc
ExfInterlockedAddUlong textequ <@ExfInterlockedAddUlong@12>

externdef syscall @ExfInterlockedInsertHeadList@12:proc
ExfInterlockedInsertHeadList textequ <@ExfInterlockedInsertHeadList@12>

externdef syscall @ExfInterlockedInsertTailList@12:proc
ExfInterlockedInsertTailList textequ <@ExfInterlockedInsertTailList@12>

externdef syscall @ExfInterlockedPopEntryList@8:proc
ExfInterlockedPopEntryList textequ <@ExfInterlockedPopEntryList@8>

externdef syscall @ExfInterlockedPushEntryList@12:proc
ExfInterlockedPushEntryList textequ <@ExfInterlockedPushEntryList@12>

externdef syscall @ExfInterlockedRemoveHeadList@8:proc
ExfInterlockedRemoveHeadList textequ <@ExfInterlockedRemoveHeadList@8>

externdef syscall @Exfi386InterlockedDecrementLong@4:proc
Exfi386InterlockedDecrementLong textequ <@Exfi386InterlockedDecrementLong@4>

externdef syscall @Exfi386InterlockedExchangeUlong@8:proc
Exfi386InterlockedExchangeUlong textequ <@Exfi386InterlockedExchangeUlong@8>

externdef syscall @Exfi386InterlockedIncrementLong@4:proc
Exfi386InterlockedIncrementLong textequ <@Exfi386InterlockedIncrementLong@4>

externdef syscall @InterlockedCompareExchange@12:proc
InterlockedCompareExchange textequ <@InterlockedCompareExchange@12>

externdef syscall @InterlockedDecrement@4:proc
InterlockedDecrement textequ <@InterlockedDecrement@4>

externdef syscall @InterlockedExchange@8:proc
InterlockedExchange textequ <@InterlockedExchange@8>

externdef syscall @InterlockedExchangeAdd@8:proc
InterlockedExchangeAdd textequ <@InterlockedExchangeAdd@8>

externdef syscall @InterlockedIncrement@4:proc
InterlockedIncrement textequ <@InterlockedIncrement@4>

externdef syscall @IofCallDriver@8:proc
IofCallDriver textequ <@IofCallDriver@8>

externdef syscall @IofCompleteRequest@8:proc
IofCompleteRequest textequ <@IofCompleteRequest@8>

externdef syscall @KeSetSwapContextNotifyRoutine@4:proc
KeSetSwapContextNotifyRoutine textequ <@KeSetSwapContextNotifyRoutine@4>

externdef syscall @KeSetThreadSelectNotifyRoutine@4:proc
KeSetThreadSelectNotifyRoutine textequ <@KeSetThreadSelectNotifyRoutine@4>

externdef syscall @KeSetTimeUpdateNotifyRoutine@4:proc
KeSetTimeUpdateNotifyRoutine textequ <@KeSetTimeUpdateNotifyRoutine@4>

externdef syscall @KefAcquireSpinLockAtDpcLevel@4:proc
KefAcquireSpinLockAtDpcLevel textequ <@KefAcquireSpinLockAtDpcLevel@4>

externdef syscall @KefReleaseSpinLockFromDpcLevel@4:proc
KefReleaseSpinLockFromDpcLevel textequ <@KefReleaseSpinLockFromDpcLevel@4>

externdef syscall @KiAcquireSpinLock@4:proc
KiAcquireSpinLock textequ <@KiAcquireSpinLock@4>

externdef syscall @KiReleaseSpinLock@4:proc
KiReleaseSpinLock textequ <@KiReleaseSpinLock@4>

externdef syscall @ObfDereferenceObject@4:proc
ObfDereferenceObject textequ <@ObfDereferenceObject@4>

externdef syscall @ObfReferenceObject@4:proc
ObfReferenceObject textequ <@ObfReferenceObject@4>

externdef syscall @RtlUlongByteSwap@4:proc
RtlUlongByteSwap textequ <@RtlUlongByteSwap@4>

externdef syscall @RtlUlonglongByteSwap@4:proc
RtlUlonglongByteSwap textequ <@RtlUlonglongByteSwap@4>

externdef syscall @RtlUshortByteSwap@4:proc
RtlUshortByteSwap textequ <@RtlUshortByteSwap@4>
