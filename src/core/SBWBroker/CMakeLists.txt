####################################################################
# CMake Build Script for SBW Broker
#
# Author: Frank T. Bergmann
#

if (WITH_BUILD_BROKER)


####################################################################
#
# Locate Libxml2 if needed
# 
if (NOT LIBXML_LIBRARY)
find_library(LIBXML_LIBRARY
  NAMES libxml2.lib xml2
  PATHS /usr/lib 
        /usr/local/lib
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/lib
)
endif()
if (NOT LIBXML_LIBRARY)

  message(FATAL_ERROR 
"SBW depends on the libxml2, please specify the LIBXML_LIBRARY variable.")

endif()

if (NOT LIBXML_INCLUDE_DIR)
find_path(LIBXML_INCLUDE_DIR
    NAMES libxml/parser.h
    PATHS /usr/include /usr/local/include
          /usr/include/libxml2
          /usr/local/include/libxml2
          ${CMAKE_SOURCE_DIR}/include
)
endif()

if (NOT LIBXML_INCLUDE_DIR)
message( FATAL_ERROR "Could not find the libxml2 include directory, please specify the 
LIBXML_INCLUDE_DIR variable with full path to libxml/parser.h")
endif()


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Broker.rc.cmake ${CMAKE_CURRENT_SOURCE_DIR}/Broker.rc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BrokerDefines.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/BrokerDefines.h)

# Find all sources 
file(GLOB SBW_SOURCES  *.cpp *.h)

# since the broker links against xml2, we need the 
# iconv header as well if statically linked

if (NOT ICONV_INCLUDE_DIR)
find_path(ICONV_INCLUDE_DIR
    NAMES iconv.h
    PATHS /usr/include 
          /usr/local/include
          /usr/include/libxml2
          /usr/local/include/libxml2
          ${CMAKE_SOURCE_DIR}/include
)
endif()

if (NOT ICONV_LIBRARIES)
find_library(ICONV_LIBRARIES
    NAMES iconv libiconv
    PATHS /usr/lib 
          /usr/local/lib
          ${CMAKE_SOURCE_DIR}/lib
)
endif()

if (EXTRA_DEFS)
add_definitions(${EXTRA_DEFS})
endif()

# setup includes
if (ICONV_INCLUDE_DIR)
  include_directories(${ICONV_INCLUDE_DIR})
endif(ICONV_INCLUDE_DIR)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../include/SBW)
include_directories(${LIBXML_INCLUDE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/../include)

set(BROKER_NAME Broker)


# Build Broker
set (EXECUTABLE_FLAGS)
if (MSVC OR USING_INTEL)
  set (BROKER_NAME C++Broker)
  set (SBW_SOURCES ${SBW_SOURCES} Broker.rc)
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
  set (EXECUTABLE_FLAGS WIN32)
endif(MSVC OR USING_INTEL)

add_executable (${BROKER_NAME} ${EXECUTABLE_FLAGS} ${SBW_SOURCES} )
target_link_libraries(${BROKER_NAME} SBW-static)

if (LIBXML_LIBRARY)
target_link_libraries(${BROKER_NAME} ${LIBXML_LIBRARY})
endif()

if (EXTRA_LIBS)
target_link_libraries(${BROKER_NAME} ${EXTRA_LIBS})
endif()

if (ICONV_LIBRARIES)
target_link_libraries(${BROKER_NAME} ${ICONV_LIBRARIES})
endif()

INSTALL(TARGETS ${BROKER_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

endif(WITH_BUILD_BROKER)

option(BUILD_TRIG_EXAMPLE "Builds an example Trig service" OFF)
if(BUILD_TRIG_EXAMPLE)
add_executable (trig_server example/TrigPlusServerModule.cpp)
target_link_libraries(trig_server SBW-static)
endif()

