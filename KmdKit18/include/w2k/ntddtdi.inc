
comment ^
Module Name:
    ntddtdi.inc

Abstract:
    This is the include file that defines all constants and types for
    accessing the Transport driver interface device.

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

IFNDEF _NTDDTDI_
_NTDDTDI_ equ 1

;
; Device Name - this string is the name of the device.  It is the name
; that should be passed to NtOpenFile when accessing the device.
;
; Note:  For devices that support multiple units, it should be suffixed
;        with the Ascii representation of the unit number.
;

;#define DD_TDI_DEVICE_NAME "\\Device\\UNKNOWN"

;
; NtDeviceIoControlFile IoControlCode values for this device.
;
; Warning:  Remember that the low two bits of the code specify how the
;           buffers are passed to the driver!
;

IOCTL_TDI_ACCEPT                equ CTL_CODE(FILE_DEVICE_TRANSPORT, 0, METHOD_BUFFERED , FILE_ANY_ACCESS)
IOCTL_TDI_CONNECT               equ CTL_CODE(FILE_DEVICE_TRANSPORT, 1, METHOD_BUFFERED , FILE_ANY_ACCESS)
IOCTL_TDI_DISCONNECT            equ CTL_CODE(FILE_DEVICE_TRANSPORT, 2, METHOD_BUFFERED , FILE_ANY_ACCESS)
IOCTL_TDI_LISTEN                equ CTL_CODE(FILE_DEVICE_TRANSPORT, 3, METHOD_BUFFERED , FILE_ANY_ACCESS)
IOCTL_TDI_QUERY_INFORMATION     equ CTL_CODE(FILE_DEVICE_TRANSPORT, 4, METHOD_OUT_DIRECT , FILE_ANY_ACCESS)
IOCTL_TDI_RECEIVE               equ CTL_CODE(FILE_DEVICE_TRANSPORT, 5, METHOD_OUT_DIRECT , FILE_ANY_ACCESS)
IOCTL_TDI_RECEIVE_DATAGRAM      equ CTL_CODE(FILE_DEVICE_TRANSPORT, 6, METHOD_OUT_DIRECT , FILE_ANY_ACCESS)
IOCTL_TDI_SEND                  equ CTL_CODE(FILE_DEVICE_TRANSPORT, 7, METHOD_IN_DIRECT , FILE_ANY_ACCESS)
IOCTL_TDI_SEND_DATAGRAM         equ CTL_CODE(FILE_DEVICE_TRANSPORT, 8, METHOD_IN_DIRECT , FILE_ANY_ACCESS)
IOCTL_TDI_SET_EVENT_HANDLER     equ CTL_CODE(FILE_DEVICE_TRANSPORT, 9, METHOD_BUFFERED , FILE_ANY_ACCESS)
IOCTL_TDI_SET_INFORMATION       equ CTL_CODE(FILE_DEVICE_TRANSPORT, 10, METHOD_IN_DIRECT , FILE_ANY_ACCESS)
IOCTL_TDI_ASSOCIATE_ADDRESS     equ CTL_CODE(FILE_DEVICE_TRANSPORT, 11, METHOD_BUFFERED , FILE_ANY_ACCESS)
IOCTL_TDI_DISASSOCIATE_ADDRESS  equ CTL_CODE(FILE_DEVICE_TRANSPORT, 12, METHOD_BUFFERED , FILE_ANY_ACCESS)
IOCTL_TDI_ACTION                equ CTL_CODE(FILE_DEVICE_TRANSPORT, 13, METHOD_OUT_DIRECT , FILE_ANY_ACCESS)

ENDIF ; _NTDDTDI_

