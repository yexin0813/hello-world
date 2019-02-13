@echo off
set CURPATH=%cd%
cd /d %~dp0
echo %cd%
echo %ProgramFiles(x86)%
for /r "%ProgramFiles(x86)%\SEGGER\" %%i in (*JFlash.exe) ^
do start "" "%%i" "-openprj.\flash_prog_1v8.jflash" "-open%1\flash.hex"
cd /d %CURPATH%