@echo off
REM 
REM Utility script building all VS11 release / debug projects
REM 

SET THIS_DIR=%~dp0

if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build" SET VC_VARS=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build" SET VC_VARS=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat

if "%QTDIR%"=="" SET QTDIR=C:\Qt\Qt5.11.1\5.11.1\msvc2015\lib\cmake\Qt5
if "%SELECT_QT%"=="" SET SELECT_QT=Qt5


SET CMAKE_OVERRIDES=-DWITH_STATIC_RUNTIME=ON
SET STATIC=_static
Setlocal EnableDelayedExpansion

::REM set up 32bit environment
call  "%VC_VARS%" x86
title Building x86 debug
cd /d %THIS_DIR%
call createX86_vs15_x86_debug 

title Building x86 release
cd /d %THIS_DIR%
call createX86_vs15_x86_release 
 
::REM set up 64bit environment
call  "%VC_VARS%" x86_amd64
title Building x64 debug
cd /d %THIS_DIR%
call createX86_vs15_x64_debug 

title Building x64 release
cd /d %THIS_DIR%
call createX86_vs15_x64_release 
