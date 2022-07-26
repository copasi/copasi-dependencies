####################################################################
#
# CMake Build Script for SBW library 
#
# Author: Frank T. Bergmann 

# used to create CMake config files for projects using this library
if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)
include(CMakePackageConfigHelpers)
endif()

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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../include)  
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../include/SBW)  
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/../include)

####################################################################
#
# Build shared library
#
option(WITH_BUILD_SHARED "Build the shared SBW library" ON)

if (WITH_BUILD_SHARED)

  if (WIN32)
    
    configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY)
    
    add_library (SBW SHARED ${SBW_SOURCES} ${SBW_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
    set_target_properties(SBW PROPERTIES 
      COMPILE_DEFINITIONS "SBW_FORCE_EXPORT=1;SBW_EXPORTS=1;SBW_STATIC=0")
    
  else()
  
    add_library (SBW SHARED ${SBW_SOURCES} ${SBW_HEADERS})
    set_target_properties(SBW PROPERTIES 
      COMPILE_DEFINITIONS "SBW_STATIC=0")
    
  endif()

  
  target_link_libraries(SBW ${SBW_LIBS})
  set_target_properties(SBW PROPERTIES VERSION "${SBW_VERSION_MAJOR}.${SBW_VERSION_MINOR}.${SBW_VERSION_PATCH}")
  set_target_properties(SBW PROPERTIES SOVERSION "${SBW_VERSION_MAJOR}.${SBW_VERSION_MINOR}")
  
  INSTALL(TARGETS SBW EXPORT SBW-config
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
  
  # and install the exported target configuration
  INSTALL(EXPORT SBW-config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
  )

  if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)
  WRITE_BASIC_PACKAGE_VERSION_FILE(
    ${CMAKE_CURRENT_BINARY_DIR}/SBW-config-version.cmake 
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
  )
    
  INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/SBW-config-version.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
  )
  endif()


endif(WITH_BUILD_SHARED)

####################################################################
#
# Build static library
#
option(WITH_BUILD_STATIC "Build the static SBW library" ON)

if (WITH_BUILD_STATIC)

  add_library (SBW-static STATIC ${SBW_SOURCES} ${SBW_HEADERS})
  SET_TARGET_PROPERTIES(SBW-static PROPERTIES COMPILE_DEFINITIONS "SBW_STATIC=1")
  
  target_link_libraries(SBW-static ${SBW_LIBS})
  target_include_directories(SBW-static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include> 
  )
  
  INSTALL(TARGETS SBW-static EXPORT SBW-static-config
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
  
  # and install the exported target configuration
  INSTALL(EXPORT SBW-static-config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
  )

  if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)
  WRITE_BASIC_PACKAGE_VERSION_FILE(
    ${CMAKE_CURRENT_BINARY_DIR}/SBW-static-config-version.cmake 
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
  )
    
  INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/SBW-static-config-version.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
  )
  endif()

endif (WITH_BUILD_STATIC)
