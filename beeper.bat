set drv=beeper
\masm32\bin\ml /nologo /c /coff %drv%.asm
\masm32\bin\link /nologo /driver /base:0x10000 /align:32 /out:%drv%.sys /subsystem:native %drv%.obj
del %drv%.obj
echo.
pause
