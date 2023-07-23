 ;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 ;
 ;  beeper – Kernel Mode Drive
 ;   Makes beep thorough computer speaker
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
 ;                                          E Q U A T E S                                 
 ;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

 TIMER_FREQUENCY        equ 1193167                   ; 1,193,167 Гц
 OCTAVE                 equ 2                         ; octave multiplier

 PITCH_C                equ 523                       ; c    -  523,25 Гц
 PITCH_Cs               equ 554                       ; c#   -  554,37 Гц
 PITCH_D                equ 587                       ; d    -  587,33 Гц
 PITCH_Ds               equ 622                       ; d#   -  622,25 Гц
 PITCH_E                equ 659                       ; e    -  659,25 Гц
 PITCH_F                equ 698                       ; f    -  698,46 Гц
 PITCH_Fs               equ 740                       ; f#   -  739,99 Гц
 PITCH_G                equ 784                       ; g    -  783,99 Гц
 PITCH_Gs               equ 831                       ; g#   -  830,61 Гц
 PITCH_A                equ 880                       ; a    -  880,00 Гц
 PITCH_As               equ 988                       ; a#   -  987,77 Гц
 PITCH_H                equ 1047                      ; h    - 1046,50 Гц


 ; We are going to play c-major chord

 TONE_1                 equ TIMER_FREQUENCY/(PITCH_C*OCTAVE)
 TONE_2                 equ TIMER_FREQUENCY/(PITCH_E*OCTAVE)
 TONE_3                 equ (PITCH_G*OCTAVE)           ; for HalMakeBeep

 DELAY                  equ 1800000h                   ; for my ~800mHz machine

 ;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 ;                                            M A C R O S                                   
 ;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

 DO_DELAY MACRO
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
 ;                                            MakeBeep1                                              
 ;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

 MakeBeep1 proc dwPitch:DWORD

     ; Direct hardware access

     cli

     mov al, 10110110y
     out 43h, al

     mov eax, dwPitch
     out 42h, al

     mov al, ah
     out 42h, al

     ; Turn speaker ON

     in al, 61h
     or  al, 11y
     out 61h, al

     sti

     DO_DELAY

     cli

     ; Turn speaker OFF

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

     ; Hardware access using WRITE_PORT_UCHAR и READ_PORT_UCHAR
     ; functions from hal.dll

     cli

     invoke WRITE_PORT_UCHAR, 43h, 10110110y

     mov eax, dwPitch
     invoke WRITE_PORT_UCHAR, 42h, al
     mov eax, dwPitch
     invoke WRITE_PORT_UCHAR, 42h, ah

     ; Turn speaker ON

     invoke READ_PORT_UCHAR, 61h
     or  al, 11y
     invoke WRITE_PORT_UCHAR, 61h, al

     sti

     DO_DELAY	

     cli

     ; Turn speaker OFF

     invoke READ_PORT_UCHAR, 61h
     and al, 11111100y
     invoke WRITE_PORT_UCHAR, 61h, al

     sti

     ret

 MakeBeep2 endp

 ;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 ;                                       DriverEntry                                                 
 ;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

 DriverEntry proc pDriverObject:PDRIVER_OBJECT, pusRegistryPath:PUNICODE_STRING

     invoke MakeBeep1, TONE_1
     invoke MakeBeep2, TONE_2

     ; Hardware access using hal.dll HalMakeBeep function 

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
