@echo off
REM 
REM Utility script building all VS11 release / debug projects
REM 

SET THIS_DIR=%~dp0
SET VC_VARS=%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat
SET CMAKE_OVERRIDES=-DWITH_STATIC_RUNTIME=ON
SET STATIC=_static
Setlocal EnableDelayedExpansion

::REM set up 32bit environment
call  "%VC_VARS%" x86
title Building x86 debug
cd /d %THIS_DIR%
call createX86_vs14_x86_debug 

title Building x86 release
cd /d %THIS_DIR%
call createX86_vs14_x86_release 
 
::REM set up 64bit environment
call  "%VC_VARS%" x86_amd64
title Building x64 debug
cd /d %THIS_DIR%
call createX86_vs14_x64_debug 

title Building x64 release
cd /d %THIS_DIR%
call createX86_vs14_x64_release 
