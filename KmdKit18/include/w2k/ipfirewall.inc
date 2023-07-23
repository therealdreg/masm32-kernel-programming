comment ^
Module Name:
    ipfirewall.inc

Abstract:
    Header file for IP firewall hook clients.

Author:
    Four-F (four-f@mail.ru)

Last update:
	24-September-2004
^

INVALID_IF_INDEX	equ 0ffffffffh	; The invalid inteface index.

LOCAL_IF_INDEX		equ 0			; Local inteface index.

; Indicates whether it is a transmitted or received packet.

;typedef enum _IP_DIRECTION_E {
	IP_TRANSMIT	equ 0
	IP_RECEIVE	equ 1

FIREWALL_CONTEXT_T STRUCT	; sizeof = 14h
	Direction	DWORD	?	; 000h DIRECTION_E
	NTE			PVOID	?	; 004h
	LinkCtxt	PVOID	?	; 008h
	LContext1	DWORD	?	; 00Ch NDIS_HANDLE
	LContext2	DWORD	?	; 010h
FIREWALL_CONTEXT_T ENDS
PFIREWALL_CONTEXT_T typedef ptr FIREWALL_CONTEXT_T

;  Definition of an IP receive buffer chain.

IPRcvBuf STRUCT					; sizeof = 24h
	ipr_next		PVOID	?	; 000h PTR IPRcvBuf  Next buffer descriptor in chain.
	ipr_owner		DWORD	?	; 004h               Owner of buffer.
	ipr_buffer		PVOID	?	; 008h PTR UCHAR     Pointer to buffer.
	ipr_size		DWORD	?	; 00Ch               Buffer size.
	ipr_pMdl		PVOID	?	; 010h PMDL
	ipr_pClientCnt	PVOID	?	; 014h PTR UINT
	ipr_RcvContext	PVOID	?	; 018h PTR UCHAR
	ipr_RcvOffset	DWORD	?	; 01Ch
	ipr_promiscuous	DWORD	?	; 020h               promiscuousmode or not
IPRcvBuf ENDS

; Enum for values that may be returned from filter routine.

;typedef enum _FORWARD_ACTION {
	FORWARD			equ 0
	DROP			equ 1
	ICMP_ON_DROP	equ 2

;typedef enum _IPROUTEINFOCLASS {
	IPRouteNoInformation			equ 0
	IPRouteOutgoingFirewallContext	equ 1
	IPRouteOutgoingFilterContext	equ 2
	MaxIPRouteInfoClass				equ 3

; Structure passed to the IPSetFirewallHook call

IP_SET_FIREWALL_HOOK_INFO STRUCT	; sizeof = 0Ch
	FirewallPtr	PVOID	?	; 000h 	IPPacketFirewallPtr      Packet filter callout.
	Priority	DWORD	?	; 004h                           Priority of the hook
	_Add		BOOLEAN	?	; 008h                           if TRUE then ADD else DELETE
				db 3 dup(?)	; 009h pading
IP_SET_FIREWALL_HOOK_INFO ENDS
PIP_SET_FIREWALL_HOOK_INFO typedef ptr IP_SET_FIREWALL_HOOK_INFO

DEST_LOCAL      equ 0		; Destination is local.
DEST_BCAST      equ 1		; Destination is net or local bcast.
DEST_SN_BCAST   equ 3		; A subnet bcast.
DEST_MCAST      equ 5		; A local mcast.
DEST_REMOTE     equ 8		; Destination is remote.
DEST_REM_BCAST  equ 11		; Destination is a remote broadcast
DEST_REM_MCAST  equ 13		; Destination is a remote mcast.
DEST_INVALID    equ 0ffh	; Invalid destination

DEST_PROMIS     equ 32		; Dest is promiscuous(32=0x20)

DEST_BCAST_BIT  equ 1
DEST_OFFNET_BIT equ 10h		; Destination is offnet -
							; used only by upper layer callers.
DEST_MCAST_BIT  equ 5

;#define DD_IP_DEVICE_NAME   L"\\Device\\Ip"

FSCTL_IP_BASE	equ FILE_DEVICE_NETWORK

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

IOCTL_IP_SET_FIREWALL_HOOK	equ CTL_CODE(FSCTL_IP_BASE, 12, METHOD_BUFFERED, FILE_WRITE_ACCESS)


