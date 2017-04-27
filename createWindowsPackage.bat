@echo off
Setlocal EnableDelayedExpansion

rmdir /S /Q tmp
rmdir /S /Q bin-MD

set CMAKE_OVERRIDES="-DWITH_STATIC_RUNTIME=OFF"
call createWindows.bat expat raptor SBW libSBML libnuml libSEDML MML qwt qwtplot3d 
move bin bin-MD

rmdir /S /Q tmp
rmdir /S /Q bin-MT

set CMAKE_OVERRIDES="-DWITH_STATIC_RUNTIME=ON"
call createWindows.bat expat raptor SBW libSBML libnuml libSEDML
move bin bin-MT 
./
