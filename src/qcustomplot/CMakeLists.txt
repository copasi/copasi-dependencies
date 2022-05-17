cmake_minimum_required (VERSION 2.6...3.19.0)
project (qcustomplot)

include(CheckCXXCompilerFlag)
if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()

include(GNUInstallDirs)

include_directories("${PROJECT_SOURCE_DIR}")


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
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif(WITH_STATIC_RUNTIME)
endif(MSVC OR USING_INTEL)

# compile as position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (UNIX OR CYGWIN)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()


set(SOURCES qcustomplot.cpp)
set(HEADERS qcustomplot.h)

include(${CMAKE_CURRENT_SOURCE_DIR}/ChooseQtVersion.cmake)
QT_FIND_MODULES(Core OpenGL Xml PrintSupport)

if (Qt6_FOUND)
  QT_FIND_MODULES(Widgets)
  qt6_wrap_cpp(HEADERS_MOC ${HEADERS})
  set(QTVERSION 6)
elseif (Qt5_FOUND)
  qt5_wrap_cpp(HEADERS_MOC ${HEADERS})
  set(QTVERSION 5)
elseif(Qt4_FOUND OR QT4_FOUND)
  qt4_wrap_cpp(HEADERS_MOC ${HEADERS})
endif()


add_library(qcustomplot STATIC ${SOURCES} ${HEADERS} ${HEADERS_MOC})
QT_BIND_TO_TARGET(qcustomplot)

install(FILES ${HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
INSTALL(TARGETS qcustomplot 
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
