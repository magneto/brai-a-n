:: Author: Sebastien Maire
:: Contribs:

@echo off
set argc=0
for %%x in (%*) do Set /A argc+=1

set build_folder=".\build\"

if %argc% == 0 goto :all
goto :%1

:all
mkdir %build_folder%
cd %build_folder%
call "..\findBuildSoft.bat"
set /p cmakePath=<CmakeBinLocation.nfo
set /p MSPath=<MsBuildBinLocation.nfo
%cmakePath% ".."
%cmakePath% --build . --target braian --config Debug
%MSPath% braian.sln /m
cd ..
goto :wait_key

:sol
mkdir %build_folder%
cd %build_folder%
call "..\findBuildSoft.bat"
set /p cmakePath=<CmakeBinLocation.nfo
%cmakePath% ".."
cd ..
goto :wait_key

:clean
rmdir /q /s %build_folder%
goto :wait_key

:re
rmdir /q /s %build_folder%
goto :all

:resol
rmdir /q /s %build_folder%
goto :sol

:help
echo Commands are:
echo all : generates solution and build it
echo sol : only generates solution
echo re : rebuild entire project
echo resol : regenerates solution
echo help : diplay this help

:wait_key
pause