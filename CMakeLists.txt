cmake_minimum_required (VERSION 3.10)
project (copasi-dependencies VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

option(BUILD_UI_DEPS "Build the GUI dependencies" ON)

if (BUILD_UI_DEPS)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ChooseQtVersion.cmake)
QT_FIND_MODULES(Core Gui)
endif()

# Default CMAKE_INSTALL_PREFIX
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (WIN32)
    set (CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR} CACHE PATH "..." FORCE)
  else ()
    set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "..." FORCE)
  endif (WIN32)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Default CMAKE_BUILD_TYPE
if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt AND
    NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Default BUILD_OPTIONS
if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt AND
    NOT BUILD_OPTIONS)
  if (CMAKE_GENERATOR MATCHES "Unix Makefiles" OR
      CMAKE_GENERATOR MATCHES "NMake Makefiles JOM" OR
      CMAKE_GENERATOR MATCHES "Ninja")
    set (BUILD_OPTIONS -j4)
  endif()
endif()

set (COMMON_CMAKE_OPTIONS ${COMMON_CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX} -G ${CMAKE_GENERATOR})

if (APPLE)
set (DEFAULT_OSX_TARGET 10.9)
if (CMAKE_OSX_DEPLOYMENT_TARGET)
set (DEFAULT_OSX_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET})
endif()
set (COMMON_CMAKE_OPTIONS ${COMMON_CMAKE_OPTIONS} -DCMAKE_OSX_DEPLOYMENT_TARGET=${DEFAULT_OSX_TARGET})
endif()

if (BUILD_UI_DEPS)
set (COMMON_CMAKE_OPTIONS_UI -DSELECT_QT=${SELECT_QT})

if (Qt5_DIR AND EXISTS ${Qt5_DIR})
  set (COMMON_CMAKE_OPTIONS_UI ${COMMON_CMAKE_OPTIONS_UI} -DQt5_DIR=${Qt5_DIR})
endif()
endif()

if (CMAKE_GENERATOR MATCHES "Visual Studio*")
  message(FATAL_ERROR "

    The Visual Studio generator is not currently supported, please choose either 
    'NMake Makefiles', or 'NMake Makefiles JOM' as generator to compile using MSVC

  ")
endif()

set (BUILD_OPTIONS)

if (WIN32)
  option(WITH_STATIC_RUNTIME "Compile using the static MSVC Runtime." OFF)
  if (WITH_STATIC_RUNTIME)
    set (COMMON_CMAKE_OPTIONS ${COMMON_CMAKE_OPTIONS} -DWITH_STATIC_RUNTIME=${WITH_STATIC_RUNTIME})
  endif (WITH_STATIC_RUNTIME)
endif (WIN32)

message(STATUS "
--
--   COPASI Dependencies
--   ===================
--
--   cmake generator: ${CMAKE_GENERATOR}
--   common options:")

foreach(entry ${COMMON_CMAKE_OPTIONS})
message(STATUS "    ${entry}")
endforeach()
message(STATUS "
--")

option(BUILD_expat "Build the expat library" ON)

option(BUILD_raptor "Build the raptor library" ON)
if (BUILD_raptor)
  set (BUILD_expat ON)
endif (BUILD_raptor)

option(BUILD_crossguid "Build the crossguid library" ON)

option(BUILD_clapack "Build the clapack library" ON)

option(BUILD_SBW "Build the SBW library" ${BUILD_UI_DEPS})

option(BUILD_libSBML "Build the libSBML library" ON)
if (BUILD_libSBML)
  set (BUILD_expat ON)
endif (BUILD_libSBML)

option(BUILD_libnuml "Build the libnuml library" ON)
if (BUILD_libnuml)
  set (BUILD_expat ON)
  set (BUILD_libSBML ON)
endif (BUILD_libnuml)

option(BUILD_libSEDML "Build the libSEDML library" ON)
if (BUILD_libSEDML)
  set (BUILD_expat ON)
  set (BUILD_libSBML ON)
  set (BUILD_libnuml ON)
endif (BUILD_libSEDML)

option(BUILD_zlib "Build the zlib library" ON)

option(BUILD_zipper "Build the zipper library" ON)
if (BUILD_zipper)
  set (BUILD_zlib ON)
endif (BUILD_zipper)

option(BUILD_libCombine "Build the libCombine library" ON)
if (BUILD_libCombine)
  set (BUILD_expat ON)
  set (BUILD_zipper ON)
  set (BUILD_libSBML ON)
endif (BUILD_libCombine)

if (BUILD_UI_DEPS )

option(BUILD_mml "Build the mml library" ${BUILD_UI_DEPS})

option(BUILD_qwt6 "Build the qwt-6 library" OFF)

option(BUILD_qwt "Build the qwt library" ${BUILD_UI_DEPS})

if (BUILD_qwt AND ${SELECT_QT} MATCHES "Qt5")
  set (BUILD_qwt OFF)
  set (BUILD_qwt6 ON)
endif (BUILD_qwt AND ${SELECT_QT} MATCHES "Qt5")

option(BUILD_qwtplot3d "Build the qwtplot3d library" ${BUILD_UI_DEPS})

option(BUILD_archive "Build a binary archive of all dependencies" ON)

endif()

add_subdirectory(src)
