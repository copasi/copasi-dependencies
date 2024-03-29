###############################################################################
#
# Description       : CMake build script for libCOMBINE Java bindings
# Original author(s): Frank Bergmann <fbergman@caltech.edu>
# Organization      : California Institute of Technology
#
# <!--------------------------------------------------------------------------
# This file is part of libCOMBINE.  Please visit http://sed-ml.org for more
# information about SED-ML. The latest version of libCOMBINE can be found on
# github: https://github.com/fbergmann/libCOMBINE/
#
# Copyright (c) 2013-2016, Frank T. Bergmann
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ------------------------------------------------------------------------ -->
#
###############################################################################

find_package(SWIG REQUIRED)
find_package(JNI)
find_package(Java COMPONENTS Development REQUIRED)
include(${SWIG_USE_FILE})


####################################################################
#
# create manifest file
# 
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/Manifest.txt.in"
  "${CMAKE_CURRENT_BINARY_DIR}/Manifest.txt"
  )

####################################################################
#
# ensure that javac, jar and python are available
# 

if (NOT EXISTS "${Java_JAVAC_EXECUTABLE}")
  if (NOT EXISTS "${JAVA_COMPILE}")
    message(FATAL_ERROR "The java language bindings require Java_JAVAC_EXECUTABLE to be set to the javac command.")
  else()
    SET(Java_JAVAC_EXECUTABLE "${JAVA_COMPILE}" CACHE STRING "Javac compiler" FORCE)
  endif()
endif()


if (NOT EXISTS "${Java_JAR_EXECUTABLE}")
  if (NOT EXISTS "${JAVA_ARCHIVE}")	
    message(FATAL_ERROR "The java language bindings require Java_JAR_EXECUTABLE to be set to the jar command.")
  else()
        set (Java_JAR_EXECUTABLE "${JAVA_ARCHIVE}" CACHE STRING "jar command" FORCE)
  endif()
endif()


####################################################################
#
# determine local dependencies, so as to re-swig if one of them changed
# 

file(GLOB SWIG_DEPENDENCIES 
  ${CMAKE_CURRENT_SOURCE_DIR}/*.i 
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../swig/*.i
  ${CMAKE_CURRENT_SOURCE_DIR}/../swig/*.h
  )


# 
# Determine the installation directory
#
set(JAVA_PACKAGE_BIN_INSTALL_DIR)
set(JAVA_PACKAGE_JAR_INSTALL_DIR)
if (UNIX OR CYGWIN) 
  set(JAVA_PACKAGE_BIN_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
  set(JAVA_PACKAGE_JAR_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/java)  
else()
  set(JAVA_PACKAGE_BIN_INSTALL_DIR ${MISC_PREFIX}bindings/java)
  set(JAVA_PACKAGE_JAR_INSTALL_DIR ${MISC_PREFIX}bindings/java)  
endif()

#
# Remove SWIG wrappers if requested
#
if (LIBCOMBINE_REMOVE_WRAPPERS)
  foreach(file 
    ${CMAKE_CURRENT_BINARY_DIR}/libcombine_wrap.cpp 
  )
    if (EXISTS ${file})
      FILE(REMOVE ${file})
    endif()
  endforeach()
endif(LIBCOMBINE_REMOVE_WRAPPERS)

set(SWIG_EXTRA_FLAGS -DSWIGEXPORT -DLIBSBML_CPP_NAMESPACE_BEGIN= -DLIBSBML_CPP_NAMESPACE_END= -DLIBSBML_CPP_NAMESPACE_QUALIFIER= -DLIBSBML_CPP_NAMESPACE_USE=)
if(NOT UNIX)
  set(SWIG_EXTRA_FLAGS ${SWIG_EXTRA_FLAGS} -DSWIGWIN -DSWIG_CSHARP_NO_WSTRING_HELPER )
endif()


####################################################################
#
# Swig bindings
#
#
ADD_CUSTOM_COMMAND(	
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libcombine_wrap.cpp 
         ${CMAKE_CURRENT_BINARY_DIR}/java-files/org/sbml/libcombine/libcombineJNI.java
  COMMAND "${CMAKE_COMMAND}"
  ARGS    -E make_directory  ${CMAKE_CURRENT_BINARY_DIR}/java-files/org/sbml/libcombine
    COMMAND "${SWIG_EXECUTABLE}"
    ARGS -I${CMAKE_CURRENT_SOURCE_DIR}/../swig/
         -I${CMAKE_CURRENT_SOURCE_DIR}/../../
         -I${CMAKE_CURRENT_BINARY_DIR}/../../
         -I${CMAKE_CURRENT_SOURCE_DIR}/../../omex
         -I${CMAKE_CURRENT_SOURCE_DIR}
         -I${LIBNUML_INCLUDE_DIR}/
         -I${LIBSBML_INCLUDE_DIR}/
     -c++
     -java
     -package org.sbml.libcombine
     ${SWIG_EXTRA_ARGS}
     ${SWIG_EXTRA_FLAGS}
     -outdir ${CMAKE_CURRENT_BINARY_DIR}/java-files/org/sbml/libcombine
         -o ${CMAKE_CURRENT_BINARY_DIR}/libcombine_wrap.cpp 
         ${CMAKE_CURRENT_SOURCE_DIR}/../swig/libcombine.i
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/../swig/libcombine.i
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
          ${SWIG_DEPENDENCIES}
    COMMENT "Swig Java source") 
  
add_custom_target(binding_java_swig ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libcombine_wrap.cpp)

  
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../swig)
include_directories(${LIBSBML_INCLUDE_DIR})
include_directories(BEFORE ${LIBNUML_INCLUDE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/../../)
add_definitions(-DLIBCOMBINE_STATIC)
if (EXTRA_INCLUDE_DIRS) 
 include_directories(${EXTRA_INCLUDE_DIRS})
endif(EXTRA_INCLUDE_DIRS)

# if additional directories are set be sure to include them
if (JNI_INCLUDE_DIRS)
include_directories(${JNI_INCLUDE_DIRS})
endif(JNI_INCLUDE_DIRS)
if (JAVA_INCLUDE_PATH)
include_directories(${JAVA_INCLUDE_PATH})
endif(JAVA_INCLUDE_PATH)
if (JAVA_INCLUDE_PATH2)
include_directories(${JAVA_INCLUDE_PATH2})
endif(JAVA_INCLUDE_PATH2)


add_library(binding_java_lib SHARED ${CMAKE_CURRENT_BINARY_DIR}/libcombine_wrap.cpp )
add_dependencies( binding_java_lib binding_java_swig) 
set_target_properties (binding_java_lib PROPERTIES OUTPUT_NAME "combinej")


if(APPLE)
  set_target_properties (binding_java_lib PROPERTIES SUFFIX ".jnilib")
endif()

install(TARGETS binding_java_lib DESTINATION ${JAVA_PACKAGE_BIN_INSTALL_DIR} )

target_link_libraries(binding_java_lib ${LIBCOMBINE_LIBRARY}-static )

ADD_CUSTOM_COMMAND(	
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libcombine.jar
  COMMAND "${CMAKE_COMMAND}"
  ARGS    -E make_directory  ${CMAKE_CURRENT_BINARY_DIR}/test
  COMMAND "${CMAKE_COMMAND}"
  ARGS -DJava_JAVAC_EXECUTABLE=\"${Java_JAVAC_EXECUTABLE}\"
     -DJava_JAR_EXECUTABLE=\"${Java_JAR_EXECUTABLE}\"
     -DBIN_DIRECTORY=\"${CMAKE_CURRENT_BINARY_DIR}\"
     -DSRC_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}\"
     -DFILE_SEP=\"${FILE_SEP}\"
     -DPATH_SEP=${PATH_SEP}
     -P "${CMAKE_CURRENT_SOURCE_DIR}/compile-native-files.cmake"
  
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Building: libcombine.jar"
  ) 

add_custom_target(binding_java_jar ALL DEPENDS 
  ${CMAKE_CURRENT_BINARY_DIR}/libcombine.jar
  ${CMAKE_CURRENT_BINARY_DIR}/java-files/org/sbml/libcombine/libcombineJNI.java
  )
  
add_dependencies(binding_java_jar binding_java_swig)
  
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcombine.jar DESTINATION ${JAVA_PACKAGE_JAR_INSTALL_DIR}) 
  

