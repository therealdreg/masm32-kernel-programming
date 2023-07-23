;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  RegistryWorks - Creates, sets/reads and deletes registry key
;
;  Written by Four-F (four-f@mail.ru)
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.386
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\w2k\ntstatus.inc
include \masm32\include\w2k\ntddk.inc
include \masm32\include\w2k\ntoskrnl.inc

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 R E A D O N L Y    D A T A                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

;CCOUNTED_UNICODE_STRING	"\\Registry\\User", g_usKeyName, 4
;CCOUNTED_UNICODE_STRING	"\\Registry\\Machine", g_usKeyName, 4
;CCOUNTED_UNICODE_STRING	"\\Registry\\CurrentConfig", g_usKeyName, 4
;"\\Registry\\User\\.Default"

CCOUNTED_UNICODE_STRING	"\\Registry\\Machine\\Software\\CoolApp", g_usMachineKeyName, 4
CCOUNTED_UNICODE_STRING	"SomeData", g_usValueName, 4

CTW0 "It's just a string", g_wszStringData, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       CreateKey                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CreateKey proc

local oa:OBJECT_ATTRIBUTES
local hKey:HANDLE
local dwDisposition:DWORD

	invoke DbgPrint, $CTA0("\nRegistryWorks: *** Creating registry key\n")

	; Pay attention at OBJ_KERNEL_HANDLE flag.
	; It's not necessary to specify it here because we call CreateKey and other routines from DriverEntry
	; running in system process context. So the handle will belong to the kernel anyway.
	; But if you’re running in the context of a user-mode process the OBJ_KERNEL_HANDLE must be set.
	; This restricts the use of the handle to processes running only in kernel mode.
	; Otherwise, the handle can be accessed by the process in whose context the driver is running.
	; A process-specific handle will go away if the process terminates.
	; A kernel handle doesn’t disappear until the operating system shuts down
	; and can be used without ambiguity in any process.

	InitializeObjectAttributes addr oa, addr g_usMachineKeyName, OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	; REG_OPTION_VOLATILE means that key is not to be stored across boots.
	; We don't have to specify this flag here since we'll delete the key right after it will be created
	invoke ZwCreateKey, addr hKey, KEY_WRITE, addr oa, 0, NULL, REG_OPTION_VOLATILE, addr dwDisposition
	.if eax == STATUS_SUCCESS

		.if dwDisposition == REG_CREATED_NEW_KEY
			; A new key object was created. 
			invoke DbgPrint, $CTA0("RegistryWorks: Registry key \\Registry\\Machine\\Software\\CoolApp created\n")
		.elseif dwDisposition == REG_OPENED_EXISTING_KEY
			; An existing key object was opened. 
			invoke DbgPrint, $CTA0("RegistryWorks: Registry key \\Registry\\Machine\\Software\\CoolApp opened\n")
		.endif

		invoke ZwClose, hKey
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key handle closed\n")
	.else
		invoke DbgPrint, $CTA0("RegistryWorks: Can't create registry key. Status: %08X\n"), eax
	.endif

	ret

CreateKey endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       SetValueKey                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SetValueKey proc

local oa:OBJECT_ATTRIBUTES
local hKey:HANDLE

	invoke DbgPrint, $CTA0("\nRegistryWorks: *** Opening registry key to set new value\n")

	InitializeObjectAttributes addr oa, addr g_usMachineKeyName, OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	invoke ZwOpenKey, addr hKey, KEY_SET_VALUE, ecx

	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key openeded\n")

		invoke ZwSetValueKey, hKey, addr g_usValueName, 0, REG_SZ, \
								addr g_wszStringData, sizeof g_wszStringData
		.if eax == STATUS_SUCCESS
			invoke DbgPrint, $CTA0("RegistryWorks: Registry key value added\n")
		.else
			invoke DbgPrint, \
					$CTA0("RegistryWorks: Can't set registry key value. Status: %08X\n"), eax
		.endif

		invoke ZwClose, hKey
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key handle closed\n")
	.else
		invoke DbgPrint, $CTA0("RegistryWorks: Can't open registry key. Status: %08X\n"), eax
	.endif

	ret

SetValueKey endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      QueryValueKey                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

QueryValueKey proc

local oa:OBJECT_ATTRIBUTES
local hKey:HANDLE
local cb:DWORD
local ppi:PKEY_VALUE_PARTIAL_INFORMATION
local as:ANSI_STRING
local us:UNICODE_STRING

	invoke DbgPrint, $CTA0("\nRegistryWorks: *** Opening registry key to read value\n")

	InitializeObjectAttributes addr oa, addr g_usMachineKeyName, OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	invoke ZwOpenKey, addr hKey, KEY_QUERY_VALUE, addr oa

	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key openeded\n")

		and cb, 0

		invoke ZwQueryValueKey, hKey, addr g_usValueName, \
								KeyValuePartialInformation, NULL, 0, addr cb
		.if cb != 0

			invoke ExAllocatePool, PagedPool, cb

			.if eax != NULL
				mov ppi, eax

				invoke ZwQueryValueKey, hKey, addr g_usValueName, \
									KeyValuePartialInformation, ppi, cb, addr cb
				.if ( eax == STATUS_SUCCESS ) && ( cb != 0 )

					mov eax, ppi
					.if [KEY_VALUE_PARTIAL_INFORMATION PTR [eax]]._Type == REG_SZ
						lea eax, (KEY_VALUE_PARTIAL_INFORMATION PTR [eax]).Data
						invoke RtlInitUnicodeString, addr us, eax
						invoke RtlUnicodeStringToAnsiString, addr as, addr us, TRUE
						.if eax == STATUS_SUCCESS
							invoke DbgPrint, \
								$CTA0("RegistryWorks: Registry key value is: \=%s\=\n"), as.Buffer
							invoke RtlFreeAnsiString, addr as
						.endif
					.endif
				.else
					invoke DbgPrint, \
							$CTA0("RegistryWorks: Can't query registry key value. Status: %08X\n"), eax
				.endif
				invoke ExFreePool, ppi
			.else
				invoke DbgPrint, $CTA0("RegistryWorks: Can't allocate memory. Status: %08X\n"), eax
			.endif
		.else
			invoke DbgPrint, \
			$CTA0("RegistryWorks: Can't get bytes count needed for key partial information. Status: %08X\n"), eax
		.endif
		invoke ZwClose, hKey
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key handle closed\n")
	.else
		invoke DbgPrint, $CTA0("RegistryWorks: Can't open registry key. Status: %08X\n"), eax
	.endif

	ret

QueryValueKey endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        DeleteKey                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DeleteKey proc

local oa:OBJECT_ATTRIBUTES
local hKey:HANDLE

	invoke DbgPrint, $CTA0("\nRegistryWorks: *** Deleting registry key\n")

	InitializeObjectAttributes addr oa, addr g_usMachineKeyName, OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	invoke ZwOpenKey, addr hKey, KEY_ALL_ACCESS, addr oa

	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key opened\n")
		invoke ZwDeleteKey, hKey
		.if eax == STATUS_SUCCESS
			invoke DbgPrint, $CTA0("RegistryWorks: Registry key deleted\n")
		.else
			invoke DbgPrint, $CTA0("RegistryWorks: Can't delete registry key. Status: %08X\n"), eax
		.endif
		invoke ZwClose, hKey
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key handle closed\n")
	.else
		invoke DbgPrint, $CTA0("RegistryWorks: Can't open registry key. Status: %08X\n"), eax
	.endif

	ret

DeleteKey endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      EnumerateKey                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EnumerateKey proc

local oa:OBJECT_ATTRIBUTES
local hKey:HANDLE
local cb:DWORD
local pbi:PKEY_BASIC_INFORMATION
local pfi:PKEY_FULL_INFORMATION
local as:ANSI_STRING
local us:UNICODE_STRING
local dwSubKeys:DWORD
local pwszKeyName:PWCHAR

	invoke DbgPrint, $CTA0("\nRegistryWorks: *** Opening \\Registry\\User key to enumerate\n")

	CCOUNTED_UNICODE_STRING	"\\Registry\\User", g_usUserKeyName, 4

	InitializeObjectAttributes addr oa, addr g_usUserKeyName, OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	; Open key to enumerate subkeys
	invoke ZwOpenKey, addr hKey, KEY_ENUMERATE_SUB_KEYS, addr oa

	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key openeded\n")

		and cb, 0

		invoke ZwQueryKey, hKey, KeyFullInformation, NULL, 0, addr cb

		.if cb != 0

			invoke ExAllocatePool, PagedPool, cb
			.if eax != NULL
				mov pfi, eax

				invoke ZwQueryKey, hKey, KeyFullInformation, pfi, cb, addr cb
				.if ( eax == STATUS_SUCCESS ) && ( cb != 0 )

					mov eax, pfi
					push (KEY_FULL_INFORMATION PTR [eax]).SubKeys
					pop dwSubKeys

					invoke DbgPrint, \
						$CTA0("RegistryWorks: ---------- Starting enumerate subkeys ----------\n")

					push ebx
					xor ebx, ebx
					.while ebx < dwSubKeys

						and cb, 0

						invoke ZwEnumerateKey, hKey, ebx, KeyBasicInformation, NULL, 0, addr cb

						.if cb != 0

							invoke ExAllocatePool, PagedPool, cb

							.if eax != NULL
								mov pbi, eax

								invoke ZwEnumerateKey, hKey, ebx, KeyBasicInformation, pbi, cb, addr cb
								.if ( eax == STATUS_SUCCESS ) && ( cb != 0 )

									; Allocate memory for subkey name
									mov eax, pbi
									mov eax, (KEY_BASIC_INFORMATION PTR [eax]).NameLength
									add eax, sizeof WCHAR				; place for terminating zero
									mov cb, eax

									invoke ExAllocatePool, PagedPool, cb

									.if eax != NULL
										mov pwszKeyName, eax

										; Zero buffer
										invoke memset, pwszKeyName, 0, cb

										; The unicode-string pointed by KEY_BASIC_INFORMATION._Name
										; is not null-terminated. To avoid BSOD copy it into zeroed temporary buffer.
										mov ecx, pbi
										mov eax, (KEY_BASIC_INFORMATION PTR [ecx]).NameLength
										shr eax, 1					; / sizeof WCHAR. NumOfBytes -> NumOfChars
										lea ecx, (KEY_BASIC_INFORMATION PTR [ecx])._Name
										invoke wcsncpy, pwszKeyName, ecx, eax

										invoke RtlInitUnicodeString, addr us, pwszKeyName
										invoke RtlUnicodeStringToAnsiString, addr as, addr us, TRUE
										.if eax == STATUS_SUCCESS
											invoke DbgPrint, $CTA0("RegistryWorks: \=%s\=\n"), as.Buffer
											invoke RtlFreeAnsiString, addr as
										.endif

										invoke ExFreePool, pwszKeyName
									.endif
								.else
									invoke DbgPrint, \
										$CTA0("RegistryWorks: Can't enumerate registry keys. Status: %08X\n"), eax								
								.endif
								invoke ExFreePool, pbi
							.endif
						.endif
						inc ebx					; next subkey
					.endw
					pop ebx

					invoke DbgPrint, \
						$CTA0("RegistryWorks: ------------------------------------------------\n")

				.else
					invoke DbgPrint, \
						$CTA0("RegistryWorks: Can't query registry key information. Status: %08X\n"), eax
				.endif
				invoke ExFreePool, pfi
			.else
				invoke DbgPrint, $CTA0("RegistryWorks: Can't allocate memory. Status: %08X\n"), eax
			.endif
		.endif

		invoke ZwClose, hKey
		invoke DbgPrint, $CTA0("RegistryWorks: Registry key handle closed\n")

	.else
		invoke DbgPrint, $CTA0("RegistryWorks: Can't open registry key. Status: %08X\n"), eax
	.endif

	ret

EnumerateKey endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

	invoke DbgPrint, $CTA0("\nRegistryWorks: Entering DriverEntry\n")
		

	;:::::::::::::::::::::::::::::::::::::::
	; Create new registry key              ;
	;:::::::::::::::::::::::::::::::::::::::

	invoke CreateKey

	;:::::::::::::::::::::::::::::::::::::::
	; Set registry key value               ;
	;:::::::::::::::::::::::::::::::::::::::

	invoke SetValueKey

	;:::::::::::::::::::::::::::::::::::::::
	; Query registry key value             ;
	;:::::::::::::::::::::::::::::::::::::::

	invoke QueryValueKey

	;:::::::::::::::::::::::::::::::::::::::
	; Delete registry key                  ;
	;:::::::::::::::::::::::::::::::::::::::

	invoke DeleteKey

	;:::::::::::::::::::::::::::::::::::::::
	; Enumerating registry keys            ;
	;:::::::::::::::::::::::::::::::::::::::

	invoke EnumerateKey


	invoke DbgPrint, $CTA0("\nRegistryWorks: Leaving DriverEntry\n")

	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=RegistryWorks

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
