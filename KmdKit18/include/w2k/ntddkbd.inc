comment ^
Module Name:
    ntddkbd.inc

Abstract:
    This is the include file that defines all constants and types for
    accessing the keyboard device.

Author:
    Four-F (four-f@mail.ru)

Last update:
	20-November-2003
^

IFNDEF _NTDDKBD_
_NTDDKBD_ equ 1

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

; NtDeviceIoControlFile IoControlCode values for this device.
;
; Warning:  Remember that the low two bits of the code specify how the
;           buffers are passed to the driver!

IOCTL_KEYBOARD_QUERY_ATTRIBUTES      equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0000, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_KEYBOARD_SET_TYPEMATIC         equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0001, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_KEYBOARD_SET_INDICATORS        equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0002, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_KEYBOARD_QUERY_TYPEMATIC       equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0008, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_KEYBOARD_QUERY_INDICATORS      equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0010h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_KEYBOARD_QUERY_INDICATOR_TRANSLATION   equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0020h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_KEYBOARD_INSERT_DATA           equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0040h, METHOD_BUFFERED, FILE_ANY_ACCESS)

; These Device IO control query/set IME status to keyboard hardware.

IOCTL_KEYBOARD_QUERY_IME_STATUS      equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0400h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_KEYBOARD_SET_IME_STATUS        equ CTL_CODE(FILE_DEVICE_KEYBOARD, 0401h, METHOD_BUFFERED, FILE_ANY_ACCESS)

; NtReadFile Output Buffer record structures for this device.

KEYBOARD_INPUT_DATA STRUCT

	; Unit number.  E.g., for \Device\KeyboardPort0 the unit is '0',
	; for \Device\KeyboardPort1 the unit is '1', and so on.

	UnitId				WORD	?

	; The "make" scan code (key depression).

	MakeCode			WORD	?

	; The flags field indicates a "break" (key release) and other
	; miscellaneous scan code information defined below.

	Flags				WORD	?

	Reserved			WORD	?

	; Device-specific additional information for the event.

	ExtraInformation	DWORD	?

KEYBOARD_INPUT_DATA ENDS
PKEYBOARD_INPUT_DATA typedef ptr KEYBOARD_INPUT_DATA

; Define the keyboard overrun MakeCode.

KEYBOARD_OVERRUN_MAKE_CODE    equ 0FFh

; Define the keyboard input data Flags.

KEY_MAKE			equ 0
KEY_BREAK			equ 1
KEY_E0				equ 2
KEY_E1				equ 4
KEY_TERMSRV_SET_LED	equ 8

; NtDeviceIoControlFile Input/Output Buffer record structures for
; IOCTL_KEYBOARD_QUERY_TYPEMATIC/IOCTL_KEYBOARD_SET_TYPEMATIC.

KEYBOARD_TYPEMATIC_PARAMETERS STRUCT

	; Unit identifier.  Specifies the device unit for which this
	; request is intended.

	UnitId	WORD	?

	; Typematic rate, in repeats per second.

	Rate	WORD	?

	; Typematic delay, in milliseconds.

	Delay	WORD	?

KEYBOARD_TYPEMATIC_PARAMETERS ENDS
PKEYBOARD_TYPEMATIC_PARAMETERS typedef ptr KEYBOARD_TYPEMATIC_PARAMETERS

; NtDeviceIoControlFile OutputBuffer record structures for
; IOCTL_KEYBOARD_QUERY_ATTRIBUTES.

KEYBOARD_ID STRUCT
	_Type	BYTE	?	; Keyboard type
	Subtype	BYTE	?	; Keyboard subtype (OEM-dependent value)
KEYBOARD_ID ENDS
PKEYBOARD_ID typedef ptr KEYBOARD_ID

KEYBOARD_ATTRIBUTES STRUCT

	; Keyboard ID value.  Used to distinguish between keyboard types.

	KeyboardIdentifier		KEYBOARD_ID	<>

	; Scan code mode.

	KeyboardMode			WORD		?

	; Number of function keys located on the keyboard.

	NumberOfFunctionKeys	WORD		?

	; Number of LEDs located on the keyboard.

	NumberOfIndicators		WORD		?

	; Total number of keys located on the keyboard.

	NumberOfKeysTotal		WORD		?
							WORD		?	; padding

	; Length of the typeahead buffer, in bytes.

	InputDataQueueLength	DWORD		?

	; Minimum allowable values of keyboard typematic rate and delay.

	KeyRepeatMinimum		KEYBOARD_TYPEMATIC_PARAMETERS <>

	; Maximum allowable values of keyboard typematic rate and delay.

	KeyRepeatMaximum		KEYBOARD_TYPEMATIC_PARAMETERS <>

KEYBOARD_ATTRIBUTES ENDS
PKEYBOARD_ATTRIBUTES typedef ptr KEYBOARD_ATTRIBUTES
comment ^
//
// ENHANCED_KEYBOARD() is TRUE if the value for keyboard type indicates an
// Enhanced (101- or 102-key) or compatible keyboard.  The result is FALSE
// if the keyboard is an old-style AT keyboard (83- or 84- or 86-key keyboard).
//
#define ENHANCED_KEYBOARD(Id) ((Id).Type == 2 || (Id).Type == 4 || FAREAST_KEYBOARD(Id))
//
// Japanese keyboard(7) and Korean keyboard(8) are also Enhanced (101-)
// or compatible keyboard.
//
#define FAREAST_KEYBOARD(Id)  ((Id).Type == 7 || (Id).Type == 8)
^

; NtDeviceIoControlFile Input/Output Buffer record structures for
; IOCTL_KEYBOARD_QUERY_INDICATORS/IOCTL_KEYBOARD_SET_INDICATORS.

KEYBOARD_INDICATOR_PARAMETERS STRUCT

	; Unit identifier.  Specifies the device unit for which this
	; request is intended.

	UnitId		WORD	?

	; LED indicator state.

	LedFlags	WORD	?

KEYBOARD_INDICATOR_PARAMETERS ENDS
PKEYBOARD_INDICATOR_PARAMETERS typedef ptr KEYBOARD_INDICATOR_PARAMETERS

; NtDeviceIoControlFile Output Buffer record structures for
; IOCTL_KEYBOARD_QUERY_INDICATOR_TRANSLATION.

INDICATOR_LIST STRUCT

	; The "make" scan code (key depression).

	MakeCode		WORD	?

	; The associated LED indicators.

	IndicatorFlags	WORD	?

INDICATOR_LIST ENDS
PINDICATOR_LIST typedef ptr INDICATOR_LIST

KEYBOARD_INDICATOR_TRANSLATION STRUCT

	; Number of entries in IndicatorList.

	NumberOfIndicatorKeys	WORD	?

	; List of the scancode-to-indicator mappings.

	IndicatorList			INDICATOR_LIST 1 dup(<>)

KEYBOARD_INDICATOR_TRANSLATION ENDS
PKEYBOARD_INDICATOR_TRANSLATION typedef ptr KEYBOARD_INDICATOR_TRANSLATION

; Define the keyboard indicators.

KEYBOARD_LED_INJECTED     equ 8000h ; Used by Terminal Server
;//#if defined(FE_SB) || defined(WINDOWS_FE) || defined(DBCS)
KEYBOARD_KANA_LOCK_ON     equ 8		; Japanese keyboard
;//#endif // defined(FE_SB) || defined(WINDOWS_FE) || defined(DBCS)
KEYBOARD_CAPS_LOCK_ON     equ 4
KEYBOARD_NUM_LOCK_ON      equ 2
KEYBOARD_SCROLL_LOCK_ON   equ 1

; Generic NtDeviceIoControlFile Input Buffer record structure for
; various keyboard IOCTLs.

KEYBOARD_UNIT_ID_PARAMETER STRUCT

	; Unit identifier.  Specifies the device unit for which this
	; request is intended.

	UnitId	WORD	?

KEYBOARD_UNIT_ID_PARAMETER ENDS
PKEYBOARD_UNIT_ID_PARAMETER typedef ptr KEYBOARD_UNIT_ID_PARAMETER

; Define the base values for the keyboard error log packet's
; UniqueErrorValue field.

KEYBOARD_ERROR_VALUE_BASE        equ 10000

; NtDeviceIoControlFile Input/Output Buffer record structures for
; IOCTL_KEYBOARD_QUERY_IME_STATUS/IOCTL_KEYBOARD_SET_IME_STATUS.

KEYBOARD_IME_STATUS STRUCT

	; Unit identifier.  Specifies the device unit for which this
	; request is intended.

	UnitId	WORD	?
			WORD	?	; padding

	; Ime open or close status.

	ImeOpen	DWORD	?

	; Ime conversion status.

	ImeConvMode	DWORD	?

KEYBOARD_IME_STATUS ENDS
PKEYBOARD_IME_STATUS typedef ptr KEYBOARD_IME_STATUS

ENDIF ; _NTDDKBD_
