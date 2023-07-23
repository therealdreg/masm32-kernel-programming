
; See "Intel Architecture Software Developer’s Manual Volume 3: System Programming" for details

; bit 15   Determines the scaling of the segment limit field.
; bit 14   Default operation size - should always be set to 1 for 32-bit code and data segments and to 0 for 16-bit code and data segments.
; bit 13   Reserved and should always be set to 0.
; bit 12   Is available for use by system software.
; bit 8-11 Segment Limit 19:16
; bit 7    Indicates whether the segment is present in memory (set) or not present (clear).
; bit 5-6  Specifies the privilege level of the segment.
; bit 4    Descriptor type (0 = system; 1 = code or data)

; +
;  GDT descriptor entry
;
; A GDT descriptor entry is a data structure in a GDT that provides the processor with the
; size and location of a segment, as well as access control and status information.
;
; Segment limit field - Specifies the size of the segment.
;
; Base address fields - Defines the location of byte 0 of the segment within the 4-GByte linear address
; space.
; -

		; Damn! ...record field names must be unique...
		; So i prepend each field name with 'gdt'
		KGDTENTRY_FLAGS RECORD \
			gdtBaseHi:8,		; bits24-31 BaseHi
			gdtGranularity:1,	; bits23-23 Granularity
			gdtDefault_Big:1,	; bits22-22 Default_Big
			gdtReserved_0:1,	; bits21-21 Reserved_0
			gdtSys:1,			; bits20-20 Sys
			gdtLimitHi:4,		; bits16-19 LimitHi
			gdtPres:1,			; bits15-15 Pres
			gdtDpl:2,			; bits13-14 Dpl
			gdtType:5,			; bits8-12  Type
			gdtBaseMid:8		; bits0-7   BaseMid

KGDTENTRY STRUCT		; sizeof = 8
	LimitLow		WORD	?
	BaseLow			WORD	?
	union _HighWord		; original HighWord
		struct Bytes
			BaseMid	BYTE	?
			Flags1	BYTE	?
			Flags2	BYTE	?
			BaseHi	BYTE	?
		ends
		gdtBits		KGDTENTRY_FLAGS		<>
	ends ; HighWord
KGDTENTRY ENDS
PKGDTENTRY typedef PTR KGDTENTRY
