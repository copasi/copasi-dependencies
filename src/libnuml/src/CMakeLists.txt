###############################################################################
#
# Description       : CCMake build script for libNUML
# Original author(s): Joseph O. Dada <joseph.dada@manchester.ac.uk>
# Organization      : University of Manchester
#
# This file is part of libNUML.  Please visit http://code.google.com/p/numl/ for more
# information about NUML, and the latest version of libNUML.
#
# Copyright (C) 2013: 
#  The University of Manchester, UK
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation.  A copy of the license agreement is provided
# in the file named "LICENSE.txt" included with this software distribution
#
###############################################################################

# used to create CMake config files for projects using this library
if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)
include(CMakePackageConfigHelpers)
endif()

include(CheckLibraryExists)
check_library_exists(m sqrt "" HAVE_LIBM)

# generate configuration for this system
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/numl/common/libnuml-config-common.h.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/numl/common/libnuml-config-common.h"
  )

# Replace: @LIBNUML_VERSION@ and @LIBNUML_VERSION_NUMERIC@
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/numl/common/libnuml-version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/numl/common/libnuml-version.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/numl/common/libnuml-package.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/numl/common/libnuml-package.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/numl/common/libnuml-namespace.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/numl/common/libnuml-namespace.h"
  )
  

###############################################################################
#
# mark header files for installation
#
foreach(dir common util xml  )

  file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/numl/${dir}/*.h")
  install(FILES ${header_files} DESTINATION include/numl/${dir})

endforeach(dir)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/numl/*.h")
install(FILES ${files} DESTINATION include/numl)


###############################################################################
#
# specify include directories
#

# install generated headers
install(FILES 
       "${CMAKE_CURRENT_BINARY_DIR}/numl/common/libnuml-version.h"
       "${CMAKE_CURRENT_BINARY_DIR}/numl/common/libnuml-config-common.h"
       "${CMAKE_CURRENT_BINARY_DIR}/numl/common/libnuml-package.h"
       "${CMAKE_CURRENT_BINARY_DIR}/numl/common/libnuml-namespace.h"
        DESTINATION include/numl/common)

###############################################################################
#
# specify include directories
#
include_directories(BEFORE ${CMAKE_BINARY_DIR})
include_directories(BEFORE ${CMAKE_BINARY_DIR}/src)
include_directories(BEFORE ${CMAKE_BINARY_DIR}/src/numl)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/src/numl)
include_directories(${CMAKE_SOURCE_DIR}/src)

if (EXTRA_INCLUDE_DIRS)
  include_directories(${EXTRA_INCLUDE_DIRS})
endif(EXTRA_INCLUDE_DIRS)



if (EXTRA_DEFS)
add_definitions(${EXTRA_DEFS})
endif()

###############################################################################
#
# Find all sources
#

foreach (directory common numl )

  set(prefix "numl/")

  if ("${directory}" STREQUAL "numl")
    set(prefix "")
  endif()


  file(GLOB temp
      ${prefix}${directory}/*.cpp
      ${prefix}${directory}/*.c
      ${prefix}${directory}/*.h)  
  
  # create source group for IDEs
  source_group(${directory} FILES ${temp})

  # add to libnuml sources
  set(LIBNUML_SOURCES ${LIBNUML_SOURCES} ${temp})

endforeach()

###############################################################################
#
# this is a directory level operation!
#
if (MSVC)
if (WITH_STATIC_RUNTIME)
  foreach(flag_var
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)

    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")


  endforeach(flag_var)
  add_definitions( -D_MT)
endif(WITH_STATIC_RUNTIME)
endif(MSVC)

###############################################################################
#
# Build library
#

SET (INCLUDE_DESTINATION)
if (${CMAKE_VERSION} VERSION_GREATER "2.8.11")
  set(INCLUDE_DESTINATION INCLUDES DESTINATION include)
endif()


if (NOT LIBNUML_SKIP_SHARED_LIBRARY)

add_library (${LIBNUML_LIBRARY} SHARED ${LIBNUML_SOURCES} )

if (LIBNUML_SHARED_VERSION)
  set_target_properties(${LIBNUML_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "LIBNUML_EXPORTS=1")
  set_target_properties(${LIBNUML_LIBRARY} PROPERTIES
                      SOVERSION ${LIBNUML_VERSION_MAJOR}
                      VERSION ${LIBNUML_VERSION_MAJOR}.${LIBNUML_VERSION_MINOR}.${LIBNUML_VERSION_PATCH})
endif()

target_link_libraries(${LIBNUML_LIBRARY} ${LIBSBML_LIBRARY_NAME} ${EXTRA_LIBS})

INSTALL(TARGETS ${LIBNUML_LIBRARY}
  EXPORT ${LIBNUML_LIBRARY}-config
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ${INCLUDE_DESTINATION}
)

# and install the exported target configuration
install(EXPORT ${LIBNUML_LIBRARY}-config DESTINATION ${PACKAGE_CONFIG_DIR})

if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)

  WRITE_BASIC_PACKAGE_VERSION_FILE(
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBNUML_LIBRARY}-config-version.cmake 
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
  )
  
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${LIBNUML_LIBRARY}-config-version.cmake
    DESTINATION ${PACKAGE_CONFIG_DIR}
  )

endif()

endif()

if (NOT LIBNUML_SKIP_STATIC_LIBRARY)
add_library (${LIBNUML_LIBRARY}-static STATIC ${LIBNUML_SOURCES} )
if (WIN32 AND NOT CYGWIN)
  # don't decorate static library 
  set_target_properties(${LIBNUML_LIBRARY}-static PROPERTIES COMPILE_DEFINITIONS "LIBNUML_STATIC=1")
endif(WIN32 AND NOT CYGWIN)

target_link_libraries(${LIBNUML_LIBRARY}-static ${LIBSBML_LIBRARY_NAME} ${EXTRA_LIBS})

INSTALL(TARGETS ${LIBNUML_LIBRARY}-static
  EXPORT ${LIBNUML_LIBRARY}-static-config
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ${INCLUDE_DESTINATION}
)

# and install the exported target configuration
install(EXPORT ${LIBNUML_LIBRARY}-static-config DESTINATION ${PACKAGE_CONFIG_DIR})

if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)

  WRITE_BASIC_PACKAGE_VERSION_FILE(
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBNUML_LIBRARY}-static-config-version.cmake 
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
  )
  
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${LIBNUML_LIBRARY}-static-config-version.cmake
    DESTINATION ${PACKAGE_CONFIG_DIR}
  )

endif()
  
endif (NOT LIBNUML_SKIP_STATIC_LIBRARY)
  
##############################################################################
#
# create language bindings
# 
add_subdirectory(bindings)
