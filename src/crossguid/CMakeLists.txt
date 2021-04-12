cmake_minimum_required(VERSION 2.8.12)
project(CrossGuid)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

option(CROSSGUID_TESTS "Build test runner" ON)

if (CMAKE_MAJOR_VERSION LESS 3 OR CMAKE_MINOR_VERSION LESS 1)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
else()
  set(CMAKE_CXX_STANDARD 11)
endif()


set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CrossGuid is a minimal, cross platform, C++ GUID library. ")
set(CPACK_PACKAGE_NAME "crossguid")
set(CPACK_PACKAGE_VENDOR "The COPASI Developers")
set(CPACK_PACKAGE_CONTACT "COPASI Developers <developers@copasi.org>")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")
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


set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "-dgb")

# Set the build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Define Install Directories
# Install stuff (default location is not some where on the system! for safty reasons)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING "Install prefix (e.g. /usr/local/)" FORCE)
endif()

include(GNUInstallDirs)
set(CROSSGUID_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(CROSSGUID_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
set(CROSSGUID_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(CROSSGUID_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(CROSSGUID_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")

set(CROSSGUID_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake")
set(CROSSGUID_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/crossguid")

add_library(crossguid STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/guid.cpp)
target_include_directories(crossguid PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if(WIN32)
    target_compile_definitions(crossguid PRIVATE GUID_WINDOWS)
elseif(APPLE)
    find_library(CFLIB CoreFoundation)
    target_link_libraries(crossguid ${CFLIB})
    target_compile_definitions(crossguid PRIVATE GUID_CFUUID)
elseif(ANDROID)
    # GUID_ANDROID is used in the headers, so make PUBLIC
    target_compile_definitions(crossguid PUBLIC GUID_ANDROID)
else()
    find_package(Libuuid REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
            "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -fPIC")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -fPIC")

    target_include_directories(crossguid PRIVATE ${LIBUUID_INCLUDE_DIR})
      target_link_libraries(crossguid ${LIBUUID_LIBRARY})
      target_compile_definitions(crossguid PRIVATE GUID_LIBUUID)
endif()

# if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#     set(WARNINGS "-Werror" "-Wall")
# elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     set(WARNINGS "-Werror" "-Wall")
# elseif(MSVC)
#     set(WARNINGS "/WX" "/W4")
# endif()
# target_compile_options(crossguid PRIVATE ${WARNINGS})

if(WIN32)
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
endif()

# Properties
# for a version number add
# VERSION ${CROSSGUID_VERSION_STRING} SOVERSION ${CROSSGUID_VERSION_STRING}
set_target_properties(crossguid
					  PROPERTIES
					  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Install target
install(TARGETS crossguid EXPORT crossguidTargets
    RUNTIME       DESTINATION ${CROSSGUID_RUNTIME_INSTALL_DIR}
    LIBRARY       DESTINATION ${CROSSGUID_LIBRARY_INSTALL_DIR}
    ARCHIVE       DESTINATION ${CROSSGUID_ARCHIVE_INSTALL_DIR}
    FRAMEWORK     DESTINATION ${CROSSGUID_FRAMEWORK_INSTALL_DIR})

# Install headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION ${CROSSGUID_INC_INSTALL_DIR}
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# Make cmake config files for all targets
install(EXPORT  crossguidTargets
    DESTINATION ${CROSSGUID_CMAKE_CONFIG_INSTALL_DIR}
    FILE crossguid-config.cmake)

# Install readme and license
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION ${CROSSGUID_ADDITIONAL_FILES_INSTALL_DIR}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

if (CROSSGUID_TESTS)
    add_executable(crossguid-test test/TestMain.cpp test/Test.cpp)
    target_link_libraries(crossguid-test crossguid)
endif()
