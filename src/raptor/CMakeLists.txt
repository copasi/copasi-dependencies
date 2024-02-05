cmake_minimum_required (VERSION 2.8...3.25)
project (raptor)

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
include (CheckIncludeFiles)

# are we using the intel compiler
set(USING_INTEL)
if (WIN32 AND CMAKE_C_COMPILER AND ${CMAKE_C_COMPILER} MATCHES ".*icl.*$")
  message(STATUS "Detected Intel Compiler")
  set(USING_INTEL TRUE)
endif ()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/" ${CMAKE_MODULE_PATH})

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Raptor RDF Parser Library")
set(CPACK_PACKAGE_NAME "raptor")
set(CPACK_PACKAGE_VENDOR "The COPASI Developers")
set(CPACK_PACKAGE_CONTACT "COPASI Developers <developers@copasi.org>")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "4")
set(CPACK_PACKAGE_VERSION_PATCH "19")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Libraries/Development")
set(CPACK_DEBIAN_PACKAGE_SECTION "Libraries")

set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_CURRENT_BINARY_DIR};/.vs/;/.vscode/;/.svn/;/.libs/;/.deps/;/.bzr/;.*.o$;.*.lo$;.*.la$;${CPACK_SOURCE_IGNORE_FILES};/.DS_Store;/.svnignore;blib;libsbml-dist")

if(UNIX)
    set(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
    set(CPACK_SET_DESTDIR "ON")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
else()
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
endif()

include(CPack)

# if using msvc or intel windows compiler allow for the use of the static runtime
if(MSVC OR USING_INTEL)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
    set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -D_CRT_SECURE_NO_WARNINGS")
    option(WITH_STATIC_RUNTIME "Compile using the static MSVC Runtime." OFF)
    if(WITH_STATIC_RUNTIME)
      if(CMAKE_VERSION VERSION_LESS 3.15.0)
        foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)

            if(${flag_var} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            endif(${flag_var} MATCHES "/MD")
        endforeach(flag_var)
      else()
        # Set MSVC runtime to MultiThreaded (/MT)
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
      endif()
      add_definitions( -D_MT)
    endif(WITH_STATIC_RUNTIME)
endif(MSVC OR USING_INTEL)


# compile as position independent code
if (UNIX OR CYGWIN)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()


include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/librdfa")


file(GLOB SOURCES 
			${PROJECT_SOURCE_DIR}/librdfa/*.c
			${PROJECT_SOURCE_DIR}/src/*.c
) 

list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/raptor_grddl.c)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/raptor_nfc_test.c)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/raptor_www_test.c)

find_package(EXPAT REQUIRED)
if(NOT TARGET EXPAT::EXPAT)
  add_library(EXPAT::EXPAT UNKNOWN IMPORTED)
  set_target_properties(EXPAT::EXPAT PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${EXPAT_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${EXPAT_INCLUDE_DIR}")
endif()


add_definitions(-DXML_STATIC -DRAPTOR_STATIC -DRAPTOR_V2_EXPERIMENTAL -DHAVE_ERRNO_H -DRAPTOR_INTERNAL -DLIBRDFA_IN_RAPTOR -DHAVE_EXPAT_H)

if (WIN32 AND NOT CYGWIN)
add_definitions( -DWIN32 )
else()
add_definitions( -DHAVE_CONFIG_H )
check_include_files (stdlib.h HAVE_STDLIB_H)
check_include_files (getopt.h HAVE_GETOPT_H)
check_include_files (unistd.h HAVE_UNISTD_H)
check_symbol_exists (stricmp string.h HAVE_STRICMP)
check_symbol_exists (strcasecmp string.h HAVE_STRCASECMP)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/raptor_config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/src/raptor_config.h)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/src)
endif()


add_library(raptor ${SOURCES} ${HEADERS})
target_link_libraries(raptor EXPAT::EXPAT)

include(GNUInstallDirs)
install(TARGETS raptor EXPORT raptorTargets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

file(GLOB HEADERS src/*.h ) 
  
install(FILES ${HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Make cmake config files for all targets
install(EXPORT raptorTargets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
    FILE raptor-config.cmake)


