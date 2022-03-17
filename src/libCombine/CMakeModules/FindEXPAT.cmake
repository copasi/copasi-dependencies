# Copyright (C) 2019 - 2020 by Pedro Mendes, Rector and Visitors of the 
# University of Virginia, University of Heidelberg, and University 
# of Connecticut School of Medicine. 
# All rights reserved. 

# Copyright (C) 2017 - 2018 by Pedro Mendes, Virginia Tech Intellectual 
# Properties, Inc., University of Heidelberg, and University of 
# of Connecticut School of Medicine. 
# All rights reserved. 

# Copyright (C) 2013 - 2016 by Pedro Mendes, Virginia Tech Intellectual 
# Properties, Inc., University of Heidelberg, and The University 
# of Manchester. 
# All rights reserved. 

# - Try to find the Expat XML parsing library 

# Once done this will define
#
#  EXPAT_FOUND       - system has Expat
#  EXPAT_LIBRARIES   - Link these to use Expat
#  EXPAT_INCLUDE_DIR - Include directory for using Expat
#  EXPAT_DEFINITIONS - Compiler switches required for using Expat
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


MACRO (FIND_EXPAT)

ENDMACRO ()



# Check if we have cached results in case the last round was successful.
if (NOT (EXPAT_INCLUDE_DIR AND EXPAT_LIBRARIES) OR NOT EXPAT_FOUND)

    set(EXPAT_LDFLAGS)
	
    find_path(EXPAT_INCLUDE_DIR expat.h
	    PATHS $ENV{EXPAT_DIR}/include
	          $ENV{EXPAT_DIR}
	          ~/Library/Frameworks
	          /Library/Frameworks
	          /sw/include        # Fink
	          /opt/local/include # MacPorts
	          /opt/csw/include   # Blastwave
	          /opt/include
	          /usr/freeware/include
             NO_DEFAULT_PATH)

    if (NOT EXPAT_INCLUDE_DIR)
        find_path(EXPAT_INCLUDE_DIR expat.h)
    endif ()

    find_library(EXPAT_LIBRARY 
	    NAMES expat
	    PATHS $ENV{EXPAT_DIR}/lib
	          $ENV{EXPAT_DIR}/lib-dbg
	          $ENV{EXPAT_DIR}
              ${COPASI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}
              ${COPASI_DEPENDENCY_DIR}
	          ~/Library/Frameworks
	          /Library/Frameworks
	          /sw/lib        # Fink
	          /opt/local/lib # MacPorts
	          /opt/csw/lib   # Blastwave
	          /opt/lib
	          /usr/freeware/lib64
             NO_DEFAULT_PATH)

    if (NOT EXPAT_LIBRARY)
        find_library(EXPAT_LIBRARY NAMES expat)
    endif ()

    if (NOT WIN32)
        find_package(PkgConfig)
        pkg_check_modules(PC_EXPAT QUIET expat)

        message(STATUS "${PC_EXPAT_STATIC_LDFLAGS}")

        if (PC_EXPAT_FOUND)
            set(EXPAT_DEFINITIONS ${PC_EXPAT_CFLAGS_OTHER})
            set(EXPAT_VERSION ${PC_EXPAT_VERSION} CACHE STRING "Expat Version found" )
        endif (PC_EXPAT_FOUND)
    endif (NOT WIN32)
    
    mark_as_advanced(EXPAT_INCLUDE_DIR EXPAT_LIBRARY)

endif () # Check for cached values

# create an expat target to link against
if(NOT TARGET EXPAT::EXPAT)
  add_library(EXPAT::EXPAT UNKNOWN IMPORTED)
  set_target_properties(EXPAT::EXPAT PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${EXPAT_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${EXPAT_INCLUDE_DIR}")
endif()


include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    EXPAT
    VERSION_VAR   EXPAT_VERSION
    REQUIRED_VARS EXPAT_LIBRARY EXPAT_INCLUDE_DIR)

mark_as_advanced(EXPAT_VERSION)
