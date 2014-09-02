@echo off
REM 
REM Utility script building all intel release / debug projects
REM 

SET THIS_DIR=%~dp0
Setlocal EnableDelayedExpansion
SET VC_VARS=C:\Program Files (x86)\Intel\Compiler\11.1\048\bin\iclvars.bat

REM set up 64bit environment
call  "%VC_VARS%" intel64
title Building x64 debug (intel)
cd /d %THIS_DIR%
call create_icc_x64_release 
move bin bin_icc_x64_release
move tmp tmp_icc_x64_release


REM set up 32bit environment
call  "%VC_VARS%" ia32
title Building x86 release (intel)
cd /d %THIS_DIR%
call create_icc_x86_release 
move bin bin_icc_x86_release
move tmp tmp_icc_x86_release

