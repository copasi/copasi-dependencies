cmake_minimum_required (VERSION 3.10)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# update submodules as needed
  option(GIT_SUBMODULE "Check submodules during build" ON)
  if(GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
  endif()
endif()

# if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/jansson/CMakeLists.txt" OR
#    NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libpqxx/CMakeLists.txt" OR
#    NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/spdlog/CMakeLists.txt")
#   message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
# endif()

set (CLEAN_TARGETS)
set (ARCHIVE_DEPENDENCIES)
set (ZIPPER_DEPENDENCIES)

include(ExternalProject)


if ((UNIX AND NOT APPLE) OR EMSCRIPTEN)
  option(BUILD_uuid "Build uuid  (default off)" OFF)

  if (BUILD_uuid AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libuuid-1.0.3)
  # we just use the normal gnumake build for uuid, so look for 
  # gnumake or make
  find_program(GNUMAKE NAMES gmake make)

  message(STATUS "adding project: uuid")

  ExternalProject_Add(Libuuid

    PREFIX            ${CMAKE_BINARY_DIR}/libuuid
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/libuuid-1.0.3
  
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env 
                      CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER}  
                      CFLAGS=-fPIC
                      ${CMAKE_CURRENT_SOURCE_DIR}/libuuid-1.0.3/configure 
                      --prefix=${CMAKE_INSTALL_PREFIX} 
                      --disable-dependency-tracking 
                      --with-pic 
                      --enable-static=yes 
                      --enable-shared=no 
                      
    BUILD_COMMAND      ${GNUMAKE}
    INSTALL_COMMAND    ${GNUMAKE} install
  )

  endif()
endif()

if (BUILD_expat)

  message(STATUS "adding project: expat")
  ExternalProject_Add(expat
    PREFIX            ${CMAKE_BINARY_DIR}/expat
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/libexpat/expat
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DEXPAT_SHARED_LIBS=OFF 
      -DEXPAT_BUILD_EXAMPLES=OFF 
      -DEXPAT_BUILD_TESTS=OFF 
      -DEXPAT_BUILD_TOOLS=OFF
      -DCMAKE_DEBUG_POSTFIX=
      -DCMAKE_RELEASE_POSTFIX=
      -DEXPAT_MSVC_STATIC_CRT=${WITH_STATIC_RUNTIME} 
      ${CMAKE_CURRENT_SOURCE_DIR}/libexpat/expat
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_expat ${CMAKE_BINARY_DIR}/expat/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_expat})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} expat)
endif (BUILD_expat)

if (BUILD_raptor)
  message(STATUS "adding project: raptor")
  ExternalProject_Add(raptor
    PREFIX            ${CMAKE_BINARY_DIR}/raptor
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/raptor
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DEXPAT_INCLUDE_DIR=${CMAKE_INSTALL_PREFIX}/include
      -DBUILD_shared=OFF
  	  ${CMAKE_CURRENT_SOURCE_DIR}/raptor
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
    DEPENDS            expat
  )

  file(GLOB CLEAN_TARGETS_raptor ${CMAKE_BINARY_DIR}/raptor/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_raptor})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} raptor)
endif (BUILD_raptor)

if (BUILD_crossguid)
  message(STATUS "adding project: crossguid")
  ExternalProject_Add(crossguid
    PREFIX            ${CMAKE_BINARY_DIR}/crossguid
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/crossguid
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DBUILD_shared=OFF
  	  ${CMAKE_CURRENT_SOURCE_DIR}/crossguid
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_crossguid ${CMAKE_BINARY_DIR}/crossguid/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_crossguid})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} crossguid)
endif (BUILD_crossguid)

if (BUILD_clapack)
  message(STATUS "adding project: clapack")
  ExternalProject_Add(clapack
    PREFIX            ${CMAKE_BINARY_DIR}/clapack
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/clapack
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DBUILD_TESTING=OFF
  	  ${CMAKE_CURRENT_SOURCE_DIR}/clapack
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_clapack ${CMAKE_BINARY_DIR}/clapack/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_clapack})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} clapack)
endif (BUILD_clapack)

if (BUILD_SBW)
  message(STATUS "adding project: SBW")
  ExternalProject_Add(SBW
    PREFIX            ${CMAKE_BINARY_DIR}/SBW
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/core
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DWITH_BUILD_BROKER=OFF
      -DWITH_BUILD_SHARED=OFF
  	  ${CMAKE_CURRENT_SOURCE_DIR}/core
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_SBW ${CMAKE_BINARY_DIR}/SBW/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_SBW})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} SBW)
endif (BUILD_SBW)

if (BUILD_libSBML)
  message(STATUS "adding project: libSBML")
  ExternalProject_Add(libSBML
    PREFIX            ${CMAKE_BINARY_DIR}/libSBML
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/libSBML
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DENABLE_LAYOUT=ON
      -DENABLE_REQUIREDELEMENTS=OFF
      -DENABLE_RENDER=ON
      -DENABLE_COMP=ON
      -DENABLE_FBC=OFF
      -DENABLE_SPATIAL=OFF
      -DENABLE_GROUPS=OFF
      -DWITH_EXPAT=ON
      -DWITH_LIBXML=OFF
      -DLIBSBML_USE_LEGACY_MATH=ON
      -DLIBSBML_DEPENDENCY_DIR=${INSTALL_DIR}
      -DLIBSBML_SKIP_SHARED_LIBRARY=ON
      -DWITH_BZIP2=OFF
      -DWITH_ZLIB=OFF
      -DLIBZ_LIBRARY=
      -DEXPAT_INCLUDE_DIR=${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libSBML
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
    DEPENDS            expat
  )

  file(GLOB CLEAN_TARGETS_libSBML ${CMAKE_BINARY_DIR}/libSBML/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_libSBML})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} libSBML)
endif (BUILD_libSBML)

if (BUILD_libnuml)
  message(STATUS "adding project: libnuml")
  ExternalProject_Add(libnuml
    PREFIX            ${CMAKE_BINARY_DIR}/libnuml
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/libnuml
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DLIBSBML_STATIC=ON
      -DLIBNUML_SHARED_VERSION=OFF
      -DLIBNUML_SKIP_SHARED_LIBRARY=ON
      -DWITH_ZLIB=OFF
      -DLIBSBML_INCLUDE_DIR=${CMAKE_INSTALL_PREFIX}/include
      -DLIBNUML_DEPENDENCY_DIR=${CMAKE_INSTALL_PREFIX}
    ${CMAKE_CURRENT_SOURCE_DIR}/libnuml
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
    DEPENDS            libSBML expat
  )

  file(GLOB CLEAN_TARGETS_libnuml ${CMAKE_BINARY_DIR}/libnuml/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_libnuml})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} libnuml)
endif (BUILD_libnuml)

if (BUILD_libSEDML)
  message(STATUS "adding project: libSEDML")
  ExternalProject_Add(libSEDML
    PREFIX            ${CMAKE_BINARY_DIR}/libSEDML
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/libSEDML
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DLIBSBML_STATIC=ON
      -DLIBSEDML_SHARED_VERSION=OFF
      -DLIBSEDML_SKIP_SHARED_LIBRARY=ON
      -DWITH_ZLIB=OFF
      -DLIBZ_LIBRARY=
      -DLIBSBML_INCLUDE_DIR=${CMAKE_INSTALL_PREFIX}/include
      -DLIBSEDML_DEPENDENCY_DIR=${CMAKE_INSTALL_PREFIX}
    ${CMAKE_CURRENT_SOURCE_DIR}/libSEDML
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
    DEPENDS            libnuml libSBML expat
  )

  file(GLOB CLEAN_TARGETS_libSEDML ${CMAKE_BINARY_DIR}/libSEDML/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_libSEDML})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} libSEDML)
endif (BUILD_libSEDML)

if (BUILD_zlib)
  message(STATUS "adding project: zlib")
  ExternalProject_Add(zlib
    PREFIX            ${CMAKE_BINARY_DIR}/zlib
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/zlib
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
  	  ${CMAKE_CURRENT_SOURCE_DIR}/zlib
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_zlib ${CMAKE_BINARY_DIR}/zlib/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_zlib})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} zlib)
  set (ZIPPER_DEPENDENCIES ${ZIPPER_DEPENDENCIES} zlib)
endif (BUILD_zlib)

if (BUILD_zipper)
  message(STATUS "adding project: zipper")
  ExternalProject_Add(zipper
    PREFIX            ${CMAKE_BINARY_DIR}/zipper
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/zipper
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DZIPPER_DEPENDENCY_DIR=${CMAKE_INSTALL_PREFIX}
      -DBUILD_TEST=OFF
  	  ${CMAKE_CURRENT_SOURCE_DIR}/zipper
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
    DEPENDS            ${ZIPPER_DEPENDENCIES}
  )

  file(GLOB CLEAN_TARGETS_zipper ${CMAKE_BINARY_DIR}/zipper/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_zipper})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} zipper)
endif (BUILD_zipper)

if (BUILD_libCombine)
  message(STATUS "adding project: libCombine")
  ExternalProject_Add(libCombine
    PREFIX            ${CMAKE_BINARY_DIR}/libCombine
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/libCombine
  
    CONFIGURE_COMMAND ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DLIBCOMBINE_SKIP_SHARED_LIBRARY=ON
      -DLIBCOMBINE_DEPENDENCY_DIR=${CMAKE_INSTALL_PREFIX}
      -DBUILD_TEST=OFF
      -DEXPAT_INCLUDE_DIR=${CMAKE_INSTALL_PREFIX}/include
  	  ${CMAKE_CURRENT_SOURCE_DIR}/libCombine
  	
    BUILD_COMMAND     ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND   ${CMAKE_MAKE_PROGRAM} install
    DEPENDS           zipper libSBML expat
  )

  file(GLOB CLEAN_TARGETS_libCombine ${CMAKE_BINARY_DIR}/libCombine/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_libCombine})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} libCombine)
endif (BUILD_libCombine)

if (BUILD_mml)
  message(STATUS "adding project: mml")
  ExternalProject_Add(mml
    PREFIX            ${CMAKE_BINARY_DIR}/mml
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/mml
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      ${COMMON_CMAKE_OPTIONS_UI}
  	  ${CMAKE_CURRENT_SOURCE_DIR}/mml
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_mml ${CMAKE_BINARY_DIR}/mml/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_mml})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} mml)
endif (BUILD_mml)

if (BUILD_qwt)
  message(STATUS "adding project: qwt")
  ExternalProject_Add(qwt
    PREFIX            ${CMAKE_BINARY_DIR}/qwt
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/qwt
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      ${COMMON_CMAKE_OPTIONS_UI}
  	  ${CMAKE_CURRENT_SOURCE_DIR}/qwt
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_qwt ${CMAKE_BINARY_DIR}/qwt/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_qwt})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} qwt)
endif (BUILD_qwt)

if (BUILD_qwt6)
  message(STATUS "adding project: qwt-6")
  ExternalProject_Add(qwt-6
    PREFIX            ${CMAKE_BINARY_DIR}/qwt-6
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/qwt-6
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      ${COMMON_CMAKE_OPTIONS_UI}
  	  ${CMAKE_CURRENT_SOURCE_DIR}/qwt-6
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_qwt-6 ${CMAKE_BINARY_DIR}/qwt-6/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_qwt-6})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} qwt-6)
endif (BUILD_qwt6)

if (BUILD_qwtplot3d)
  message(STATUS "adding project: qwtplot3d")
  ExternalProject_Add(qwtplot3d
    PREFIX            ${CMAKE_BINARY_DIR}/qwtplot3d
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/qwtplot3d-qt4
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      ${COMMON_CMAKE_OPTIONS_UI}
  	  ${CMAKE_CURRENT_SOURCE_DIR}/qwtplot3d-qt4
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_qwtplot3d ${CMAKE_BINARY_DIR}/qwtplot3d/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_qwtplot3d})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} qwtplot3d)
endif (BUILD_qwtplot3d)

if (BUILD_NativeJIT)
  message(STATUS "adding project: NativeJIT")
  ExternalProject_Add(NativeJIT
    PREFIX            ${CMAKE_BINARY_DIR}/NativeJIT
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/NativeJIT

    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DDISABLE_TESTING=ON
      ${CMAKE_CURRENT_SOURCE_DIR}/NativeJIT
    
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_NativeJIT ${CMAKE_BINARY_DIR}/NativeJIT/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_NativeJIT})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} NativeJIT)
endif (BUILD_NativeJIT)

if (BUILD_cpu_features)
  message(STATUS "adding project: cpu_features")
  ExternalProject_Add(cpu_features
    PREFIX            ${CMAKE_BINARY_DIR}/cpu_features
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/cpu_features

    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      -DBUILD_PIC=ON
      ${CMAKE_CURRENT_SOURCE_DIR}/cpu_features
    
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_cpu_features ${CMAKE_BINARY_DIR}/cpu_features/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_cpu_features})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} cpu_features)
endif (BUILD_cpu_features)

if (BUILD_qcustomplot)
  message(STATUS "adding project: qcustomplot")
  ExternalProject_Add(qcustomplot
    PREFIX            ${CMAKE_BINARY_DIR}/qcustomplot
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot
  
    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      ${COMMON_CMAKE_OPTIONS_UI}
  	  ${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot
  	
    BUILD_COMMAND      ${CMAKE_MAKE_PROGRAM} ${BUILD_OPTIONS}
    INSTALL_COMMAND    ${CMAKE_MAKE_PROGRAM} install
  )

  file(GLOB CLEAN_TARGETS_qcustomplot ${CMAKE_BINARY_DIR}/qcustomplot/*)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_qcustomplot})
  set (ARCHIVE_DEPENDENCIES ${ARCHIVE_DEPENDENCIES} qcustomplot)
endif (BUILD_qcustomplot)


if (BUILD_archive)
  message(STATUS "adding project: archive")
  if (NOT EXISTS ${CMAKE_INSTALL_PREFIX}/readme.md)
    # the SOURCE_DIR is not allowed to be empty 
    # starting cmake 3.17
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../readme.md DESTINATION ${CMAKE_INSTALL_PREFIX})
  endif()
  ExternalProject_Add(archive
    PREFIX             ${CMAKE_BINARY_DIR}/archive
    SOURCE_DIR         ${CMAKE_INSTALL_PREFIX}

    CONFIGURE_COMMAND  ${CMAKE_COMMAND} 
      ${COMMON_CMAKE_OPTIONS}
      ${CMAKE_CURRENT_SOURCE_DIR}/archive

    INSTALL_COMMAND      ${CMAKE_MAKE_PROGRAM} package
      
    DEPENDS            ${ARCHIVE_DEPENDENCIES}
  )

  file(GLOB CLEAN_TARGETS_archive ${CMAKE_BINARY_DIR}/copasi-dependencies-*.zip)
  set (CLEAN_TARGETS ${CLEAN_TARGETS} ${CLEAN_TARGETS_archive})
endif (BUILD_archive)

set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES "${CLEAN_TARGETS}")
