####################################################################
#
# CMake Build Script for the SBW Core and Broker
#
# Author: Frank T. Bergmann
#

cmake_minimum_required(VERSION 2.8)
project(SBW)

include (CMakeTestCCompiler)
include (CheckCSourceCompiles)
include (CheckCXXSourceCompiles)
include (CheckStructHasMember)
include (CheckLibraryExists)
include (CheckFunctionExists)
include (CheckCCompilerFlag)
include (CheckCSourceRuns)
include (CheckSymbolExists)
include (CheckTypeSize)


####################################################################
#
# Set up version information, it might be better to just write it here
# rather than parsing VERSION.txt
#
SET(SBW_VERSION_MAJOR 2)
SET(SBW_VERSION_MINOR 10)
SET(SBW_VERSION_PATCH 0)
SET(SBW_VERSION_RELEASE "")

SET(SBW_VERSION_NUMERIC 20100)
SET(PACKAGE_VERSION "${SBW_VERSION_MAJOR}.${SBW_VERSION_MINOR}.${SBW_VERSION_PATCH}${SBW_VERSION_RELEASE}")


####################################################################
#
# the next lines configure the parameters for packaging the binaries
# they can be invoked with: make package / nmake package or by using
# cpack -G zip|deb|rpm|dmg|nsis
#

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The SBW Core, including C/C++ bindings and Broker")
SET(CPACK_PACKAGE_NAME "SBWCore")
SET(CPACK_PACKAGE_VENDOR "The SBW Team")
SET(CPACK_PACKAGE_CONTACT "SBW Team <sbwteam@gmail.com>")
SET(CPACK_PACKAGE_VERSION_MAJOR "${SBW_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${SBW_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${SBW_VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME  "sbw-core-${SBW_VERSION_MAJOR}.${SBW_VERSION_MINOR}.${SBW_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES ".svn/;/build/;.*~")
include(CPack)

# set build type default
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel")



####################################################################
#
# Locate Libxml2 if needed
#
find_library(LIBXML_LIBRARY
	NAMES libxml2.lib xml2
	PATHS /usr/lib /usr/local/lib
		  ${CMAKE_SOURCE_DIR}
		  ${CMAKE_SOURCE_DIR}/lib
)

find_path(LIBXML_INCLUDE_DIR
    NAMES libxml/parser.h
    PATHS /usr/include /usr/local/include
          /usr/include/libxml2
          /usr/local/include/libxml2
          ${CMAKE_SOURCE_DIR}/include

)

set(SBW_XML_LIBRARY "libxml2")
set(SBW_XML_LIBRARY_INCLUDE ${LIBXML_INCLUDE_DIR})
set(SBW_XML_LIBRARY_LIBS ${LIBXML_LIBRARY})

set(SBW_LIBS ${SBW_LIBS} ${LIBXML_LIBRARY})
include_directories(${LIBXML_INCLUDE_DIR})

#
# Allow to build core library only by disabling the build of the broker
# 
option(WITH_BUILD_BROKER "Build the SBW Broker" ON)

####################################################################
#
# Need some variables set up, such as the name for the libSBML
# library and the Path and file separator characters
#

if(UNIX)
	set(PATH_SEP "/")
	set(FILE_SEP ":")
	set(SBW_LIBRARY SBW)
else()
	set(PATH_SEP "\\")
	set(FILE_SEP ";")
	if (MINGW)
		set(SBW_LIBRARY SBW)
	else()
		set(SBW_LIBRARY libSBW)
	endif()
endif()

####################################################################
#
# Set up remaining variables, add option for universal binaries
#
set(BUILD_DEFINITIONS)
if(UNIX)
    add_definitions(-DLINUX)
    set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DLINUX")
    if(APPLE)
      add_definitions(-DDARWIN)
      set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DDARWIN")
      # On OSX it is common to build universal binaries to support multiple
        # processor architectures. The default behavior is not to build
        # multiple architectures, as most users might not need that.
        option(ENABLE_UNIVERSAL "Create universal binaries on Mac OS X." OFF)

        set(CMAKE_OSX_ARCHITECTURES "${CMAKE_OSX_ARCHITECTURES}" CACHE STRING
           "A semicolon-separated list of build architectures to be used.")
        if(ENABLE_UNIVERSAL)
            # if universal binaries are requested and none defined so far
            # overwrite them with all three common architectures. If the user
            # specified their own list of architectures do not touch!
            if(CMAKE_OSX_ARCHITECTURES STREQUAL "")
                STRING(REGEX REPLACE "^.*MacOSX([0-9]*\\.[0-9]*)\\.sdk$" "\\1"
                       OSX_SDK_VERSION "${CMAKE_OSX_SYSROOT}")
                if(OSX_SDK_VERSION  VERSION_EQUAL "10.7" OR OSX_SDK_VERSION  VERSION_EQUAL "10.8")                   # OSX Lion no longer supports ppc architecture
                   set(CMAKE_OSX_ARCHITECTURES "i386;x86_64" CACHE STRING
                      "A semicolon-separated list of build architectures to be used." FORCE)
                else()
                   set(CMAKE_OSX_ARCHITECTURES "i386;ppc;x86_64" CACHE STRING
                       "A semicolon-separated list of build architectures to be used." FORCE)
                endif()
            endif()
        endif(ENABLE_UNIVERSAL)
    else()
      set(SBW_LIBS ${SBW_LIBS} 'pthread')
    endif(APPLE)
    add_definitions( -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\"  -DPACKAGE_NAME=\"${PROJECT_NAME}\")
    set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\"  -DPACKAGE_NAME=\"${PROJECT_NAME}\"")

else(UNIX)

    set(USING_INTEL)
    if (WIN32 AND CMAKE_C_COMPILER AND ${CMAKE_C_COMPILER} MATCHES ".*icl.*$")
      message(STATUS "Detected Intel Compiler")
      set(USING_INTEL TRUE)
    endif ()

	set(SBW_LIBS ${SBW_LIBS} ws2_32.lib)
	add_definitions(-DWIN32 -DSBW_EXPORTS -DLIBLAX_EXPORTS)
	set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DWIN32 -DSBW_EXPORTS -DLIBLAX_EXPORTS")
	if(MSVC OR USING_INTEL)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
		set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -D_CRT_SECURE_NO_WARNINGS")
		option(WITH_STATIC_RUNTIME "Compile using the static MSVC Runtime" OFF)
		if (WITH_STATIC_RUNTIME)
			foreach(flag_var
				CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
				CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

				if(${flag_var} MATCHES "/MD")
					string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
				endif(${flag_var} MATCHES "/MD")
			endforeach(flag_var)
		add_definitions( -D_MT)
		endif(WITH_STATIC_RUNTIME)

		file(GLOB WIN32_BINARIES ${CMAKE_SOURCE_DIR}/dependencies/bin/*.dll)
		INSTALL(FILES ${WIN32_BINARIES} DESTINATION bin)

	elseif(CYGWIN)
		add_definitions(-DCYGWIN)
		set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DCYGWIN")
	elseif(MINGW)
		if(WITH_LIBXML)
			# this is necessary to build with libxml2 on mingw
			add_definitions(-DLIBXML_STATIC)
		endif(WITH_LIBXML)
	endif(MSVC OR USING_INTEL)

endif(UNIX)


####################################################################
#
# Build the actual SBW library
#
add_subdirectory(SBWCore)
add_subdirectory(SBWBroker)

file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/SBW/*.h )
install(FILES ${HEADER_FILES} DESTINATION include/SBW)
