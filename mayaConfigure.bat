setlocal

SET MAYA_VERSION=2020
SET BUILD=mayabuild_%MAYA_VERSION%
SET COMPILER=Visual Studio 15 2017 Win64

SET PFX=%~dp0
cd %PFX%
rmdir %BUILD% /s /q
mkdir %BUILD%
cd %BUILD%


REM I like to move folders and installs around, so I'm adding some code to programmatically get some folder locations
REM If these lines don't work, you can just hard code MYDOCUMENTS and ADLOC

REM So programmatically get the current MyDocuments folder on Windows
for /f "skip=2 tokens=2*" %%A ^
in ('reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" /v "Personal"') ^
do set "MYDOCUMENTS=%%B"

REM And programmatically get the current Maya install location
REM I will get the parent of this location (ADLOC) to use as the MAYA_INSTALL_BASE_PATH
for /f "skip=2 tokens=2*" %%A ^
in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Autodesk\Maya\%MAYA_VERSION%\Setup\InstallPath" /v "MAYA_INSTALL_LOCATION"') ^
do set "MAYALOC=%%B"
set ADLOC=%MAYALOC%..


cmake ^
    -DMAYA_VERSION=%MAYA_VERSION% ^
    -DMAYA_INSTALL_BASE_PATH="%ADLOC%" ^
    -DCMAKE_INSTALL_PREFIX="%MYDOCUMENTS%\maya\modules" ^
    -G "%COMPILER%" ..\

cmake --build . --config Release --target Install

pause

