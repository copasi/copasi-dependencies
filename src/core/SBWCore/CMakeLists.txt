####################################################################
#
# CMake Build Script for SBW library 
#
# Author: Frank T. Bergmann 


####################################################################
#
# Find all sources 
# 
file(GLOB SBW_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
  *.cpp 
  )

file(GLOB SBW_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/SBW/*.h 
  )

# remove stdafx file to remove warning
if (NOT WIN32)
  list(REMOVE_ITEM SBW_SOURCES "stdafx.cpp" )
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)  
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/SBW)  

####################################################################
#
# Build shared library
#
option(WITH_BUILD_SHARED "Build the shared SBW library" ON)

if (WITH_BUILD_SHARED)

  add_library (SBW SHARED ${SBW_SOURCES} ${SBW_HEADERS})
  target_link_libraries(SBW ${SBW_LIBS})
  set_target_properties(SBW PROPERTIES VERSION "${SBW_VERSION_MAJOR}.${SBW_VERSION_MINOR}.${SBW_VERSION_PATCH}")
  set_target_properties(SBW PROPERTIES SOVERSION "${SBW_VERSION_MAJOR}.${SBW_VERSION_MINOR}")
  
  INSTALL(TARGETS SBW
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

endif(WITH_BUILD_SHARED)

####################################################################
#
# Build static library
#
option(WITH_BUILD_STATIC "Build the static SBW library" ON)

if (WITH_BUILD_STATIC)

  add_library (SBW-static STATIC ${SBW_SOURCES} ${SBW_HEADERS})
  if (WIN32 AND NOT CYGWIN)
  
    # don't decorate static library 
    add_definitions(-DSBW_STATIC)
    SET_TARGET_PROPERTIES(SBW-static PROPERTIES COMPILE_DEFINITIONS "SBW_STATIC=1")
    
  endif(WIN32 AND NOT CYGWIN)
  
  target_link_libraries(SBW-static ${SBW_LIBS})
  
  INSTALL(TARGETS SBW-static 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

endif (WITH_BUILD_STATIC)
