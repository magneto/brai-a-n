:: Author: Sebastien Maire
:: Contribs:

:: This file is used to automatically locate binaries needed for compilation (cmake and MsBuild)
:: It avoids setup troubles

@echo off
setlocal enabledelayedexpansion
setlocal enableextensions
set error_not_found="Cannot find : "

echo Configuring build binaries paths...

::locate MsBuild.exe
set MsBuildBinLoc="MsBuildBinLocation.nfo"
::locate cmake.exe
set cmakeBinLoc="CmakeBinLocation.nfo"

set /a cmakeFound=0
set /a MSFound=0

if exist "%MsBuildBinLoc%" (
	echo Already setup MsBuildPath
	set /a MSFound=1
)

if "%MSFound%"=="0" (
	echo "TOTOTO"
	for /r C:\Windows\Microsoft.NET\Framework\v4.0.30319 %%a in (*) do (
		if "%%~nxa"=="MSBuild.exe" (

				echo "%%~dpnxa" >"%MsBuildBinLoc%"
				echo Succesfully found MSBuild.exe
				set MSFound=1
				goto :findCmake
		)
	)
)

echo %error_not_found% "MSBuild.exe"
goto :quitStep
:findCmake
if exist "%cmakeBinLoc%" (
	echo Already setup cmakePath
	set /a cmakeFound=1
	goto :finalStep
)
if "%cmakeFound%"=="0" (
	for /r "C:\Program Files\cmake" %%a in (*) do (
		if "%%~nxa"=="cmake.exe" (
			echo "%%~dpnxa" > "%cmakeBinLoc%"
			echo Succesfully found cmake.exe
			set cmakeFound=1
			goto :finalStep
		)
	)
	for /r "C:\Program Files (x86)\cmake" %%a in (*) do (
		if "%%~nxa"=="cmake.exe" (
			echo "%%~dpnxa" > "%cmakeBinLoc%"
			echo Succesfully found cmake.exe
			set cmakeFound=1
			goto :finalStep
		)
	)
)

echo %error_not_found% "cmake.exe"
goto :quitStep

:finalStep
echo Paths configuration Done.
goto :end

:quitStep
echo Errors occured.

:end

