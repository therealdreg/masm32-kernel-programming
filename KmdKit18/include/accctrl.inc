comment ^

Module Name:
    accctrl.inc

Abstract:
    common includes for new style Win32 Access Control APIs

Author:
    Four-F (four-f@mail.ru)

Last Update:
	25-November-2003
^

;#ifndef __ACCESS_CONTROL__
;#define __ACCESS_CONTROL__

;#define AccFree LocalFree

; Definition: TRUSTEE_TYPE
; This enumerated type specifies the type of trustee account for the trustee
; returned by the API described in this document.
; TRUSTEE_IS_UNKNOWN - The trustee is an unknown, but not necessarily invalid
;                      type.  This field is not validated on input to the APIs
;                      that take Trustees.
; TRUSTEE_IS_USER      The trustee account is a user account.
; TRUSTEE_IS_GROUP     The trustee account is a group account.

;typedef enum _TRUSTEE_TYPE
    TRUSTEE_IS_UNKNOWN			equ 0
    TRUSTEE_IS_USER				equ 1
    TRUSTEE_IS_GROUP			equ 2
    TRUSTEE_IS_DOMAIN			equ 3
    TRUSTEE_IS_ALIAS			equ 4
    TRUSTEE_IS_WELL_KNOWN_GROUP	equ 5
    TRUSTEE_IS_DELETED			equ 6
    TRUSTEE_IS_INVALID			equ 7

; Definition: TRUSTEE_FORM
; This enumerated type specifies the form the trustee identifier is in for a
; particular trustee.
; TRUSTEE_IS_SID       The trustee is identified with a SID rather than with a name.
; TRUSTEE_IS_NAME      The trustee is identified with a name.

;typedef enum _TRUSTEE_FORM
    TRUSTEE_IS_SID		equ 0
    TRUSTEE_IS_NAME		equ 1
    TRUSTEE_BAD_FORM	equ 2

; Definition: MULTIPLE_TRUSTEE_OPERATION
; If the trustee is a multiple trustee, this enumerated type specifies the type.
; TRUSTEE_IS_IMPERSONATE       The trustee is an impersonate trustee and the multiple
;                          trustee field in the trustee points to another trustee
;                          that is a trustee for the server that will be doing the
;                          impersonation.

;typedef enum _MULTIPLE_TRUSTEE_OPERATION
    NO_MULTIPLE_TRUSTEE		equ 0
    TRUSTEE_IS_IMPERSONATE	equ 1

; Definition: TRUSTEE
; This structure is used to pass account information into and out of the system
; using the API defined in this document.
; PMultipleTrustee     - if NON-NULL, points to another trustee structure, as
;                    defined by the multiple trustee operation field.
; MultipleTrusteeOperation - Defines the multiple trustee operation/type.
; TrusteeForm - defines if the trustee is defined by name or SID.
; TrusteeType - defines if the trustee type is unknown, a user or a group.
; PwcsName     - points to the trustee name or the trustee SID.

TRUSTEE_A STRUCT
	pMultipleTrustee			DWORD	?	; PTR TRUSTEE_A
	MultipleTrusteeOperation	DWORD	?	; MULTIPLE_TRUSTEE_OPERATION
	TrusteeForm					DWORD	?	; TRUSTEE_FORM
	TrusteeType					DWORD	?	; TRUSTEE_TYPE
	union
		ptstrName				DWORD	?	; LPSTR
		pSid					DWORD	?	; PTR SID
	ends
TRUSTEE_A ENDS
PTRUSTEE_A typedef ptr TRUSTEE_A
TRUSTEEA equ <TRUSTEE_A>
PTRUSTEEA typedef ptr TRUSTEE_A

TRUSTEE_W STRUCT
	pMultipleTrustee			DWORD	?	; PTR TRUSTEE_W
	MultipleTrusteeOperation	DWORD	?	; MULTIPLE_TRUSTEE_OPERATION
	TrusteeForm					DWORD	?	; TRUSTEE_FORM
	TrusteeType					DWORD	?	; TRUSTEE_TYPE
	union
		ptstrName				DWORD	?	; LPWSTR
		pSid					DWORD	?	; PTR SID
	ends
TRUSTEE_W ENDS
PTRUSTEE_W typedef ptr TRUSTEE_W
TRUSTEEW equ <TRUSTEE_W>
PTRUSTEEW typedef ptr TRUSTEE_W

IFDEF UNICODE
	DEV_BROADCAST_PORT equ DEV_BROADCAST_PORT_W
	PDEV_BROADCAST_PORT equ PDEV_BROADCAST_PORT_W
ELSE
	DEV_BROADCAST_PORT equ DEV_BROADCAST_PORT_A
	PDEV_BROADCAST_PORT equ PDEV_BROADCAST_PORT_A
ENDIF

IFDEF UNICODE
	TRUSTEE_ equ <TRUSTEE_W>
	PTRUSTEE_ equ PTRUSTEE_W
	TRUSTEE equ <TRUSTEEW>
	PTRUSTEE equ PTRUSTEEW
ELSE
	TRUSTEE_ equ <TRUSTEE_A>
	PTRUSTEE_ equ PTRUSTEE_A
	TRUSTEE equ <TRUSTEEA>
	PTRUSTEE equ PTRUSTEEA
ENDIF

; Definition: ACCESS_MODE
; This enumerated type specifies how permissions are (requested)/to be applied
;  for the trustee by the access control entry.  On input this field can by any
;  of the values, although it is not meaningful to mix access control and audit
;  control entries.  On output this field will be either SET_ACCESS, DENY_ACCESS,
; SET_AUDIT_SUCCESS, SET_AUDIT_FAILURE.
; The following descriptions define how this type effects an explicit access
; request to apply access permissions to an object.
; GRANT_ACCESS - The trustee will have at least the requested permissions upon
;                successful completion of the command. (If the trustee has
;                additional permissions they will not be removed).
; SET_ACCESS - The trustee will have exactly the requested permissions upon
;              successful completion of the command.
; DENY_ACCESS - The trustee will be denied the specified permissions.
; REVOKE_ACCESS - Any explicit access rights the trustee has will be revoked.
; SET_AUDIT_SUCCESS - The trustee will be audited for successful opens of the
;                     object using the requested permissions.
; SET_AUDIT_FAILURE - The trustee will be audited for failed opens of the object
;                     using the requested permissions.

;typedef enum _ACCESS_MODE
    NOT_USED_ACCESS		equ 0
    GRANT_ACCESS		equ 1
    SET_ACCESS			equ 2
    DENY_ACCESS			equ 3
    REVOKE_ACCESS		equ 4
    SET_AUDIT_SUCCESS	equ 5
    SET_AUDIT_FAILURE	equ 6

; Definition: Inheritance flags
; These bit masks are provided to allow simple application of inheritance in
; explicit access requests on containers.
; NO_INHERITANCE       The specific access permissions will only be applied to
;                  the container, and will not be inherited by objects created
;                  within the container.
; SUB_CONTAINERS_ONLY_INHERIT  The specific access permissions will be inherited
;                              and applied to sub containers created within the
;                              container, and will be applied to the container
;                              itself.
; SUB_OBJECTS_ONLY_INHERIT     The specific access permissions will only be inherited
;                              by objects created within the specific container.
;                              The access permissions will not be applied to the
;                              container itself.
; SUB_CONTAINERS_AND_OBJECTS_INHERIT   The specific access permissions will be
;                                      inherited by containers created within the
;                                      specific container, will be applied to
;                                      objects created within the container, but
;                                      will not be applied to the container itself.

NO_INHERITANCE						equ 0
SUB_OBJECTS_ONLY_INHERIT            equ 1
SUB_CONTAINERS_ONLY_INHERIT         equ 2
SUB_CONTAINERS_AND_OBJECTS_INHERIT  equ 3
INHERIT_NO_PROPAGATE                equ 4
INHERIT_ONLY                        equ 8

; Informational bit that is returned

INHERITED_ACCESS_ENTRY              equ 10h

; Informational bit that tells where a node was inherited from.  Valid only
; for NT 5 APIs

INHERITED_PARENT                    equ 10000000h
INHERITED_GRANDPARENT               equ 20000000h

; Definition:
; This enumerated type defines the objects supported by the get/set API within
; this document.  See section 3.1, Object Types for a detailed definition of the
; supported object types, and their name formats.

;typedef enum _SE_OBJECT_TYPE
    SE_UNKNOWN_OBJECT_TYPE		equ 0
    SE_FILE_OBJECT				equ 1
    SE_SERVICE					equ 2
    SE_PRINTER					equ 3
    SE_REGISTRY_KEY				equ 4
    SE_LMSHARE					equ 5
    SE_KERNEL_OBJECT			equ 6
    SE_WINDOW_OBJECT			equ 7
    SE_DS_OBJECT				equ 8
    SE_DS_OBJECT_ALL			equ 9
    SE_PROVIDER_DEFINED_OBJECT	equ 10

; Definition: EXPLICIT_ACCESS
; This structure is used to pass access control entry information into and out
; of the system using the API defined in this document.
; grfAccessPermissions - This contains the access permissions to assign for the
;                     trustee.  It is in the form of an NT access mask.
; grfAccessMode - This field defines how the permissions are to be applied for
;                 the trustee.
; grfInheritance - For containers, this field defines how the access control
;                  entry is/(is requested) to be inherited on
;                  objects/sub-containers created within the container.
; Trustee - This field contains the definition of the trustee account the
;           explicit access applies to.

EXPLICIT_ACCESS_A STRUCT
	grfAccessPermissions	DWORD	?
	grfAccessMode			DWORD	?	; ACCESS_MODE
	grfInheritance			DWORD	?
	Trustee					TRUSTEE_A <>
EXPLICIT_ACCESS_A ENDS
PEXPLICIT_ACCESS_A typedef ptr EXPLICIT_ACCESS_A
EXPLICIT_ACCESSA equ <EXPLICIT_ACCESS_A>
PEXPLICIT_ACCESSA typedef ptr EXPLICIT_ACCESS_A

EXPLICIT_ACCESS_W STRUCT
	grfAccessPermissions	DWORD	?
	grfAccessMode			DWORD	?	; ACCESS_MODE
	grfInheritance			DWORD	?
	Trustee					TRUSTEE_W <>
EXPLICIT_ACCESS_W ENDS
PEXPLICIT_ACCESS_W typedef ptr EXPLICIT_ACCESS_W
EXPLICIT_ACCESSW equ <EXPLICIT_ACCESS_W>
PEXPLICIT_ACCESSW typedef ptr EXPLICIT_ACCESS_W

IFDEF UNICODE
	EXPLICIT_ACCESS_ equ <EXPLICIT_ACCESS_W>
	PEXPLICIT_ACCESS_ equ PEXPLICIT_ACCESS_W
	EXPLICIT_ACCESS equ <EXPLICIT_ACCESSW>
	PEXPLICIT_ACCESS equ PEXPLICIT_ACCESSW
ELSE
	EXPLICIT_ACCESS_ equ <EXPLICIT_ACCESS_A>
	PEXPLICIT_ACCESS_ equ PEXPLICIT_ACCESS_A
	EXPLICIT_ACCESS equ <EXPLICIT_ACCESSA>
	PEXPLICIT_ACCESS equ PEXPLICIT_ACCESSA
ENDIF

;----------------------------------------------------------------------------
;
;                                  NT5 APIs
;
;----------------------------------------------------------------------------
comment ^
; Default provider

#define ACCCTRL_DEFAULT_PROVIDER  TEXT("Windows NT Access Provider")
^
; Access rights

ACCESS_RIGHTS typedef DWORD
PACCESS_RIGHTS typedef ptr DWORD

; Inheritance flags

INHERIT_FLAGS typedef DWORD
PINHERIT_FLAGS typedef ptr DWORD

; Access / Audit structures

ACTRL_ACCESS_ENTRYA STRUCT
	Trustee				TRUSTEE_A	<>
	fAccessFlags		DWORD		?
	Access				DWORD		?	; ACCESS_RIGHTS
	ProvSpecificAccess	DWORD		?	; ACCESS_RIGHTS
	Inheritance			DWORD		?	; INHERIT_FLAGS
	lpInheritProperty	DWORD		?	; LPSTR
ACTRL_ACCESS_ENTRYA ENDS
PACTRL_ACCESS_ENTRYA typedef ptr ACTRL_ACCESS_ENTRYA

; Access / Audit structures

ACTRL_ACCESS_ENTRYW STRUCT
	Trustee				TRUSTEE_W	<>
	fAccessFlags		DWORD		?
	Access				DWORD		?	; ACCESS_RIGHTS
	ProvSpecificAccess	DWORD		?	; ACCESS_RIGHTS
	Inheritance			DWORD		?	; INHERIT_FLAGS
	lpInheritProperty	DWORD		?	; LPWSTR
ACTRL_ACCESS_ENTRYW ENDS
PACTRL_ACCESS_ENTRYW typedef ptr ACTRL_ACCESS_ENTRYW

IFDEF UNICODE
	ACTRL_ACCESS_ENTRY equ <ACTRL_ACCESS_ENTRYW>
	PACTRL_ACCESS_ENTRY equ PACTRL_ACCESS_ENTRYW
ELSE
	ACTRL_ACCESS_ENTRY equ <ACTRL_ACCESS_ENTRYA>
	PACTRL_ACCESS_ENTRY equ PACTRL_ACCESS_ENTRYA
ENDIF
comment ^
typedef struct _ACTRL_ACCESS_ENTRY_LISTA
{
    ULONG                   cEntries;
#ifdef __midl
    [size_is(cEntries)]
#endif
    ACTRL_ACCESS_ENTRYA    *pAccessList;
} ACTRL_ACCESS_ENTRY_LISTA, *PACTRL_ACCESS_ENTRY_LISTA;
typedef struct _ACTRL_ACCESS_ENTRY_LISTW
{
    ULONG                   cEntries;
#ifdef __midl
    [size_is(cEntries)]
#endif
    ACTRL_ACCESS_ENTRYW    *pAccessList;
} ACTRL_ACCESS_ENTRY_LISTW, *PACTRL_ACCESS_ENTRY_LISTW;

#ifdef UNICODE
typedef ACTRL_ACCESS_ENTRY_LISTW ACTRL_ACCESS_ENTRY_LIST;
typedef PACTRL_ACCESS_ENTRY_LISTW PACTRL_ACCESS_ENTRY_LIST;
#else
typedef ACTRL_ACCESS_ENTRY_LISTA ACTRL_ACCESS_ENTRY_LIST;
typedef PACTRL_ACCESS_ENTRY_LISTA PACTRL_ACCESS_ENTRY_LIST;
#endif // UNICODE
^


ACTRL_PROPERTY_ENTRYA STRUCT
	lpProperty				DWORD	?	; LPSTR
	pAccessEntryList		DWORD	?	; PACTRL_ACCESS_ENTRY_LISTA
	fListFlags				DWORD	?
ACTRL_PROPERTY_ENTRYA ENDS
PACTRL_PROPERTY_ENTRYA typedef ptr ACTRL_PROPERTY_ENTRYA

ACTRL_PROPERTY_ENTRYW STRUCT
	lpProperty				DWORD	?	; LPWSTR
	pAccessEntryList		DWORD	?	; PACTRL_ACCESS_ENTRY_LISTW
	fListFlags				DWORD	?
ACTRL_PROPERTY_ENTRYW ENDS
PACTRL_PROPERTY_ENTRYW typedef ptr ACTRL_PROPERTY_ENTRYW

IFDEF UNICODE
	ACTRL_PROPERTY_ENTRY equ <ACTRL_PROPERTY_ENTRYW>
	PACTRL_PROPERTY_ENTRY equ PACTRL_PROPERTY_ENTRYW
ELSE
	ACTRL_PROPERTY_ENTRY equ <ACTRL_PROPERTY_ENTRYA>
	PACTRL_PROPERTY_ENTRY equ PACTRL_PROPERTY_ENTRYA
ENDIF


comment ^
typedef struct _ACTRL_ALISTA
{
    ULONG                       cEntries;
#ifdef __midl
    [size_is(cEntries)]
#endif
    PACTRL_PROPERTY_ENTRYA      pPropertyAccessList;
} ACTRL_ACCESSA, *PACTRL_ACCESSA, ACTRL_AUDITA, *PACTRL_AUDITA;
typedef struct _ACTRL_ALISTW
{
    ULONG                       cEntries;
#ifdef __midl
    [size_is(cEntries)]
#endif
    PACTRL_PROPERTY_ENTRYW      pPropertyAccessList;
} ACTRL_ACCESSW, *PACTRL_ACCESSW, ACTRL_AUDITW, *PACTRL_AUDITW;

#ifdef UNICODE
typedef ACTRL_ACCESSW ACTRL_ACCESS;
typedef PACTRL_ACCESSW PACTRL_ACCESS;
typedef ACTRL_AUDITW ACTRL_AUDIT;
typedef PACTRL_AUDITW PACTRL_AUDIT;
#else
typedef ACTRL_ACCESSA ACTRL_ACCESS;
typedef PACTRL_ACCESSA PACTRL_ACCESS;
typedef ACTRL_AUDITA ACTRL_AUDIT;
typedef PACTRL_AUDITA PACTRL_AUDIT;
#endif // UNICODE
^


; TRUSTEE_ACCESS flags

TRUSTEE_ACCESS_ALLOWED      equ 00000001
TRUSTEE_ACCESS_READ         equ 00000002
TRUSTEE_ACCESS_WRITE        equ 00000004

TRUSTEE_ACCESS_EXPLICIT     equ 00000001
TRUSTEE_ACCESS_READ_WRITE   equ (TRUSTEE_ACCESS_READ or TRUSTEE_ACCESS_WRITE)

TRUSTEE_ACCESS_ALL          equ 0FFFFFFFFh


TRUSTEE_ACCESSA STRUCT
    LPSTR           lpProperty;
    ACCESS_RIGHTS   Access;
    ULONG           fAccessFlags;
    ULONG           fReturnedAccess;
TRUSTEE_ACCESSA ENDS
PTRUSTEE_ACCESSA typedef ptr TRUSTEE_ACCESSA

TRUSTEE_ACCESSW STRUCT
    LPWSTR          lpProperty;
    ACCESS_RIGHTS   Access;
    ULONG           fAccessFlags;
    ULONG           fReturnedAccess;
TRUSTEE_ACCESSW ENDS
PTRUSTEE_ACCESSW typedef ptr TRUSTEE_ACCESSW

IFDEF UNICODE
	TRUSTEE_ACCESS equ <TRUSTEE_ACCESSW>
	PTRUSTEE_ACCESS equ PTRUSTEE_ACCESSW
ELSE
	TRUSTEE_ACCESS equ <TRUSTEE_ACCESSA>
	PTRUSTEE_ACCESS equ PTRUSTEE_ACCESSA
ENDIF


; Generic permission values

ACTRL_RESERVED          equ 00000000
ACTRL_PERM_1            equ 00000001
ACTRL_PERM_2            equ 00000002
ACTRL_PERM_3            equ 00000004
ACTRL_PERM_4            equ 00000008
ACTRL_PERM_5            equ 00000010h
ACTRL_PERM_6            equ 00000020h
ACTRL_PERM_7            equ 00000040h
ACTRL_PERM_8            equ 00000080h
ACTRL_PERM_9            equ 00000100h
ACTRL_PERM_10           equ 00000200h
ACTRL_PERM_11           equ 00000400h
ACTRL_PERM_12           equ 00000800h
ACTRL_PERM_13           equ 00001000h
ACTRL_PERM_14           equ 00002000h
ACTRL_PERM_15           equ 00004000h
ACTRL_PERM_16           equ 00008000h
ACTRL_PERM_17           equ 00010000h
ACTRL_PERM_18           equ 00020000h
ACTRL_PERM_19           equ 00040000h
ACTRL_PERM_20           equ 00080000h

; Access permissions

ACTRL_ACCESS_ALLOWED        equ 00000001
ACTRL_ACCESS_DENIED         equ 00000002
ACTRL_AUDIT_SUCCESS         equ 00000004
ACTRL_AUDIT_FAILURE         equ 00000008

; Property list flags

ACTRL_ACCESS_PROTECTED      equ 00000001

; Standard and object rights

ACTRL_SYSTEM_ACCESS         equ 04000000h
ACTRL_DELETE                equ 08000000h
ACTRL_READ_CONTROL          equ 10000000h
ACTRL_CHANGE_ACCESS         equ 20000000h
ACTRL_CHANGE_OWNER          equ 40000000h
ACTRL_SYNCHRONIZE           equ 80000000h
ACTRL_STD_RIGHTS_ALL        equ 0f8000000h

ACTRL_DS_OPEN                           equ ACTRL_RESERVED
ACTRL_DS_CREATE_CHILD                   equ ACTRL_PERM_1
ACTRL_DS_DELETE_CHILD                   equ ACTRL_PERM_2
ACTRL_DS_LIST                           equ ACTRL_PERM_3
ACTRL_DS_SELF                           equ ACTRL_PERM_4
ACTRL_DS_READ_PROP                      equ ACTRL_PERM_5
ACTRL_DS_WRITE_PROP                     equ ACTRL_PERM_6
ACTRL_FILE_READ                         equ ACTRL_PERM_1
ACTRL_FILE_WRITE                        equ ACTRL_PERM_2
ACTRL_FILE_APPEND                       equ ACTRL_PERM_3
ACTRL_FILE_READ_PROP                    equ ACTRL_PERM_4
ACTRL_FILE_WRITE_PROP                   equ ACTRL_PERM_5
ACTRL_FILE_EXECUTE                      equ ACTRL_PERM_6
ACTRL_FILE_READ_ATTRIB                  equ ACTRL_PERM_7
ACTRL_FILE_WRITE_ATTRIB                 equ ACTRL_PERM_8
ACTRL_DIR_LIST                          equ ACTRL_PERM_1
ACTRL_DIR_CREATE_OBJECT                 equ ACTRL_PERM_2
ACTRL_DIR_CREATE_CHILD                  equ ACTRL_PERM_3
ACTRL_DIR_DELETE_CHILD                  equ ACTRL_PERM_4
ACTRL_DIR_TRAVERSE                      equ ACTRL_PERM_6
ACTRL_KERNEL_TERMINATE                  equ ACTRL_PERM_1
ACTRL_KERNEL_THREAD                     equ ACTRL_PERM_2
ACTRL_KERNEL_VM                         equ ACTRL_PERM_3
ACTRL_KERNEL_VM_READ                    equ ACTRL_PERM_4
ACTRL_KERNEL_VM_WRITE                   equ ACTRL_PERM_5
ACTRL_KERNEL_DUP_HANDLE                 equ ACTRL_PERM_6
ACTRL_KERNEL_PROCESS                    equ ACTRL_PERM_7
ACTRL_KERNEL_SET_INFO                   equ ACTRL_PERM_8
ACTRL_KERNEL_GET_INFO                   equ ACTRL_PERM_9
ACTRL_KERNEL_CONTROL                    equ ACTRL_PERM_10
ACTRL_KERNEL_ALERT                      equ ACTRL_PERM_11
ACTRL_KERNEL_GET_CONTEXT                equ ACTRL_PERM_12
ACTRL_KERNEL_SET_CONTEXT                equ ACTRL_PERM_13
ACTRL_KERNEL_TOKEN                      equ ACTRL_PERM_14
ACTRL_KERNEL_IMPERSONATE                equ ACTRL_PERM_15
ACTRL_KERNEL_DIMPERSONATE               equ ACTRL_PERM_16
ACTRL_PRINT_SADMIN                      equ ACTRL_PERM_1
ACTRL_PRINT_SLIST                       equ ACTRL_PERM_2
ACTRL_PRINT_PADMIN                      equ ACTRL_PERM_3
ACTRL_PRINT_PUSE                        equ ACTRL_PERM_4
ACTRL_PRINT_JADMIN                      equ ACTRL_PERM_5
ACTRL_SVC_GET_INFO                      equ ACTRL_PERM_1
ACTRL_SVC_SET_INFO                      equ ACTRL_PERM_2
ACTRL_SVC_STATUS                        equ ACTRL_PERM_3
ACTRL_SVC_LIST                          equ ACTRL_PERM_4
ACTRL_SVC_START                         equ ACTRL_PERM_5
ACTRL_SVC_STOP                          equ ACTRL_PERM_6
ACTRL_SVC_PAUSE                         equ ACTRL_PERM_7
ACTRL_SVC_INTERROGATE                   equ ACTRL_PERM_8
ACTRL_SVC_UCONTROL                      equ ACTRL_PERM_9
ACTRL_REG_QUERY                         equ ACTRL_PERM_1
ACTRL_REG_SET                           equ ACTRL_PERM_2
ACTRL_REG_CREATE_CHILD                  equ ACTRL_PERM_3
ACTRL_REG_LIST                          equ ACTRL_PERM_4
ACTRL_REG_NOTIFY                        equ ACTRL_PERM_5
ACTRL_REG_LINK                          equ ACTRL_PERM_6
ACTRL_WIN_CLIPBRD                       equ ACTRL_PERM_1
ACTRL_WIN_GLOBAL_ATOMS                  equ ACTRL_PERM_2
ACTRL_WIN_CREATE                        equ ACTRL_PERM_3
ACTRL_WIN_LIST_DESK                     equ ACTRL_PERM_4
ACTRL_WIN_LIST                          equ ACTRL_PERM_5
ACTRL_WIN_READ_ATTRIBS                  equ ACTRL_PERM_6
ACTRL_WIN_WRITE_ATTRIBS                 equ ACTRL_PERM_7
ACTRL_WIN_SCREEN                        equ ACTRL_PERM_8
ACTRL_WIN_EXIT                          equ ACTRL_PERM_9



ACTRL_OVERLAPPED STRUCT
	Reserved1	DWORD	?
	Reserved2	DWORD	?
	hEvent		DWORD	?	; HANDLE
ACTRL_OVERLAPPED ENDS
PACTRL_OVERLAPPED typedef ptr ACTRL_OVERLAPPED

ACTRL_ACCESS_INFOA STRUCT
	fAccessPermission		DWORD	?
	lpAccessPermissionName	DWORD	?	; LPSTR
ACTRL_ACCESS_INFOA ENDS
PACTRL_ACCESS_INFOA typedef ptr ACTRL_ACCESS_INFOA

ACTRL_ACCESS_INFOW STRUCT
	fAccessPermission		DWORD	?
	lpAccessPermissionName	DWORD	?	; LPWSTR
ACTRL_ACCESS_INFOW ENDS
PACTRL_ACCESS_INFOW typedef ptr ACTRL_ACCESS_INFOW

IFDEF UNICODE
	ACTRL_ACCESS_INFO equ <ACTRL_ACCESS_INFOW>
	PACTRL_ACCESS_INFO equ PACTRL_ACCESS_INFOW
ELSE
	ACTRL_ACCESS_INFO equ <ACTRL_ACCESS_INFOA>
	PACTRL_ACCESS_INFO equ PACTRL_ACCESS_INFOA
ENDIF

ACTRL_CONTROL_INFOA STRUCT
	lpControlId		DWORD	?	; LPSTR
	lpControlName	DWORD	?	; LPSTR
ACTRL_CONTROL_INFOA ENDS
PACTRL_CONTROL_INFOA typedef ptr ACTRL_CONTROL_INFOA

ACTRL_CONTROL_INFOW STRUCT
	lpControlId		DWORD	?	; LPWSTR
	lpControlName	DWORD	?	; LPWSTR
ACTRL_CONTROL_INFOW ENDS
PACTRL_CONTROL_INFOW typedef ptr ACTRL_CONTROL_INFOW

IFDEF UNICODE
	ACTRL_CONTROL_INFO equ <ACTRL_CONTROL_INFOW>
	PACTRL_CONTROL_INFO equ PACTRL_CONTROL_INFOW
ELSE
	ACTRL_CONTROL_INFO equ <ACTRL_CONTROL_INFOA>
	PACTRL_CONTROL_INFO equ PACTRL_CONTROL_INFOA
ENDIF


ACTRL_ACCESS_NO_OPTIONS                 equ 00000000
ACTRL_ACCESS_SUPPORTS_OBJECT_ENTRIES    equ 00000001

;#endif // __ACCESS_CONTROL__

