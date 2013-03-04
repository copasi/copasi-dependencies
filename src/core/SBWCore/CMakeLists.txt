####################################################################
#
# CMake Build Script for SBW library 
#
# Author: Frank T. Bergmann
# $Id$
# $HeadURL$


####################################################################
#
# Find all sources 
#
	
file(GLOB SBW_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	*.cpp 	
	)

# remove stdafx file to remove warning
if (NOT WIN32)
	list(REMOVE_ITEM SBW_SOURCES "stdafx.cpp" )
endif()
	
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)	
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/SBW)	

####################################################################
#
# Build library
#
add_library (SBW SHARED ${SBW_SOURCES})
target_link_libraries(SBW ${SBW_LIBS})
set_target_properties(SBW PROPERTIES VERSION "${SBW_VERSION_MAJOR}.${SBW_VERSION_MINOR}.${SBW_VERSION_PATCH}")
set_target_properties(SBW PROPERTIES SOVERSION "${SBW_VERSION_MAJOR}.${SBW_VERSION_MINOR}")

INSTALL(TARGETS SBW
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )

add_library (SBW-static STATIC ${SBW_SOURCES} )
target_link_libraries(SBW-static ${SBW_LIBS})

INSTALL(TARGETS SBW-static 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )

