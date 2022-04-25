# NativeJIT

cmake_minimum_required(VERSION 2.6)

# Remove the warning that WIN32 is not defined in Cygwin any more.
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# Project-wide settings for NativeJIT
project(NativeJIT CXX C)

if (WIN32 OR CYGWIN)
    set(NATIVEJIT_PLATFORM_POSIX 0)
    set(NATIVEJIT_PLATFORM_WINDOWS 1)
else()
    set(NATIVEJIT_PLATFORM_POSIX 1)
    set(NATIVEJIT_PLATFORM_WINDOWS 0)
endif()


if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    message(STATUS "######################################## NativeJIT is root")
  else()
    message(STATUS "######################################## BitFunnel is root")
    set(NATIVEJIT_PREFIX "NativeJIT/")
endif (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})

if(NATIVEJIT_PLATFORM_WINDOWS)
  # Note: -D works for both MSVC and GCC/Clang.
  set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -DNATIVEJIT_PLATFORM_WINDOWS")
else()
  set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -DNATIVEJIT_PLATFORM_POSIX")
endif()


set(GTEST_REQUIRED_FLAGS "-Wno-shift-sign-overflow -Wno-missing-noreturn -Wno-used-but-marked-unused -Wno-deprecated -Wno-missing-variable-declarations -Wno-global-constructors -Wno-weak-vtables -Wno-undef")
set(WEVERYTHING_FLAGS "-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic ${GTEST_REQUIRED_FLAGS}")

if(MSVC)
  set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /W4 /WX -D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
  set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_CXX_FLAGS} /MDd")
  set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} ${COMMON_CXX_FLAGS} /MD")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  # Need gnu++ instead of c++ so that GTest can access fdopen() etc.
  if (NOT DISABLE_TESTING)
  set(CMAKE_CXX_FLAGS "-msse4.2 -std=gnu++14 -Wall -Wextra -Werror -Wold-style-cast -fstrict-aliasing -Wstrict-aliasing -Wno-type-limits -Wno-noexcept-type")
  else()
  set(CMAKE_CXX_FLAGS "-msse4.2 -std=gnu++11 -Wall -Wextra -Werror -Wold-style-cast -fstrict-aliasing -Wstrict-aliasing -Wno-type-limits -Wno-noexcept-type")
  endif()
else()
  # TODO: define a target for -Weverything.
  # set(CMAKE_CXX_FLAGS "-msse4.2 -std=c++14 -Wall -Wextra -Werror -Wold-style-cast ${WEVERYTHING_FLAGS}")
  set(CMAKE_CXX_FLAGS "-msse4.2 -std=c++14 -Wall -Wextra -Wold-style-cast -Wno-deprecated-copy")
endif()

# compile as position independent code
if (UNIX OR CYGWIN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# are we using the intel compiler
set(USING_INTEL)
if (WIN32 AND CMAKE_C_COMPILER AND ${CMAKE_C_COMPILER} MATCHES ".*icl.*$")
  message(STATUS "Detected Intel Compiler")
  set(USING_INTEL TRUE)
endif ()

# if using msvc or intel windows compiler allow for the use of the static runtime
if(MSVC OR USING_INTEL)
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
endif(MSVC OR USING_INTEL)

###############################################################################

# gtest

option(DISABLE_TESTING "Allows to disable testing (which requires c++14)." OFF)

if (NOT DISABLE_TESTING)
add_subdirectory(googletest)

include_directories(
  ${gtest_SOURCE_DIR}/include
)
endif()
###############################################################################


# This allows the solution to group projects into folders like src and test.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (NOT DISABLE_TESTING)
enable_testing()
endif()

include_directories(inc)
add_subdirectory(src/CodeGen)
add_subdirectory(src/NativeJIT)
if (NOT DISABLE_TESTING)
add_subdirectory(test/CodeGen)
add_subdirectory(test/NativeJIT)
add_subdirectory(test/Shared)
add_subdirectory(Examples)
endif()

add_custom_target(TOPLEVEL_NATIVEJIT SOURCES
  Configure_Make.bat
  Configure_Make.sh
  Configure_MSVC.bat
  LICENSE.txt
  README.md
)

set(ENV{GTEST_COLOR} 1)

# Install all headers
foreach(dir CodeGen Nodes )
    file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/inc/NativeJIT/${dir}/*.h")
    install(FILES ${header_files} 
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/NativeJIT/${dir}"
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
        endforeach(dir)

file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/inc/NativeJIT/*.h")
install(FILES ${header_files} 
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/NativeJIT"
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/inc/Temporary/*.h")
install(FILES ${header_files} 
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Temporary/${dir}"
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

if (APPLE)
  target_compile_options(NativeJIT PUBLIC "-msse4.2")
elseif (MSVC)
  target_compile_options(NativeJIT PUBLIC "-DNATIVEJIT_PLATFORM_WINDOWS -EHsc")
else ()
  target_compile_options(NativeJIT PUBLIC "-msse4.2")
endif ()

target_include_directories(NativeJIT PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>)

# Make cmake config files for all targets
install(EXPORT  NativeJITTargets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
    FILE nativejit-config.cmake)

# add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
