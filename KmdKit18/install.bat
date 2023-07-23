rem ------------------------------------------------------------------
rem Run this file to install KmdKit package.
rem ------------------------------------------------------------------

cd lib
xcopy *.* \masm32\lib /S /I
cd..
cd include
xcopy *.* \masm32\include /S /I
cd..
cd macros
xcopy *.* \masm32\macros /S /I
cd..

echo.
pause
