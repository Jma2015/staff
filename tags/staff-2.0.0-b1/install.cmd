@echo off
rem start this file to install project into %STAFF_HOME%

set arch=%1%

if '%arch%' == '' (
  echo.
  echo Arch is not set. To install please enter
  echo  %0% ARCH
  echo Where ARCH is either of `Win32` or `x64`.
  echo.
  goto errexit
)

if not '%arch%' == 'Win32' (
if not '%arch%' == 'x64' (
  echo.
  echo Error: Invalid arch set: [%arch%]. Supported architectures are: Win32, x64
  goto errhelp
)
)

if "%STAFF_HOME%" == "" (
  echo Error: STAFF_HOME is not set
  goto errexit
)

if "%AXIS2C_HOME%" == "" (
  echo Error: AXIS2C_HOME is not set
  goto errexit
)

set staffdeploydir=%cd%\staff\deploy\win_%arch%
set dasdeploydir=%cd%\das\deploy\win_%arch%

if not exist %STAFF_HOME% mkdir %STAFF_HOME%

echo installing axis2/c staff modules and service...
xcopy /Y /S %staffdeploydir%\axis2 %AXIS2C_HOME% >>install.log
if %ERRORLEVEL% gtr 0 (
    echo Failed to install axis2 staff modules and service >&2
    echo Please see install.log >&2
    goto errexit
)

echo installing staff...
xcopy /Y /S %staffdeploydir%\staff %STAFF_HOME% >>install.log
if %ERRORLEVEL% gtr 0 (
    echo Failed to install staff >&2
    echo Please see install.log >&2
    goto errexit
)

echo installing das...
xcopy /Y /S %dasdeploydir%\staff %STAFF_HOME% >>install.log
if %ERRORLEVEL% gtr 0 (
    echo Failed to install das >&2
    echo Please see install.log >&2
    goto errexit
)

echo installation succesful

goto exit
:errexit
pause
:exit
