###############################################################################
#
# Description       : CMake build script for zipper
# Original author(s): Frank Bergmann <fbergman@caltech.edu>
# Organization      : California Institute of Technology
#
###############################################################################

set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required (VERSION 2.8...3.31)
project(zipper)

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
include(GNUInstallDirs)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt")

    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt" VersionString NEWLINE_CONSUME)
    string(STRIP "${VersionString}" VersionString)
    set(ZIPPER_DOTTED_VERSION ${VersionString})
    string(REPLACE "." ";" VersionString "${VersionString}" )
    string(REPLACE "-" ";" VersionString "${VersionString}" )
    list(LENGTH VersionString versionLength)
    list(GET VersionString 0 ZIPPER_VERSION_MAJOR )
    list(GET VersionString 1 ZIPPER_VERSION_MINOR )
    list(GET VersionString 2 ZIPPER_VERSION_PATCH )

    if(${versionLength} GREATER 3)
        list(GET VersionString 3 ZIPPER_VERSION_RELEASE )
    endif()

endif()

# version number needs to be calculated correctly
MATH(EXPR ZIPPER_VERSION_NUMERIC "${ZIPPER_VERSION_MAJOR} * 10000 + ${ZIPPER_VERSION_MINOR} * 100 + ${ZIPPER_VERSION_PATCH}" )
set(PACKAGE_VERSION "${ZIPPER_VERSION_MAJOR}.${ZIPPER_VERSION_MINOR}.${ZIPPER_VERSION_PATCH}${ZIPPER_VERSION_RELEASE}")
set(PACKAGE_NAME "Zipper")

# add make dist and make check target as they are already familiar for
# everyone using the gnumake build
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
add_custom_target(check COMMAND ${CMAKE_MAKE_PROGRAM} test)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ wrapper around minizip compression library.")
set(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
set(CPACK_PACKAGE_VENDOR "The SBML Team")
set(CPACK_PACKAGE_CONTACT "SBML Team <sbml-team@caltech.edu>, Sebastian <devsebas@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${ZIPPER_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ZIPPER_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${ZIPPER_VERSION_PATCH}")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Libraries/Development")
set(CPACK_DEBIAN_PACKAGE_SECTION "Libraries")

set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_CURRENT_BINARY_DIR};/.svn/;/.libs/;/.deps/;/.bzr/;.*.o$;.*.lo$;.*.la$;/.git/;${CPACK_SOURCE_IGNORE_FILES};/.DS_Store;/.svnignore;blib;libsbml-dist;*.txt.user")

set(ADDITIONAL_LIB_DIRS)
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
else()
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
    set(ADDITIONAL_LIB_DIRS "/usr/lib64" "/usr/lib/x86_64-linux-gnu/")
endif()

include(CPack)

# Add an option to compile with all warnings shown
option(WITH_WALL     "Compile with -Wall, so that the compiler will display all warnings." OFF)
mark_as_advanced(WITH_WALL)

if(WITH_WALL)
    if(MSVC OR USING_INTEL)
        add_definitions(/W4)
    else()
        add_definitions(-Wall)
    endif()
endif()

if (UNIX)
  set(ZIPPER_LIBRARY Zipper)
else()
  set(ZIPPER_LIBRARY libZipper)
endif()
# Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build to perform. The options are: None (CMAKE_CXX_FLAGS
or CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build to perform. The options are: None (CMAKE_CXX_FLAGS
or CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

# Set the default dependency directory.
set(ZIPPER_DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/dependencies/ CACHE PATH
    "Directory containing libraries that zipper depends upon. (Particularly important on Windows.)")
if("${ZIPPER_DEPENDENCY_DIR}" STREQUAL "")
  set(ZIPPER_DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/dependencies/ CACHE PATH
  "Directory containing libraries that zipper depends upon. (Particularly important on Windows.)" FORCE)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
find_package(ZLIB REQUIRED)

# allow the user to define additional compilation symbols
if (EXTRA_DEFS)
  foreach(var ${EXTRA_DEFS})
    add_definitions(-D${var})
  endforeach()
endif()

set(USING_INTEL)
if (WIN32 AND CMAKE_C_COMPILER AND ${CMAKE_C_COMPILER} MATCHES ".*icl.*$")
  message(STATUS "Detected Intel Compiler")
  set(USING_INTEL TRUE)
endif ()

if (MINGW)
  message(STATUS "Detected MinGW Compiler")
  add_definitions(-DMINGW=1)
endif ()

if(MSVC OR USING_INTEL OR USING_MINGW)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
    set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -D_CRT_SECURE_NO_WARNINGS")
    option(WITH_STATIC_RUNTIME "Compile using the static MSVC Runtime." OFF)
    if(WITH_STATIC_RUNTIME)
        foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)

            if(${flag_var} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            endif(${flag_var} MATCHES "/MD")
            
        endforeach(flag_var)
    add_definitions( -D_MT)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif(WITH_STATIC_RUNTIME)


  # CMake no longer creates PDB files for static libraries after 2.8.11
  # so we store debug information in the object files instead
  if (${CMAKE_VERSION} VERSION_GREATER "2.8.11")
          foreach(flag_var
              CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
              CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
              CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
              CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
  
              if(${flag_var} MATCHES "/Zi")
          STRING(REPLACE "/Zi"  "/Z7" "${flag_var}" "${${flag_var}}")
              endif(${flag_var} MATCHES "/Zi")
  
          endforeach(flag_var)      
  endif()

else()
   
   if(APPLE)
       # on osx starting with xcode 4.3 the system root is in
       # the app bundle, however cmake (up to 2.8.8) does not seem
       # to update the path, so lets try it here
       if (CMAKE_OSX_SYSROOT AND NOT EXISTS ${CMAKE_OSX_SYSROOT})
         if (EXISTS "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/${CMAKE_OSX_SYSROOT}")
           set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/${CMAKE_OSX_SYSROOT}" CACHE STRING "The SDK root to be used" FORCE)
         endif()
       endif()
       add_definitions(-DMACOSX)
       set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DMACOSX")
   
       # Since we are encountering errors with the use of libc++ on OSX
       # this option allows to override which stdlib to use
       if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
         option(CLANG_USE_STDLIB "Use libstdc++ rather than libc++" OFF)
         if (CLANG_USE_STDLIB)
           SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++" )
         endif()
       endif()
       
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
               if(OSX_SDK_VERSION  VERSION_EQUAL "10.7" OR OSX_SDK_VERSION  VERSION_EQUAL "10.8")
                  # OSX Lion no longer supports ppc architecture
                  set(CMAKE_OSX_ARCHITECTURES "i386;x86_64" CACHE STRING
                     "A semicolon-separated list of build architectures to be used." FORCE)
               else()
                  set(CMAKE_OSX_ARCHITECTURES "i386;ppc;x86_64" CACHE STRING
                      "A semicolon-separated list of build architectures to be used." FORCE)
               endif()
           endif()
       endif(ENABLE_UNIVERSAL)
   else(APPLE)
       add_definitions(-DLINUX)
   
       if(NOT CYGWIN)
           # on cygwin all code is position independent so -fPIC is not needed
           set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -fPIC")
           set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -fPIC")
       endif()
   
       set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DLINUX")
   endif(APPLE)

endif()

# need an extra definition of __APPLE__ to prevent ioapi.c to 
# change the name for the fopen/ftell/fseek commands
# unfortunately that won't work for qt 
if (CYGWIN AND NOT WITH_QT_FILESYSTEM)
add_definitions(-D__APPLE__)
endif()


include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/zipper)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/minizip)

# allow definition of extra include dirs
if (EXTRA_INCLUDE_DIRS)
include_directories(${EXTRA_INCLUDE_DIRS})
endif()

file(GLOB MINIZIP_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/minizip/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/minizip/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/minizip/aes/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/minizip/aes/*.h
)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/minizip/zip.h
          ${CMAKE_CURRENT_SOURCE_DIR}/minizip/unzip.h
          ${CMAKE_CURRENT_SOURCE_DIR}/minizip/ioapi_mem.h
     DESTINATION
          ${CMAKE_CURRENT_BINARY_DIR}/zipper
)

list(REMOVE_ITEM MINIZIP_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/minizip/minizip.c
${CMAKE_CURRENT_SOURCE_DIR}/minizip/miniunz.c
)

if(UNIX AND NOT CYGWIN)
  list(REMOVE_ITEM MINIZIP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/minizip/iowin32.c
  )
endif()
source_group(minizip FILES ${MINIZIP_SOURCES})


file(GLOB ZIPPER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/zipper/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/zipper/*.h
)
source_group(zipper FILES ${ZIPPER_SOURCES})

file(GLOB TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp
)

option(BUILD_SHARED_VERSION "Build the shared version of the library." OFF)
if (BUILD_SHARED_VERSION)
  add_library (${ZIPPER_LIBRARY} SHARED ${ZIPPER_SOURCES} ${MINIZIP_SOURCES} )
  
  if (UNIX)
    SET_TARGET_PROPERTIES(${ZIPPER_LIBRARY} PROPERTIES
                          SOVERSION ${ZIPPER_VERSION_MAJOR}
                          VERSION ${ZIPPER_VERSION_MAJOR}.${ZIPPER_VERSION_MINOR}.${ZIPPER_VERSION_PATCH})
  endif()
  
  set_property(TARGET ${ZIPPER_LIBRARY} PROPERTY CXX_STANDARD 17)
  
  target_link_libraries(${ZIPPER_LIBRARY} ZLIB::ZLIB ${EXTRA_LIBS})
  
  INSTALL(TARGETS ${ZIPPER_LIBRARY}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()

option(BUILD_STATIC_VERSION "Build the static version of the library." ON)
if (BUILD_STATIC_VERSION)
  add_library (${ZIPPER_LIBRARY}-static STATIC ${ZIPPER_SOURCES} ${MINIZIP_SOURCES})
  set_property(TARGET ${ZIPPER_LIBRARY}-static PROPERTY CXX_STANDARD 17)
  target_link_libraries(${ZIPPER_LIBRARY}-static ZLIB::ZLIB ${EXTRA_LIBS})
  
  INSTALL(TARGETS ${ZIPPER_LIBRARY}-static
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()



option(BUILD_TEST "Build the test program." ON)
if (BUILD_TEST)
  enable_testing()

  if (UNIX)
        # setup valgrind

        set(CMAKE_MEMORYCHECK_COMMAND valgrind)
        set(CMAKE_MEMORYCHECK_COMMAND_OPTIONS
           "--error-exitcode=1 --trace-children=yes --leak-check=full --show-reachable=yes --leak-resolution=high --track-origins=yes --error-limit=no ")

        set(MEMCHECK_COMMAND
           "${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}")
        separate_arguments(MEMCHECK_COMMAND)
  endif()

  include(CTest)

  add_executable(${ZIPPER_LIBRARY}-test ${TEST_SOURCES})
  target_link_libraries(${ZIPPER_LIBRARY}-test  ${ZIPPER_LIBRARY}-static ZLIB::ZLIB ${EXTRA_LIBS})
  set_property(TARGET ${ZIPPER_LIBRARY}-test PROPERTY CXX_STANDARD 17)

  add_test(
    NAME test_zipper_run
    COMMAND $<TARGET_FILE:${ZIPPER_LIBRARY}-test>
  )
  
  set_tests_properties(test_zipper_run 
    PROPERTIES PASS_REGULAR_EXPRESSION "All tests passed"
  )
endif()

file(GLOB ZIPPER_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/zipper/*.h ${CMAKE_CURRENT_SOURCE_DIR}/minizip/*.h  )
install(FILES 
        ${ZIPPER_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zipper
)

message(STATUS "
----------------------------------------------------------------------
Zipper version ${PACKAGE_VERSION}
----------------------------------------------------------------------

   Configured on host '${HOSTNAME}'
     host type                       = ${CMAKE_SYSTEM_NAME}
     host operating system           = ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}
     host CPU architecture           = ${CMAKE_SYSTEM_PROCESSOR}

   General build flags:
     CC                              = ${CMAKE_C_COMPILER}
     CXX                             = ${CMAKE_CXX_COMPILER}
     CPPFLAGS                        = ${BUILD_DEFINITIONS}
     CFLAGS                          = ${CMAKE_C_FLAGS}
     CXXFLAGS                        = ${CMAKE_CXX_FLAGS}
     LDFLAGS                         = ${CMAKE_EXE_LINKER_FLAGS}

   Zlib library configuration:
     Zlib library                    = ${ZLIB_LIBRARY}
     Zlib include dir                = ${ZLIB_INCLUDE_DIR}

   Other configuration settings:
     Installation $prefix            = ${CMAKE_INSTALL_PREFIX}
     "
)

