comment ^
Module Name:
    mountdev.inc

Abstract:
    This file defines the private interfaces between the mount point manager
    and the mounted devices.

Author:
    Four-F (four-f@mail.ru)

Last update:
	30-June-2004

Comments:
    Some fields name have been changed because of collision with masm reserved words.
    All such words are prefixed with underscore ('_') symbol.
^

IFNDEF _MOUNTDEV_
_MOUNTDEV_ equ 1

include mountmgr.inc

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

IOCTL_MOUNTDEV_QUERY_UNIQUE_ID              equ CTL_CODE(MOUNTDEVCONTROLTYPE, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_MOUNTDEV_UNIQUE_ID_CHANGE_NOTIFY      equ CTL_CODE(MOUNTDEVCONTROLTYPE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_MOUNTDEV_QUERY_SUGGESTED_LINK_NAME    equ CTL_CODE(MOUNTDEVCONTROLTYPE, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_MOUNTDEV_LINK_CREATED                 equ CTL_CODE(MOUNTDEVCONTROLTYPE, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_MOUNTDEV_LINK_DELETED                 equ CTL_CODE(MOUNTDEVCONTROLTYPE, 5, METHOD_BUFFERED, FILE_ANY_ACCESS)

ENDIF ; _MOUNTDEV_
