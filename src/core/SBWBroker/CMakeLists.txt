####################################################################
# CMake Build Script for SBW Broker
#
# Author: Frank T. Bergmann
#

if (WITH_BUILD_BROKER)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Broker.rc.cmake ${CMAKE_CURRENT_SOURCE_DIR}/Broker.rc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BrokerDefines.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/BrokerDefines.h)

# Find all sources 
file(GLOB SBW_SOURCES  *.cpp *.h)

# since the broker links against xml2, we need the 
# iconv header as well if statically linked

find_path(ICONV_INCLUDE_DIR
    NAMES iconv.h
    PATHS /usr/include 
          /usr/local/include
          /usr/include/libxml2
          /usr/local/include/libxml2
          ${CMAKE_SOURCE_DIR}/include
)

find_library(ICONV_LIBRARIES
    NAMES iconv libiconv
    PATHS /usr/lib 
          /usr/local/lib
          ${CMAKE_SOURCE_DIR}/lib
)


# setup includes
if (ICONV_INCLUDE_DIR)
  include_directories(${ICONV_INCLUDE_DIR})	
endif(ICONV_INCLUDE_DIR)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)	
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/SBW)	


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
target_link_libraries(${BROKER_NAME} ${SBW_LIBS} SBW-static)

INSTALL(TARGETS ${BROKER_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

endif(WITH_BUILD_BROKER)
