####################################################################
#
# CMake Build Script for SBW Broker
#
# Author: Frank T. Bergmann
# $Id$
# $HeadURL$


####################################################################
#
# Find all sources 
#
	
file(GLOB SBW_SOURCES 
	*.cpp 	
	)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)	
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/SBW)	

####################################################################
#
# Build library
#
if (MSVC)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()
add_executable (Broker WIN32 ${SBW_SOURCES} )
target_link_libraries(Broker ${SBW_LIBS} SBW-static)

INSTALL(TARGETS Broker
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )
