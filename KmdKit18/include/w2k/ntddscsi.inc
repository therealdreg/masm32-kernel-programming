
comment ^
Module Name:
    ntddscsi.inc

Abstract:
    This is the include file that defines all constants and types for
    accessing the SCSI port adapters.

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

IFNDEF _NTDDSCSIH_
_NTDDSCSIH_ equ 1

;
; Device Name - this string is the name of the device.  It is the name
; that should be passed to NtOpenFile when accessing the device.
;
; Note:  For devices that support multiple units, it should be suffixed
;        with the Ascii representation of the unit number.
;

IOCTL_SCSI_BASE	equ FILE_DEVICE_CONTROLLER

;#define DD_SCSI_DEVICE_NAME "\\Device\\ScsiPort"


;
; NtDeviceIoControlFile IoControlCode values for this device.
;
; Warning:  Remember that the low two bits of the code specify how the
;           buffers are passed to the driver!
;

IOCTL_SCSI_PASS_THROUGH         equ CTL_CODE(IOCTL_SCSI_BASE, 0401h, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)
IOCTL_SCSI_MINIPORT             equ CTL_CODE(IOCTL_SCSI_BASE, 0402h, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)
IOCTL_SCSI_GET_INQUIRY_DATA     equ CTL_CODE(IOCTL_SCSI_BASE, 0403h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_SCSI_GET_CAPABILITIES     equ CTL_CODE(IOCTL_SCSI_BASE, 0404h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_SCSI_PASS_THROUGH_DIRECT  equ CTL_CODE(IOCTL_SCSI_BASE, 0405h, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)
IOCTL_SCSI_GET_ADDRESS          equ CTL_CODE(IOCTL_SCSI_BASE, 0406h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_SCSI_RESCAN_BUS           equ CTL_CODE(IOCTL_SCSI_BASE, 0407h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_SCSI_GET_DUMP_POINTERS    equ CTL_CODE(IOCTL_SCSI_BASE, 0408h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_SCSI_FREE_DUMP_POINTERS   equ CTL_CODE(IOCTL_SCSI_BASE, 0409h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_IDE_PASS_THROUGH          equ CTL_CODE(IOCTL_SCSI_BASE, 040ah, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)

ENDIF ; _NTDDSCSIH_
