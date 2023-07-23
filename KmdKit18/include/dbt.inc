comment ^
Module Name:
    dbt.inc

Abstract:
    Equates for WM_DEVICECHANGE and BroadcastSystemMessage

Author:
    Four-F (four-f@mail.ru)

Last updated:
	28-November-2003
^

; BroadcastSpecialMessage constants.

WM_DEVICECHANGE         equ 0219h

; Broadcast message and receipient flags.
;
; Note that there is a third "flag". If the wParam has:
;
; bit 15 on:   lparam is a pointer and bit 14 is meaningfull.
; bit 15 off:  lparam is just a UNLONG data type.
;
; bit 14 on:   lparam is a pointer to an ASCIIZ string.
; bit 14 off:  lparam is a pointer to a binary struture starting with
;              a dword describing the length of the structure.

BSF_QUERY               equ 00000001
BSF_IGNORECURRENTTASK   equ 00000002      ; Meaningless for VxDs
BSF_FLUSHDISK           equ 00000004      ; Shouldn't be used by VxDs
BSF_NOHANG              equ 00000008
BSF_POSTMESSAGE         equ 00000010h
BSF_FORCEIFHUNG         equ 00000020h
BSF_NOTIMEOUTIFNOTHUNG  equ 00000040h
BSF_MSGSRV32ISOK        equ 80000000h      ; Called synchronously from PM API
BSF_MSGSRV32ISOK_BIT    equ 31             ; Called synchronously from PM API

BSM_ALLCOMPONENTS       equ 00000000
BSM_VXDS                equ 00000001
BSM_NETDRIVER           equ 00000002
BSM_INSTALLABLEDRIVERS  equ 00000004
BSM_APPLICATIONS        equ 00000008

; Message = WM_DEVICECHANGE
; wParam  = DBT_APPYBEGIN
; lParam  = (not used)
;
;      'Appy-time is now available.  This message is itself sent
;      at 'Appy-time.
;
; Message = WM_DEVICECHANGE
; wParam  = DBT_APPYEND
; lParam  = (not used)
;
;      'Appy-time is no longer available.  This message is *NOT* sent
;      at 'Appy-time.  (It cannot be, because 'Appy-time is gone.)
;
; NOTE!  It is possible for DBT_APPYBEGIN and DBT_APPYEND to be sent
; multiple times during a single Windows session.  Each appearance of
; 'Appy-time is bracketed by these two messages, but 'Appy-time may
; momentarily become unavailable during otherwise normal Windows
; processing.  The current status of 'Appy-time availability can always
; be obtained from a call to _SHELL_QueryAppyTimeAvailable.

DBT_APPYBEGIN                   equ 0000
DBT_APPYEND                     equ 0001

; Message = WM_DEVICECHANGE
; wParam  = DBT_DEVNODES_CHANGED
; lParam  = 0
;
;      send when configmg finished a process tree batch. Some devnodes
;      may have been added or removed. This is used by ring3 people which
;      need to be refreshed whenever any devnode changed occur (like
;      device manager). People specific to certain devices should use
;      DBT_DEVICE* instead.

DBT_DEVNODES_CHANGED            equ 0007

; Message = WM_DEVICECHANGE
; wParam  = DBT_QUERYCHANGECONFIG
; lParam  = 0
;
;      sent to ask if a config change is allowed

DBT_QUERYCHANGECONFIG           equ 0017h

; Message = WM_DEVICECHANGE
; wParam  = DBT_CONFIGCHANGED
; lParam  = 0
;
;      sent when a config has changed

DBT_CONFIGCHANGED               equ 0018h

; Message = WM_DEVICECHANGE
; wParam  = DBT_CONFIGCHANGECANCELED
; lParam  = 0
;
;      someone cancelled the config change

DBT_CONFIGCHANGECANCELED        equ 0019h

; Message = WM_DEVICECHANGE
; wParam  = DBT_MONITORCHANGE
; lParam  = new resolution to use (LOWORD=x, HIWORD=y)
;           if 0, use the default res for current config
;
;      this message is sent when the display monitor has changed
;      and the system should change the display mode to match it.

DBT_MONITORCHANGE               equ 001Bh

; Message = WM_DEVICECHANGE
; wParam  = DBT_SHELLLOGGEDON
; lParam  = 0
;
;      The shell has finished login on: VxD can now do Shell_EXEC.

DBT_SHELLLOGGEDON               equ 0020h

; Message = WM_DEVICECHANGE
; wParam  = DBT_CONFIGMGAPI
; lParam  = CONFIGMG API Packet
;
;      CONFIGMG ring 3 call.

DBT_CONFIGMGAPI32               equ 0022h

; Message = WM_DEVICECHANGE
; wParam  = DBT_VXDINITCOMPLETE
; lParam  = 0
;
;      CONFIGMG ring 3 call.

DBT_VXDINITCOMPLETE             equ 0023h

; Message = WM_DEVICECHANGE
; wParam  = DBT_VOLLOCK*
; lParam  = pointer to VolLockBroadcast structure described below
;
;      Messages issued by IFSMGR for volume locking purposes on WM_DEVICECHANGE.
;      All these messages pass a pointer to a struct which has no pointers.

DBT_VOLLOCKQUERYLOCK    equ 8041h
DBT_VOLLOCKLOCKTAKEN    equ 8042h
DBT_VOLLOCKLOCKFAILED   equ 8043h
DBT_VOLLOCKQUERYUNLOCK  equ 8044h
DBT_VOLLOCKLOCKRELEASED equ 8045h
DBT_VOLLOCKUNLOCKFAILED equ 8046h

; Device broadcast header

DEV_BROADCAST_HDR STRUCT
	dbch_size			DWORD	?
	dbch_devicetype		DWORD	?
	dbch_reserved		DWORD	?
DEV_BROADCAST_HDR ENDS
PDEV_BROADCAST_HDR typedef ptr DEV_BROADCAST_HDR

; Structure for volume lock broadcast

VolLockBroadcast STRUCT
	vlb_dbh			DEV_BROADCAST_HDR	<>
	vlb_owner		DWORD				?	; thread on which lock request is being issued
	vlb_perms		BYTE				?	; lock permission flags defined below
	vlb_lockType	BYTE				?	; type of lock
	vlb_drive		BYTE				?	; drive on which lock is issued
	vlb_flags		BYTE				?	; miscellaneous flags
VolLockBroadcast ENDS
pVolLockBroadcast typedef ptr VolLockBroadcast

; Values for vlb_perms

LOCKP_ALLOW_WRITES              equ 01    ; Bit 0 set - allow writes
LOCKP_FAIL_WRITES               equ 00    ; Bit 0 clear - fail writes
LOCKP_FAIL_MEM_MAPPING          equ 02    ; Bit 1 set - fail memory mappings
LOCKP_ALLOW_MEM_MAPPING         equ 00    ; Bit 1 clear - allow memory mappings
LOCKP_USER_MASK                 equ 03    ; Mask for user lock flags
LOCKP_LOCK_FOR_FORMAT           equ 04    ; Level 0 lock for format

; Values for vlb_flags

LOCKF_LOGICAL_LOCK              equ 00    ; Bit 0 clear - logical lock
LOCKF_PHYSICAL_LOCK             equ 01    ; Bit 0 set - physical lock

; Message = WM_DEVICECHANGE
; wParam  = DBT_NODISKSPACE
; lParam  = drive number of drive that is out of disk space (1-based)
;
; Message issued by IFS manager when it detects that a drive is run out of
; free space.

DBT_NO_DISK_SPACE               equ 0047h

; Message = WM_DEVICECHANGE
; wParam  = DBT_LOW_DISK_SPACE
; lParam  = drive number of drive that is low on disk space (1-based)
;
; Message issued by VFAT when it detects that a drive it has mounted
; has the remaning free space below a threshold specified by the
; registry or by a disk space management application.
; The broadcast is issued by VFAT ONLY when space is either allocated
; or freed by VFAT.

DBT_LOW_DISK_SPACE      equ 0048h

DBT_CONFIGMGPRIVATE             equ 7FFFh

; The following messages are for WM_DEVICECHANGE. The immediate list
; is for the wParam. ALL THESE MESSAGES PASS A POINTER TO A STRUCT
; STARTING WITH A DWORD SIZE AND HAVING NO POINTER IN THE STRUCT.

DBT_DEVICEARRIVAL               equ 8000h  ; system detected a new device
DBT_DEVICEQUERYREMOVE           equ 8001h  ; wants to remove, may fail
DBT_DEVICEQUERYREMOVEFAILED     equ 8002h  ; removal aborted
DBT_DEVICEREMOVEPENDING         equ 8003h  ; about to remove, still avail.
DBT_DEVICEREMOVECOMPLETE        equ 8004h  ; device is gone
DBT_DEVICETYPESPECIFIC          equ 8005h  ; type specific event
;#if(WINVER >= 0x040A)
DBT_CUSTOMEVENT                 equ 8006h  ; user-defined event
;#endif /* WINVER >= 0x040A */

DBT_DEVTYP_OEM                  equ 00000000  ; oem-defined device type
DBT_DEVTYP_DEVNODE              equ 00000001  ; devnode number
DBT_DEVTYP_VOLUME               equ 00000002  ; logical volume
DBT_DEVTYP_PORT                 equ 00000003  ; serial, parallel
DBT_DEVTYP_NET                  equ 00000004  ; network resource

;#if(WINVER >= 0x040A)
DBT_DEVTYP_DEVICEINTERFACE      equ 00000005  ; device interface class
DBT_DEVTYP_HANDLE               equ 00000006  ; file system handle
;#endif /* WINVER >= 0x040A */

DEV_BROADCAST_HEADER STRUCT
	dbcd_size		DWORD	?
	dbcd_devicetype	DWORD	?
	dbcd_reserved	DWORD	?
DEV_BROADCAST_HEADER ENDS
PDEV_BROADCAST_HEADER typedef ptr DEV_BROADCAST_HEADER

DEV_BROADCAST_OEM STRUCT
	dbco_size		DWORD	?
	dbco_devicetype	DWORD	?
	dbco_reserved	DWORD	?
	dbco_identifier	DWORD	?
	dbco_suppfunc	DWORD	?
DEV_BROADCAST_OEM ENDS
PDEV_BROADCAST_OEM typedef ptr DEV_BROADCAST_OEM

DEV_BROADCAST_DEVNODE STRUCT
	dbcd_size		DWORD	?
	dbcd_devicetype	DWORD	?
	dbcd_reserved	DWORD	?
	dbcd_devnode	DWORD	?
DEV_BROADCAST_DEVNODE ENDS
PDEV_BROADCAST_DEVNODE typedef ptr DEV_BROADCAST_DEVNODE

DEV_BROADCAST_VOLUME STRUCT
	dbcv_size			DWORD	?
	dbcv_devicetype		DWORD	?
	dbcv_reserved		DWORD	?
	dbcv_unitmask		DWORD	?
	dbcv_flags			WORD	?
						WORD	?
DEV_BROADCAST_VOLUME ENDS
PDEV_BROADCAST_VOLUME typedef ptr DEV_BROADCAST_VOLUME

DBTF_MEDIA      equ 0001          ; media comings and goings
DBTF_NET        equ 0002          ; network volume

DEV_BROADCAST_PORT_A STRUCT
	dbcp_size			DWORD	?
	dbcp_devicetype		DWORD	?
	dbcp_reserved		DWORD	?
	dbcp_name			BYTE 1 dup(?)
DEV_BROADCAST_PORT_A ENDS
PDEV_BROADCAST_PORT_A typedef ptr DEV_BROADCAST_PORT_A

DEV_BROADCAST_PORT_W STRUCT
	dbcp_size			DWORD	?
	dbcp_devicetype		DWORD	?
	dbcp_reserved		DWORD	?
	dbcp_name			WORD 1 dup(?)
DEV_BROADCAST_PORT_W ENDS
PDEV_BROADCAST_PORT_W  typedef ptr DEV_BROADCAST_PORT_W

IFDEF UNICODE
	DEV_BROADCAST_PORT equ <DEV_BROADCAST_PORT_W>
	PDEV_BROADCAST_PORT equ PDEV_BROADCAST_PORT_W
ELSE
	DEV_BROADCAST_PORT equ <DEV_BROADCAST_PORT_A>
	PDEV_BROADCAST_PORT equ PDEV_BROADCAST_PORT_A
ENDIF

DEV_BROADCAST_NET STRUCT
	dbcn_size			DWORD	?
	dbcn_devicetype		DWORD	?
	dbcn_reserved		DWORD	?
	dbcn_resource		DWORD	?
	dbcn_flags			DWORD	?
DEV_BROADCAST_NET ENDS
PDEV_BROADCAST_NET typedef ptr DEV_BROADCAST_NET

;#if(WINVER >= 0x040A)

DEV_BROADCAST_DEVICEINTERFACE_A STRUCT
	dbcc_size			DWORD	?
	dbcc_devicetype		DWORD	?
	dbcc_reserved		DWORD	?
	dbcc_classguid		GUID	<>
	dbcc_name			BYTE 1 dup(?)
DEV_BROADCAST_DEVICEINTERFACE_A ENDS
PDEV_BROADCAST_DEVICEINTERFACE_A typedef ptr DEV_BROADCAST_DEVICEINTERFACE_A

DEV_BROADCAST_DEVICEINTERFACE_W STRUCT
	dbcc_size			DWORD	?
	dbcc_devicetype		DWORD	?
	dbcc_reserved		DWORD	?
	dbcc_classguid		GUID	<>
	dbcc_name			WORD 1 dup(?)
DEV_BROADCAST_DEVICEINTERFACE_W ENDS
PDEV_BROADCAST_DEVICEINTERFACE_W typedef ptr DEV_BROADCAST_DEVICEINTERFACE_W

IFDEF UNICODE
	DEV_BROADCAST_DEVICEINTERFACE equ DEV_BROADCAST_DEVICEINTERFACE_W
	PDEV_BROADCAST_DEVICEINTERFACE equ PDEV_BROADCAST_DEVICEINTERFACE_W
ELSE
	DEV_BROADCAST_DEVICEINTERFACE equ DEV_BROADCAST_DEVICEINTERFACE_A
	PDEV_BROADCAST_DEVICEINTERFACE equ PDEV_BROADCAST_DEVICEINTERFACE_A
ENDIF

DEV_BROADCAST_HANDLE STRUCT
	dbch_size			DWORD	?
	dbch_devicetype		DWORD	?
	dbch_reserved		DWORD	?
	dbch_handle			HANDLE	?	; file handle used in call to RegisterDeviceNotification
	dbch_hdevnotify		DWORD	?	; HDEVNOTIFY returned from RegisterDeviceNotification
	; The following 3 fields are only valid if wParam is DBT_CUSTOMEVENT.
	dbch_eventguid		GUID	<>
	dbch_nameoffset		SDWORD	?	; offset (bytes) of variable-length string buffer (-1 if none)
	dbch_data			BYTE 1 dup(?)	; variable-sized buffer, potentially containing binary and/or text data
DEV_BROADCAST_HANDLE ENDS
PDEV_BROADCAST_HANDLE typedef ptr DEV_BROADCAST_HANDLE

;#endif /* WINVER >= 0x040A */

DBTF_RESOURCE   equ 00000001      ; network resource
DBTF_XPORT      equ 00000002      ; new transport coming or going
DBTF_SLOWNET    equ 00000004      ; new incoming transport is slow
									; (dbcn_resource undefined for now)
DBT_VPOWERDAPI  equ 8100h          ; VPOWERD API for Win95

;  User-defined message types all use wParam = 0FFFFh with the
;  lParam a pointer to the structure below.
;
;  dbud_dbh - DEV_BROADCAST_HEADER must be filled in as usual.
;
;  dbud_szName contains a case-sensitive ASCIIZ name which names the
;  message.  The message name consists of the vendor name, a backslash,
;  then arbitrary user-defined ASCIIZ text.  For example:
;
;      "WidgetWare\QueryScannerShutdown"
;      "WidgetWare\Video Q39S\AdapterReady"
;
;  After the ASCIIZ name, arbitrary information may be provided.
;  Make sure that dbud_dbh.dbch_size is big enough to encompass
;  all the data.  And remember that nothing in the structure may
;  contain pointers.

DBT_USERDEFINED equ 0FFFFh

DEV_BROADCAST_USERDEFINED STRUCT
	dbud_dbh	DEV_BROADCAST_HDR	<>
	dbud_szName	BYTE 1 dup(?)	; ASCIIZ name
;  BYTE        dbud_rgbUserDefined[];User-defined contents
DEV_BROADCAST_USERDEFINED ENDS
PDEV_BROADCAST_USERDEFINED typedef ptr DEV_BROADCAST_USERDEFINED

