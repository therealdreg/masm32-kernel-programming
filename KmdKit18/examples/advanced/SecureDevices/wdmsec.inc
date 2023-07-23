
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                    This file was created with Protogen v1.0 by Four-F.                         ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                            STDCALL                                             ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CmRegUtilCreateUcKey                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilCreateWstrKey                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilOpenExistingUcKey                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilOpenExistingWstrKey               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilUcValueGetDword                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilUcValueGetFullBuffer              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilUcValueSetFullBuffer              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilUcValueSetUcString                proto stdcall :DWORD, :DWORD, :DWORD
CmRegUtilUcValueSetWstrString              proto stdcall :DWORD, :DWORD, :DWORD
CmRegUtilWstrValueGetDword                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilWstrValueGetFullBuffer            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilWstrValueSetFullBuffer            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CmRegUtilWstrValueSetUcString              proto stdcall :DWORD, :DWORD, :DWORD
CmRegUtilWstrValueSetWstrString            proto stdcall :DWORD, :DWORD, :DWORD
CmpRegUtilAllocateUnicodeString            proto stdcall :DWORD, :DWORD
CmpRegUtilFreeAllocatedUnicodeString       proto stdcall :DWORD
IoDevObjCreateDeviceSecure                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
IopDevObjAdjustNewDeviceParameters         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
IopDevObjApplyPostCreationSettings         proto stdcall :DWORD, :DWORD
PiRegStateOpenClassKey                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PiRegStateReadStackCreationSettingsFromKey proto stdcall :DWORD, :DWORD
PpRegStateFreeStackCreationSettings        proto stdcall :DWORD
PpRegStateInitEmptyCreationSettings        proto stdcall :DWORD
PpRegStateLoadSecurityDescriptor           proto stdcall :DWORD, :DWORD
PpRegStateReadCreateClassCreationSettings  proto stdcall :DWORD, :DWORD, :DWORD
PpRegStateUpdateStackCreationSettings      proto stdcall :DWORD, :DWORD
SeSddlSecurityDescriptorFromSDDL           proto stdcall :DWORD, :DWORD, :DWORD
SeSetSecurityAccessMask                    proto stdcall :DWORD, :DWORD
SeUtilSecurityInfoFromSecurityDescriptor   proto stdcall :DWORD, :DWORD, :DWORD
SepSddlAddAceToAcl                         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
SepSddlDaclFromSDDLString                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
SepSddlGetAclForString                     proto stdcall :DWORD, :DWORD, :DWORD
SepSddlGetSidForString                     proto stdcall :DWORD, :DWORD, :DWORD
SepSddlLookupAccessMaskInTable             proto stdcall :DWORD, :DWORD, :DWORD
SepSddlParseWideStringUlong                proto stdcall :DWORD, :DWORD, :DWORD
SepSddlSecurityDescriptorFromSDDLString    proto stdcall :DWORD, :DWORD, :DWORD
WdmlibInit                                 proto stdcall
WdmlibIoCreateDeviceSecure                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
WdmlibIoValidateDeviceIoControlAccess      proto stdcall :DWORD, :DWORD
WdmlibRtlInitUnicodeStringEx               proto stdcall :DWORD, :DWORD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                             CDECL                                              ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PfnIoCreateDeviceSecure                              proto c :VARARG
PfnIoValidateDeviceIoControlAccess                   proto c :VARARG
PiRegStateDiscriptor                                 proto c :VARARG
PiRegStateSysAllInherittedSecurityDescriptor         proto c :VARARG
SDDL_DEVOBJ_KERNEL_ONLY                              proto c :VARARG
SDDL_DEVOBJ_KERNEL_ONLY_buffer                       proto c :VARARG
SDDL_DEVOBJ_SYS_ALL                                  proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_ALL                          proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_ALL_buffer                   proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_R                  proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RWX_RES_RWX        proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RWX_RES_RWX_buffer proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RW_RES_R           proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RW_RES_R_buffer    proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_R_RES_R            proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_R_RES_R_buffer     proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_R_buffer           proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RX                           proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_ADM_RX_buffer                    proto c :VARARG
SDDL_DEVOBJ_SYS_ALL_buffer                           proto c :VARARG
WdmlibInitialized                                    proto c :VARARG
