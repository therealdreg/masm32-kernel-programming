;@echo off
;goto make

.386
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\w2k\ntddk.inc
include \masm32\include\w2k\ntstatus.inc

include \masm32\include\kernel32.inc
include \masm32\include\w2k\ntdll.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\w2k\ntdll.lib

include \masm32\Macros\Strings.mac

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                         F U N C T I O N S   P R O T O T Y P E S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include protos.inc

;g_pfnRtlInitUnicodeString pproto02
;externdef g_pfnRtlInitUnicodeString:pproto02
;externdef g_pfnRtlInitUnicodeString:ptr proto02

;g_pfnRtlInitUnicodeString	pproto02	?
;externdef g_pfnNtOpenSection:pproto03

;g_pfnNtMapViewOfSectiong	pproto02	?
;g_pfnNtUnmapViewOfSection	pproto10	?
;g_pfnRtlNtStatusToDosError	pproto01	?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const
CCOUNTED_UNICODE_STRING	"\\Device\\PhysicalMemory", g_usPhysicalMemory, 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?
;g_pfnRtlInitUnicodeString	pproto02	?
comment ^
g_pfnNtOpenSection			pproto03	?
g_pfnNtMapViewOfSection		pproto10	?
g_pfnNtUnmapViewOfSection	pproto02	?
g_pfnRtlNtStatusToDosError	pproto01	?
^
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              I N I T I A L I Z E D  D A T A                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;.data
;name1	UNICODE_STRING 		<>
;oa		OBJECT_ATTRIBUTES	<>

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    NtStatusToDosError                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

NtStatusToDosError proc status:NTSTATUS

	invoke RtlNtStatusToDosError, status
	ret

NtStatusToDosError endp
comment ^
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                      GetNtdllEntries                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GetNtdllEntries proc

local hinstanceNtdll:PVOID

	invoke GetModuleHandle, $CTA0("ntdll.dll")
	.if eax != NULL
		mov hinstanceNtdll, eax


;		invoke GetProcAddress, hinstanceNtdll, $CTA0("RtlInitUnicodeString")
;		.if eax == NULL
;			jmp @F
;		.endif
;		mov g_pfnRtlInitUnicodeString, eax

		invoke GetProcAddress, hinstanceNtdll, $CTA0("NtOpenSection")
		.if eax == NULL
			jmp @F
		.endif
		mov g_pfnNtOpenSection, eax

		invoke GetProcAddress, hinstanceNtdll, $CTA0("NtMapViewOfSection")
		.if eax == NULL
			jmp @F
		.endif
		mov g_pfnNtMapViewOfSection, eax

		invoke GetProcAddress, hinstanceNtdll, $CTA0("NtUnmapViewOfSection")
		.if eax == NULL
			jmp @F
		.endif
		mov g_pfnNtUnmapViewOfSection, eax

		invoke GetProcAddress, hinstanceNtdll, $CTA0("RtlNtStatusToDosError")
		.if eax == NULL
			jmp @F
		.endif
		mov g_pfnRtlNtStatusToDosError, eax

	.endif
@@:
	ret

GetNtdllEntries endp
^
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                    OpenPhysicalMemory                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OpenPhysicalMemory proc

local status:NTSTATUS
local hPhysMem:HANDLE
local oa:OBJECT_ATTRIBUTES

	and hPhysMem, NULL

	lea ecx, oa
	InitializeObjectAttributes ecx, offset g_usPhysicalMemory, OBJ_CASE_INSENSITIVE, NULL, NULL

	invoke NtOpenSection, addr hPhysMem, SECTION_MAP_READ, ecx
;	invoke NtOpenSection, addr hPhysMem, SECTION_MAP_WRITE, ecx
	mov eax, hPhysMem
	ret

OpenPhysicalMemory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       MapPhysicalMemory                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MapPhysicalMemory proc hPhysMem:HANDLE, pdwAddress:PDWORD, pdwLength:PDWORD, pdwBaseAddress:PDWORD

local status:NTSTATUS
local SectionOffset:PHYSICAL_ADDRESS

	mov eax, pdwBaseAddress
	and dword ptr [eax], 0

	and SectionOffset.HighPart, 0
	mov eax, pdwAddress
	push dword ptr [eax]
	pop SectionOffset.LowPart

	mov ecx, pdwLength
	mov ecx, [ecx]
	invoke NtMapViewOfSection, hPhysMem, -1, pdwBaseAddress, 0, ecx, addr SectionOffset, pdwLength, ViewShare, 0, PAGE_READONLY

	.if eax == STATUS_SUCCESS
		mov ecx, pdwAddress
		push SectionOffset.LowPart
		pop dword ptr [ecx]
	.endif

	ret

MapPhysicalMemory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     UnmapPhysicalMemory                                           
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

UnmapPhysicalMemory proc dwBaseAddress:DWORD

	invoke NtUnmapViewOfSection, -1, dwBaseAddress

	ret

UnmapPhysicalMemory endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end

;:make
;\masm32\bin\ml /nologo /c /coff PhysMemWorks.bat
;echo.
;pause
