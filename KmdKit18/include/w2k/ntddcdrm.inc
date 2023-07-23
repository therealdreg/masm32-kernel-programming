
comment ^
Module Name:
    ntddcdrm.inc

Abstract:
    This module contains structures and definitions
    associated with CDROM IOCTls.

Author:
    Four-F (four-f@mail.ru)

Last updated:
	08-July-2004
^

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

; begin_winioctl

IFNDEF _NTDDCDRM_
_NTDDCDRM_ equ 1

;
; NtDeviceIoControlFile IoControlCode values for this device.
;
; Warning:  Remember that the low two bits of the code specify how the
;           buffers are passed to the driver!
;

IOCTL_CDROM_BASE	equ FILE_DEVICE_CD_ROM

IOCTL_CDROM_UNLOAD_DRIVER    equ CTL_CODE(IOCTL_CDROM_BASE, 0402h, METHOD_BUFFERED, FILE_READ_ACCESS)

; CDROM Audio Device Control Functions

IOCTL_CDROM_READ_TOC         equ CTL_CODE(IOCTL_CDROM_BASE, 0000, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_GET_CONTROL      equ CTL_CODE(IOCTL_CDROM_BASE, 000Dh, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_PLAY_AUDIO_MSF   equ CTL_CODE(IOCTL_CDROM_BASE, 0006, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_SEEK_AUDIO_MSF   equ CTL_CODE(IOCTL_CDROM_BASE, 0001, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_STOP_AUDIO       equ CTL_CODE(IOCTL_CDROM_BASE, 0002, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_PAUSE_AUDIO      equ CTL_CODE(IOCTL_CDROM_BASE, 0003, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_RESUME_AUDIO     equ CTL_CODE(IOCTL_CDROM_BASE, 0004, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_GET_VOLUME       equ CTL_CODE(IOCTL_CDROM_BASE, 0005, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_SET_VOLUME       equ CTL_CODE(IOCTL_CDROM_BASE, 000Ah, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_READ_Q_CHANNEL   equ CTL_CODE(IOCTL_CDROM_BASE, 000Bh, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_GET_LAST_SESSION equ CTL_CODE(IOCTL_CDROM_BASE, 000Eh, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_RAW_READ         equ CTL_CODE(IOCTL_CDROM_BASE, 000Fh, METHOD_OUT_DIRECT, FILE_READ_ACCESS)
IOCTL_CDROM_DISK_TYPE        equ CTL_CODE(IOCTL_CDROM_BASE, 0010h, METHOD_BUFFERED, FILE_ANY_ACCESS)

IOCTL_CDROM_GET_DRIVE_GEOMETRY equ CTL_CODE(IOCTL_CDROM_BASE, 0013h, METHOD_BUFFERED, FILE_READ_ACCESS)

; end_winioctl

;
; The following device control codes are common for all class drivers.  The
; functions codes defined here must match all of the other class drivers.
;
; Warning: these codes will be replaced in the future with the IOCTL_STORAGE
; codes included below
;

IOCTL_CDROM_CHECK_VERIFY    equ CTL_CODE(IOCTL_CDROM_BASE, 0200h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_MEDIA_REMOVAL   equ CTL_CODE(IOCTL_CDROM_BASE, 0201h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_EJECT_MEDIA     equ CTL_CODE(IOCTL_CDROM_BASE, 0202h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_LOAD_MEDIA      equ CTL_CODE(IOCTL_CDROM_BASE, 0203h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_RESERVE         equ CTL_CODE(IOCTL_CDROM_BASE, 0204h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_RELEASE         equ CTL_CODE(IOCTL_CDROM_BASE, 0205h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CDROM_FIND_NEW_DEVICES equ CTL_CODE(IOCTL_CDROM_BASE, 0206h, METHOD_BUFFERED, FILE_READ_ACCESS)

; The following file contains the IOCTL_STORAGE class ioctl definitions

include ntddstor.inc

; begin_winioctl

; The following device control code is for the SIMBAD simulated bad
; sector facility. See SIMBAD.H in this directory for related structures.

IOCTL_CDROM_SIMBAD        equ CTL_CODE(IOCTL_CDROM_BASE, 01003h, METHOD_BUFFERED, FILE_READ_ACCESS)

; Maximum CD Rom size

MAXIMUM_NUMBER_TRACKS	equ 100
MAXIMUM_CDROM_SIZE		equ 804

;
; CD ROM Table OF Contents (TOC)
;
; Format 0 - Get table of contents
;

; Damn! ...record field names must be unique...
; So i prepend names with 'td'
tdBits RECORD \
	tdAdr:4,			; bits4-7  Adr
	tdControl:4			; bits0-3  Control
			
TRACK_DATA STRUCT				; sizeof = 8
	Reserved	BYTE	?		; 000h
	tdBits		<>				; 001h
	TrackNumber	BYTE	?		; 002h
	Reserved1	BYTE	?		; 003h
	Address		BYTE 4 dup(?)	; 004h
TRACK_DATA ENDS
PTRACK_DATA typedef ptr TRACK_DATA

CDROM_TOC STRUCT		; sizeof = 0324h

	; Header

	_Length	BYTE 2 dup(?)	; 000h  original name Length
	FirstTrack	BYTE	?	; 002h
	LastTrack	BYTE	?	; 003h

	; Track data

	TrackData	TRACK_DATA	MAXIMUM_NUMBER_TRACKS dup(<?>)		; 004h
CDROM_TOC ENDS
PCDROM_TOC typedef ptr CDROM_TOC

CDROM_TOC_SIZE	equ sizeof CDROM_TOC

; Play audio starting at MSF and ending at MSF

CDROM_PLAY_AUDIO_MSF STRUCT	; sizeof = 6
	StartingM	BYTE	?	; 000
	StartingS	BYTE	?	; 001
	StartingF	BYTE	?	; 002
	EndingM		BYTE	?	; 003
	EndingS		BYTE	?	; 004
	EndingF		BYTE	?	; 005
CDROM_PLAY_AUDIO_MSF ENDS
PCDROM_PLAY_AUDIO_MSF typedef ptr CDROM_PLAY_AUDIO_MSF

; Seek to MSF

CDROM_SEEK_AUDIO_MSF STRUCT	; sizeof = 3
	M			BYTE	?	; 000
	S			BYTE	?	; 001
	F			BYTE	?	; 002
CDROM_SEEK_AUDIO_MSF ENDS
PCDROM_SEEK_AUDIO_MSF typedef ptr CDROM_SEEK_AUDIO_MSF

ENDIF ; _NTDDCDRM_

; end_winioctl
