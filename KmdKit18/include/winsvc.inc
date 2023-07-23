comment ^

Module Name:
    winsvc.inc

Abstract:
    Header file for the Service Control Manager
    
Author:
    Four-F (four-f@mail.ru)

Last Update:
	25-November-2003
^

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service database names
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICES_ACTIVE_DATABASEW      dw "S","e","r","v","i","c","e","s","A","c","t","i","v","e",0
SERVICES_FAILED_DATABASEW      dw "S","e","r","v","i","c","e","s","F","a","i","l","e","d",0

SERVICES_ACTIVE_DATABASEA      db "ServicesActive",0
SERVICES_FAILED_DATABASEA      db "ServicesFailed",0

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Character to designate that a name is a group
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SC_GROUP_IDENTIFIERW           dw "+",0
SC_GROUP_IDENTIFIERA           db "+",0

IFDEF UNICODE
	SERVICES_ACTIVE_DATABASE equ SERVICES_ACTIVE_DATABASEW
	SERVICES_FAILED_DATABASE equ SERVICES_FAILED_DATABASEW
	SC_GROUP_IDENTIFIER      equ SC_GROUP_IDENTIFIERW
ELSE
	SERVICES_ACTIVE_DATABASE   equ    SERVICES_ACTIVE_DATABASEA
	SERVICES_FAILED_DATABASE   equ    SERVICES_FAILED_DATABASEA
	SC_GROUP_IDENTIFIER        equ    SC_GROUP_IDENTIFIERA
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Value to indicate no change to an optional parameter
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_NO_CHANGE              equ 0ffffffffh

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service State -- for Enum Requests (Bit Mask)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_ACTIVE                 equ 00000001
SERVICE_INACTIVE               equ 00000002
SERVICE_STATE_ALL              equ SERVICE_ACTIVE or SERVICE_INACTIVE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Controls
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_CONTROL_STOP           equ 00000001h
SERVICE_CONTROL_PAUSE          equ 00000002h
SERVICE_CONTROL_CONTINUE       equ 00000003h
SERVICE_CONTROL_INTERROGATE    equ 00000004h
SERVICE_CONTROL_SHUTDOWN       equ 00000005h
SERVICE_CONTROL_PARAMCHANGE    equ 00000006h
SERVICE_CONTROL_NETBINDADD     equ 00000007h
SERVICE_CONTROL_NETBINDREMOVE  equ 00000008h
SERVICE_CONTROL_NETBINDENABLE  equ 00000009h
SERVICE_CONTROL_NETBINDDISABLE equ 0000000Ah

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service State -- for CurrentState
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_STOPPED                equ 00000001
SERVICE_START_PENDING          equ 00000002
SERVICE_STOP_PENDING           equ 00000003
SERVICE_RUNNING                equ 00000004
SERVICE_CONTINUE_PENDING       equ 00000005
SERVICE_PAUSE_PENDING          equ 00000006
SERVICE_PAUSED                 equ 00000007

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Controls Accepted  (Bit Mask)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_ACCEPT_STOP            equ 00000001h
SERVICE_ACCEPT_PAUSE_CONTINUE  equ 00000002h
SERVICE_ACCEPT_SHUTDOWN        equ 00000004h
SERVICE_ACCEPT_PARAMCHANGE     equ 00000008h
SERVICE_ACCEPT_NETBINDCHANGE   equ 00000010h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service Control Manager object specific access types
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SC_MANAGER_CONNECT             equ 0001h
SC_MANAGER_CREATE_SERVICE      equ 0002h
SC_MANAGER_ENUMERATE_SERVICE   equ 0004h
SC_MANAGER_LOCK                equ 0008h
SC_MANAGER_QUERY_LOCK_STATUS   equ 0010h
SC_MANAGER_MODIFY_BOOT_CONFIG  equ 0020h

SC_MANAGER_ALL_ACCESS          equ STANDARD_RIGHTS_REQUIRED or SC_MANAGER_CONNECT or SC_MANAGER_CREATE_SERVICE or SC_MANAGER_ENUMERATE_SERVICE or SC_MANAGER_LOCK or SC_MANAGER_QUERY_LOCK_STATUS or SC_MANAGER_MODIFY_BOOT_CONFIG

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service object specific access type
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_QUERY_CONFIG           equ 0001h
SERVICE_CHANGE_CONFIG          equ 0002h
SERVICE_QUERY_STATUS           equ 0004h
SERVICE_ENUMERATE_DEPENDENTS   equ 0008h
SERVICE_START                  equ 0010h
SERVICE_STOP                   equ 0020h
SERVICE_PAUSE_CONTINUE         equ 0040h
SERVICE_INTERROGATE            equ 0080h
SERVICE_USER_DEFINED_CONTROL   equ 0100h

SERVICE_ALL_ACCESS             equ STANDARD_RIGHTS_REQUIRED or SERVICE_QUERY_CONFIG or SERVICE_CHANGE_CONFIG or SERVICE_QUERY_STATUS or SERVICE_ENUMERATE_DEPENDENTS or SERVICE_START or SERVICE_STOP or SERVICE_PAUSE_CONTINUE or SERVICE_INTERROGATE or SERVICE_USER_DEFINED_CONTROL


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Info levels for ChangeServiceConfig2 and QueryServiceConfig2
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_CONFIG_DESCRIPTION     1
SERVICE_CONFIG_FAILURE_ACTIONS 2

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service description string
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_DESCRIPTION STRUCT
    LPSTR       lpDescription;
SERVICE_DESCRIPTION ENDS
LPSERVICE_DESCRIPTION typedef PTR SERVICE_DESCRIPTION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Actions to take on service failure
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _SC_ACTION_TYPE {
        SC_ACTION_NONE          equ 0
        SC_ACTION_RESTART       equ 1
        SC_ACTION_REBOOT        equ 2
        SC_ACTION_RUN_COMMAND   equ 3

SC_ACTION STRUCT
	Type	DWORD	?	; SC_ACTION_TYPE
	Delay	DWORD	?
SC_ACTION ENDS
LPSC_ACTION typedef PTR SC_ACTION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Handle Types
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SC_HANDLE typedef DWORD
LPSC_HANDLE typedef PTR DWORD
SERVICE_STATUS_HANDLE typedef DWORD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service Status Structure
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_STATUS STRUCT
	dwServiceType				DWORD	?
	dwCurrentState				DWORD	?
	dwControlsAccepted			DWORD	?
	dwWin32ExitCode				DWORD	?
	dwServiceSpecificExitCode	DWORD	?
	dwCheckPoint				DWORD	?
	dwWaitHint					DWORD	?
SERVICE_STATUS ENDS
LPSERVICE_STATUS typedef PTR SERVICE_STATUS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service Status Enumeration Structure
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ENUM_SERVICE_STATUS STRUCT
    LPSTR          lpServiceName;
    LPSTR          lpDisplayName;
    SERVICE_STATUS ServiceStatus;
ENUM_SERVICE_STATUS ENDS
LPENUM_SERVICE_STATUS typedef PTR ENUM_SERVICE_STATUS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Structures for the Lock API functions
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SC_LOCK typedef DWORD

QUERY_SERVICE_LOCK_STATUS STRUCT
    DWORD   fIsLocked;
    LPSTR   lpLockOwner;
    DWORD   dwLockDuration;
QUERY_SERVICE_LOCK_STATUS ENDS
LPQUERY_SERVICE_LOCK_STATUS typedef PTR QUERY_SERVICE_LOCK_STATUS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Query Service Configuration Structure
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

QUERY_SERVICE_CONFIG STRUCT
	dwServiceType		DWORD	?
	dwStartType			DWORD	?
	dwErrorControl		DWORD	?
	lpBinaryPathName	DWORD	?	; LPSTR
	lpLoadOrderGroup	DWORD	?	; LPSTR
	dwTagId				DWORD	?
	lpDependencies		DWORD	?	; LPSTR
	lpServiceStartName	DWORD	?	; LPSTR
	lpDisplayName		DWORD	?	; LPSTR
QUERY_SERVICE_CONFIG ENDS
LPQUERY_SERVICE_CONFIG typedef PTR QUERY_SERVICE_CONFIG

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Service Start Table
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SERVICE_TABLE_ENTRY STRUCT
	lpServiceName		DWORD	?	; LPSTR
	lpServiceProc		DWORD	?	; LPSERVICE_MAIN_FUNCTIONA
SERVICE_TABLE_ENTRY ENDS
LPSERVICE_TABLE_ENTRY typedef PTR SERVICE_TABLE_ENTRY
