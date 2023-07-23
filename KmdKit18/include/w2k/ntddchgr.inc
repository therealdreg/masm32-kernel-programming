
comment ^
Module Name:
    ntddchgr.inc

Abstract:
    This is the include file that defines all constants and types for
    accessing medium changer devices.

Author:
    Four-F (four-f@mail.ru)

Last update:
	07-July-2004

^

IFNDEF _NTDDCHGR_H_
_NTDDCHGR_H_ equ 1

;
; Device Name - this string is the name of the device.  It is the name
; that should be passed to NtOpenFile when accessing the device.
;
; Note:  For devices that support multiple units, it should be suffixed
;        with the Ascii representation of the unit number.
;

;#define DD_CHANGER_DEVICE_NAME "\\Device\\Changer"

; NtDeviceIoControlFile IoControlCode values for changer devices.

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

; begin_winioctl

IOCTL_CHANGER_BASE	equ FILE_DEVICE_CHANGER

IOCTL_CHANGER_GET_PARAMETERS         equ CTL_CODE(IOCTL_CHANGER_BASE, 0000, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CHANGER_GET_STATUS             equ CTL_CODE(IOCTL_CHANGER_BASE, 0001, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CHANGER_GET_PRODUCT_DATA       equ CTL_CODE(IOCTL_CHANGER_BASE, 0002, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CHANGER_SET_ACCESS             equ CTL_CODE(IOCTL_CHANGER_BASE, 0004, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)
IOCTL_CHANGER_GET_ELEMENT_STATUS     equ CTL_CODE(IOCTL_CHANGER_BASE, 0005, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)
IOCTL_CHANGER_INITIALIZE_ELEMENT_STATUS  equ CTL_CODE(IOCTL_CHANGER_BASE, 0006, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CHANGER_SET_POSITION           equ CTL_CODE(IOCTL_CHANGER_BASE, 0007, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CHANGER_EXCHANGE_MEDIUM        equ CTL_CODE(IOCTL_CHANGER_BASE, 0008, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CHANGER_MOVE_MEDIUM            equ CTL_CODE(IOCTL_CHANGER_BASE, 0009, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CHANGER_REINITIALIZE_TRANSPORT equ CTL_CODE(IOCTL_CHANGER_BASE, 000Ah, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_CHANGER_QUERY_VOLUME_TAGS      equ CTL_CODE(IOCTL_CHANGER_BASE, 000Bh, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)

; end_winioctl

; The following file contains the IOCTL_STORAGE class ioctls

include ntddstor.inc

; begin_winioctl

MAX_VOLUME_ID_SIZE       equ 36
MAX_VOLUME_TEMPLATE_SIZE equ 40

VENDOR_ID_LENGTH         equ  8
PRODUCT_ID_LENGTH        equ 16
REVISION_LENGTH          equ  4
SERIAL_NUMBER_LENGTH     equ 32

ENDIF ; _NTDDCHGR_H_


