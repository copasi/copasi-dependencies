cmake_minimum_required (VERSION 2.8...3.31)

# option() honors normal variables.
# see: https://cmake.org/cmake/help/git-stage/policy/CMP0077.html
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(CpuFeatures VERSION 0.6.0 LANGUAGES C)

if (NOT CMAKE_C_STANDARD)
set(CMAKE_C_STANDARD 99)
endif()

# Default Build Type to be Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# BUILD_TESTING is a standard CMake variable, but we declare it here to make it
# prominent in the GUI.
option(BUILD_TESTING "Enable test (depends on googletest)." OFF)
# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to make
# it prominent in the GUI.
# cpu_features uses bit-fields which are - to some extends - implementation-defined (see https://en.cppreference.com/w/c/language/bit_field).
# As a consequence it is discouraged to use cpu_features as a shared library because different compilers may interpret the code in different ways.
# Prefer static linking from source whenever possible.
option(BUILD_SHARED_LIBS "Build library as shared." OFF)
# PIC
option(BUILD_PIC "Build with Position Independant Code." OFF) # Default is off at least for GCC

# Force PIC on unix when building shared libs
# see: https://en.wikipedia.org/wiki/Position-independent_code
if(BUILD_SHARED_LIBS AND UNIX)
  set(BUILD_PIC ON)
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

include(CheckIncludeFile)
include(CheckSymbolExists)
include(GNUInstallDirs)

macro(setup_include_and_definitions TARGET_NAME)
  target_include_directories(${TARGET_NAME}
    PUBLIC  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/internal>
  )
  target_compile_definitions(${TARGET_NAME}
    PUBLIC STACK_LINE_READER_BUFFER_SIZE=1024
  )
endmacro()

set(PROCESSOR_IS_MIPS FALSE)
set(PROCESSOR_IS_ARM FALSE)
set(PROCESSOR_IS_AARCH64 FALSE)
set(PROCESSOR_IS_X86 FALSE)
set(PROCESSOR_IS_POWER FALSE)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^mips")
  set(PROCESSOR_IS_MIPS TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  set(PROCESSOR_IS_ARM TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
  set(PROCESSOR_IS_AARCH64 TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)|(^i.86$)")
  set(PROCESSOR_IS_X86 TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)")
  set(PROCESSOR_IS_POWER TRUE)
endif()

if (CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
  set(PROCESSOR_IS_ARM TRUE)
  set(PROCESSOR_IS_X86 FALSE)
elseif (CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
  set(PROCESSOR_IS_X86 TRUE)
  set(PROCESSOR_IS_ARM FALSE)
endif()

message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
message(STATUS "PROCESSOR_IS_MIPS: ${PROCESSOR_IS_MIPS}")
message(STATUS "PROCESSOR_IS_ARM: ${PROCESSOR_IS_ARM}")
message(STATUS "PROCESSOR_IS_AARCH64: ${PROCESSOR_IS_AARCH64}")
message(STATUS "PROCESSOR_IS_X86: ${PROCESSOR_IS_X86}")
message(STATUS "PROCESSOR_IS_POWER: ${PROCESSOR_IS_POWER}")

macro(add_cpu_features_headers_and_sources HDRS_LIST_NAME SRCS_LIST_NAME)
  list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features_macros.h)
  list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpu_features_cache_info.h)
  if(PROCESSOR_IS_MIPS)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpuinfo_mips.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_mips.c)
  elseif(PROCESSOR_IS_ARM)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpuinfo_arm.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_arm.c)
  elseif(PROCESSOR_IS_AARCH64)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpuinfo_aarch64.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_aarch64.c)
  elseif(PROCESSOR_IS_X86)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpuinfo_x86.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/internal/cpuid_x86.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_x86.c)
  elseif(PROCESSOR_IS_POWER)
      list(APPEND ${HDRS_LIST_NAME} ${PROJECT_SOURCE_DIR}/include/cpuinfo_ppc.h)
      list(APPEND ${SRCS_LIST_NAME} ${PROJECT_SOURCE_DIR}/src/cpuinfo_ppc.c)
  else()
    message(FATAL_ERROR "Unsupported architectures ${CMAKE_SYSTEM_PROCESSOR}")
  endif()
endmacro()

#
# library : utils
#

add_library(utils OBJECT
  ${PROJECT_SOURCE_DIR}/include/internal/bit_utils.h
  ${PROJECT_SOURCE_DIR}/include/internal/filesystem.h
  ${PROJECT_SOURCE_DIR}/include/internal/stack_line_reader.h
  ${PROJECT_SOURCE_DIR}/include/internal/string_view.h
  ${PROJECT_SOURCE_DIR}/src/filesystem.c
  ${PROJECT_SOURCE_DIR}/src/stack_line_reader.c
  ${PROJECT_SOURCE_DIR}/src/string_view.c
)
set_property(TARGET utils PROPERTY POSITION_INDEPENDENT_CODE ${BUILD_PIC})
setup_include_and_definitions(utils)

#
# library : unix_based_hardware_detection
#

if(UNIX)
  add_library(unix_based_hardware_detection OBJECT
    ${PROJECT_SOURCE_DIR}/include/internal/hwcaps.h
    ${PROJECT_SOURCE_DIR}/src/hwcaps.c
  )
  setup_include_and_definitions(unix_based_hardware_detection)
  check_include_file(dlfcn.h HAVE_DLFCN_H)
  if(HAVE_DLFCN_H)
    target_compile_definitions(unix_based_hardware_detection PRIVATE HAVE_DLFCN_H)
  endif()
  check_symbol_exists(getauxval "sys/auxv.h" HAVE_STRONG_GETAUXVAL)
  if(HAVE_STRONG_GETAUXVAL)
    target_compile_definitions(unix_based_hardware_detection PRIVATE HAVE_STRONG_GETAUXVAL)
  endif()
  set_property(TARGET unix_based_hardware_detection PROPERTY POSITION_INDEPENDENT_CODE ${BUILD_PIC})
endif()

#
# library : cpu_features
#
set (CPU_FEATURES_HDRS)
set (CPU_FEATURES_SRCS)
add_cpu_features_headers_and_sources(CPU_FEATURES_HDRS CPU_FEATURES_SRCS)
list(APPEND CPU_FEATURES_SRCS $<TARGET_OBJECTS:utils>)
if(NOT PROCESSOR_IS_X86 AND UNIX)
  list(APPEND CPU_FEATURES_SRCS $<TARGET_OBJECTS:unix_based_hardware_detection>)
endif()
add_library(cpu_features ${CPU_FEATURES_HDRS} ${CPU_FEATURES_SRCS})
set_target_properties(cpu_features PROPERTIES PUBLIC_HEADER "${CPU_FEATURES_HDRS}")
setup_include_and_definitions(cpu_features)
target_link_libraries(cpu_features PUBLIC ${CMAKE_DL_LIBS})
set_property(TARGET cpu_features PROPERTY POSITION_INDEPENDENT_CODE ${BUILD_PIC})
target_include_directories(cpu_features
  PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/cpu_features>
)
if(PROCESSOR_IS_X86)
  if(APPLE)
    target_compile_definitions(cpu_features PRIVATE HAVE_SYSCTLBYNAME)
  endif()
endif()
add_library(CpuFeature::cpu_features ALIAS cpu_features)

#
# program : list_cpu_features
#

add_executable(list_cpu_features ${PROJECT_SOURCE_DIR}/src/utils/list_cpu_features.c)
target_link_libraries(list_cpu_features PRIVATE cpu_features)
add_executable(CpuFeature::list_cpu_features ALIAS list_cpu_features)

#
# ndk_compat
#

if(ANDROID)
add_subdirectory(ndk_compat)
endif()

#
# tests
#

include(CTest)
if(BUILD_TESTING)
  # Automatically incorporate googletest into the CMake Project if target not
  # found.
  enable_language(CXX)

  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF) # prefer use of -std11 instead of -gnustd11

  if(NOT TARGET gtest OR NOT TARGET gmock_main)
    # Download and unpack googletest at configure time.
    configure_file(
      cmake/googletest.CMakeLists.txt.in
      googletest-download/CMakeLists.txt
    )

    execute_process(
      COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(
      COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker settings on
    # Windows.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines the gtest and
    # gtest_main targets.
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                     ${CMAKE_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)
  endif()

  add_subdirectory(test)
endif()

#
# Install cpu_features and list_cpu_features
#

include(GNUInstallDirs)
install(TARGETS cpu_features list_cpu_features
  EXPORT CpuFeaturesTargets
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cpu_features
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT CpuFeaturesTargets
  NAMESPACE CpuFeatures::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CpuFeatures
  COMPONENT Devel
)
include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/CpuFeaturesConfig.cmake.in
  "${PROJECT_BINARY_DIR}/CpuFeaturesConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CpuFeatures"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/CpuFeaturesConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
)
install(
  FILES
    "${PROJECT_BINARY_DIR}/CpuFeaturesConfig.cmake"
    "${PROJECT_BINARY_DIR}/CpuFeaturesConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CpuFeatures"
  COMPONENT Devel
)
