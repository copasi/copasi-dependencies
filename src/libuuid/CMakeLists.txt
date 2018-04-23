cmake_minimum_required (VERSION 2.6)
project (libuuid)

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


# compile as position independent code
if (UNIX OR CYGWIN)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
  list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/test_uuid.c)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/src/*.h )

check_symbol_exists (memcpy string.h HAVE_MEMPCPY)
if (${HAVE_MEMPCPY})
  add_definitions(-DHAVE_MEMPCPY)
endif()

check_include_file(unistd.h HAVE_UNISTD_H)
if (HAVE_UNISTD_H)
  add_definitions( -DHAVE_UNISTD_H )
endif()

if (WIN32 AND NOT CYGWIN)
  add_definitions( -DWIN32 )
else()
  check_symbol_exists (nanosleep time.h HAVE_NANOSLEEP)
  if (${HAVE_NANOSLEEP})
    add_definitions(-DHAVE_NANOSLEEP)
  endif()

  check_symbol_exists (flock sys/file.h HAVE_SYS_FILE_H)
  if (${HAVE_SYS_FILE_H})
    add_definitions(-DHAVE_SYS_FILE_H)
  endif()
endif()

add_library(uuid ${SOURCES} ${HEADERS})

install(TARGETS uuid
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )

 install(FILES src/uuid.h DESTINATION include/uuid)


