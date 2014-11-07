@echo off 

REM This file will build libsbml for windows (32 and 64 bit)
REM
REM 2011/02/26 Frank Bergmann
REM

REM ensure visual studio is in the path
if "%INCLUDE%"=="" call vs10.bat

REM set up directory variables
SET BASE_DIR=%~dp0
SET LIBSBML_ROOT=D:\Development\libsbml

REM if set to a directory, zip files will be produced
REM and copied into it. To disable simply comment out 
REM the next line.
SET ZIP_DIR=D:/Development/out


cd  /d %BASE_DIR%

REM this version won't build the dependency libraries 
REM (though it could). Instead we just assume the 
REM 32bit (static, static CRT) and 64bit (static, static CRT) 
REM dependencies are present

SET DEPENDENCIES_32_BIT=D:\Development\libsbml_dependencies\dist\libSBML Dependencies-1.0.0-b1-win32
SET DEPENDENCIES_64_BIT=

SET BUILD_DIR_32_BIT=D:\Development\libsbml_32
SET BUILD_DIR_64_BIT=D:\Development\libsbml_64

REM IF EXIST "%BUILD_DIR_32_BIT%" goto BUILD_DIR_EXISTS
REM IF EXIST "%BUILD_DIR_64_BIT%" goto BUILD_DIR_EXISTS


if "%1" == "--64bitonly" goto DONE_32

:BUILD_32
REM BUILD 32bit libsbml
mkdir "%BUILD_DIR_32_BIT%"
cd /d "%BUILD_DIR_32_BIT%"
copy "%BASE_DIR%\CMakeCache_32.txt" CMakeCache.txt
cmake -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=D:\Development\libsbml_32_inst -DCMAKE_SIZEOF_VOID_P=4 %LIBSBML_ROOT% 
cmake -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=D:\Development\libsbml_32_inst -DCMAKE_SIZEOF_VOID_P=4 %LIBSBML_ROOT% 
nmake
rd /s /q D:\Development\libsbml_32_inst\include
rd /s /q D:\Development\libsbml_32_inst\win32\include
rd /s /q D:\Development\libsbml_32_inst\win64\include
nmake install

if "%ZIP_DIR%" == "" goto DONE_32
cpack -G ZIP
move /Y *.zip "%ZIP_DIR%"

:DONE_32
if "%1" == "--32bitonly" goto ALL_DONE

REM REM BUILD 64bit libsbml
mkdir "%BUILD_DIR_64_BIT%"
cd /d "%BUILD_DIR_64_BIT%"
copy "%BASE_DIR%\CMakeCache_64.txt" CMakeCache.txt
cmake -G "Visual Studio 10 Win64" -DCMAKE_INSTALL_PREFIX= -DCMAKE_SIZEOF_VOID_P=8 %LIBSBML_ROOT% 
cmake -G "Visual Studio 10 Win64" -DCMAKE_INSTALL_PREFIX= -DCMAKE_SIZEOF_VOID_P=8 %LIBSBML_ROOT%  
devenv libsbml.sln /build RelWithDebInfo
rd /s /q \include
devenv libsbml.sln /build RelWithDebInfo /project INSTALL

if "%ZIP_DIR%" == "" goto DONE_64
cpack -G ZIP -C RelWithDebInfo
move /Y *.zip "%ZIP_DIR%"

:DONE_64

goto ALL_DONE
:BUILD_DIR_EXISTS
echo.
echo The build directory already exists, instead of overwriting
echo it please remove it first. The build directories are: 
echo. 
echo 32bit: %BUILD_DIR_32_BIT%
echo 64bit: %BUILD_DIR_64_BIT%
echo.
echo Please remove them before continuing.
echo.
goto ALL_DONE
:DEPENDENCIES_PRESENT
echo.
echo The dependencies directory already exists. This usually
echo is the case if a previous run was not finished:
echo. 
echo    %DEPENDENCIES_CURRENT%
echo. 
echo Please remove it first. 
echo. 
goto ALL_DONE
:MISSING_DEPENDENCIES
echo.
echo An error occured while trying to build libsbml for windows
echo please ensure that the dependencies are present. They are 
echo assumed to be in:
echo.
echo 32bit: %DEPENDENCIES_32_BIT%
echo 64bit: %DEPENDENCIES_64_BIT%
echo. 
echo Other than that, Visual Studio needs to be installed, and
echo CMake needs to be in the path. 
echo. 
goto ALL_DONE

:ALL_DONE
cd  /d %BASE_DIR%
