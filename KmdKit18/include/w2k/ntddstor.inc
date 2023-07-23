comment ^
Module Name:
    ntddstor.inc

Abstract:
    This is the include file that defines all common constants and types
    accessing the storage class drivers

Author:
    Four-F (four-f@mail.ru)

Last updated:
	29-December-2004
^

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

comment ^
//
// Interface GUIDs
//

DEFINE_GUID(DiskClassGuid,               0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(CdRomClassGuid,              0x53f56308L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(PartitionClassGuid,          0x53f5630aL, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(TapeClassGuid,               0x53f5630bL, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(WriteOnceDiskClassGuid,      0x53f5630cL, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(VolumeClassGuid,             0x53f5630dL, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(MediumChangerClassGuid,      0x53f56310L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(FloppyClassGuid,             0x53f56311L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(CdChangerClassGuid,          0x53f56312L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
DEFINE_GUID(StoragePortClassGuid,        0x2accfe60L, 0xc130, 0x11d2, 0xb0, 0x82, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
#endif
^
; begin_winioctl

IFNDEF _NTDDSTOR_H_
_NTDDSTOR_H_ equ 1

; IoControlCode values for storage devices

IOCTL_STORAGE_BASE equ FILE_DEVICE_MASS_STORAGE

; The following device control codes are common for all class drivers.  They
; should be used in place of the older IOCTL_DISK, IOCTL_CDROM and IOCTL_TAPE
; common codes

IOCTL_STORAGE_CHECK_VERIFY     equ CTL_CODE(IOCTL_STORAGE_BASE, 0200h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_STORAGE_CHECK_VERIFY2    equ CTL_CODE(IOCTL_STORAGE_BASE, 0200h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_STORAGE_MEDIA_REMOVAL    equ CTL_CODE(IOCTL_STORAGE_BASE, 0201h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_STORAGE_EJECT_MEDIA      equ CTL_CODE(IOCTL_STORAGE_BASE, 0202h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_STORAGE_LOAD_MEDIA       equ CTL_CODE(IOCTL_STORAGE_BASE, 0203h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_STORAGE_LOAD_MEDIA2      equ CTL_CODE(IOCTL_STORAGE_BASE, 0203h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_STORAGE_RESERVE          equ CTL_CODE(IOCTL_STORAGE_BASE, 0204h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_STORAGE_RELEASE          equ CTL_CODE(IOCTL_STORAGE_BASE, 0205h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_STORAGE_FIND_NEW_DEVICES equ CTL_CODE(IOCTL_STORAGE_BASE, 0206h, METHOD_BUFFERED, FILE_READ_ACCESS)

IOCTL_STORAGE_EJECTION_CONTROL equ CTL_CODE(IOCTL_STORAGE_BASE, 0250h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_STORAGE_MCN_CONTROL      equ CTL_CODE(IOCTL_STORAGE_BASE, 0251h, METHOD_BUFFERED, FILE_ANY_ACCESS)

IOCTL_STORAGE_GET_MEDIA_TYPES  equ CTL_CODE(IOCTL_STORAGE_BASE, 0300h, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_STORAGE_GET_MEDIA_TYPES_EX equ CTL_CODE(IOCTL_STORAGE_BASE, 0301h, METHOD_BUFFERED, FILE_ANY_ACCESS)

IOCTL_STORAGE_RESET_BUS        equ CTL_CODE(IOCTL_STORAGE_BASE, 0400h, METHOD_BUFFERED, FILE_READ_ACCESS)
IOCTL_STORAGE_RESET_DEVICE     equ CTL_CODE(IOCTL_STORAGE_BASE, 0401h, METHOD_BUFFERED, FILE_READ_ACCESS)

IOCTL_STORAGE_GET_DEVICE_NUMBER equ CTL_CODE(IOCTL_STORAGE_BASE, 0420h, METHOD_BUFFERED, FILE_ANY_ACCESS)

IOCTL_STORAGE_PREDICT_FAILURE equ CTL_CODE(IOCTL_STORAGE_BASE, 0440h, METHOD_BUFFERED, FILE_ANY_ACCESS)

; end_winioctl


IOCTL_STORAGE_QUERY_PROPERTY   equ CTL_CODE(IOCTL_STORAGE_BASE, 0500h, METHOD_BUFFERED, FILE_ANY_ACCESS)


; begin_winioctl

; These ioctl codes are obsolete.  They are defined here to avoid resuing them
; and to allow class drivers to respond to them more easily.

OBSOLETE_IOCTL_STORAGE_RESET_BUS        equ CTL_CODE(IOCTL_STORAGE_BASE, 0400h, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)
OBSOLETE_IOCTL_STORAGE_RESET_DEVICE     equ CTL_CODE(IOCTL_STORAGE_BASE, 0401h, METHOD_BUFFERED, FILE_READ_ACCESS or FILE_WRITE_ACCESS)


; +
; IOCTL_STORAGE_GET_DEVICE_NUMBER
;
; input - none
;
; output - STORAGE_DEVICE_NUMBER structure
;          The values in the STORAGE_DEVICE_NUMBER structure are guaranteed
;          to remain unchanged until the system is rebooted.  They are not
;          guaranteed to be persistant across boots.
; -

STORAGE_DEVICE_NUMBER STRUCT

	; The FILE_DEVICE_XXX type for this device.

	DeviceType	DWORD	?	; DEVICE_TYPE

	; The number of this device

	DeviceNumber	DWORD	?

	; If the device is partitionable, the partition number of the device.
	; Otherwise -1

	PartitionNumber	DWORD	?
STORAGE_DEVICE_NUMBER ENDS
PSTORAGE_DEVICE_NUMBER typedef ptr STORAGE_DEVICE_NUMBER

; Define the structures for scsi resets

STORAGE_BUS_RESET_REQUEST STRUCT	; sizeof = 1
	PathId	BYTE	?
STORAGE_BUS_RESET_REQUEST ENDS
PSTORAGE_BUS_RESET_REQUEST typedef ptr STORAGE_BUS_RESET_REQUEST

; +
; IOCTL_STORAGE_MEDIA_REMOVAL disables the mechanism
; on a storage device that ejects media. This function
; may or may not be supported on storage devices that
; support removable media.
;
; TRUE means prevent media from being removed.
; FALSE means allow media removal.
; -

PREVENT_MEDIA_REMOVAL STRUCT	; sizeof = 1
	PreventMediaRemoval	BOOLEAN	?
PREVENT_MEDIA_REMOVAL ENDS
PPREVENT_MEDIA_REMOVAL typedef ptr PREVENT_MEDIA_REMOVAL

; begin_ntminitape
comment ^
typedef struct _TAPE_STATISTICS {
    ULONG Version;
    ULONG Flags;
    LARGE_INTEGER RecoveredWrites;
    LARGE_INTEGER UnrecoveredWrites;
    LARGE_INTEGER RecoveredReads;
    LARGE_INTEGER UnrecoveredReads;
    UCHAR         CompressionRatioReads;
    UCHAR         CompressionRatioWrites;
} TAPE_STATISTICS, *PTAPE_STATISTICS;
^
RECOVERED_WRITES_VALID			equ 00000001
UNRECOVERED_WRITES_VALID		equ 00000002
RECOVERED_READS_VALID			equ 00000004
UNRECOVERED_READS_VALID			equ 00000008h
WRITE_COMPRESSION_INFO_VALID	equ 00000010h
READ_COMPRESSION_INFO_VALID		equ 00000020h

TAPE_GET_STATISTICS STRUCT		; sizeof = 4
	Operation	DWORD	?
TAPE_GET_STATISTICS ENDS
PTAPE_GET_STATISTICS typedef ptr TAPE_GET_STATISTICS

TAPE_RETURN_STATISTICS equ 0
TAPE_RETURN_ENV_INFO   equ 1
TAPE_RESET_STATISTICS  equ 2

; IOCTL_STORAGE_GET_MEDIA_TYPES_EX will return an array of DEVICE_MEDIA_INFO
; structures, one per supported type, embedded in the GET_MEDIA_TYPES struct.

;typedef enum _STORAGE_MEDIA_TYPE {

	; Following are defined in ntdddisk.h in the MEDIA_TYPE enum

    ; Unknown,                // Format is unknown
    ; F5_1Pt2_512,            // 5.25", 1.2MB,  512 bytes/sector
    ; F3_1Pt44_512,           // 3.5",  1.44MB, 512 bytes/sector
    ; F3_2Pt88_512,           // 3.5",  2.88MB, 512 bytes/sector
    ; F3_20Pt8_512,           // 3.5",  20.8MB, 512 bytes/sector
    ; F3_720_512,             // 3.5",  720KB,  512 bytes/sector
    ; F5_360_512,             // 5.25", 360KB,  512 bytes/sector
    ; F5_320_512,             // 5.25", 320KB,  512 bytes/sector
    ; F5_320_1024,            // 5.25", 320KB,  1024 bytes/sector
    ; F5_180_512,             // 5.25", 180KB,  512 bytes/sector
    ; F5_160_512,             // 5.25", 160KB,  512 bytes/sector
    ; RemovableMedia,         // Removable media other than floppy
    ; FixedMedia,             // Fixed hard disk media
    ; F3_120M_512,            // 3.5", 120M Floppy
    ; F3_640_512,             // 3.5" ,  640KB,  512 bytes/sector
    ; F5_640_512,             // 5.25",  640KB,  512 bytes/sector
    ; F5_720_512,             // 5.25",  720KB,  512 bytes/sector
    ; F3_1Pt2_512,            // 3.5" ,  1.2Mb,  512 bytes/sector
    ; F3_1Pt23_1024,          // 3.5" ,  1.23Mb, 1024 bytes/sector
    ; F5_1Pt23_1024,          // 5.25",  1.23MB, 1024 bytes/sector
    ; F3_128Mb_512,           // 3.5" MO 128Mb   512 bytes/sector
    ; F3_230Mb_512,           // 3.5" MO 230Mb   512 bytes/sector
    ; F8_256_128,             // 8",     256KB,  128 bytes/sector

    DDS_4mm				equ 020h	; Tape - DAT DDS1,2,... (all vendors)
    MiniQic				equ 021h	; Tape - miniQIC Tape
    Travan				equ 022h	; Tape - Travan TR-1,2,3,...
    QIC					equ 023h	; Tape - QIC
    MP_8mm				equ 024h	; Tape - 8mm Exabyte Metal Particle
    AME_8mm				equ 025h	; Tape - 8mm Exabyte Advanced Metal Evap
    AIT1_8mm			equ 026h	; Tape - 8mm Sony AIT1
    DLT					equ 027h	; Tape - DLT Compact IIIxt, IV
    NCTP				equ 028h	; Tape - Philips NCTP
    IBM_3480			equ 029h	; Tape - IBM 3480
    IBM_3490E			equ 02Ah	; Tape - IBM 3490E
    IBM_Magstar_3590	equ 02Bh	; Tape - IBM Magstar 3590
    IBM_Magstar_MP		equ 02Ch	; Tape - IBM Magstar MP
    STK_DATA_D3			equ 02Dh	; Tape - STK Data D3
    SONY_DTF			equ 02Eh	; Tape - Sony DTF
    DV_6mm				equ 02Fh	; Tape - 6mm Digital Video
    DMI					equ 030h	; Tape - Exabyte DMI and compatibles
    SONY_D2				equ 031h	; Tape - Sony D2S and D2L
    CLEANER_CARTRIDGE	equ 032h	; Cleaner - All Drive types that support Drive Cleaners
    CD_ROM				equ 033h	; Opt_Disk - CD
    CD_R				equ 034h	; Opt_Disk - CD-Recordable (Write Once)
    CD_RW				equ 035h	; Opt_Disk - CD-Rewriteable
    DVD_ROM				equ 036h	; Opt_Disk - DVD-ROM
    DVD_R				equ 037h	; Opt_Disk - DVD-Recordable (Write Once)
    DVD_RW				equ 038h	; Opt_Disk - DVD-Rewriteable
    MO_3_RW				equ 039h	; Opt_Disk - 3.5" Rewriteable MO Disk
    MO_5_WO				equ 03Ah	; Opt_Disk - MO 5.25" Write Once
    MO_5_RW				equ 03Bh	; Opt_Disk - MO 5.25" Rewriteable (not LIMDOW)
    MO_5_LIMDOW			equ 03Ch	; Opt_Disk - MO 5.25" Rewriteable (LIMDOW)
    PC_5_WO				equ 03Dh	; Opt_Disk - Phase Change 5.25" Write Once Optical
    PC_5_RW				equ 03Eh	; Opt_Disk - Phase Change 5.25" Rewriteable
    PD_5_RW				equ 03Fh	; Opt_Disk - PhaseChange Dual Rewriteable
    ABL_5_WO			equ 040h	; Opt_Disk - Ablative 5.25" Write Once Optical
    PINNACLE_APEX_5_RW	equ 041h	; Opt_Disk - Pinnacle Apex 4.6GB Rewriteable Optical
    SONY_12_WO			equ 042h	; Opt_Disk - Sony 12" Write Once
    PHILIPS_12_WO		equ 043h	; Opt_Disk - Philips/LMS 12" Write Once
    HITACHI_12_WO		equ 044h	; Opt_Disk - Hitachi 12" Write Once
    CYGNET_12_WO		equ 045h	; Opt_Disk - Cygnet/ATG 12" Write Once
    KODAK_14_WO			equ 046h	; Opt_Disk - Kodak 14" Write Once
    MO_NFR_525			equ 047h	; Opt_Disk - Near Field Recording (Terastor)
    NIKON_12_RW			equ 048h	; Opt_Disk - Nikon 12" Rewriteable
    IOMEGA_ZIP			equ 049h	; Mag_Disk - Iomega Zip
    IOMEGA_JAZ			equ 04Ah	; Mag_Disk - Iomega Jaz
    SYQUEST_EZ135		equ 04Bh	; Mag_Disk - Syquest EZ135
    SYQUEST_EZFLYER		equ 04Ch	; Mag_Disk - Syquest EzFlyer
    SYQUEST_SYJET		equ 04Dh	; Mag_Disk - Syquest SyJet
    AVATAR_F2			equ 04Eh	; Mag_Disk - 2.5" Floppy
    MP2_8mm				equ 04Fh	; Tape - 8mm Hitachi
    DST_S				equ 050h	; Ampex DST Small Tapes
    DST_M				equ 051h	; Ampex DST Medium Tapes
    DST_L				equ 052h	; Ampex DST Large Tapes
    VXATape_1			equ 053h	; Ecrix 8mm Tape
    VXATape_2			equ 054h	; Ecrix 8mm Tape
    STK_EAGLE			equ 055h	; STK Eagle
    LTO_Ultrium			equ 056h	; IBM, HP, Seagate LTO Ultrium
    LTO_Accelis			equ 057h	; IBM, HP, Seagate LTO Accelis
    DVD_RAM				equ 058h	; Opt_Disk - DVD-RAM
    AIT_8mm				equ 059h	; AIT2 or higher
    ADR_1				equ 05Ah	; 
    ADR_2				equ 05Bh	; 
    STK_9940			equ 05Ch	; STK 9940

MEDIA_ERASEABLE         equ 00000001
MEDIA_WRITE_ONCE        equ 00000002
MEDIA_READ_ONLY         equ 00000004
MEDIA_READ_WRITE        equ 00000008h

MEDIA_WRITE_PROTECTED   equ 00000100h
MEDIA_CURRENTLY_MOUNTED equ 80000000h

; Define the different storage bus types
; Bus types below 128 (0x80) are reserved for Microsoft use

;typedef enum _STORAGE_BUS_TYPE {
    BusTypeUnknown			equ 0
    BusTypeScsi				equ 1
    BusTypeAtapi			equ 2
    BusTypeAta				equ 3
    BusType1394				equ 4
    BusTypeSsa				equ 5
    BusTypeFibre			equ 6
    BusTypeUsb				equ 7
    BusTypeRAID				equ 8
    BusTypeMaxReserved		equ 07Fh

; Property Query Structures

; +
; IOCTL_STORAGE_QUERY_PROPERTY
;
; Input Buffer:
;      a STORAGE_PROPERTY_QUERY structure which describes what type of query
;      is being done, what property is being queried for, and any additional
;      parameters which a particular property query requires.
;
;  Output Buffer:
;      Contains a buffer to place the results of the query into.  Since all
;      property descriptors can be cast into a STORAGE_DESCRIPTOR_HEADER,
;      the IOCTL can be called once with a small buffer then again using
;      a buffer as large as the header reports is necessary.
; -

; Types of queries

;typedef enum _STORAGE_QUERY_TYPE {
    PropertyStandardQuery		equ 0	; Retrieves the descriptor
    PropertyExistsQuery			equ 1	; Used to test whether the descriptor is supported
    PropertyMaskQuery			equ 2	; Used to retrieve a mask of writeable fields in the descriptor
    PropertyQueryMaxDefined		equ 3	; use to validate the value


; define some initial property id's

;typedef enum _STORAGE_PROPERTY_ID {
    StorageDeviceProperty		equ 0
    StorageAdapterProperty		equ 1

; Query structure - additional parameters for specific queries can follow
; the header

STORAGE_PROPERTY_QUERY STRUCT			; sizeof = 0Ch

	; ID of the property being retrieved

	PropertyId				DWORD	?	; STORAGE_PROPERTY_ID

	; Flags indicating the type of query being performed

	QueryType				DWORD	?	; STORAGE_QUERY_TYPE

	; Space for additional parameters if necessary

	AdditionalParameters	db 1 dup(?)

							db 3 dup(?)	; padding

STORAGE_PROPERTY_QUERY ENDS
PSTORAGE_PROPERTY_QUERY typedef ptr STORAGE_PROPERTY_QUERY

; Standard property descriptor header.  All property pages should use this
; as their first element or should contain these two elements

STORAGE_DESCRIPTOR_HEADER STRUCT
	Version		DWORD	?
	_Size		DWORD	?
STORAGE_DESCRIPTOR_HEADER ENDS
PSTORAGE_DESCRIPTOR_HEADER typedef ptr STORAGE_DESCRIPTOR_HEADER

; +
; Device property descriptor - this is really just a rehash of the inquiry
; data retrieved from a scsi device
;
; This may only be retrieved from a target device.  Sending this to the bus
; will result in an error
; -

STORAGE_DEVICE_DESCRIPTOR STRUCT		; sizeof = 28h

	; Sizeof(STORAGE_DEVICE_DESCRIPTOR)

	Version						DWORD	?	; 000

	; Total size of the descriptor, including the space for additional
	; data and id strings

	_Size						DWORD	?	; 004

	; The SCSI-2 device type

	DeviceType					BYTE	?	; 008

	; The SCSI-2 device type modifier (if any) - this may be zero

	DeviceTypeModifier			BYTE	?	; 009

	; Flag indicating whether the device's media (if any) is removable.  This
	; field should be ignored for media-less devices

	RemovableMedia				BOOLEAN	?	; 00A

	; Flag indicating whether the device can support mulitple outstanding
	; commands.  The actual synchronization in this case is the responsibility
	; of the port driver.

	CommandQueueing				BOOLEAN	?	; 00B

	; Byte offset to the zero-terminated ascii string containing the device's
	; vendor id string.  For devices with no such ID this will be zero

	VendorIdOffset				DWORD	?	; 00C

	; Byte offset to the zero-terminated ascii string containing the device's
	; product id string.  For devices with no such ID this will be zero

	ProductIdOffset				DWORD	?	; 010

	; Byte offset to the zero-terminated ascii string containing the device's
	; product revision string.  For devices with no such string this will be
	; zero

	ProductRevisionOffset		DWORD	?	; 014

	; Byte offset to the zero-terminated ascii string containing the device's
	; serial number.  For devices with no serial number this will be zero

	SerialNumberOffset			DWORD	?	; 018

	; Contains the bus type (as defined above) of the device.  It should be
	; used to interpret the raw device properties at the end of this structure
	; (if any)

	BusType						DWORD	?	; 01C  STORAGE_BUS_TYPE

	; The number of bytes of bus-specific data which have been appended to
	; this descriptor

	RawPropertiesLength			DWORD	?	; 020

	; Place holder for the first byte of the bus specific property data

	RawDeviceProperties			db 1 dup(?)	; 024  start of array
	
								db 3 dup(?)	; padding

STORAGE_DEVICE_DESCRIPTOR ENDS
PSTORAGE_DEVICE_DESCRIPTOR typedef ptr STORAGE_DEVICE_DESCRIPTOR

; +
; Adapter properties
;
; This descriptor can be retrieved from a target device object of from the
; device object for the bus.  Retrieving from the target device object will
; forward the request to the underlying bus
; -

STORAGE_ADAPTER_DESCRIPTOR STRUCT			; sizeof 20h
	Version					DWORD		?	; 000
	_Size					DWORD		?	; 004
	MaximumTransferLength	DWORD		?	; 008
	MaximumPhysicalPages	DWORD		?	; 00C
	AlignmentMask			DWORD		?	; 010
	AdapterUsesPio			BOOLEAN		?	; 014
	AdapterScansDown		BOOLEAN		?	; 015
	CommandQueueing			BOOLEAN		?	; 016
	AcceleratedTransfer		BOOLEAN		?	; 017
	BusType					BOOLEAN		?	; 018
							db			?	; padding
	BusMajorVersion			WORD		?	; 01A
	BusMinorVersion			WORD		?	; 01C
							dw			?	; padding
STORAGE_ADAPTER_DESCRIPTOR ENDS
PSTORAGE_ADAPTER_DESCRIPTOR typedef ptr STORAGE_ADAPTER_DESCRIPTOR

ENDIF ; _NTDDSTOR_H_
