comment ^
Module Name:
    gameport.inc

Abstract:
    This module contains the common public declarations for the game port
    enumerator.

Author:
    Four-F (four-f@mail.ru)

Last update:
	06-July-2004
^

IFNDEF __GAMEPORT_H
__GAMEPORT_H equ 1

FILE_DEVICE_GAMEENUM         equ FILE_DEVICE_BUS_EXTENDER

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

; ***************************************************************************
; IOCTL interface to the bus (fdo)
;
; Clients use this to tell the enumerator what gaming devices on legacy ports
; exist.  (like for instance a control panel)
; ***************************************************************************

; Define an Interface Guid to access the game port enumerator

;DEFINE_GUID (GUID_GAMEENUM_BUS_ENUMERATOR, 0xcae56030, 0x684a, 0x11d0, 0xd6, 0xf6, 0x00, 0xa0, 0xc9, 0x0f, 0x57, 0xda);
;//  cae56030-684a-11d0-b6f6-00a0c90f57da

IOCTL_GAMEENUM_EXPOSE_HARDWARE	equ CTL_CODE (FILE_DEVICE_GAMEENUM, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_GAMEENUM_REMOVE_HARDWARE	equ CTL_CODE (FILE_DEVICE_GAMEENUM, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_GAMEENUM_PORT_DESC		equ CTL_CODE (FILE_DEVICE_GAMEENUM, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

; **************************************************************************
; Internal IOCTL interface for (pdo)
; The HID to legacy game port minidriver uses this interface to
; find the address of the device.
; **************************************************************************

IOCTL_GAMEENUM_PORT_PARAMETERS		equ CTL_CODE (FILE_DEVICE_GAMEENUM, 100h, METHOD_NEITHER, FILE_ANY_ACCESS)
IOCTL_GAMEENUM_EXPOSE_SIBLING		equ CTL_CODE (FILE_DEVICE_GAMEENUM, 101h, METHOD_NEITHER, FILE_ANY_ACCESS)
IOCTL_GAMEENUM_REMOVE_SELF			equ CTL_CODE (FILE_DEVICE_GAMEENUM, 102h, METHOD_NEITHER, FILE_ANY_ACCESS)
IOCTL_GAMEENUM_ACQUIRE_ACCESSORS	equ CTL_CODE (FILE_DEVICE_GAMEENUM, 103h, METHOD_NEITHER, FILE_ANY_ACCESS)

; Of which IO_STACK_LOCATION->Parameters.Others.Argument1 is set to
; a pointer to struct _GAMEENUM_GAME_PARAMETERS

GAMEENUM_BUTTON_1   equ 01
GAMEENUM_BUTTON_2   equ 02
GAMEENUM_BUTTON_3   equ 04
GAMEENUM_BUTTON_4   equ 08

GAMEENUM_AXIS_X     equ 10h
GAMEENUM_AXIS_Y     equ 20h
GAMEENUM_AXIS_R     equ 40h
GAMEENUM_AXIS_Z     equ 80h

ENDIF ; __GAMEPORT_H
