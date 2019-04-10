@echo off
SET BASE_DIR=%~dp0
Setlocal EnableDelayedExpansion

if "%BUILD_DIR%"=="" SET BUILD_DIR=%BASE_DIR%\tmp
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

if "%INSTALL_DIR%"=="" SET INSTALL_DIR=%BASE_DIR%\bin
if not exist %INSTALL_DIR% mkdir %INSTALL_DIR%
if not exist %INSTALL_DIR%\include mkdir %INSTALL_DIR%\include
if not exist %INSTALL_DIR%\lib mkdir %INSTALL_DIR%\lib

IF "%1"=="" (
  SET TO_BE_BUILD=expat raptor crossguid clapack SBW libSBML libnuml libSEDML zlib zipper libCombine MML qwt qwt-6 qwtplot3d
  GOTO :END_ARGUMENTS
)

IF "%1"=="--rebuild" (
  CALL "%BUILD_DIR%\.packages.bat"
  GOTO :END_ARGUMENTS
)

SET TO_BE_BUILD=%*

:END_ARGUMENTS

ECHO SET TO_BE_BUILD=%TO_BE_BUILD% > "%BUILD_DIR%\.packages.bat"

if "%BUILD_TYPE%"=="" SET BUILD_TYPE=Release
if "%BUILD_TOOL%"=="" SET BUILD_TOOL=nmake
if "%BUILD_GENERATOR%"=="" SET BUILD_GENERATOR=NMake Makefiles


SET BUILD_COMMAND=
SET INSTALL_COMMAND=install

SET CMAKE=cmake -G "%BUILD_GENERATOR%" %CMAKE_OVERRIDES% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR%

FOR %%A IN (%TO_BE_BUILD%) DO (
  ECHO Building:  %%A

  IF %%A==clapack (
    REM Build Clapack
    mkdir %BUILD_DIR%\clapack
    cd /d %BUILD_DIR%\clapack
    %CMAKE% -DBUILD_TESTING=OFF %BASE_DIR%\src\clapack
  )

  IF %%A==cppunit (
    REM Build CPP Unit
    mkdir %BUILD_DIR%\cppunit
    cd /d %BUILD_DIR%\cppunit
    %CMAKE% %BASE_DIR%\src\cppunit
  )

  IF %%A==expat (
    REM Build Expat
    mkdir %BUILD_DIR%\expat
    cd /d %BUILD_DIR%\expat
    %CMAKE% -DBUILD_shared=OFF -DBUILD_tests=OFF -DBUILD_tools=OFF -DBUILD_examples=OFF %BASE_DIR%\src\expat
  )

  IF %%A==raptor (
    REM Build Raptor
    mkdir %BUILD_DIR%\raptor
    cd /d %BUILD_DIR%\raptor
    %CMAKE% %BASE_DIR%\src\raptor
  )

  IF %%A==crossguid (
    REM Build crossguid
    mkdir %BUILD_DIR%\crossguid
    cd /d %BUILD_DIR%\crossguid
    %CMAKE% %BASE_DIR%\src\crossguid
  )

  IF %%A==SBW (
    REM Build SBW
    mkdir %BUILD_DIR%\SBW
    cd /d %BUILD_DIR%\SBW
    %CMAKE% -DWITH_BUILD_BROKER=OFF -DWITH_BUILD_SHARED=OFF %BASE_DIR%\src\core
  )

  IF %%A==libSBML (
    REM Build libSBML
    mkdir %BUILD_DIR%\libsbml
    cd /d %BUILD_DIR%\libsbml
    %CMAKE% -DENABLE_COMP=ON -DENABLE_LAYOUT=ON -DENABLE_REQUIREDELEMENTS=OFF -DLIBSBML_USE_LEGACY_MATH=ON -DENABLE_RENDER=ON -DENABLE_COMP=ON -DENABLE_FBC=OFF -DENABLE_SPATIAL=OFF -DENABLE_GROUPS=OFF -DWITH_EXPAT=ON -DWITH_LIBXML=OFF -DLIBSBML_DEPENDENCY_DIR=%INSTALL_DIR% -DLIBSBML_SKIP_SHARED_LIBRARY=ON -DWITH_BZIP2=OFF -DWITH_ZLIB=OFF -DENABLE_UNIVERSAL=ON %BASE_DIR%/src/libSBML
  )

  IF %%A==libnuml (
    REM build libnuml
    mkdir %BUILD_DIR%\libnuml
    cd /d %BUILD_DIR%\libnuml
    %CMAKE% -DLIBSBML_STATIC=ON -DLIBNUML_SHARED_VERSION=OFF -DLIBNUML_SKIP_SHARED_LIBRARY=ON -DLIBSBML_INCLUDE_DIR=%INSTALL_DIR%/include/sbml -DLIBSBML_LIBRARY=%INSTALL_DIR%/lib/libsbml-static.a -DLIBNUML_DEPENDENCY_DIR=%INSTALL_DIR% -DEXTRA_LIBS=%INSTALL_DIR%/lib/expat.lib -DWITH_ZLIB=OFF %BASE_DIR%/src/libnuml
  )

  IF %%A==libSEDML (
    REM build libSEDML
    mkdir %BUILD_DIR%\libSEDML
    cd /d %BUILD_DIR%\libSEDML
    %CMAKE% -DLIBSBML_STATIC=ON -DLIBSEDML_SHARED_VERSION=OFF  -DLIBSEDML_SKIP_SHARED_LIBRARY=ON -DLIBSEDML_DEPENDENCY_DIR=%INSTALL_DIR% -DEXTRA_LIBS=%INSTALL_DIR%/lib/expat.lib %BASE_DIR%/src/libSEDML
  )

  IF %%A==zlib (
    REM Build Zlib
    mkdir %BUILD_DIR%\zlib
    cd /d %BUILD_DIR%\zlib
    %CMAKE% -DBUILD_shared=OFF %BASE_DIR%\src\zlib
  )
  
  IF %%A==zipper (
    REM Build Zipper
    mkdir %BUILD_DIR%\zipper
    cd /d %BUILD_DIR%\zipper
    %CMAKE% -DLIBZ_LIBRARY=%INSTALL_DIR%\lib\zdll.lib -DLIBZ_INCLUDE_DIR=%INSTALL_DIR%\include -DBUILD_TEST=OFF -DBUILD_shared=OFF %BASE_DIR%\src\zipper
  )
  
  IF %%A==libCombine (
    REM Build libCombine
    mkdir %BUILD_DIR%\libCombine
    cd /d %BUILD_DIR%\libCombine
    %CMAKE% -DCOMBINE_DEPENDENCY_DIR=%INSTALL_DIR% -DBUILD_TEST=OFF -DLIBCOMBINE_SKIP_SHARED_LIBRARY=ON -DEXTRA_LIBS=%INSTALL_DIR%\lib\expat.lib  %BASE_DIR%/src/libCombine
  )
  
  IF %%A==MML (
    REM mml
    mkdir %BUILD_DIR%\mml
    cd /d %BUILD_DIR%\mml
    %CMAKE% %BASE_DIR%\src\mml
  )

  IF %%A==qwt (
    REM Build QWT
    ::mkdir %BUILD_DIR%\qwt
    ::cd /d %BUILD_DIR%\qwt
    ::%CMAKE% %BASE_DIR%\src\qwt
    mkdir %BUILD_DIR%\qwt-6
    cd /d %BUILD_DIR%\qwt-6
    %CMAKE% %BASE_DIR%\src\qwt-6
  )

  IF %%A==qwt-6 (
    REM Build QWT
    mkdir %BUILD_DIR%\qwt-6
    cd /d %BUILD_DIR%\qwt-6
    %CMAKE% %BASE_DIR%\src\qwt-6
  )

  IF %%A==qwtplot3d (
    REM Build QWT Plot
    mkdir %BUILD_DIR%\qwtplot3d
    cd /d %BUILD_DIR%\qwtplot3d
    %CMAKE% %BASE_DIR%\src\qwtplot3d-qt4
  )

  %BUILD_TOOL% %BUILD_COMMAND%
  %BUILD_TOOL% %INSTALL_COMMAND%

  cd /d %BASE_DIR%
)

