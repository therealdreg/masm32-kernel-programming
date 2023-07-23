;@echo off
;goto make

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
;  beeper - Kernel Mode Driver
;  Makes beep thorough computer speaker
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

include \masm32\include\w2k\hal.inc

includelib \masm32\lib\w2k\hal.lib

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                           U S E R   D E F I N E D   E Q U A T E S                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TIMER_FREQUENCY	equ 1193167			; 1,193,167 Hz
OCTAVE			equ 2

;PITCH_A		equ 440				;  440,00 Hz
;PITCH_As		equ 446				;  466,16 Hz
;PITCH_H		equ 494				;  493,88 Hz
PITCH_C			equ 523				;  523,25 Hz
PITCH_Cs		equ 554				;  554,37 Hz
PITCH_D			equ 587				;  587,33 Hz
PITCH_Ds		equ 622				;  622,25 Hz
PITCH_E			equ 659				;  659,25 Hz
PITCH_F			equ 698				;  698,46 Hz
PITCH_Fs		equ 740				;  739,99 Hz
PITCH_G			equ 784				;  783,99 Hz
PITCH_Gs		equ 831				;  830,61 Hz
PITCH_A			equ 880				;  880,00 Hz
PITCH_As		equ 988				;  987,77 Hz
PITCH_H			equ 1047			; 1046,50 Hz

; We are going to play c-major chord

TONE_1			equ TIMER_FREQUENCY/(PITCH_C*OCTAVE)
TONE_2			equ TIMER_FREQUENCY/(PITCH_E*OCTAVE)
TONE_3			equ (PITCH_G*OCTAVE); for HalMakeBeep

DELAY			equ 2000000h		; for my ~1000mHz machine

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                           U S E R   D E F I N E D   M A C R O S                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DO_DELAY MACRO
	; Silly method, but it works ;-)
	mov eax, DELAY
	.while eax
		dec eax
	.endw
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                          C O D E                                                  
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                         MakeBeep1                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MakeBeep1 proc dwPitch:DWORD

	; Direct hardware access

	cli

	mov al, 10110110y
	out 43h, al         ; Timer 8253-5 (AT: 8254.2).

	mov eax, dwPitch
	out 42h, al

	mov al, ah
	out 42h, al

	; speaker ON
	in al, 61h
	or  al, 11y
	out 61h, al

	sti

	DO_DELAY

	cli

	; speaker OFF
	in al, 61h
	and al, 11111100y
	out 61h, al

	sti

	ret

MakeBeep1 endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                            MakeBeep2                                              
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MakeBeep2 proc dwPitch:DWORD

	; Hardware access via HAL using *_PORT_UCHAR/*_PORT_UCHAR functions

	cli

	invoke WRITE_PORT_UCHAR, 43h, 10110110y

	mov eax, dwPitch
	and eax, 0FFh
	invoke WRITE_PORT_UCHAR, 42h, eax
	mov eax, dwPitch
	shr eax, 8
	and eax, 0FFh
	invoke WRITE_PORT_UCHAR, 42h, eax

	; speaker ON
	invoke READ_PORT_UCHAR, 61h
	or  al, 11y
	and eax, 0FFh
	invoke WRITE_PORT_UCHAR, 61h, eax

	sti

	DO_DELAY	

	cli

	; speaker OFF
	invoke READ_PORT_UCHAR, 61h
	and al, 11111100y
	and eax, 0FFh
	invoke WRITE_PORT_UCHAR, 61h, eax

	sti

	ret

MakeBeep2 endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       DriverEntry                                                 
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

	invoke MakeBeep1, TONE_1
	invoke MakeBeep2, TONE_2

	; Hardware access via hal.dll function HalMakeBeep
	invoke HalMakeBeep, TONE_3
	DO_DELAY
	invoke HalMakeBeep, 0

	mov eax, STATUS_DEVICE_CONFIGURATION_ERROR
	ret

DriverEntry endp

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                                                                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

end DriverEntry

:make

set drv=beeper

\masm32\bin\ml /nologo /c /coff %drv%.bat
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj

del %drv%.obj

echo.
pause
