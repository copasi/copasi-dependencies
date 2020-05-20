cmake_minimum_required(VERSION 2.6)
project(CLAPACK C)
enable_testing()
include(CTest)

include(GNUInstallDirs)

# are we using the intel compiler
set(USING_INTEL)
if (WIN32 AND CMAKE_C_COMPILER AND ${CMAKE_C_COMPILER} MATCHES ".*icl.*$")
  message(STATUS "Detected Intel Compiler")
  set(USING_INTEL TRUE)
endif ()

# compile as position independent code
if (UNIX OR CYGWIN)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# if using msvc or intel windows compiler allow for the use of the static runtime
if(MSVC OR USING_INTEL)
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
endif(MSVC OR USING_INTEL)


macro(copy_if_different_and_remove source target)
file(READ ${source} CONTENT)

if (EXISTS ${target})
    file(READ ${target} CURRENT_CONTENT)
    string(COMPARE EQUAL "${CURRENT_CONTENT}" "${CONTENT}" IS_SAME)
    if (NOT ${IS_SAME})
        file(WRITE ${target} "${CONTENT}")
    endif()
else()
    file(WRITE ${target} "${CONTENT}")
endif()

file(REMOVE ${source})

endmacro(copy_if_different_and_remove)


set(F2C_INTEGER "long int" CACHE STRING
    "f2c integer type")
set(F2C_LOGICAL "long int" CACHE STRING
    "f2c logical type")

include_directories(${CLAPACK_BINARY_DIR}/INCLUDE)
# allow to overwrite the f2c integer size
configure_file(${CLAPACK_SOURCE_DIR}/INCLUDE/f2c.h.cmake
  ${CLAPACK_BINARY_DIR}/INCLUDE/f2c.h.new @ONLY)
configure_file(${CLAPACK_SOURCE_DIR}/INCLUDE/f2c.h.cmake
  ${CLAPACK_BINARY_DIR}/F2CLIBS/libf2c/f2c.h.new @ONLY)
  
copy_if_different_and_remove(
  ${CLAPACK_BINARY_DIR}/INCLUDE/f2c.h.new
  ${CLAPACK_BINARY_DIR}/INCLUDE/f2c.h
)

copy_if_different_and_remove(
  ${CLAPACK_BINARY_DIR}/F2CLIBS/libf2c/f2c.h.new
  ${CLAPACK_BINARY_DIR}/F2CLIBS/libf2c/f2c.h
)

if(WIN32 AND NOT CYGWIN)
  set(SECOND_SRC  ${CLAPACK_SOURCE_DIR}/INSTALL/winsecond.c)
  set(DSECOND_SRC  ${CLAPACK_SOURCE_DIR}/INSTALL/windsecnd.c)
  add_definitions(-DNO_ISATTY -DMSDOS -DUSE_CLOCK)
else()
  set(SECOND_SRC  ${CLAPACK_SOURCE_DIR}/INSTALL/second.c)
  set(DSECOND_SRC  ${CLAPACK_SOURCE_DIR}/INSTALL/dsecnd.c)
endif()
enable_testing()
option(USE_BLAS_WRAP "pre-pend f2c_ to each function in blas" OFF)
if(NOT USE_BLAS_WRAP)
# _zrotg_ seems to be missing in the wrap header
  add_definitions(-DNO_BLAS_WRAP)
endif()
include_directories(${CLAPACK_SOURCE_DIR}/INCLUDE)
add_subdirectory(F2CLIBS)
add_subdirectory(BLAS)
add_subdirectory(SRC)
if (BUILD_TESTING)
add_subdirectory(TESTING)
endif()
set(CLAPACK_VERSION 3.2.1)
set(CPACK_PACKAGE_VERSION_MAJOR 3)
set(CPACK_PACKAGE_VERSION_MINOR 2)
set(CPACK_PACKAGE_VERSION_PATCH 1)
include(CPack)

file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/INCLUDE/*.h")
install(FILES ${header_files} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
file(GLOB bin_header_files "${CMAKE_CURRENT_BINARY_DIR}/INCLUDE/*.h")
install(FILES ${bin_header_files} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
  
configure_file(${CLAPACK_SOURCE_DIR}/clapack-config-version.cmake.in
  ${CLAPACK_BINARY_DIR}/clapack-config-version.cmake @ONLY)
configure_file(${CLAPACK_SOURCE_DIR}/clapack-config.cmake.in
  ${CLAPACK_BINARY_DIR}/clapack-config.cmake @ONLY)

message("--
-- CLAPACK ${CLAPACK_VERSION}
-- =============
--
--  integer type: ${F2C_INTEGER}
--  logical type: ${F2C_LOGICAL}
--")

