@echo off
SET BASE_DIR=%~dp0
Setlocal EnableDelayedExpansion

::if exist tmp-MD rmdir /S /Q tmp-MD
::if exist bin-MD rmdir /S /Q bin-MD

set CMAKE_OVERRIDES=-DWITH_STATIC_RUNTIME=OFF -DSELECT_QT=Qt5 -DQt5_DIR=C:/Qt/Qt5.11.1/5.11.1/msvc2015/lib/cmake/Qt5
SET BUILD_DIR=%BASE_DIR%\tmp-MD
SET INSTALL_DIR=%BASE_DIR%\bin-MD
call createWindows.bat expat raptor crossguid SBW libSBML libnuml libSEDML zlib zipper libCombine MML qwt qwtplot3d 


::if exist tmp-MD rmdir /S /Q tmp-MT
::if exist bin-MD rmdir /S /Q bin-MT

set CMAKE_OVERRIDES=-DWITH_STATIC_RUNTIME=ON
SET BUILD_DIR=%BASE_DIR%\tmp-MT
SET INSTALL_DIR=%BASE_DIR%\bin-MT
call createWindows.bat expat raptor crossguid SBW libSBML libnuml libSEDML zlib zipper libCombine 
