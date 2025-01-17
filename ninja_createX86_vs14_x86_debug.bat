@echo off
SET BASE_DIR=%~dp0
Setlocal EnableDelayedExpansion

if "%INCLUDE%"=="" call  "%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

if "%QTDIR%"=="" SET QTDIR=C:\Qt\qt-everywhere-opensource-src-4.8.6_vs14
if "%QMAKESPEC%"=="" SET QMAKESPEC=win32-msvc2013

if "%BUILD_TYPE%"=="" SET BUILD_TYPE=Debug

SET BUILD_TOOL=ninja
SET BUILD_COMMAND=
SET INSTALL_COMMAND=install
SET CMAKE=cmake -G "Ninja" -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe %CMAKE_OVERRIDES% 

SET TEMP_DIR=tmp_vs14_x86_%BUILD_TYPE%%STATIC%
SET BIN_DIR=bin_vs14_x86_%BUILD_TYPE%%STATIC%

if not exist %BASE_DIR%\%TEMP_DIR% mkdir %BASE_DIR%\%TEMP_DIR%
if not exist %BASE_DIR%\%BIN_DIR% mkdir %BASE_DIR%\%BIN_DIR%
if not exist %BASE_DIR%\%BIN_DIR%\include mkdir %BASE_DIR%\%BIN_DIR%\include
if not exist %BASE_DIR%\%BIN_DIR%\lib mkdir %BASE_DIR%\%BIN_DIR%\lib


REM Build Clapack
mkdir %BASE_DIR%\%TEMP_DIR%\clapack
cd /d %BASE_DIR%\%TEMP_DIR%\clapack
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% -DBUILD_TESTING=OFF %BASE_DIR%\src\clapack
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build SBW
mkdir %BASE_DIR%\%TEMP_DIR%\SBW
cd /d %BASE_DIR%\%TEMP_DIR%\SBW
%CMAKE% -DWITH_BUILD_BROKER=OFF -DWITH_BUILD_SHARED=OFF -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% %BASE_DIR%\src\core
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build CPP Unit
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\cppunit
cd /d %BASE_DIR%\%TEMP_DIR%\cppunit
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% %BASE_DIR%\src\cppunit
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build Zlib
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\zlib
cd /d %BASE_DIR%\%TEMP_DIR%\zlib
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% -DBUILD_shared=OFF %BASE_DIR%\src\zlib
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build Zipper
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\zipper
cd /d %BASE_DIR%\%TEMP_DIR%\zipper
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DLIBZ_LIBRARY=%BASE_DIR%\%BIN_DIR%\lib\zdll.lib -DLIBZ_INCLUDE_DIR=%BASE_DIR%\%BIN_DIR%\include -DBUILD_TEST=OFF -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% -DBUILD_shared=OFF %BASE_DIR%\src\zipper
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%


REM Build Expat
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\expat
cd /d %BASE_DIR%\%TEMP_DIR%\expat
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% -DBUILD_shared=OFF -DBUILD_tests=OFF -DBUILD_tools=OFF -DBUILD_examples=OFF %BASE_DIR%\src\expat
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build libSBML
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\libsbml
cd /d %BASE_DIR%\%TEMP_DIR%\libsbml
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%/%BIN_DIR% -DLIBSBML_USE_LEGACY_MATH=ON -DENABLE_COMP=ON -DENABLE_LAYOUT=ON -DENABLE_REQUIREDELEMENTS=OFF -DENABLE_RENDER=ON -DENABLE_COMP=ON -DENABLE_FBC=OFF -DENABLE_SPATIAL=OFF -DENABLE_GROUPS=OFF -DWITH_EXPAT=ON -DWITH_LIBXML=OFF -DLIBSBML_DEPENDENCY_DIR=%BASE_DIR%\%BIN_DIR% -DLIBSBML_SKIP_SHARED_LIBRARY=ON -DWITH_BZIP2=OFF -DWITH_ZLIB=OFF -DENABLE_UNIVERSAL=ON %BASE_DIR%/src/libSBML
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build libCombine
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\libCombine
cd /d %BASE_DIR%\%TEMP_DIR%\libCombine
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DWITH_QT_FILESYSTEM=ON -DCOMBINE_DEPENDENCY_DIR=%BASE_DIR%\%BIN_DIR% -DBUILD_TEST=OFF -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% -DLIBCOMBINE_SKIP_SHARED_LIBRARY=OFF -DEXTRA_LIBS=%BASE_DIR%\%BIN_DIR%\lib\expat.lib  %BASE_DIR%/src/libCombine
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%


REM Build libnuml
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\numl
cd /d %BASE_DIR%\%TEMP_DIR%\numl
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%/%BIN_DIR% -DWITH_EXPAT=ON -DWITH_LIBXML=OFF -DLIBNUML_DEPENDENCY_DIR=%BASE_DIR%\%BIN_DIR% -DLIBNUML_SKIP_SHARED_LIBRARY=ON -DWITH_BZIP2=OFF -DWITH_ZLIB=OFF -DENABLE_UNIVERSAL=ON -DLIBSBML_STATIC=ON  %BASE_DIR%/src/libnuml
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM build libSEDML
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\libSEDML
cd /d %BASE_DIR%\%TEMP_DIR%\libSEDML
%CMAKE%  -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%/%BIN_DIR% -DLIBSBML_STATIC=ON  -DLIBNUML_STATIC=ON -DLIBSEDML_SHARED_VERSION=OFF  -DLIBSEDML_SKIP_SHARED_LIBRARY=ON -DLIBSEDML_DEPENDENCY_DIR=%BASE_DIR%/%BIN_DIR% -DEXTRA_LIBS=%BASE_DIR%/lib/expat.lib %BASE_DIR%/src/libSEDML
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build Raptor
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\raptor
cd /d %BASE_DIR%\%TEMP_DIR%\raptor
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% %BASE_DIR%\src\raptor
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM mml
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\mml
cd /d %BASE_DIR%\%TEMP_DIR%\mml
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% %BASE_DIR%\src\mml
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build QWT
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\qwt
cd /d %BASE_DIR%\%TEMP_DIR%\qwt
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% %BASE_DIR%\src\qwt
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%

REM Build QWT Plot
cd /d %BASE_DIR%
mkdir %BASE_DIR%\%TEMP_DIR%\qwtplot3d
cd /d %BASE_DIR%\%TEMP_DIR%\qwtplot3d
%CMAKE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_INSTALL_PREFIX=%BASE_DIR%\%BIN_DIR% %BASE_DIR%\src\qwtplot3d-qt4
%BUILD_TOOL% %BUILD_COMMAND%
%BUILD_TOOL% %INSTALL_COMMAND%


REM copy PDBs
cd /d %BASE_DIR%

xcopy /y /s %BASE_DIR%\%TEMP_DIR%\clapack\BLAS\SRC\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\clapack\F2CLIBS\libf2c\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\libsbml\src\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\cppunit\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\expat\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\raptor\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\mml\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\qwt\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\qwtplot3d\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\SBW\SBWCore\*.pdb %BASE_DIR%\%BIN_DIR%\lib
xcopy /y /s %BASE_DIR%\%TEMP_DIR%\SBW\SBWBroker\*.pdb %BASE_DIR%\%BIN_DIR%\lib

cd /d %BASE_DIR%
