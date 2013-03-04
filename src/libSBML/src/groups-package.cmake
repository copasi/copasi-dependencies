###############################################################################
#
# $URL: https://sbml.svn.sourceforge.net/svnroot/sbml/branches/libsbml-5/src/groups-package.cmake $
# $Id: groups-package.cmake 13541 2011-04-08 22:16:45Z fbergmann $
#
# Description       : CMake include file for SBML Level 3 Groups package
# Original author(s): Frank Bergmann <fbergman@caltech.edu>
# Organization      : California Institute of Technology
#
# This file is part of libSBML.  Please visit http://sbml.org for more
# information about SBML, and the latest version of libSBML.
#
# Copyright (C) 2009-2011 jointly by the following organizations: 
#     1. California Institute of Technology, Pasadena, CA, USA
#     2. EMBL European Bioinformatics Institute (EBML-EBI), Hinxton, UK
#  
# Copyright (C) 2006-2008 by the California Institute of Technology,
#     Pasadena, CA, USA 
#  
# Copyright (C) 2002-2005 jointly by the following organizations: 
#     1. California Institute of Technology, Pasadena, CA, USA
#     2. Japan Science and Technology Agency, Japan
# 
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation.  A copy of the license agreement is provided
# in the file named "LICENSE.txt" included with this software distribution
# and also available online as http://sbml.org/software/libsbml/license.html
#
###############################################################################

if(ENABLE_GROUPS)

include(${CMAKE_SOURCE_DIR}/groups-package.cmake)

#build up sources
set(GROUPS_SOURCES)

# go through all directtories: common, extension and sbml
foreach(dir common extension sbml)

	# add to include directory
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/groups/${dir})
	
	# file sources
	file(GLOB current ${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/groups/${dir}/*.cpp
	                  ${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/groups/${dir}/*.c
	                  ${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/groups/${dir}/*.h)
	
	# add sources 
	set(GROUPS_SOURCES ${GROUPS_SOURCES} ${current})
	
    # mark header files for installation 
	file(GLOB groups_headers
	                  ${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/groups/${dir}/*.h)
	
	install(FILES       ${groups_headers} 
		    DESTINATION include/sbml/packages/groups/${dir} )
	
endforeach()

# create source group for IDEs
source_group(groups_package FILES ${GROUPS_SOURCES})

# add groups sources to SBML sources
SET(LIBSBML_SOURCES ${LIBSBML_SOURCES} ${GROUPS_SOURCES})

####################################################################
#
# add test scripts
#
if(WITH_CHECK)

	add_subdirectory(sbml/packages/groups/extension/test)

endif()

endif()
