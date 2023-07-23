
comment ^
Module Name:
    guiddef.inc

Abstract:
    GUID definition

Author:
    Four-F (four-f@mail.ru)

Last update:
	27-November-2004

^

IFNDEF GUID_DEFINED
GUID_DEFINED equ 1

GUID STRUCT
	Data1	DWORD	?
	Data2	WORD	?
	Data3	WORD	?
	Data4	BYTE 8 dup(?)
GUID ENDS
PGUID typedef ptr GUID

DEFINE_GUID MACRO name:REQ, l:REQ, w1:REQ, w2:REQ, b1:REQ, b2:REQ, b3:REQ, b4:REQ, b5:REQ, b6:REQ, b7:REQ, b8:REQ

	local segn

	segn TEXTEQU @CurSeg
	
	.const
		ALIGN 4
		name	dd l
				dw w1
				dw w2
				db b1
				db b2
				db b3
				db b4
				db b5
				db b6
				db b7
				db b8

	@CurSeg ENDS
	segn SEGMENT

ENDM

ENDIF ; GUID_DEFINED

