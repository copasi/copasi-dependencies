cmake_minimum_required (VERSION 3.11)
project (copasi-dependencies VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ChooseQtVersion.cmake)
QT_FIND_MODULES(Core Gui)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "..." FORCE)
endif()

set (COMMON_CMAKE_OPTIONS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})

option(BUILD_expat "Build the expat library" ON)

option(BUILD_raptor "Build the raptor library" ON)
if (BUILD_raptor)
  set (BUILD_expat ON)
endif (BUILD_raptor)

option(BUILD_crossguid "Build the crossguid library" ON)

option(BUILD_clapack "Build the clapack library" ON)

option(BUILD_SBW "Build the SBW library" ON)

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
endif (BUILD_libCombine)

option(BUILD_mml "Build the mml library" ON)

option(BUILD_qwt6 "Build the qwt-6 library" OFF)

option(BUILD_qwt "Build the qwt library" ON)
if (BUILD_qwt AND ${SELECT_QT} MATCHES "Qt5")
  set (BUILD_qwt OFF)
  set (BUILD_qwt6 ON)
endif (BUILD_qwt AND ${SELECT_QT} MATCHES "Qt5")

option(BUILD_qwtplot3d "Build the qwtplot3d library" ON)

add_subdirectory(src)
