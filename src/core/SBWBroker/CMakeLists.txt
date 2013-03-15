####################################################################
# CMake Build Script for SBW Broker
#
# Author: Frank T. Bergmann
#

if (WITH_BUILD_BROKER)

# Find all sources 
file(GLOB SBW_SOURCES  *.cpp )

# setup includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)	
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/SBW)	

# Build library
set (EXECUTABLE_FLAGS)
if (MSVC)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
set (EXECUTABLE_FLAGS WIN32)
endif()
add_executable (Broker ${EXECUTABLE_FLAGS} ${SBW_SOURCES} )
target_link_libraries(Broker ${SBW_LIBS} SBW-static)

INSTALL(TARGETS Broker
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )

endif(WITH_BUILD_BROKER)
  