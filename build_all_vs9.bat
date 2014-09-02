@echo off
REM 
REM Utility script building all VS9 release / debug projects
REM 

SET THIS_DIR=%~dp0
Setlocal EnableDelayedExpansion

REM set up 64bit environment
@SET WindowsSdkDir=C:\Program Files\Microsoft SDKs\Windows\v6.0A\
call "c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars64.bat"  %*
title Building x64 debug
cd /d %THIS_DIR%
call createX86_vs11_x64_debug 
move bin bin_vs9_x64_debug
move tmp tmp_vs9_x64_debug

title Building x64 release
cd /d %THIS_DIR%
call createX86_vs11_x64_release 
move bin bin_vs9_x64_release 
move tmp tmp_vs9_x64_release 

REM set up 32bit environment
@SET WindowsSdkDir=C:\Program Files\Microsoft SDKs\Windows\v6.0A\
call "c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"  %*
title Building x86 debug
cd /d %THIS_DIR%
call createX86_vs11_x86_debug 
move bin bin_vs9_x86_debug
move tmp tmp_vs9_x86_debug

title Building x86 release
cd /d %THIS_DIR%
call createX86_vs11_x86_release 
move bin bin_vs9_x86_release 
move tmp tmp_vs9_x86_release 

