;@echo off
;goto make

.386
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\w2k\ntstatus.inc
include \masm32\include\w2k\ntddk.inc

include \masm32\include\w2k\ntoskrnl.inc

includelib \masm32\lib\w2k\ntoskrnl.lib

;:::::::::::::::::::::::::::::::: PAGED DATA ::::::::::::::::::::::::::::::::::::

PAGEDAT1 SEGMENT
PagedDword1 DWORD 0
PAGEDAT1 ENDS

PAGEDAT2 SEGMENT
PagedDword2 DWORD 0
PAGEDAT2 ENDS		
		
;:::::::::::::::::::::::::::::: NONPAGED DATA :::::::::::::::::::::::::::::::::::

.data
NonpagedDword DWORD 0

;:::::::::::::::::::::::::::::: NONPAGED CODE ::::::::::::::::::::::::::::::::::::

.code

NonpageableProc proc
	mov eax, NonpagedDword
	ret
NonpageableProc endp

;:::::::::::::::::::::::::::::::: PAGED CODE :::::::::::::::::::::::::::::::::::

.code PAGED1

PageableProc1 proc
	mov eax, PagedDword1
	ret
PageableProc1 endp

.code PAGED2

PageableProc2 proc
	mov eax, PagedDword2
	ret
PageableProc2 endp

.code INIT

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING
	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret
DriverEntry endp

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=Sections

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /out:%drv%.sys /subsystem:native /ignore:4078 %drv%.obj

del %drv%.obj

echo.
pause
