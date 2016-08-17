###############################################################################
#
# Description       : CMake build script for libCombine
# Original author(s): Frank Bergmann <fbergman@caltech.edu>
# Organization      : California Institute of Technology
#
###############################################################################

set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.6)
project(libCombine)

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

###############################################################################
#
# Parse VERSION.txt to determine the package version
#

set(LIBCOMBINE_VERSION_MAJOR)
set(LIBCOMBINE_VERSION_MINOR)
set(LIBCOMBINE_VERSION_PATCH)
set(LIBCOMBINE_VERSION_RELEASE)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt")

    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt" VersionString NEWLINE_CONSUME)
    string(STRIP "${VersionString}" VersionString)
    set(COMBINE_DOTTED_VERSION ${VersionString})
    string(REPLACE "." ";" VersionString "${VersionString}" )
    string(REPLACE "-" ";" VersionString "${VersionString}" )
    list(LENGTH VersionString versionLength)
    list(GET VersionString 0 LIBCOMBINE_VERSION_MAJOR )
    list(GET VersionString 1 LIBCOMBINE_VERSION_MINOR )
    list(GET VersionString 2 LIBCOMBINE_VERSION_PATCH )

    if(${versionLength} GREATER 3)
        list(GET VersionString 3 LIBCOMBINE_VERSION_RELEASE )
    endif()

endif()

MATH(EXPR LIBCOMBINE_VERSION_NUMERIC "${LIBCOMBINE_VERSION_MAJOR} * 10000 + ${LIBCOMBINE_VERSION_MINOR} * 100 + ${LIBCOMBINE_VERSION_PATCH}" )
set(PACKAGE_VERSION "${LIBCOMBINE_VERSION_MAJOR}.${LIBCOMBINE_VERSION_MINOR}.${LIBCOMBINE_VERSION_PATCH}${LIBCOMBINE_VERSION_RELEASE}")
set(PACKAGE_NAME "libCombine")

# add make dist and make check target as they are already familiar for
# everyone using the gnumake build
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
add_custom_target(check COMMAND ${CMAKE_MAKE_PROGRAM} test)

###############################################################################
#
# The next lines configure the parameters for packaging the binaries.
# They can be invoked with "make package" or "nmake package" or by using
# cpack -G zip|deb|rpm|dmg|nsis
#

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An API library for reading/writing/manipulating OMEX.")
set(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
set(CPACK_PACKAGE_VENDOR "The SBML Team")
set(CPACK_PACKAGE_CONTACT "LibSBML Team <libsbml-team@caltech.edu>")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${LIBCOMBINE_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${LIBCOMBINE_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${LIBCOMBINE_VERSION_PATCH}")
set(CPACK_RPM_PACKAGE_LICENSE "BSD")
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


###############################################################################
#
# Here we have the main configuration options for libcombine.
#

# Build static / shared library
# option(BUILD_SHARED_LIBS  "Build shared library. (Set to OFF to build static libraries.)" OFF)

# Whether to compile examples
option(WITH_EXAMPLES "Compile the libCombine example programs."  OFF)

# Which language bindings should be built
option(WITH_CSHARP   "Generate C# language bindings."     OFF)
option(WITH_JAVA     "Generate Java language bindings."   OFF)
option(WITH_PYTHON   "Generate Python language bindings." OFF)
option(WITH_PERL     "Generate Perl language bindings."   OFF)
option(WITH_RUBY     "Generate Ruby language bindings"    OFF)
option(WITH_R        "Generate R language bindings"       OFF)
option(WITH_OCTAVE   "Generate Octave language bindings." OFF)
option(WITH_MATLAB   "Generate MATLAB language bindings." OFF)

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


set(LIBCOMBINE_BUILD_TYPE "native")
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(LIBCOMBINE_BUILD_TYPE "32bit")
else()
  set(LIBCOMBINE_BUILD_TYPE "64bit")
endif()
if (APPLE AND ENABLE_UNIVERSAL)
  set(LIBCOMBINE_BUILD_TYPE "universal")
endif()


# Use the version number in the shared library.
#
option (LIBCOMBINE_SHARED_VERSION "Build the libcombine shared library with version information" ON)
mark_as_advanced(LIBCOMBINE_SHARED_VERSION)

# Enable the generation of unit tests. If enabled, all test runners
# will be created and can be run with "make test" or ctest.
# This won't work in Visual Studio 2003, so we disable this option there.
#
if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio 6" AND NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio 7")
    option(WITH_CHECK    "Compile unit tests. Run with 'make test' or 'ctest'." OFF)
endif()

# Use C++ namespace.
option(WITH_CPP_NAMESPACE "Use a C++ namespace for libCombine."   OFF)

# Generate documentation.
option(WITH_DOXYGEN  "Generate documentation for libCombine using Doxygen."  OFF )
# marks as advanced, so as to hide documentation generation
mark_as_advanced(WITH_DOXYGEN)

# Re-generate the swig bindings? This really should be on by default
# otherwise one might have the wrong wrapper code without support for
# the libcombine packages one wants.
option(WITH_SWIG     "Regenerate SWIG-based language bindings."  ON )


if (UNIX)
  set(COMBINE_LIBRARY Combine)
else()
  set(COMBINE_LIBRARY libCombine)
endif()
# Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build to perform. The options are: None (CMAKE_CXX_FLAGS
or CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the build type. The options are: None (CMAKE_CXX_FLAGS or
  CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

# Set the default dependency directory.
set(COMBINE_DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/dependencies/ CACHE PATH
    "Directory containing libraries that libCombine depends upon. (Particularly important on Windows.)")
if("${COMBINE_DEPENDENCY_DIR}" STREQUAL "")
  set(COMBINE_DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/dependencies/ CACHE PATH
  "Directory containing libraries that libCombine depends upon. (Particularly important on Windows.)" FORCE)
endif()

set(LIBCOMBINE_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBCOMBINE_ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

###############################################################################
#
# If WITH_SWIG is selected, we need to find swig
#

set (LIBCOMBINE_USE_CPP_NAMESPACE 0)
if(WITH_CPP_NAMESPACE)
    add_definitions(-DLIBCOMBINE_USE_CPP_NAMESPACE)
    set (LIBCOMBINE_USE_CPP_NAMESPACE 1)
endif()

if(WITH_SWIG)
    find_program(SWIG_EXECUTABLE
      NAMES swig
      PATHS
        c:/swigwin-3.0.5
        c:/swigwin-3.0.2
        c:/swigwin-3.0.0
        c:/swigwin-2.0.12
        c:/swigwin-2.0.11
        c:/swigwin-2.0.10
        c:/swigwin-2.0.9
        c:/swigwin-2.0.8
        c:/swigwin-2.0.7
        c:/swigwin-2.0.4
        c:/swigwin-2.0.2
        c:/swigwin-2.0.1
        c:/swigwin-2.0.0
        /usr/local/bin
        /opt/local/bin
        /usr/bin
      DOC "The file name of the swig executable."
      )
    set(SWIG_EXTRA_ARGS)
endif(WITH_SWIG)

###############################################################################
#
# Locate libsbml
#
find_library(LIBSBML_LIBRARY
    NAMES libsbml-static.lib sbml-static libsbml.lib sbml
    PATHS ${COMBINE_DEPENDENCY_DIR}/lib
          /usr/local/lib          
          /usr/lib 
    DOC "The file name of the sbml library."
)

find_path(LIBSBML_INCLUDE_DIR
    NAMES sbml/SBase.h
    PATHS 
          ${COMBINE_DEPENDENCY_DIR}/include
          /usr/local/include
          /usr/include 
    DOC "The directory containing the sbml include files."
)

set(LIBSBML_STATIC OFF CACHE BOOL "is libsbml statically compiled")
if (WIN32 AND NOT CYGWIN)
  if (LIBSBML_LIBRARY AND LIBSBML_LIBRARY MATCHES ".*-static*$")
    set(LIBSBML_STATIC ON CACHE BOOL "is libsbml statically compiled")
  else()
    option(LIBSBML_STATIC "The LibSBML library was statically compiled" ON)
  endif()
endif()

if(NOT EXISTS "${LIBSBML_INCLUDE_DIR}/sbml/SBase.h")
    message(FATAL_ERROR 
"The include directory specified for sbml appears to be invalid.  It should contain the 
file sbml/SBase.h, but it does not. Please specify the LIBSBML_INCLUDE_DIR variable."
)
endif()


find_library(ZIPPER_LIBRARY
    NAMES Zipper-static Zipper libZipper-static libZipper
    PATHS /usr/lib /usr/local/lib
          ${CMAKE_OSX_SYSROOT}/usr/lib
          ${COMBINE_DEPENDENCY_DIR}/lib
          ${ADDITIONAL_LIB_DIRS}
          $ENV{ZLIBROOT}/lib
    DOC "The file name of the ZIPPER library."
)


find_path(ZIPPER_INCLUDE_DIR
      NAMES zipper/zipper.h
      PATHS ${CMAKE_OSX_SYSROOT}/usr/include
            /usr/include /usr/local/include
            ${COMBINE_DEPENDENCY_DIR}/include
            $ENV{ZLIBROOT}/include
            NO_DEFAULT_PATH
      DOC "The directory containing the ZIPPER include files."
            )

find_library(LIBZ_LIBRARY
    NAMES zdll.lib z zlib.lib
    PATHS /usr/lib /usr/local/lib
          ${CMAKE_OSX_SYSROOT}/usr/lib
          ${COMBINE_DEPENDENCY_DIR}/lib
          ${ADDITIONAL_LIB_DIRS}
          $ENV{ZLIBROOT}/lib
    DOC "The file name of the zip compression library."
)


if (NOT LIBZ_INCLUDE_DIR)
  find_path(LIBZ_INCLUDE_DIR
      NAMES zlib.h zlib/zlib.h
      PATHS ${CMAKE_OSX_SYSROOT}/usr/include
            /usr/include /usr/local/include
            ${COMBINE_DEPENDENCY_DIR}/include
            $ENV{ZLIBROOT}/include
            NO_DEFAULT_PATH
      DOC "The directory containing the zlib include files."
            )
endif()
    
if (NOT LIBZ_INCLUDE_DIR)
    find_path(LIBZ_INCLUDE_DIR
    NAMES zlib.h zlib/zlib.h
    PATHS ${CMAKE_OSX_SYSROOT}/usr/include
          /usr/include /usr/local/include
          ${COMBINE_DEPENDENCY_DIR}/include
          $ENV{ZLIBROOT}/include
    DOC "The directory containing the zlib include files."
          )
endif()

if(WITH_CSHARP)
    find_program(CSHARP_COMPILER
        NAMES gmcs csc
        PATHS C:/Windows/Microsoft.NET/Framework/v2.0.50727/ /usr/bin /usr/local/bin
        DOC "The file name of the C# compiler."
        )
    if(UNIX)
    else()
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            # mark libcombine library as x86
            set(CSHARP_EXTRA_ARGS -platform:x86 )
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # mark libcombine library as x64
            set(CSHARP_EXTRA_ARGS -platform:x64 )
        endif()
    endif()

endif(WITH_CSHARP)

# allow the user to define additional compilation symbols
if (EXTRA_DEFS)
  foreach(var ${EXTRA_DEFS})
    add_definitions(-D${var})
  endforeach()
endif()

set(USE_ZLIB ON)
add_definitions( -DUSE_ZLIB )

if (LIBZ_LIBRARY)
  set(LIBCOMBINE_LIBS ${LIBCOMBINE_LIBS} ${LIBZ_LIBRARY})
endif()

if (LIBSBML_LIBRARY)
  set(LIBCOMBINE_LIBS ${LIBCOMBINE_LIBS} ${LIBSBML_LIBRARY})
endif()

if (ZIPPER_LIBRARY)
  set(LIBCOMBINE_LIBS ${LIBCOMBINE_LIBS} ${ZIPPER_LIBRARY})
endif()

# make sure that we have a valid zip library
check_library_exists("${LIBZ_LIBRARY}" "gzopen" "" LIBZ_FOUND_SYMBOL)
if(NOT LIBZ_FOUND_SYMBOL)
    # this is odd, but on windows this check always fails! must be a
    # bug in the current cmake version so for now only issue this
    # warning on linux
    if(UNIX)
        message(WARNING
"The chosen zlib library does not appear to be valid because it is
missing certain required symbols. Please check that ${LIBZ_LIBRARY} is
the correct zlib library. For details about the error, please see
${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log")
    endif()
endif()

if(NOT EXISTS "${LIBZ_INCLUDE_DIR}/zlib.h")
    message(FATAL_ERROR
"The include directory specified for zlib does not appear to be
valid. It should contain the file zlib.h, but it does not. Please
verify the LIBZ_INCLUDE_DIR variable.")
endif()

set(USING_INTEL)
if (WIN32 AND CMAKE_C_COMPILER AND ${CMAKE_C_COMPILER} MATCHES ".*icl.*$")
  message(STATUS "Detected Intel Compiler")
  set(USING_INTEL TRUE)
endif ()

option(WITH_QT_FILESYSTEM "Use Qt filesystem api instead of the c++17 one." OFF)
if (WITH_QT_FILESYSTEM)
  add_definitions(-DUSE_QT)
  find_library(QT_CORE_LIBRARY
    NAMES libQtCore QtCore
    PATHS /usr/lib /usr/local/lib
          ${CMAKE_OSX_SYSROOT}/usr/lib
          ${COMBINE_DEPENDENCY_DIR}/lib
          ${ADDITIONAL_LIB_DIRS}
    DOC "The qt core library."
  )
find_path(QT_INCLUDE_DIR
    NAMES QtCore/QDir
    PATHS ${CMAKE_OSX_SYSROOT}/usr/include
          /usr/include/qt4 /usr/local/include/qt4
          ${COMBINE_DEPENDENCY_DIR}/include
    DOC "The directory containing the boost header files."
          )
endif (WITH_QT_FILESYSTEM)


option(WITH_BOOST_FILESYSTEM "Use Boost filesystem api instead of the c++17 one." OFF)
if (WITH_BOOST_FILESYSTEM)
add_definitions(-DUSE_BOOST)
find_library(BOOST_SYSTEM_LIBRARY
    NAMES boost_system libboost_system libboost_system-vc140-mt-gd-1_61.lib
    PATHS /usr/lib /usr/local/lib
          ${CMAKE_OSX_SYSROOT}/usr/lib
          ${COMBINE_DEPENDENCY_DIR}/lib
          ${ADDITIONAL_LIB_DIRS}
    DOC "The boost system library."
)
find_library(BOOST_FILESYSTEM_LIBRARY
    NAMES boost_filesystem libboost_filesystem libboost_filesystem-vc140-mt-gd-1_61.lib
    PATHS /usr/lib /usr/local/lib
          ${CMAKE_OSX_SYSROOT}/usr/lib
          ${COMBINE_DEPENDENCY_DIR}/lib
          ${ADDITIONAL_LIB_DIRS}
    DOC "The boost filesystem library."
)
find_path(BOOST_INCLUDE_DIR
    NAMES filesystem.hpp filesystem/path.h
    PATHS ${CMAKE_OSX_SYSROOT}/usr/include
          /usr/include /usr/local/include
          ${COMBINE_DEPENDENCY_DIR}/include
    DOC "The directory containing the boost header files."
          )
endif()
    
if(MSVC OR USING_INTEL)

  if (LIBSBML_STATIC)
      add_definitions(-DLIBSBML_STATIC -DLIBLAX_STATIC)
      set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DLIBSBML_STATIC -DLIBLAX_STATIC")
  else()
      add_definitions(-DLIBSBML_EXPORTS -DLIBLAX_EXPORTS)
      set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DLIBSBML_EXPORTS -DLIBLAX_EXPORTS")
  endif()

    add_definitions(-DWIN32 -DLIBCOMBINE_EXPORTS -DLIBLAX_EXPORTS)
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
   # enable c++11
   set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
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


include_directories(BEFORE ${LIBZ_INCLUDE_DIR})
include_directories(BEFORE ${ZIPPER_INCLUDE_DIR})

if (WITH_BOOST_FILESYSTEM AND BOOST_INCLUDE_DIR)
include_directories(${BOOST_INCLUDE_DIR})
endif()

if (WITH_QT_FILESYSTEM AND QT_INCLUDE_DIR)
include_directories(${QT_INCLUDE_DIR})
endif()

# allow definition of extra include dirs
if (EXTRA_INCLUDE_DIRS)
include_directories(${EXTRA_INCLUDE_DIRS})
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${LIBSBML_INCLUDE_DIR})

file(GLOB COMBINE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/combine/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/combine/*.h
)
source_group(combine FILES ${COMBINE_SOURCES})
set(LIBCOMBINE_SOURCES ${COMBINE_SOURCES})
set(LIBCOMBINE_LIBRARY ${COMBINE_LIBRARY})
set (CMAKE_INSTALL_LIBDIR lib)

add_subdirectory(src)

if (WITH_EXAMPLES)

  add_subdirectory(examples)

endif(WITH_EXAMPLES)


#option(BUILD_SHARED_VERSION "Build the shared version of the library." OFF)
#if (BUILD_SHARED_VERSION)
#  add_library (${COMBINE_LIBRARY} SHARED ${COMBINE_SOURCES} ${MINIZIP_SOURCES} )
#  
#  if (UNIX)
#    SET_TARGET_PROPERTIES(${COMBINE_LIBRARY} PROPERTIES
#                          SOVERSION ${COMBINE_VERSION_MAJOR}
#                          VERSION ${COMBINE_VERSION_MAJOR}.${COMBINE_VERSION_MINOR}.${COMBINE_VERSION_PATCH})
#  endif()
#  
#  target_link_libraries(${COMBINE_LIBRARY} ${LIBZ_LIBRARY} ${EXTRA_LIBS})
#  
#  if (WITH_BOOST_FILESYSTEM)
#    target_link_libraries(${COMBINE_LIBRARY} ${BOOST_SYSTEM_LIBRARY} ${BOOST_FILESYSTEM_LIBRARY})
#  endif()
#  
#  if (WITH_QT_FILESYSTEM)
#    target_link_libraries(${COMBINE_LIBRARY} ${QT_CORE_LIBRARY})
#  endif()
#
#  INSTALL(TARGETS ${COMBINE_LIBRARY}
#          RUNTIME DESTINATION bin
#          LIBRARY DESTINATION lib
#          ARCHIVE DESTINATION lib
#  )
#endif()
#
#
#

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
  
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/test)
  
  file(GLOB TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.hpp
  )

  
  add_executable(${COMBINE_LIBRARY}-test ${TEST_SOURCES})
  target_link_libraries(${COMBINE_LIBRARY}-test  ${LIBCOMBINE_LIBRARY}-static ${ZIPPER_LIBRARY} ${LIBZ_LIBRARY} ${LIBSBML_LIBRARY} ${EXTRA_LIBS})
  if (WITH_BOOST_FILESYSTEM)
    target_link_libraries(${COMBINE_LIBRARY}-test ${BOOST_SYSTEM_LIBRARY} ${BOOST_FILESYSTEM_LIBRARY})
  endif()
  if (WITH_QT_FILESYSTEM)
    target_link_libraries(${COMBINE_LIBRARY}-test ${QT_CORE_LIBRARY})
  endif()

  add_test(test_${COMBINE_LIBRARY}_run ${CMAKE_CURRENT_BINARY_DIR}/${COMBINE_LIBRARY}-test )
set_tests_properties(test_${COMBINE_LIBRARY}_run PROPERTIES ENVIRONMENT 
  "srcdir=${CMAKE_CURRENT_SOURCE_DIR}/src/test")

endif()


message(STATUS "
----------------------------------------------------------------------
libCombine version ${PACKAGE_VERSION}
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

   SBML library configuration:
     SBML library                    = ${LIBSBML_LIBRARY}
     SBML include dir                = ${LIBSBML_INCLUDE_DIR}

   Zlib library configuration:
     Zlib library                    = ${LIBZ_LIBRARY}
     Zlib include dir                = ${LIBZ_INCLUDE_DIR}

   Zipper library configuration:
     Zipper library                  = ${ZIPPER_LIBRARY}
     Zipper include dir              = ${ZIPPER_INCLUDE_DIR}

   Other configuration settings:
     Installation $prefix            = ${CMAKE_INSTALL_PREFIX}
     ")

if(WITH_QT_FILESYSTEM)
    message(STATUS " Using Qt Filesystem API
     QtCore library                  = ${QT_CORE_LIBRARY}
     Qt include dir                  = ${QT_INCLUDE_DIR}
        
    ")
elseif(WITH_BOOST_FILESYSTEM)
    message(STATUS " Using Boost Filesystem API
     Filesystem library              = ${BOOST_FILESYSTEM_LIBRARY}
     System library                  = ${BOOST_SYSTEM_LIBRARY}
     Boost include dir               = ${BOOST_INCLUDE_DIR}
        
    ")
else ()
    message(STATUS " Using C++17 Filesystem API (please ensure that your compiler supports it)
 
    ")
endif()
