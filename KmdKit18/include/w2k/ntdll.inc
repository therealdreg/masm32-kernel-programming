
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:           This file was created on 16 Jan 2005 with Protogen v1.0 by Four-F.                   ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; __cdecls was fixed by hand !!!

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                            STDCALL                                             ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CsrAllocateCaptureBuffer                           proto stdcall :DWORD, :DWORD
CsrAllocateMessagePointer                          proto stdcall :DWORD, :DWORD, :DWORD
CsrCaptureMessageBuffer                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CsrCaptureMessageString                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CsrCaptureTimeout                                  proto stdcall :DWORD, :DWORD
CsrClientCallServer                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CsrClientConnectToServer                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CsrFreeCaptureBuffer                               proto stdcall :DWORD
CsrIdentifyAlertableThread                         proto stdcall
CsrNewThread                                       proto stdcall
CsrProbeForRead                                    proto stdcall :DWORD, :DWORD, :DWORD
CsrProbeForWrite                                   proto stdcall :DWORD, :DWORD, :DWORD
CsrSetPriorityClass                                proto stdcall :DWORD, :DWORD
DbgBreakPoint                                      proto stdcall
DbgPrompt                                          proto stdcall :DWORD, :DWORD, :DWORD
DbgSsHandleKmApiMsg                                proto stdcall :DWORD, :DWORD
DbgSsInitialize                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
DbgUiConnectToDbg                                  proto stdcall
DbgUiContinue                                      proto stdcall :DWORD, :DWORD
DbgUiWaitStateChange                               proto stdcall :DWORD, :DWORD
DbgUserBreakPoint                                  proto stdcall
KiRaiseUserExceptionDispatcher                     proto stdcall
KiUserApcDispatcher                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
KiUserCallbackDispatcher                           proto stdcall :DWORD, :DWORD, :DWORD
KiUserExceptionDispatcher                          proto stdcall :DWORD, :DWORD
LdrAccessResource                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrAlternateResourcesEnabled                       proto stdcall
LdrDisableThreadCalloutsForDll                     proto stdcall :DWORD
LdrEnumResources                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
LdrFindEntryForAddress                             proto stdcall :DWORD, :DWORD
LdrFindResourceDirectory_U                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrFindResource_U                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrFlushAlternateResourceModules                   proto stdcall
LdrGetDllHandle                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrGetProcedureAddress                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrInitializeThunk                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrLoadAlternateResourceModule                     proto stdcall :DWORD, :DWORD
LdrLoadDll                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrProcessRelocationBlock                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
LdrQueryImageFileExecutionOptions                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
LdrQueryProcessModuleInformation                   proto stdcall :DWORD, :DWORD, :DWORD
LdrShutdownProcess                                 proto stdcall
LdrShutdownThread                                  proto stdcall
LdrUnloadAlternateResourceModule                   proto stdcall :DWORD
LdrUnloadDll                                       proto stdcall :DWORD
LdrVerifyImageMatchesChecksum                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtAcceptConnectPort                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAccessCheck                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAccessCheckAndAuditAlarm                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAccessCheckByType                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAccessCheckByTypeAndAuditAlarm                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAccessCheckByTypeResultList                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAccessCheckByTypeResultListAndAuditAlarm         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAccessCheckByTypeResultListAndAuditAlarmByHandle proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAddAtom                                          proto stdcall :DWORD, :DWORD, :DWORD
NtAdjustGroupsToken                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAdjustPrivilegesToken                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAlertResumeThread                                proto stdcall :DWORD, :DWORD
NtAlertThread                                      proto stdcall :DWORD
NtAllocateLocallyUniqueId                          proto stdcall :DWORD
NtAllocateUserPhysicalPages                        proto stdcall :DWORD, :DWORD, :DWORD
NtAllocateUuids                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtAllocateVirtualMemory                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtAreMappedFilesTheSame                            proto stdcall :DWORD, :DWORD
NtAssignProcessToJobObject                         proto stdcall :DWORD, :DWORD
NtCallbackReturn                                   proto stdcall :DWORD, :DWORD, :DWORD
NtCancelDeviceWakeupRequest                        proto stdcall :DWORD
NtCancelIoFile                                     proto stdcall :DWORD, :DWORD
NtCancelTimer                                      proto stdcall :DWORD, :DWORD
NtClearEvent                                       proto stdcall :DWORD
NtClose                                            proto stdcall :DWORD
NtCloseObjectAuditAlarm                            proto stdcall :DWORD, :DWORD, :DWORD
NtCompleteConnectPort                              proto stdcall :DWORD
NtConnectPort                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtContinue                                         proto stdcall :DWORD, :DWORD
NtCreateChannel                                    proto stdcall :DWORD, :DWORD
NtCreateDirectoryObject                            proto stdcall :DWORD, :DWORD, :DWORD
NtCreateEvent                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateEventPair                                  proto stdcall :DWORD, :DWORD, :DWORD
NtCreateFile                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateIoCompletion                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtCreateJobObject                                  proto stdcall :DWORD, :DWORD, :DWORD
NtCreateKey                                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateMailslotFile                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateMutant                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtCreateNamedPipeFile                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreatePagingFile                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtCreatePort                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateProcess                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateProfile                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateSection                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateSemaphore                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateSymbolicLinkObject                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtCreateThread                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateTimer                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtCreateToken                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCreateWaitablePort                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtCurrentTeb                                       proto stdcall
NtDelayExecution                                   proto stdcall :DWORD, :DWORD
NtDeleteAtom                                       proto stdcall :DWORD
NtDeleteFile                                       proto stdcall :DWORD
NtDeleteKey                                        proto stdcall :DWORD
NtDeleteObjectAuditAlarm                           proto stdcall :DWORD, :DWORD, :DWORD
NtDeleteValueKey                                   proto stdcall :DWORD, :DWORD
NtDeviceIoControlFile                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtDisplayString                                    proto stdcall :DWORD
NtDuplicateObject                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtDuplicateToken                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtEnumerateKey                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtEnumerateValueKey                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtExtendSection                                    proto stdcall :DWORD, :DWORD
NtFilterToken                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtFindAtom                                         proto stdcall :DWORD, :DWORD, :DWORD
NtFlushBuffersFile                                 proto stdcall :DWORD, :DWORD
NtFlushInstructionCache                            proto stdcall :DWORD, :DWORD, :DWORD
NtFlushKey                                         proto stdcall :DWORD
NtFlushVirtualMemory                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtFlushWriteBuffer                                 proto stdcall
NtFreeUserPhysicalPages                            proto stdcall :DWORD, :DWORD, :DWORD
NtFreeVirtualMemory                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtFsControlFile                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtGetContextThread                                 proto stdcall :DWORD, :DWORD
NtGetDevicePowerState                              proto stdcall :DWORD, :DWORD
NtGetPlugPlayEvent                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtGetTickCount                                     proto stdcall
NtGetWriteWatch                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtImpersonateAnonymousToken                        proto stdcall :DWORD
NtImpersonateClientOfPort                          proto stdcall :DWORD, :DWORD
NtImpersonateThread                                proto stdcall :DWORD, :DWORD, :DWORD
NtInitializeRegistry                               proto stdcall :DWORD
NtInitiatePowerAction                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtIsSystemResumeAutomatic                          proto stdcall
NtListenChannel                                    proto stdcall :DWORD, :DWORD
NtListenPort                                       proto stdcall :DWORD, :DWORD
NtLoadDriver                                       proto stdcall :DWORD
NtLoadKey2                                         proto stdcall :DWORD, :DWORD, :DWORD
NtLoadKey                                          proto stdcall :DWORD, :DWORD
NtLockFile                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtLockVirtualMemory                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtMakeTemporaryObject                              proto stdcall :DWORD
NtMapUserPhysicalPages                             proto stdcall :DWORD, :DWORD, :DWORD
NtMapUserPhysicalPagesScatter                      proto stdcall :DWORD, :DWORD, :DWORD
NtMapViewOfSection                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtNotifyChangeDirectoryFile                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtNotifyChangeKey                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtNotifyChangeMultipleKeys                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtOpenChannel                                      proto stdcall :DWORD, :DWORD
NtOpenDirectoryObject                              proto stdcall :DWORD, :DWORD, :DWORD
NtOpenEvent                                        proto stdcall :DWORD, :DWORD, :DWORD
NtOpenEventPair                                    proto stdcall :DWORD, :DWORD, :DWORD
NtOpenFile                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtOpenIoCompletion                                 proto stdcall :DWORD, :DWORD, :DWORD
NtOpenJobObject                                    proto stdcall :DWORD, :DWORD, :DWORD
NtOpenKey                                          proto stdcall :DWORD, :DWORD, :DWORD
NtOpenMutant                                       proto stdcall :DWORD, :DWORD, :DWORD
NtOpenObjectAuditAlarm                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtOpenProcess                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtOpenProcessToken                                 proto stdcall :DWORD, :DWORD, :DWORD
NtOpenSection                                      proto stdcall :DWORD, :DWORD, :DWORD
NtOpenSemaphore                                    proto stdcall :DWORD, :DWORD, :DWORD
NtOpenSymbolicLinkObject                           proto stdcall :DWORD, :DWORD, :DWORD
NtOpenThread                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtOpenThreadToken                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtOpenTimer                                        proto stdcall :DWORD, :DWORD, :DWORD
NtPlugPlayControl                                  proto stdcall :DWORD, :DWORD, :DWORD
NtPowerInformation                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtPrivilegeCheck                                   proto stdcall :DWORD, :DWORD, :DWORD
NtPrivilegeObjectAuditAlarm                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtPrivilegedServiceAuditAlarm                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtProtectVirtualMemory                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtPulseEvent                                       proto stdcall :DWORD, :DWORD
NtQueryAttributesFile                              proto stdcall :DWORD, :DWORD
NtQueryDefaultLocale                               proto stdcall :DWORD, :DWORD
NtQueryDefaultUILanguage                           proto stdcall :DWORD
NtQueryDirectoryFile                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryDirectoryObject                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryEaFile                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryEvent                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryFullAttributesFile                          proto stdcall :DWORD, :DWORD
NtQueryInformationAtom                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationFile                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationJobObject                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationPort                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationProcess                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationThread                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInformationToken                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryInstallUILanguage                           proto stdcall :DWORD
NtQueryIntervalProfile                             proto stdcall :DWORD, :DWORD
NtQueryIoCompletion                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryKey                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryMultipleValueKey                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryMutant                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryObject                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryOpenSubKeys                                 proto stdcall :DWORD, :DWORD
NtQueryPerformanceCounter                          proto stdcall :DWORD, :DWORD
NtQueryQuotaInformationFile                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQuerySection                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQuerySecurityObject                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQuerySemaphore                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQuerySymbolicLinkObject                          proto stdcall :DWORD, :DWORD, :DWORD
NtQuerySystemEnvironmentValue                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtQuerySystemInformation                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtQuerySystemTime                                  proto stdcall :DWORD
NtQueryTimer                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryTimerResolution                             proto stdcall :DWORD, :DWORD, :DWORD
NtQueryValueKey                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryVirtualMemory                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueryVolumeInformationFile                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtQueueApcThread                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtRaiseException                                   proto stdcall :DWORD, :DWORD, :DWORD
NtRaiseHardError                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtReadFile                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtReadFileScatter                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtReadRequestData                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtReadVirtualMemory                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtRegisterThreadTerminatePort                      proto stdcall :DWORD
NtReleaseMutant                                    proto stdcall :DWORD, :DWORD
NtReleaseSemaphore                                 proto stdcall :DWORD, :DWORD, :DWORD
NtRemoveIoCompletion                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtReplaceKey                                       proto stdcall :DWORD, :DWORD, :DWORD
NtReplyPort                                        proto stdcall :DWORD, :DWORD
NtReplyWaitReceivePort                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtReplyWaitReceivePortEx                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtReplyWaitReplyPort                               proto stdcall :DWORD, :DWORD
NtReplyWaitSendChannel                             proto stdcall :DWORD, :DWORD, :DWORD
NtRequestDeviceWakeup                              proto stdcall :DWORD
NtRequestPort                                      proto stdcall :DWORD, :DWORD
NtRequestWaitReplyPort                             proto stdcall :DWORD, :DWORD, :DWORD
NtRequestWakeupLatency                             proto stdcall :DWORD
NtResetEvent                                       proto stdcall :DWORD, :DWORD
NtResetWriteWatch                                  proto stdcall :DWORD, :DWORD, :DWORD
NtRestoreKey                                       proto stdcall :DWORD, :DWORD, :DWORD
NtResumeThread                                     proto stdcall :DWORD, :DWORD
NtSaveKey                                          proto stdcall :DWORD, :DWORD
NtSaveMergedKeys                                   proto stdcall :DWORD, :DWORD, :DWORD
NtSecureConnectPort                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtSendWaitReplyChannel                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetContextChannel                                proto stdcall :DWORD
NtSetContextThread                                 proto stdcall :DWORD, :DWORD
NtSetDefaultHardErrorPort                          proto stdcall :DWORD
NtSetDefaultLocale                                 proto stdcall :DWORD, :DWORD
NtSetDefaultUILanguage                             proto stdcall :DWORD
NtSetEaFile                                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetEvent                                         proto stdcall :DWORD, :DWORD
NtSetHighEventPair                                 proto stdcall :DWORD
NtSetHighWaitLowEventPair                          proto stdcall :DWORD
NtSetInformationFile                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtSetInformationJobObject                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetInformationKey                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetInformationObject                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetInformationProcess                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetInformationThread                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetInformationToken                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetIntervalProfile                               proto stdcall :DWORD, :DWORD
NtSetIoCompletion                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtSetLdtEntries                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtSetLowEventPair                                  proto stdcall :DWORD
NtSetLowWaitHighEventPair                          proto stdcall :DWORD
NtSetQuotaInformationFile                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtSetSecurityObject                                proto stdcall :DWORD, :DWORD, :DWORD
NtSetSystemEnvironmentValue                        proto stdcall :DWORD, :DWORD
NtSetSystemInformation                             proto stdcall :DWORD, :DWORD, :DWORD
NtSetSystemPowerState                              proto stdcall :DWORD, :DWORD, :DWORD
NtSetSystemTime                                    proto stdcall :DWORD, :DWORD
NtSetThreadExecutionState                          proto stdcall :DWORD, :DWORD
NtSetTimer                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtSetTimerResolution                               proto stdcall :DWORD, :DWORD, :DWORD
NtSetUuidSeed                                      proto stdcall :DWORD
NtSetValueKey                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtSetVolumeInformationFile                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtShutdownSystem                                   proto stdcall :DWORD
NtSignalAndWaitForSingleObject                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtStartProfile                                     proto stdcall :DWORD
NtStopProfile                                      proto stdcall :DWORD
NtSuspendThread                                    proto stdcall :DWORD, :DWORD
NtSystemDebugControl                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtTerminateJobObject                               proto stdcall :DWORD, :DWORD
NtTerminateProcess                                 proto stdcall :DWORD, :DWORD
NtTerminateThread                                  proto stdcall :DWORD, :DWORD
NtTestAlert                                        proto stdcall
NtUnloadDriver                                     proto stdcall :DWORD
NtUnloadKey                                        proto stdcall :DWORD
NtUnlockFile                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtUnlockVirtualMemory                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
NtUnmapViewOfSection                               proto stdcall :DWORD, :DWORD
NtVdmControl                                       proto stdcall :DWORD, :DWORD
NtWaitForMultipleObjects                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtWaitForSingleObject                              proto stdcall :DWORD, :DWORD, :DWORD
NtWaitHighEventPair                                proto stdcall :DWORD
NtWaitLowEventPair                                 proto stdcall :DWORD
NtWriteFile                                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtWriteFileGather                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtWriteRequestData                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtWriteVirtualMemory                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
NtYieldExecution                                   proto stdcall
PfxFindPrefix                                      proto stdcall :DWORD, :DWORD
PfxInitialize                                      proto stdcall :DWORD
PfxInsertPrefix                                    proto stdcall :DWORD, :DWORD, :DWORD
PfxRemovePrefix                                    proto stdcall :DWORD, :DWORD
RtlAbortRXact                                      proto stdcall :DWORD
RtlAbsoluteToSelfRelativeSD                        proto stdcall :DWORD, :DWORD, :DWORD
RtlAcquirePebLock                                  proto stdcall
RtlAcquireResourceExclusive                        proto stdcall :DWORD, :DWORD
RtlAcquireResourceShared                           proto stdcall :DWORD, :DWORD
RtlAddAccessAllowedAce                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAccessAllowedAceEx                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAccessAllowedObjectAce                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAccessDeniedAce                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAccessDeniedAceEx                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAccessDeniedObjectAce                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAce                                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddActionToRXact                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAtomToAtomTable                              proto stdcall :DWORD, :DWORD, :DWORD
RtlAddAttributeActionToRXact                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAuditAccessAce                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAuditAccessAceEx                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddAuditAccessObjectAce                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddCompoundAce                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAddRange                                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAdjustPrivilege                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlAllocateAndInitializeSid                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAllocateHandle                                  proto stdcall :DWORD, :DWORD
RtlAllocateHeap                                    proto stdcall :DWORD, :DWORD, :DWORD
RtlAnsiCharToUnicodeChar                           proto stdcall :DWORD
RtlAnsiStringToUnicodeSize                         proto stdcall :DWORD
RtlAnsiStringToUnicodeString                       proto stdcall :DWORD, :DWORD, :DWORD
RtlAppendAsciizToString                            proto stdcall :DWORD, :DWORD
RtlAppendStringToString                            proto stdcall :DWORD, :DWORD
RtlAppendUnicodeStringToString                     proto stdcall :DWORD, :DWORD
RtlAppendUnicodeToString                           proto stdcall :DWORD, :DWORD
RtlApplyRXact                                      proto stdcall :DWORD
RtlApplyRXactNoFlush                               proto stdcall :DWORD
RtlAreAllAccessesGranted                           proto stdcall :DWORD, :DWORD
RtlAreAnyAccessesGranted                           proto stdcall :DWORD, :DWORD
RtlAreBitsClear                                    proto stdcall :DWORD, :DWORD, :DWORD
RtlAreBitsSet                                      proto stdcall :DWORD, :DWORD, :DWORD
RtlAssert                                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlCallbackLpcClient                               proto stdcall :DWORD, :DWORD, :DWORD
RtlCancelTimer                                     proto stdcall :DWORD, :DWORD
RtlCaptureStackBackTrace                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlCharToInteger                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlCheckForOrphanedCriticalSections                proto stdcall :DWORD
RtlCheckRegistryKey                                proto stdcall :DWORD, :DWORD
RtlClearAllBits                                    proto stdcall :DWORD
RtlClearBits                                       proto stdcall :DWORD, :DWORD, :DWORD
RtlCompactHeap                                     proto stdcall :DWORD, :DWORD
RtlCompareMemory                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlCompareMemoryUlong                              proto stdcall :DWORD, :DWORD, :DWORD
RtlCompareString                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlCompareUnicodeString                            proto stdcall :DWORD, :DWORD, :DWORD
RtlCompressBuffer                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlConsoleMultiByteToUnicodeN                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlConvertExclusiveToShared                        proto stdcall :DWORD
RtlConvertLongToLargeInteger                       proto stdcall :DWORD
RtlConvertSharedToExclusive                        proto stdcall :DWORD
RtlConvertSidToUnicodeString                       proto stdcall :DWORD, :DWORD, :DWORD
RtlConvertToAutoInheritSecurityObject              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlConvertUiListToApiList                          proto stdcall :DWORD, :DWORD, :DWORD
RtlConvertUlongToLargeInteger                      proto stdcall :DWORD
RtlCopyLuid                                        proto stdcall :DWORD, :DWORD
RtlCopyLuidAndAttributesArray                      proto stdcall :DWORD, :DWORD, :DWORD
RtlCopyRangeList                                   proto stdcall :DWORD, :DWORD
RtlCopySecurityDescriptor                          proto stdcall :DWORD, :DWORD
RtlCopySid                                         proto stdcall :DWORD, :DWORD, :DWORD
RtlCopySidAndAttributesArray                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCopyString                                      proto stdcall :DWORD, :DWORD
RtlCopyUnicodeString                               proto stdcall :DWORD, :DWORD
RtlCreateAcl                                       proto stdcall :DWORD, :DWORD, :DWORD
RtlCreateAndSetSD                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateAtomTable                                 proto stdcall :DWORD, :DWORD
RtlCreateEnvironment                               proto stdcall :DWORD, :DWORD
RtlCreateHeap                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateLpcServer                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateProcessParameters                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateQueryDebugBuffer                          proto stdcall :DWORD, :DWORD
RtlCreateRegistryKey                               proto stdcall :DWORD, :DWORD
RtlCreateSecurityDescriptor                        proto stdcall :DWORD, :DWORD
RtlCreateTagHeap                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateTimer                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateTimerQueue                                proto stdcall :DWORD
RtlCreateUnicodeString                             proto stdcall :DWORD, :DWORD
RtlCreateUnicodeStringFromAsciiz                   proto stdcall :DWORD, :DWORD
RtlCreateUserProcess                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateUserSecurityObject                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCreateUserThread                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCustomCPToUnicodeN                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlCutoverTimeToSystemTime                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlDeNormalizeProcessParams                        proto stdcall :DWORD
RtlDebugPrintTimes                                 proto stdcall
RtlDecompressBuffer                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDecompressFragment                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDefaultNpAcl                                    proto stdcall :DWORD
RtlDelete                                          proto stdcall :DWORD
RtlDeleteAce                                       proto stdcall :DWORD, :DWORD
RtlDeleteAtomFromAtomTable                         proto stdcall :DWORD, :DWORD
RtlDeleteCriticalSection                           proto stdcall :DWORD
RtlDeleteElementGenericTable                       proto stdcall :DWORD, :DWORD
RtlDeleteNoSplay                                   proto stdcall :DWORD, :DWORD
RtlDeleteOwnersRanges                              proto stdcall :DWORD, :DWORD
RtlDeleteRange                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDeleteRegistryValue                             proto stdcall :DWORD, :DWORD, :DWORD
RtlDeleteResource                                  proto stdcall :DWORD
RtlDeleteSecurityObject                            proto stdcall :DWORD
RtlDeleteTimer                                     proto stdcall :DWORD, :DWORD, :DWORD
RtlDeleteTimerQueue                                proto stdcall :DWORD
RtlDeleteTimerQueueEx                              proto stdcall :DWORD, :DWORD
RtlDeregisterWait                                  proto stdcall :DWORD
RtlDeregisterWaitEx                                proto stdcall :DWORD, :DWORD
RtlDestroyAtomTable                                proto stdcall :DWORD
RtlDestroyEnvironment                              proto stdcall :DWORD
RtlDestroyHandleTable                              proto stdcall :DWORD
RtlDestroyHeap                                     proto stdcall :DWORD
RtlDestroyProcessParameters                        proto stdcall :DWORD
RtlDestroyQueryDebugBuffer                         proto stdcall :DWORD
RtlDetermineDosPathNameType_U                      proto stdcall :DWORD
RtlDnsHostNameToComputerName                       proto stdcall :DWORD, :DWORD, :DWORD
RtlDoesFileExists_U                                proto stdcall :DWORD
RtlDosPathNameToNtPathName_U                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlDosSearchPath_U                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlDowncaseUnicodeString                           proto stdcall :DWORD, :DWORD, :DWORD
RtlDumpResource                                    proto stdcall :DWORD
RtlEmptyAtomTable                                  proto stdcall :DWORD, :DWORD
RtlEnableEarlyCriticalSectionEventCreation         proto stdcall
RtlEnlargedIntegerMultiply                         proto stdcall :DWORD, :DWORD
RtlEnlargedUnsignedDivide                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlEnlargedUnsignedMultiply                        proto stdcall :DWORD, :DWORD
RtlEnterCriticalSection                            proto stdcall :DWORD
RtlEnumProcessHeaps                                proto stdcall :DWORD, :DWORD
RtlEnumerateGenericTable                           proto stdcall :DWORD, :DWORD
RtlEnumerateGenericTableWithoutSplaying            proto stdcall :DWORD, :DWORD
RtlEqualComputerName                               proto stdcall :DWORD, :DWORD
RtlEqualDomainName                                 proto stdcall :DWORD, :DWORD
RtlEqualLuid                                       proto stdcall :DWORD, :DWORD
RtlEqualPrefixSid                                  proto stdcall :DWORD, :DWORD
RtlEqualSid                                        proto stdcall :DWORD, :DWORD
RtlEqualString                                     proto stdcall :DWORD, :DWORD, :DWORD
RtlEqualUnicodeString                              proto stdcall :DWORD, :DWORD, :DWORD
RtlEraseUnicodeString                              proto stdcall :DWORD
RtlExpandEnvironmentStrings_U                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlExtendHeap                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlExtendedIntegerMultiply                         proto stdcall :DWORD, :DWORD, :DWORD
RtlExtendedLargeIntegerDivide                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlExtendedMagicDivide                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlFillMemory                                      proto stdcall :DWORD, :DWORD, :DWORD
RtlFillMemoryUlong                                 proto stdcall :DWORD, :DWORD, :DWORD
RtlFindClearBits                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlFindClearBitsAndSet                             proto stdcall :DWORD, :DWORD, :DWORD
RtlFindLastBackwardRunClear                        proto stdcall :DWORD, :DWORD, :DWORD
RtlFindLeastSignificantBit                         proto stdcall :DWORD, :DWORD
RtlFindLongestRunClear                             proto stdcall :DWORD, :DWORD
RtlFindMessage                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlFindMostSignificantBit                          proto stdcall :DWORD, :DWORD
RtlFindNextForwardRunClear                         proto stdcall :DWORD, :DWORD, :DWORD
RtlFindRange                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlFindSetBits                                     proto stdcall :DWORD, :DWORD, :DWORD
RtlFindSetBitsAndClear                             proto stdcall :DWORD, :DWORD, :DWORD
RtlFirstFreeAce                                    proto stdcall :DWORD, :DWORD
RtlFormatCurrentUserKeyPath                        proto stdcall :DWORD
RtlFormatMessage                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlFreeAnsiString                                  proto stdcall :DWORD
RtlFreeHandle                                      proto stdcall :DWORD, :DWORD
RtlFreeHeap                                        proto stdcall :DWORD, :DWORD, :DWORD
RtlFreeOemString                                   proto stdcall :DWORD
RtlFreeRangeList                                   proto stdcall :DWORD
RtlFreeSid                                         proto stdcall :DWORD
RtlFreeUnicodeString                               proto stdcall :DWORD
RtlFreeUserThreadStack                             proto stdcall :DWORD, :DWORD
RtlGUIDFromString                                  proto stdcall :DWORD, :DWORD
RtlGenerate8dot3Name                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlGetAce                                          proto stdcall :DWORD, :DWORD, :DWORD
RtlGetCallersAddress                               proto stdcall :DWORD, :DWORD
RtlGetCompressionWorkSpaceSize                     proto stdcall :DWORD, :DWORD, :DWORD
RtlGetControlSecurityDescriptor                    proto stdcall :DWORD, :DWORD, :DWORD
RtlGetCurrentDirectory_U                           proto stdcall :DWORD, :DWORD
RtlGetDaclSecurityDescriptor                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlGetElementGenericTable                          proto stdcall :DWORD, :DWORD
RtlGetFirstRange                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlGetFullPathName_U                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlGetGroupSecurityDescriptor                      proto stdcall :DWORD, :DWORD, :DWORD
RtlGetLongestNtPathLength                          proto stdcall
RtlGetNextRange                                    proto stdcall :DWORD, :DWORD, :DWORD
RtlGetNtGlobalFlags                                proto stdcall
RtlGetNtProductType                                proto stdcall :DWORD
RtlGetOwnerSecurityDescriptor                      proto stdcall :DWORD, :DWORD, :DWORD
RtlGetProcessHeaps                                 proto stdcall :DWORD, :DWORD
RtlGetSaclSecurityDescriptor                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlGetSecurityDescriptorRMControl                  proto stdcall :DWORD, :DWORD
RtlGetUserInfoHeap                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlGetVersion                                      proto stdcall :DWORD
RtlIdentifierAuthoritySid                          proto stdcall :DWORD
RtlImageDirectoryEntryToData                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlImageNtHeader                                   proto stdcall :DWORD
RtlImageRvaToSection                               proto stdcall :DWORD, :DWORD, :DWORD
RtlImageRvaToVa                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlImpersonateLpcClient                            proto stdcall :DWORD, :DWORD
RtlImpersonateSelf                                 proto stdcall :DWORD
RtlInitAnsiString                                  proto stdcall :DWORD, :DWORD
RtlInitCodePageTable                               proto stdcall :DWORD, :DWORD
RtlInitNlsTables                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlInitString                                      proto stdcall :DWORD, :DWORD
RtlInitUnicodeString                               proto stdcall :DWORD, :DWORD
RtlInitializeAtomPackage                           proto stdcall :DWORD
RtlInitializeBitMap                                proto stdcall :DWORD, :DWORD, :DWORD
RtlInitializeContext                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlInitializeCriticalSection                       proto stdcall :DWORD
RtlInitializeCriticalSectionAndSpinCount           proto stdcall :DWORD, :DWORD
RtlInitializeGenericTable                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlInitializeHandleTable                           proto stdcall :DWORD, :DWORD, :DWORD
RtlInitializeRXact                                 proto stdcall :DWORD, :DWORD, :DWORD
RtlInitializeRangeList                             proto stdcall :DWORD
RtlInitializeResource                              proto stdcall :DWORD
RtlInitializeSid                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlInsertElementGenericTable                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlInt64ToUnicodeString                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlIntegerToChar                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlIntegerToUnicodeString                          proto stdcall :DWORD, :DWORD, :DWORD
RtlInvertRangeList                                 proto stdcall :DWORD, :DWORD
RtlIsDosDeviceName_U                               proto stdcall :DWORD
RtlIsGenericTableEmpty                             proto stdcall :DWORD
RtlIsNameLegalDOS8Dot3                             proto stdcall :DWORD, :DWORD, :DWORD
RtlIsRangeAvailable                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlIsTextUnicode                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlIsValidHandle                                   proto stdcall :DWORD, :DWORD
RtlIsValidIndexHandle                              proto stdcall :DWORD, :DWORD, :DWORD
RtlLargeIntegerAdd                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlLargeIntegerArithmeticShift                     proto stdcall :DWORD, :DWORD, :DWORD
RtlLargeIntegerDivide                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlLargeIntegerNegate                              proto stdcall :DWORD, :DWORD
RtlLargeIntegerShiftLeft                           proto stdcall :DWORD, :DWORD, :DWORD
RtlLargeIntegerShiftRight                          proto stdcall :DWORD, :DWORD, :DWORD
RtlLargeIntegerSubtract                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlLargeIntegerToChar                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlLeaveCriticalSection                            proto stdcall :DWORD
RtlLengthRequiredSid                               proto stdcall :DWORD
RtlLengthSecurityDescriptor                        proto stdcall :DWORD
RtlLengthSid                                       proto stdcall :DWORD
RtlLocalTimeToSystemTime                           proto stdcall :DWORD, :DWORD
RtlLockHeap                                        proto stdcall :DWORD
RtlLookupAtomInAtomTable                           proto stdcall :DWORD, :DWORD, :DWORD
RtlLookupElementGenericTable                       proto stdcall :DWORD, :DWORD
RtlMakeSelfRelativeSD                              proto stdcall :DWORD, :DWORD, :DWORD
RtlMapGenericMask                                  proto stdcall :DWORD, :DWORD
RtlMergeRangeLists                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlMoveMemory                                      proto stdcall :DWORD, :DWORD, :DWORD
RtlMultiByteToUnicodeN                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlMultiByteToUnicodeSize                          proto stdcall :DWORD, :DWORD, :DWORD
RtlNewInstanceSecurityObject                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlNewSecurityGrantedAccess                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlNewSecurityObject                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlNewSecurityObjectEx                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlNormalizeProcessParams                          proto stdcall :DWORD
RtlNtStatusToDosError                              proto stdcall :DWORD
RtlNumberGenericTableElements                      proto stdcall :DWORD
RtlNumberOfClearBits                               proto stdcall :DWORD
RtlNumberOfSetBits                                 proto stdcall :DWORD
RtlOemStringToUnicodeSize                          proto stdcall :DWORD
RtlOemStringToUnicodeString                        proto stdcall :DWORD, :DWORD, :DWORD
RtlOemToUnicodeN                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlOpenCurrentUser                                 proto stdcall :DWORD, :DWORD
RtlPcToFileHeader                                  proto stdcall :DWORD, :DWORD
RtlPinAtomInAtomTable                              proto stdcall :DWORD, :DWORD
RtlPrefixString                                    proto stdcall :DWORD, :DWORD, :DWORD
RtlPrefixUnicodeString                             proto stdcall :DWORD, :DWORD, :DWORD
RtlProtectHeap                                     proto stdcall :DWORD, :DWORD
RtlQueryAtomInAtomTable                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlQueryEnvironmentVariable_U                      proto stdcall :DWORD, :DWORD, :DWORD
RtlQueryInformationAcl                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlQueryProcessBackTraceInformation                proto stdcall :DWORD
RtlQueryProcessDebugInformation                    proto stdcall :DWORD, :DWORD, :DWORD
RtlQueryProcessHeapInformation                     proto stdcall :DWORD
RtlQueryProcessLockInformation                     proto stdcall :DWORD
RtlQueryRegistryValues                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlQuerySecurityObject                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlQueryTagHeap                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlQueryTimeZoneInformation                        proto stdcall :DWORD
RtlQueueWorkItem                                   proto stdcall :DWORD, :DWORD, :DWORD
RtlRaiseException                                  proto stdcall :DWORD
RtlRaiseStatus                                     proto stdcall :DWORD
RtlRandom                                          proto stdcall :DWORD
RtlReAllocateHeap                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlRealPredecessor                                 proto stdcall :DWORD
RtlRealSuccessor                                   proto stdcall :DWORD
RtlRegisterWait                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlReleasePebLock                                  proto stdcall
RtlReleaseResource                                 proto stdcall :DWORD
RtlRemoteCall                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlResetRtlTranslations                            proto stdcall :DWORD
RtlRunDecodeUnicodeString                          proto stdcall :DWORD, :DWORD
RtlRunEncodeUnicodeString                          proto stdcall :DWORD, :DWORD
RtlSecondsSince1970ToTime                          proto stdcall :DWORD, :DWORD
RtlSecondsSince1980ToTime                          proto stdcall :DWORD, :DWORD
RtlSelfRelativeToAbsoluteSD2                       proto stdcall :DWORD, :DWORD
RtlSelfRelativeToAbsoluteSD                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlSetAllBits                                      proto stdcall :DWORD
RtlSetAttributesSecurityDescriptor                 proto stdcall :DWORD, :DWORD, :DWORD
RtlSetBits                                         proto stdcall :DWORD, :DWORD, :DWORD
RtlSetControlSecurityDescriptor                    proto stdcall :DWORD, :DWORD, :DWORD
RtlSetCriticalSectionSpinCount                     proto stdcall :DWORD, :DWORD
RtlSetCurrentDirectory_U                           proto stdcall :DWORD
RtlSetCurrentEnvironment                           proto stdcall :DWORD, :DWORD
RtlSetDaclSecurityDescriptor                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlSetEnvironmentVariable                          proto stdcall :DWORD, :DWORD, :DWORD
RtlSetGroupSecurityDescriptor                      proto stdcall :DWORD, :DWORD, :DWORD
RtlSetInformationAcl                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlSetIoCompletionCallback                         proto stdcall :DWORD, :DWORD, :DWORD
RtlSetOwnerSecurityDescriptor                      proto stdcall :DWORD, :DWORD, :DWORD
RtlSetSaclSecurityDescriptor                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlSetSecurityDescriptorRMControl                  proto stdcall :DWORD, :DWORD
RtlSetSecurityObject                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlSetSecurityObjectEx                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlSetThreadPoolStartFunc                          proto stdcall :DWORD, :DWORD
RtlSetTimeZoneInformation                          proto stdcall :DWORD
RtlSetTimer                                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlSetUnicodeCallouts                              proto stdcall :DWORD
RtlSetUserFlagsHeap                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlSetUserValueHeap                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlShutdownLpcServer                               proto stdcall :DWORD
RtlSizeHeap                                        proto stdcall :DWORD, :DWORD, :DWORD
RtlSplay                                           proto stdcall :DWORD
RtlStartRXact                                      proto stdcall :DWORD
RtlStringFromGUID                                  proto stdcall :DWORD, :DWORD
RtlSubAuthorityCountSid                            proto stdcall :DWORD
RtlSubAuthoritySid                                 proto stdcall :DWORD, :DWORD
RtlSubtreePredecessor                              proto stdcall :DWORD
RtlSubtreeSuccessor                                proto stdcall :DWORD
RtlSystemTimeToLocalTime                           proto stdcall :DWORD, :DWORD
RtlTimeFieldsToTime                                proto stdcall :DWORD, :DWORD
RtlTimeToElapsedTimeFields                         proto stdcall :DWORD, :DWORD
RtlTimeToSecondsSince1970                          proto stdcall :DWORD, :DWORD
RtlTimeToSecondsSince1980                          proto stdcall :DWORD, :DWORD
RtlTimeToTimeFields                                proto stdcall :DWORD, :DWORD
RtlTraceDatabaseAdd                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlTraceDatabaseCreate                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlTraceDatabaseDestroy                            proto stdcall :DWORD
RtlTraceDatabaseEnumerate                          proto stdcall :DWORD, :DWORD, :DWORD
RtlTraceDatabaseFind                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlTraceDatabaseLock                               proto stdcall :DWORD
RtlTraceDatabaseUnlock                             proto stdcall :DWORD
RtlTraceDatabaseValidate                           proto stdcall :DWORD
RtlTryEnterCriticalSection                         proto stdcall :DWORD
RtlUnicodeStringToAnsiSize                         proto stdcall :DWORD
RtlUnicodeStringToAnsiString                       proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeStringToCountedOemString                 proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeStringToInteger                          proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeStringToOemSize                          proto stdcall :DWORD
RtlUnicodeStringToOemString                        proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeToCustomCPN                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUnicodeToMultiByteN                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUnicodeToMultiByteSize                          proto stdcall :DWORD, :DWORD, :DWORD
RtlUnicodeToOemN                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUniform                                         proto stdcall :DWORD
RtlUnlockHeap                                      proto stdcall :DWORD
RtlUnwind                                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeChar                               proto stdcall :DWORD
RtlUpcaseUnicodeString                             proto stdcall :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeStringToAnsiString                 proto stdcall :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeStringToCountedOemString           proto stdcall :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeStringToOemString                  proto stdcall :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeToCustomCPN                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeToMultiByteN                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeToOemN                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUpdateTimer                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlUpperChar                                       proto stdcall :DWORD
RtlUpperString                                     proto stdcall :DWORD, :DWORD
RtlUsageHeap                                       proto stdcall :DWORD, :DWORD, :DWORD
RtlValidAcl                                        proto stdcall :DWORD
RtlValidRelativeSecurityDescriptor                 proto stdcall :DWORD, :DWORD, :DWORD
RtlValidSecurityDescriptor                         proto stdcall :DWORD
RtlValidSid                                        proto stdcall :DWORD
RtlValidateHeap                                    proto stdcall :DWORD, :DWORD, :DWORD
RtlValidateProcessHeaps                            proto stdcall
RtlVerifyVersionInfo                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlWalkFrameChain                                  proto stdcall :DWORD, :DWORD, :DWORD
RtlWalkHeap                                        proto stdcall :DWORD, :DWORD
RtlWriteRegistryValue                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlZeroHeap                                        proto stdcall :DWORD, :DWORD
RtlZeroMemory                                      proto stdcall :DWORD, :DWORD
RtlpNtCreateKey                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlpNtEnumerateSubKey                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlpNtMakeTemporaryKey                             proto stdcall :DWORD
RtlpNtOpenKey                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlpNtQueryValueKey                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlpNtSetValueKey                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlpUnWaitCriticalSection                          proto stdcall :DWORD
RtlpWaitForCriticalSection                         proto stdcall :DWORD
RtlxAnsiStringToUnicodeSize                        proto stdcall :DWORD
RtlxOemStringToUnicodeSize                         proto stdcall :DWORD
RtlxUnicodeStringToAnsiSize                        proto stdcall :DWORD
RtlxUnicodeStringToOemSize                         proto stdcall :DWORD
VerSetConditionMask                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwAcceptConnectPort                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAccessCheck                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAccessCheckAndAuditAlarm                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAccessCheckByType                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAccessCheckByTypeAndAuditAlarm                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAccessCheckByTypeResultList                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAccessCheckByTypeResultListAndAuditAlarm         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAccessCheckByTypeResultListAndAuditAlarmByHandle proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAddAtom                                          proto stdcall :DWORD, :DWORD, :DWORD
ZwAdjustGroupsToken                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAdjustPrivilegesToken                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAlertResumeThread                                proto stdcall :DWORD, :DWORD
ZwAlertThread                                      proto stdcall :DWORD
ZwAllocateLocallyUniqueId                          proto stdcall :DWORD
ZwAllocateUserPhysicalPages                        proto stdcall :DWORD, :DWORD, :DWORD
ZwAllocateUuids                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwAllocateVirtualMemory                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwAreMappedFilesTheSame                            proto stdcall :DWORD, :DWORD
ZwAssignProcessToJobObject                         proto stdcall :DWORD, :DWORD
ZwCallbackReturn                                   proto stdcall :DWORD, :DWORD, :DWORD
ZwCancelDeviceWakeupRequest                        proto stdcall :DWORD
ZwCancelIoFile                                     proto stdcall :DWORD, :DWORD
ZwCancelTimer                                      proto stdcall :DWORD, :DWORD
ZwClearEvent                                       proto stdcall :DWORD
ZwClose                                            proto stdcall :DWORD
ZwCloseObjectAuditAlarm                            proto stdcall :DWORD, :DWORD, :DWORD
ZwCompleteConnectPort                              proto stdcall :DWORD
ZwConnectPort                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwContinue                                         proto stdcall :DWORD, :DWORD
ZwCreateChannel                                    proto stdcall :DWORD, :DWORD
ZwCreateDirectoryObject                            proto stdcall :DWORD, :DWORD, :DWORD
ZwCreateEvent                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateEventPair                                  proto stdcall :DWORD, :DWORD, :DWORD
ZwCreateFile                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateIoCompletion                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateJobObject                                  proto stdcall :DWORD, :DWORD, :DWORD
ZwCreateKey                                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateMailslotFile                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateMutant                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateNamedPipeFile                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreatePagingFile                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwCreatePort                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateProcess                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateProfile                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateSection                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateSemaphore                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateSymbolicLinkObject                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateThread                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateTimer                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateToken                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwCreateWaitablePort                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwDelayExecution                                   proto stdcall :DWORD, :DWORD
ZwDeleteAtom                                       proto stdcall :DWORD
ZwDeleteFile                                       proto stdcall :DWORD
ZwDeleteKey                                        proto stdcall :DWORD
ZwDeleteObjectAuditAlarm                           proto stdcall :DWORD, :DWORD, :DWORD
ZwDeleteValueKey                                   proto stdcall :DWORD, :DWORD
ZwDeviceIoControlFile                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwDisplayString                                    proto stdcall :DWORD
ZwDuplicateObject                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwDuplicateToken                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwEnumerateKey                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwEnumerateValueKey                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwExtendSection                                    proto stdcall :DWORD, :DWORD
ZwFilterToken                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwFindAtom                                         proto stdcall :DWORD, :DWORD, :DWORD
ZwFlushBuffersFile                                 proto stdcall :DWORD, :DWORD
ZwFlushInstructionCache                            proto stdcall :DWORD, :DWORD, :DWORD
ZwFlushKey                                         proto stdcall :DWORD
ZwFlushVirtualMemory                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwFlushWriteBuffer                                 proto stdcall
ZwFreeUserPhysicalPages                            proto stdcall :DWORD, :DWORD, :DWORD
ZwFreeVirtualMemory                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwFsControlFile                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwGetContextThread                                 proto stdcall :DWORD, :DWORD
ZwGetDevicePowerState                              proto stdcall :DWORD, :DWORD
ZwGetPlugPlayEvent                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwGetTickCount                                     proto stdcall
ZwGetWriteWatch                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwImpersonateAnonymousToken                        proto stdcall :DWORD
ZwImpersonateClientOfPort                          proto stdcall :DWORD, :DWORD
ZwImpersonateThread                                proto stdcall :DWORD, :DWORD, :DWORD
ZwInitializeRegistry                               proto stdcall :DWORD
ZwInitiatePowerAction                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwIsSystemResumeAutomatic                          proto stdcall
ZwListenChannel                                    proto stdcall :DWORD, :DWORD
ZwListenPort                                       proto stdcall :DWORD, :DWORD
ZwLoadDriver                                       proto stdcall :DWORD
ZwLoadKey2                                         proto stdcall :DWORD, :DWORD, :DWORD
ZwLoadKey                                          proto stdcall :DWORD, :DWORD
ZwLockFile                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwLockVirtualMemory                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwMakeTemporaryObject                              proto stdcall :DWORD
ZwMapUserPhysicalPages                             proto stdcall :DWORD, :DWORD, :DWORD
ZwMapUserPhysicalPagesScatter                      proto stdcall :DWORD, :DWORD, :DWORD
ZwMapViewOfSection                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwNotifyChangeDirectoryFile                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwNotifyChangeKey                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwNotifyChangeMultipleKeys                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenChannel                                      proto stdcall :DWORD, :DWORD
ZwOpenDirectoryObject                              proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenEvent                                        proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenEventPair                                    proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenFile                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenIoCompletion                                 proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenJobObject                                    proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenKey                                          proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenMutant                                       proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenObjectAuditAlarm                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenProcess                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenProcessToken                                 proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenSection                                      proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenSemaphore                                    proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenSymbolicLinkObject                           proto stdcall :DWORD, :DWORD, :DWORD
ZwOpenThread                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenThreadToken                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwOpenTimer                                        proto stdcall :DWORD, :DWORD, :DWORD
ZwPlugPlayControl                                  proto stdcall :DWORD, :DWORD, :DWORD
ZwPowerInformation                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwPrivilegeCheck                                   proto stdcall :DWORD, :DWORD, :DWORD
ZwPrivilegeObjectAuditAlarm                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwPrivilegedServiceAuditAlarm                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwProtectVirtualMemory                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwPulseEvent                                       proto stdcall :DWORD, :DWORD
ZwQueryAttributesFile                              proto stdcall :DWORD, :DWORD
ZwQueryDefaultLocale                               proto stdcall :DWORD, :DWORD
ZwQueryDefaultUILanguage                           proto stdcall :DWORD
ZwQueryDirectoryFile                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryDirectoryObject                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryEaFile                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryEvent                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryFullAttributesFile                          proto stdcall :DWORD, :DWORD
ZwQueryInformationAtom                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationFile                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationJobObject                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationPort                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationProcess                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationThread                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInformationToken                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryInstallUILanguage                           proto stdcall :DWORD
ZwQueryIntervalProfile                             proto stdcall :DWORD, :DWORD
ZwQueryIoCompletion                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryKey                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryMultipleValueKey                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryMutant                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryObject                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryOpenSubKeys                                 proto stdcall :DWORD, :DWORD
ZwQueryPerformanceCounter                          proto stdcall :DWORD, :DWORD
ZwQueryQuotaInformationFile                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySection                                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySecurityObject                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySemaphore                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySymbolicLinkObject                          proto stdcall :DWORD, :DWORD, :DWORD
ZwQuerySystemEnvironmentValue                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySystemInformation                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwQuerySystemTime                                  proto stdcall :DWORD
ZwQueryTimer                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryTimerResolution                             proto stdcall :DWORD, :DWORD, :DWORD
ZwQueryValueKey                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryVirtualMemory                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueryVolumeInformationFile                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwQueueApcThread                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwRaiseException                                   proto stdcall :DWORD, :DWORD, :DWORD
ZwRaiseHardError                                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwReadFile                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwReadFileScatter                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwReadRequestData                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwReadVirtualMemory                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwRegisterThreadTerminatePort                      proto stdcall :DWORD
ZwReleaseMutant                                    proto stdcall :DWORD, :DWORD
ZwReleaseSemaphore                                 proto stdcall :DWORD, :DWORD, :DWORD
ZwRemoveIoCompletion                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwReplaceKey                                       proto stdcall :DWORD, :DWORD, :DWORD
ZwReplyPort                                        proto stdcall :DWORD, :DWORD
ZwReplyWaitReceivePort                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwReplyWaitReceivePortEx                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwReplyWaitReplyPort                               proto stdcall :DWORD, :DWORD
ZwReplyWaitSendChannel                             proto stdcall :DWORD, :DWORD, :DWORD
ZwRequestDeviceWakeup                              proto stdcall :DWORD
ZwRequestPort                                      proto stdcall :DWORD, :DWORD
ZwRequestWaitReplyPort                             proto stdcall :DWORD, :DWORD, :DWORD
ZwRequestWakeupLatency                             proto stdcall :DWORD
ZwResetEvent                                       proto stdcall :DWORD, :DWORD
ZwResetWriteWatch                                  proto stdcall :DWORD, :DWORD, :DWORD
ZwRestoreKey                                       proto stdcall :DWORD, :DWORD, :DWORD
ZwResumeThread                                     proto stdcall :DWORD, :DWORD
ZwSaveKey                                          proto stdcall :DWORD, :DWORD
ZwSaveMergedKeys                                   proto stdcall :DWORD, :DWORD, :DWORD
ZwSecureConnectPort                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSendWaitReplyChannel                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetContextChannel                                proto stdcall :DWORD
ZwSetContextThread                                 proto stdcall :DWORD, :DWORD
ZwSetDefaultHardErrorPort                          proto stdcall :DWORD
ZwSetDefaultLocale                                 proto stdcall :DWORD, :DWORD
ZwSetDefaultUILanguage                             proto stdcall :DWORD
ZwSetEaFile                                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetEvent                                         proto stdcall :DWORD, :DWORD
ZwSetHighEventPair                                 proto stdcall :DWORD
ZwSetHighWaitLowEventPair                          proto stdcall :DWORD
ZwSetInformationFile                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationJobObject                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationKey                                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationObject                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationProcess                            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationThread                             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetInformationToken                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetIntervalProfile                               proto stdcall :DWORD, :DWORD
ZwSetIoCompletion                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSetLdtEntries                                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSetLowEventPair                                  proto stdcall :DWORD
ZwSetLowWaitHighEventPair                          proto stdcall :DWORD
ZwSetQuotaInformationFile                          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwSetSecurityObject                                proto stdcall :DWORD, :DWORD, :DWORD
ZwSetSystemEnvironmentValue                        proto stdcall :DWORD, :DWORD
ZwSetSystemInformation                             proto stdcall :DWORD, :DWORD, :DWORD
ZwSetSystemPowerState                              proto stdcall :DWORD, :DWORD, :DWORD
ZwSetSystemTime                                    proto stdcall :DWORD, :DWORD
ZwSetThreadExecutionState                          proto stdcall :DWORD, :DWORD
ZwSetTimer                                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSetTimerResolution                               proto stdcall :DWORD, :DWORD, :DWORD
ZwSetUuidSeed                                      proto stdcall :DWORD
ZwSetValueKey                                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwSetVolumeInformationFile                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwShutdownSystem                                   proto stdcall :DWORD
ZwSignalAndWaitForSingleObject                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwStartProfile                                     proto stdcall :DWORD
ZwStopProfile                                      proto stdcall :DWORD
ZwSuspendThread                                    proto stdcall :DWORD, :DWORD
ZwSystemDebugControl                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwTerminateJobObject                               proto stdcall :DWORD, :DWORD
ZwTerminateProcess                                 proto stdcall :DWORD, :DWORD
ZwTerminateThread                                  proto stdcall :DWORD, :DWORD
ZwTestAlert                                        proto stdcall
ZwUnloadDriver                                     proto stdcall :DWORD
ZwUnloadKey                                        proto stdcall :DWORD
ZwUnlockFile                                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwUnlockVirtualMemory                              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
ZwUnmapViewOfSection                               proto stdcall :DWORD, :DWORD
ZwVdmControl                                       proto stdcall :DWORD, :DWORD
ZwWaitForMultipleObjects                           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwWaitForSingleObject                              proto stdcall :DWORD, :DWORD, :DWORD
ZwWaitHighEventPair                                proto stdcall :DWORD
ZwWaitLowEventPair                                 proto stdcall :DWORD
ZwWriteFile                                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwWriteFileGather                                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwWriteRequestData                                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwWriteVirtualMemory                               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
ZwYieldExecution                                   proto stdcall
_rt_probe_read4                                    proto stdcall :DWORD
_seh_longjmp_unwind                                proto stdcall :DWORD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                             CDECL                                              ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;__eCommonExceptions PROTO C :VARARG
;__eEmulatorInit PROTO C :VARARG

; FPU Emulator
comment ^
__eF2XM1 PROTO C :VARARG
__eFABS PROTO C :VARARG
__eFADD32 PROTO C :VARARG
__eFADD64 PROTO C :VARARG
__eFADDPreg PROTO C :VARARG
__eFADDreg PROTO C :VARARG
__eFADDtop PROTO C :VARARG
__eFCHS PROTO C :VARARG
__eFCOM PROTO C :VARARG
__eFCOM32 PROTO C :VARARG
__eFCOM64 PROTO C :VARARG
__eFCOMP PROTO C :VARARG
__eFCOMP32 PROTO C :VARARG
__eFCOMP64 PROTO C :VARARG
__eFCOMPP PROTO C :VARARG
__eFCOS PROTO C :VARARG
__eFDECSTP PROTO C :VARARG
__eFDIV32 PROTO C :VARARG
__eFDIV64 PROTO C :VARARG
__eFDIVPreg PROTO C :VARARG
__eFDIVR32 PROTO C :VARARG
__eFDIVR64 PROTO C :VARARG
__eFDIVreg PROTO C :VARARG
__eFDIVRPreg PROTO C :VARARG
__eFDIVRreg PROTO C :VARARG
__eFDIVRtop PROTO C :VARARG
__eFDIVtop PROTO C :VARARG
__eFFREE PROTO C :VARARG
__eFIADD16 PROTO C :VARARG
__eFIADD32 PROTO C :VARARG
__eFICOM16 PROTO C :VARARG
__eFICOM32 PROTO C :VARARG
__eFICOMP16 PROTO C :VARARG
__eFICOMP32 PROTO C :VARARG
__eFIDIV16 PROTO C :VARARG
__eFIDIV32 PROTO C :VARARG
__eFIDIVR16 PROTO C :VARARG
__eFIDIVR32 PROTO C :VARARG
__eFILD16 PROTO C :VARARG
__eFILD32 PROTO C :VARARG
__eFILD64 PROTO C :VARARG
__eFIMUL16 PROTO C :VARARG
__eFIMUL32 PROTO C :VARARG
__eFINCSTP PROTO C :VARARG
__eFINIT PROTO C :VARARG
__eFIST16 PROTO C :VARARG
__eFIST32 PROTO C :VARARG
__eFISTP16 PROTO C :VARARG
__eFISTP32 PROTO C :VARARG
__eFISTP64 PROTO C :VARARG
__eFISUB16 PROTO C :VARARG
__eFISUB32 PROTO C :VARARG
__eFISUBR16 PROTO C :VARARG
__eFISUBR32 PROTO C :VARARG
__eFLD1 PROTO C :VARARG
__eFLD32 PROTO C :VARARG
__eFLD64 PROTO C :VARARG
__eFLD80 PROTO C :VARARG
__eFLDCW PROTO C :VARARG
__eFLDENV PROTO C :VARARG
__eFLDL2E PROTO C :VARARG
__eFLDLN2 PROTO C :VARARG
__eFLDPI PROTO C :VARARG
__eFLDZ PROTO C :VARARG
__eFMUL32 PROTO C :VARARG
__eFMUL64 PROTO C :VARARG
__eFMULPreg PROTO C :VARARG
__eFMULreg PROTO C :VARARG
__eFMULtop PROTO C :VARARG
__eFPATAN PROTO C :VARARG
__eFPREM PROTO C :VARARG
__eFPREM1 PROTO C :VARARG
__eFPTAN PROTO C :VARARG
__eFRNDINT PROTO C :VARARG
__eFRSTOR PROTO C :VARARG
__eFSAVE PROTO C :VARARG
__eFSCALE PROTO C :VARARG
__eFSIN PROTO C :VARARG
__eFSQRT PROTO C :VARARG
__eFST PROTO C :VARARG
__eFST32 PROTO C :VARARG
__eFST64 PROTO C :VARARG
__eFSTCW PROTO C :VARARG
__eFSTENV PROTO C :VARARG
__eFSTP PROTO C :VARARG
__eFSTP32 PROTO C :VARARG
__eFSTP64 PROTO C :VARARG
__eFSTP80 PROTO C :VARARG
__eFSTSW PROTO C :VARARG
__eFSUB32 PROTO C :VARARG
__eFSUB64 PROTO C :VARARG
__eFSUBPreg PROTO C :VARARG
__eFSUBR32 PROTO C :VARARG
__eFSUBR64 PROTO C :VARARG
__eFSUBreg PROTO C :VARARG
__eFSUBRPreg PROTO C :VARARG
__eFSUBRreg PROTO C :VARARG
__eFSUBRtop PROTO C :VARARG
__eFSUBtop PROTO C :VARARG
__eFTST PROTO C :VARARG
__eFUCOM PROTO C :VARARG
__eFUCOMP PROTO C :VARARG
__eFUCOMPP PROTO C :VARARG
__eFXAM PROTO C :VARARG
__eFXCH PROTO C :VARARG
__eFXTRACT PROTO C :VARARG
__eFYL2X PROTO C :VARARG
__eFYL2XP1 PROTO C :VARARG
^
__eGetStatusWord PROTO C
__isascii PROTO C :DWORD
__iscsym PROTO C :DWORD
__iscsymf PROTO C :DWORD
__toascii PROTO C :DWORD
;_abnormal_termination PROTO C

; Some funcs below are decorated as __cdecl
; but pop arguments from the stack.

_alldiv PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_allmul PROTO C :DWORD, :DWORD, :DWORD, :DWORD
;_alloca_probe PROTO C :VARARG
;_allrem PROTO C :DWORD, :DWORD, :DWORD, :DWORD	; ???!!!
_allshl PROTO C	; ???!!!
_allshr PROTO C	; ???!!!
_atoi64 PROTO C :DWORD
_aulldiv PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_aullrem PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_aullshr PROTO C	; ???!!!
;_chkstk PROTO C :VARARG	; !!!
;_CIpow PROTO C :VARARG
;_except_handler2 PROTO C
;_except_handler3 PROTO C
;_except_list PROTO C
;_fltused PROTO C :VARARG
;_ftol PROTO C :VARARG
;_global_unwind2 PROTO C
_i64toa PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_i64tow PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_itoa PROTO C :DWORD, :DWORD, :DWORD
_itow PROTO C :DWORD, :DWORD, :DWORD
;_local_unwind2 PROTO C
;_longjmpex PROTO C :VARARG
_ltoa PROTO C :DWORD, :DWORD, :DWORD
_ltow PROTO C :DWORD, :DWORD, :DWORD
_memccpy PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_memicmp PROTO C :DWORD, :DWORD, :DWORD
;_setjmp PROTO C :VARARG
;_setjmp3 PROTO C :VARARG
;_setjmpexused PROTO C :VARARG
_snprintf PROTO C :DWORD, :DWORD, :DWORD, :VARARG
_snwprintf PROTO C :DWORD, :DWORD, :DWORD, :VARARG
_splitpath PROTO C :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
_strcmpi PROTO C :DWORD, :DWORD
_stricmp PROTO C :DWORD, :DWORD
_strlwr PROTO C :DWORD
_strnicmp PROTO C :DWORD, :DWORD, :DWORD
_strupr PROTO C :DWORD
_tolower PROTO C :DWORD
_toupper PROTO C :DWORD
_ui64toa PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_ultoa PROTO C :DWORD, :DWORD, :DWORD
_ultow PROTO C :DWORD, :DWORD, :DWORD
_vsnprintf PROTO C :DWORD, :DWORD, :DWORD, :DWORD
_wcsicmp PROTO C :DWORD, :DWORD
_wcslwr PROTO C :DWORD
_wcsnicmp PROTO C :DWORD, :DWORD, :DWORD
_wcsupr PROTO C :DWORD
_wtoi PROTO C :DWORD
_wtoi64 PROTO C :DWORD
_wtol PROTO C :DWORD
abs PROTO C :DWORD
atan PROTO C :DWORD, :DWORD	; double
atoi PROTO C :DWORD
atol PROTO C :DWORD
ceil PROTO C :DWORD, :DWORD	; double
cos PROTO C :DWORD, :DWORD	; double
DbgPrint PROTO C :DWORD, :VARARG
DbgPrintReturnControlC PROTO C
;fabs PROTO C :VARARG
floor PROTO C :DWORD, :DWORD	; double
isalnum PROTO C :DWORD
isalpha PROTO C :DWORD
iscntrl PROTO C :DWORD
isdigit PROTO C :DWORD
isgraph PROTO C :DWORD
islower PROTO C :DWORD
isprint PROTO C :DWORD
ispunct PROTO C :DWORD
isspace PROTO C :DWORD
isupper PROTO C :DWORD
iswalpha PROTO C :DWORD
iswctype PROTO C :DWORD, :DWORD
iswdigit PROTO C :DWORD
iswlower PROTO C :DWORD
iswspace PROTO C :DWORD
iswxdigit PROTO C :DWORD
isxdigit PROTO C :DWORD
labs PROTO C :DWORD
log PROTO C :DWORD, :DWORD	; ???
longjmp PROTO C :DWORD, :DWORD
mbstowcs PROTO C :DWORD, :DWORD, :DWORD
memchr PROTO C :DWORD, :DWORD, :DWORD
memcmp PROTO C :DWORD, :DWORD, :DWORD
memcpy PROTO C :DWORD, :DWORD, :DWORD
memmove PROTO C :DWORD, :DWORD, :DWORD
memset PROTO C :DWORD, :DWORD, :DWORD
; exported variables
NlsAnsiCodePage PROTO C
NlsMbCodePageTag PROTO C
NlsMbOemCodePageTag PROTO C
;NPXEMULATORTABLE PROTO C :VARARG
pow PROTO C :DWORD, :DWORD, :DWORD, :DWORD	; ???
qsort PROTO C :DWORD, :DWORD, :DWORD, :DWORD
;raisex PROTO C :VARARG
RestoreEm87Context PROTO C :DWORD	; ???
SaveEm87Context PROTO C :DWORD	; ???
sin PROTO C :DWORD, :DWORD
sprintf PROTO C :DWORD, :DWORD, :VARARG
sqrt PROTO C :DWORD, :DWORD
sscanf PROTO C :DWORD, :DWORD, :VARARG
strcat PROTO C :DWORD, :DWORD
strchr PROTO C :DWORD, :DWORD
strcmp PROTO C :DWORD, :DWORD
strcpy PROTO C :DWORD, :DWORD
strcspn PROTO C :DWORD, :DWORD
strlen PROTO C :DWORD
strncat PROTO C :DWORD, :DWORD, :DWORD	
strncmp PROTO C :DWORD, :DWORD, :DWORD	
strncpy PROTO C :DWORD, :DWORD, :DWORD	
strpbrk PROTO C :DWORD, :DWORD
strrchr PROTO C :DWORD, :DWORD
strspn PROTO C :DWORD, :DWORD
strstr PROTO C :DWORD, :DWORD
strtol PROTO C :DWORD, :DWORD, :DWORD
strtoul PROTO C :DWORD, :DWORD, :DWORD
swprintf PROTO C :DWORD, :DWORD, :VARARG
tan PROTO C :DWORD, :DWORD
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
wcspbrk PROTO C :DWORD, :DWORD
wcsrchr PROTO C :DWORD, :DWORD
wcsspn PROTO C :DWORD, :DWORD
wcsstr PROTO C :DWORD, :DWORD
wcstol PROTO C :DWORD, :DWORD, :DWORD
wcstombs PROTO C :DWORD, :DWORD, :DWORD
wcstoul PROTO C :DWORD, :DWORD, :DWORD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                           FASTCALL                                             ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

externdef syscall @RtlUlongByteSwap@4:proc
RtlUlongByteSwap textequ <@RtlUlongByteSwap@4>

externdef syscall @RtlUlonglongByteSwap@4:proc
RtlUlonglongByteSwap textequ <@RtlUlonglongByteSwap@4>

externdef syscall @RtlUshortByteSwap@4:proc
RtlUshortByteSwap textequ <@RtlUshortByteSwap@4>
