;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  FileWorks - File creation, writing, reading etc...
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
include \masm32\include\w2k\ntifs.inc
include \masm32\include\w2k\ntoskrnl.inc

includelib \masm32\lib\w2k\ntoskrnl.lib

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                 R E A D O N L Y    D A T A                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

CCOUNTED_UNICODE_STRING	"\\??\\c:\\FileWorks\\test.txt", g_usFileName, 4
CCOUNTED_UNICODE_STRING	"\\??\\c:\\FileWorks", g_usDirName, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         C O D E                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      CreateDirectory                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CreateDirectory proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hDirectory:HANDLE

	; Remember that the Unicode format codes (%C, %S, %lc, %ls, %wc, %ws, %wZ)
	; can only be used at IRQL PASSIVE_LEVEL. 
	invoke DbgPrint, $CTA0("\nFileWorks: Creating %ws directory\n"), g_usDirName.Buffer

	; Pay attention at OBJ_KERNEL_HANDLE flag. It's applicable for all object types not only for files.

	; DDK stands:
	; "Driver routines that run in a process context other than that of the system process
	; must set the OBJ_KERNEL_HANDLE attribute for the ObjectAttributes parameter of ZwCreateFile.
	; This restricts the use of the handle returned by ZwCreateFile to processes
	; running only in kernel mode. Otherwise, the handle can be accessed by the process
	; in whose context the driver is running."
	
	; But in reality even you get a handle in system process context without specifying
	; OBJ_KERNEL_HANDLE you can NOT touch this object in any other process context.
	; So better always specify OBJ_KERNEL_HANDLE if you plan access object by handle
	; in different processes. A kernel handle doesn’t disappear until the operating system
	; shuts down and can be used without ambiguity in any process.

	InitializeObjectAttributes addr oa, addr g_usDirName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	invoke ZwCreateFile, addr hDirectory, SYNCHRONIZE, addr oa, addr iosb, 0, FILE_ATTRIBUTE_NORMAL, \
						0, FILE_OPEN_IF, FILE_DIRECTORY_FILE + FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0
	.if eax == STATUS_SUCCESS
		.if iosb.Information == FILE_CREATED
			invoke DbgPrint, $CTA0("FileWorks: Directory created\n")
		.elseif iosb.Information == FILE_OPENED
			invoke DbgPrint, $CTA0("FileWorks: Directory exists and was opened\n")
		.endif
		invoke ZwClose, hDirectory
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't create directory. Status: %08X\n"), eax
	.endif
	
	ret

CreateDirectory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        CreateFile                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CreateFile proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hFile:HANDLE

	; Remember that the Unicode format codes (%C, %S, %lc, %ls, %wc, %ws, %wZ)
	; can only be used at IRQL PASSIVE_LEVEL. 
	invoke DbgPrint, $CTA0("\nFileWorks: Creating %ws file\n"), g_usFileName.Buffer

	InitializeObjectAttributes addr oa, addr g_usFileName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL

	; If the file already exists, fail the request and do not create or open the given file.
	; If it does not, create the given file.

	invoke ZwCreateFile, addr hFile, SYNCHRONIZE, addr oa, addr iosb, 0, FILE_ATTRIBUTE_NORMAL, \
						0, FILE_CREATE, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0
	.if eax == STATUS_SUCCESS

		invoke DbgPrint, $CTA0("FileWorks: File created\n")
		invoke ZwClose, hFile
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't create file. Status: %08X\n"), eax
	.endif
	
	ret

CreateFile endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                            WriteFile                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

WriteFile proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hFile:HANDLE

	invoke DbgPrint, $CTA0("\nFileWorks: Opening file for writing\n")

	InitializeObjectAttributes addr oa, addr g_usFileName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	
	; ZwCreateFile can be used for opening existing file. FILE_OPEN should be specified.
	; I use:
	; - FILE_WRITE_DATA because only I want is to write data into the file;
	; - SYNCHRONIZE because of FILE_SYNCHRONOUS_IO_NONALERT.
	; But you can simply use less strict FILE_ALL_ACCESS.

	invoke ZwCreateFile, addr hFile, FILE_WRITE_DATA + SYNCHRONIZE, addr oa, addr iosb, \
						0, 0, FILE_SHARE_READ, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0
	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("FileWorks: File openeded\n")

		CTA0 "Data can be written to an open file", g_szData, 4

		invoke ZwWriteFile, hFile, 0, NULL, NULL, addr iosb, \
						addr g_szData, sizeof g_szData - 1, NULL, NULL
		.if eax == STATUS_SUCCESS
			invoke DbgPrint, $CTA0("FileWorks: File was written\n")
		.else
			invoke DbgPrint, $CTA0("FileWorks: Can't write to the file. Status: %08X\n"), eax
		.endif

		invoke ZwClose, hFile
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't open file. Status: %08X\n"), eax
	.endif

	ret

WriteFile endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        MarkAsReadOnly                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MarkAsReadOnly proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hFile:HANDLE
local fbi:FILE_BASIC_INFORMATION

	invoke DbgPrint, $CTA0("\nFileWorks: Opening file for changing attributes\n")

	InitializeObjectAttributes addr oa, addr g_usFileName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	
	; ZwCreateFile can be used for opening existing file. FILE_OPEN should be specified.
	; I use:
	; - FILE_READ_ATTRIBUTES because I want to query file attributes;
	; - FILE_WRITE_ATTRIBUTES because I want to change file attributes;
	; - SYNCHRONIZE because of FILE_SYNCHRONOUS_IO_NONALERT.
	; But you can simply use less strict FILE_ALL_ACCESS.

	invoke ZwCreateFile, addr hFile, FILE_READ_ATTRIBUTES + FILE_WRITE_ATTRIBUTES + SYNCHRONIZE, \
						addr oa, addr iosb, 0, 0, FILE_SHARE_READ, \
						FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0
	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("FileWorks: File openeded\n")

		; Protect the file from deletion.
		invoke ZwQueryInformationFile, hFile, addr iosb, addr fbi, sizeof fbi, FileBasicInformation
		; Undocumented ZwQueryAttributesFile does the same.
		.if eax == STATUS_SUCCESS
			invoke DbgPrint, $CTA0("FileWorks: File attributes were: %08X\n"), fbi.FileAttributes
			or fbi.FileAttributes, FILE_ATTRIBUTE_READONLY
			invoke ZwSetInformationFile, hFile, addr iosb, addr fbi, sizeof fbi, FileBasicInformation
			.if eax == STATUS_SUCCESS
				invoke DbgPrint, $CTA0("FileWorks: Now file marked as read-only\n")
			.else
				invoke DbgPrint, $CTA0("FileWorks: Can't change file attributes. Status: %08X\n"), eax
			.endif
		.else
			invoke DbgPrint, $CTA0("FileWorks: Can't query file attributes. Status: %08X\n"), eax
		.endif

		invoke ZwClose, hFile
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't open file. Status: %08X\n"), eax
	.endif

	ret

MarkAsReadOnly endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          ReadFile                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ReadFile proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hFile:HANDLE
local p:PVOID
local cb:DWORD
local fsi:FILE_STANDARD_INFORMATION

	invoke DbgPrint, $CTA0("\nFileWorks: Opening file for reading\n")

	InitializeObjectAttributes addr oa, addr g_usFileName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	invoke ZwOpenFile, addr hFile, FILE_READ_DATA + SYNCHRONIZE, addr oa, addr iosb, \
				FILE_SHARE_READ + FILE_SHARE_WRITE + FILE_SHARE_DELETE, FILE_SYNCHRONOUS_IO_NONALERT
	.if eax == STATUS_SUCCESS

		invoke DbgPrint, $CTA0("FileWorks: File openeded\n")

		invoke ZwQueryInformationFile, hFile, addr iosb, addr fsi, sizeof fsi, FileStandardInformation
		.if eax == STATUS_SUCCESS

			mov eax, fsi.EndOfFile.LowPart
			inc eax								; one byte more for terminating zero
			mov cb, eax

			invoke ExAllocatePool, PagedPool, cb
			.if eax != NULL
				mov p, eax

				invoke RtlZeroMemory, p, cb

				invoke ZwReadFile, hFile, 0, NULL, NULL, addr iosb, p, cb, 0, NULL
				.if eax == STATUS_SUCCESS
					invoke DbgPrint, $CTA0("FileWorks: File content: \=%s\=\n"), p
				.else
					invoke DbgPrint, $CTA0("FileWorks: Can't read from the file. Status: %08X\n"), eax
				.endif

				invoke ExFreePool, p

			.else
				invoke DbgPrint, $CTA0("FileWorks: Can't allocate memory. Status: %08X\n"), eax
			.endif
		.else
			invoke DbgPrint, $CTA0("FileWorks: Can't query file size. Status: %08X\n"), eax
		.endif

		invoke ZwClose, hFile

	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't open file. Status: %08X\n"), eax
	.endif

	ret

ReadFile endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        UnmarkAsReadOnly                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

UnmarkAsReadOnly proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hFile:HANDLE
local fbi:FILE_BASIC_INFORMATION

	invoke DbgPrint, $CTA0("\nFileWorks: Opening file for changing attributes\n")

	InitializeObjectAttributes addr oa, addr g_usFileName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	
	; ZwCreateFile can be used for opening existing file. FILE_OPEN should be specified.
	; I use:
	; - FILE_READ_ATTRIBUTES because I want to query file attributes;
	; - FILE_WRITE_ATTRIBUTES because I want to change file attributes;
	; - SYNCHRONIZE because of FILE_SYNCHRONOUS_IO_NONALERT.
	; But you can simply use less strict FILE_ALL_ACCESS.

	invoke ZwCreateFile, addr hFile, FILE_READ_ATTRIBUTES + FILE_WRITE_ATTRIBUTES + SYNCHRONIZE, \
						addr oa, addr iosb, 0, 0, FILE_SHARE_READ, FILE_OPEN, \
						FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0
	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("FileWorks: File openeded\n")

		; Allow delete file.
		invoke ZwQueryInformationFile, hFile, addr iosb, addr fbi, sizeof fbi, FileBasicInformation
		; Undocumented ZwQueryAttributesFile does the same.
		.if eax == STATUS_SUCCESS
			invoke DbgPrint, $CTA0("FileWorks: File attributes were: %08X\n"), fbi.FileAttributes
			and fbi.FileAttributes, not FILE_ATTRIBUTE_READONLY
			invoke ZwSetInformationFile, hFile, addr iosb, addr fbi, sizeof fbi, FileBasicInformation
			.if eax == STATUS_SUCCESS
				invoke DbgPrint, $CTA0("FileWorks: Now file can be written or deleted\n")
			.else
				invoke DbgPrint, $CTA0("FileWorks: Can't change file attributes. Status: %08X\n"), eax
			.endif
		.else
			invoke DbgPrint, $CTA0("FileWorks: Can't query file attributes. Status: %08X\n"), eax
		.endif

		invoke ZwClose, hFile
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't open file. Status: %08X\n"), eax
	.endif

	ret

UnmarkAsReadOnly endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         AppendFile                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

AppendFile proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hFile:HANDLE

	invoke DbgPrint, $CTA0("\nFileWorks: Opening file to append data\n")

	InitializeObjectAttributes addr oa, addr g_usFileName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL

	; If only the FILE_APPEND_DATA and SYNCHRONIZE flags are set, the caller can write
	; only to the end of the file, and any offset information on writes to the file is ignored.
	; However, the file will automatically be extended as necessary
	; for this type of write operation.

	invoke ZwOpenFile, addr hFile, FILE_APPEND_DATA + SYNCHRONIZE, addr oa, addr iosb, \
									FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT
	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("FileWorks: File openeded\n")

		CTA0 " using ZwWriteFile", g_szDataToAppend, 4

		; If the call to ZwOpenFile set only the DesiredAccess flag FILE_APPEND_DATA,
		; ByteOffset is ignored. Data in the given Buffer, for Length bytes,
		; is written starting at the current end of file.

		invoke ZwWriteFile, hFile, 0, NULL, NULL, addr iosb, \
						addr g_szDataToAppend, sizeof g_szDataToAppend - 1, NULL, NULL
		.if eax == STATUS_SUCCESS
			invoke DbgPrint, $CTA0("FileWorks: Data appended to the file\n")
		.else
			invoke DbgPrint, $CTA0("FileWorks: Can't append data to file. Status: %08X\n"), eax
		.endif

		invoke ZwClose, hFile
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't open file. Status: %08X\n"), eax
	.endif

	ret

AppendFile endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                        TruncateFile                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TruncateFile proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hFile:HANDLE
local fsi:FILE_STANDARD_INFORMATION
local feofi:FILE_END_OF_FILE_INFORMATION

	invoke DbgPrint, $CTA0("\nFileWorks: Opening file to truncate\n")

	InitializeObjectAttributes addr oa, addr g_usFileName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL

	; Or just use FILE_GENERIC_WRITE
	
	invoke ZwOpenFile, addr hFile, FILE_WRITE_DATA + SYNCHRONIZE, addr oa, addr iosb, \
						FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT
	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("FileWorks: File openeded\n")

		invoke ZwQueryInformationFile, hFile, addr iosb, \
						addr fsi, sizeof fsi, FileStandardInformation
		.if eax == STATUS_SUCCESS

			invoke DbgPrint, $CTA0("FileWorks: EOF was: %08X\n"), fsi.EndOfFile.LowPart

			and feofi.EndOfFile.HighPart, 0
			mov eax, fsi.EndOfFile.LowPart
			shr eax, 1								; truncate to half size
			mov feofi.EndOfFile.LowPart, eax
			invoke ZwSetInformationFile, hFile, addr iosb, \
						addr feofi, sizeof feofi, FileEndOfFileInformation
			.if eax == STATUS_SUCCESS
				invoke DbgPrint, $CTA0("FileWorks: File truncated to its half size\n")
			.else
				invoke DbgPrint, $CTA0("FileWorks: Can't truncate file. Status: %08X\n"), eax		
			.endif

		.else
			invoke DbgPrint, $CTA0("FileWorks: Can't query file info. Status: %08X\n"), eax
		.endif

		invoke ZwClose, hFile
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't open file. Status: %08X\n"), eax
	.endif

	ret

TruncateFile endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         DeleteFile                                                
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DeleteFile proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hFile:HANDLE
local fdi:FILE_DISPOSITION_INFORMATION

	invoke DbgPrint, $CTA0("\nFileWorks: Opening file for deletion\n")

	InitializeObjectAttributes addr oa, addr g_usFileName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
	invoke ZwCreateFile, addr hFile, DELETE + SYNCHRONIZE, addr oa, addr iosb, \
						0, 0, FILE_SHARE_DELETE, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0
	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("FileWorks: File openeded\n")

		mov fdi.DeleteFile, TRUE
		invoke ZwSetInformationFile, hFile, addr iosb, addr fdi, sizeof fdi, FileDispositionInformation
		.if eax == STATUS_SUCCESS
			; The file has been marked for deletion. Do nothing with the file handle except closing it.
			invoke DbgPrint, $CTA0("FileWorks: File has been marked for deletion\n")
			invoke DbgPrint, $CTA0("FileWorks: It should be deleted when the last open handle is closed\n")
		.else
			invoke DbgPrint, $CTA0("FileWorks: Can't mark file for deletion. Status: %08X\n"), eax
		.endif

		invoke ZwClose, hFile
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't open file. Status: %08X\n"), eax
	.endif

	ret

DeleteFile endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DeleteDirectory                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DeleteDirectory proc

local oa:OBJECT_ATTRIBUTES
local iosb:IO_STATUS_BLOCK
local hDirectory:HANDLE

	InitializeObjectAttributes addr oa, addr g_usDirName, \
						OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL

	; The DDK stands that ZwDeleteFile exist only on Windows XP and later
	; but it's not true.

	invoke ZwDeleteFile, addr oa
	.if eax == STATUS_SUCCESS
		invoke DbgPrint, $CTA0("\nFileWorks: Directory deleted\n")			
	.else
		invoke DbgPrint, $CTA0("\nFileWorks: Can't delete directory. Status: %08X\n"), eax
	.endif

	ret

DeleteDirectory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      EnumerateFiles                                               
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EnumerateFiles proc uses esi

local status:NTSTATUS
local oa:OBJECT_ATTRIBUTES
local hSystemRootDirectory:HANDLE
local hDriversDirectory:HANDLE
local as:ANSI_STRING
local us:UNICODE_STRING
local iosb:IO_STATUS_BLOCK
local tf:TIME_FIELDS
local cb:DWORD
local pfdi:PFILE_DIRECTORY_INFORMATION 

	invoke DbgPrint, $CTA0("\nFileWorks: Opening directory to enumerate files\n")
	
	InitializeObjectAttributes addr oa, $CCOUNTED_UNICODE_STRING("\\SystemRoot"), \
								OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, NULL, NULL
								
	invoke ZwOpenFile, addr hSystemRootDirectory, FILE_LIST_DIRECTORY + SYNCHRONIZE, addr oa, \
						addr iosb, FILE_SHARE_READ + FILE_SHARE_WRITE + FILE_SHARE_DELETE, \
						FILE_DIRECTORY_FILE + FILE_SYNCHRONOUS_IO_NONALERT
	.if eax == STATUS_SUCCESS
	
		; Specify pathname relative to the directory file represented by the hSystemRootDirectory.
		
		InitializeObjectAttributes addr oa, $CCOUNTED_UNICODE_STRING("system32\\drivers"), \
							OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE, hSystemRootDirectory, NULL
							
		invoke ZwOpenFile, addr hDriversDirectory, FILE_LIST_DIRECTORY + SYNCHRONIZE, addr oa, \
							addr iosb, FILE_SHARE_READ + FILE_SHARE_WRITE + FILE_SHARE_DELETE, \
							FILE_DIRECTORY_FILE + FILE_SYNCHRONOUS_IO_NONALERT
		.if eax == STATUS_SUCCESS

			; 256 bites is enough to hold file name
			
			mov cb, sizeof FILE_DIRECTORY_INFORMATION + 256

			invoke ExAllocatePool, PagedPool, cb
			.if eax != NULL

				mov pfdi, eax
				mov esi, eax
				assume esi:ptr FILE_DIRECTORY_INFORMATION

				invoke DbgPrint, \
						$CTA0("\nFileWorks: ---------- Starting enumerate files ----------\n")

				; DDK stands ZwQueryDirectoryFile is available on Windows XP and later
				; but it's not true.
				; Let's enumerate all files which name starts whith 'c' for example.
				
				invoke ZwQueryDirectoryFile, hDriversDirectory, NULL, NULL, NULL, addr iosb, \
							esi, cb, FileDirectoryInformation, \
							TRUE, $CCOUNTED_UNICODE_STRING("c*"), TRUE
							
				.while eax != STATUS_NO_MORE_FILES

					.if ( eax == STATUS_SUCCESS )

						; Fill UNICODE_STRING manually instead of calling RtlInitUnicodeString
						; because of FILE_DIRECTORY_INFORMATION.FileName is not null-terminated
						
						mov eax, [esi].FileNameLength
						mov us._Length, ax
						mov us.MaximumLength, ax
						lea eax, [esi].FileName
						mov us.Buffer, eax
						
						invoke RtlUnicodeStringToAnsiString, addr as, addr us, TRUE
						
						.if eax == STATUS_SUCCESS

							invoke RtlTimeToTimeFields, addr [esi].CreationTime, addr tf
							movzx eax, tf.Day
							movzx ecx, tf.Month
							movzx edx, tf.Year

							; Who knows, may be sometime driver files grow bigger then 4Gb :-(((
							; But in our days we can be shure that LowPart is enough

							invoke DbgPrint, $CTA0("    %s   size=%d   created on %d.%02d.%04d\n"), \
										as.Buffer, [esi].EndOfFile.LowPart, eax, ecx, edx

							invoke RtlFreeAnsiString, addr as
						.endif

					.endif
					
					; Continue scanning
					
					invoke ZwQueryDirectoryFile, hDriversDirectory, NULL, NULL, NULL, addr iosb, \
								esi, cb, FileDirectoryInformation, \
								TRUE, NULL, FALSE
				.endw
				
				invoke DbgPrint, \
					$CTA0("FileWorks: ------------------------------------------------\n")

				assume esi:nothing
				invoke ExFreePool, pfdi
				
			.endif
			
			invoke ZwClose, hDriversDirectory
			
		.else
			invoke DbgPrint, $CTA0("FileWorks: Can't open drivers directory. Status: %08X\n"), eax
		.endif
		
		invoke ZwClose, hSystemRootDirectory
		
	.else
		invoke DbgPrint, $CTA0("FileWorks: Can't open system root directory. Status: %08X\n"), eax
	.endif

	ret

EnumerateFiles endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

	invoke DbgPrint, $CTA0("\nFileWorks: Entering DriverEntry\n")

	invoke CreateDirectory
	invoke CreateFile
	invoke WriteFile
	invoke MarkAsReadOnly
	invoke ReadFile
	invoke UnmarkAsReadOnly
	invoke AppendFile
	invoke ReadFile
	invoke TruncateFile
	invoke ReadFile
	invoke DeleteFile
	invoke DeleteDirectory
	invoke EnumerateFiles

	invoke DbgPrint, $CTA0("\nFileWorks: Leaving DriverEntry\n\n")

	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=FileWorks

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
