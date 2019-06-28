#!/bin/bash
#

DIRECTORY=$(cd `dirname $0` && pwd)

if [ $# = 0 ]; then
  ToBeBuild="expat raptor clapack SBW libSBML libnuml libSEDML zlib libCombine MML qwt qwtplot3d"
else
  while [ _${1} != _ ]; do
    ToBeBuild="$ToBeBuild ${1}"
    shift
  done;
fi
 
#Default Values:
BUILD_TYPE=${BUILD_TYPE:="Release"}
SELECT_QT=${SELECT_QT:="Qt5"}
CMAKE=${CMAKE:="cmake"}

MAKE=${MAKE:="gmake"}
command -v $MAKE >/dev/null 2>&1 || { MAKE=make; }

# Build Directory
BUILD_DIR=${BUILD_DIR:=${DIRECTORY}/tmp}
[ -d "${BUILD_DIR}" ] || mkdir -p "${BUILD_DIR}"

# Install Directory
INSTALL_DIR=${INSTALL_DIR:=${DIRECTORY}/bin}
[ -d ${INSTALL_DIR} ] || mkdir -p ${INSTALL_DIR}
[ -d ${INSTALL_DIR}/include ] || mkdir ${INSTALL_DIR}/include
[ -d ${INSTALL_DIR}/lib ] || mkdir ${INSTALL_DIR}/lib

COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=${BUILD_TYPE}"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DEXTRA_INCLUDE_DIRS=${INSTALL_DIR}/include"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DENABLE_UNIVERSAL=OFF"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCMAKE_OSX_ARCHITECTURES=x86_64"

#use the one below to link against libstd++ instead of libc++
#COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCLANG_USE_STDLIB=ON"

COPASI_CXXFLAGS="${COPASI_CXXFLAGS} -gdwarf-2 -O2 -I${INSTALL_DIR}/include"
COPASI_CFLAGS="${COPASI_CFLAGS} -gdwarf-2 -O2 -I${INSTALL_DIR}/include"
COPASI_LDFLAGS="${COPASI_LDFLAGS} -L${INSTALL_DIR}/lib"

function build () {
case $1 in
  cppunit)
    # Build cppunit
    cd $DIRECTORY/src/cppunit
    chmod +x configure
    CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" ./configure \
      --disable-dependency-tracking \
      --enable-html-docs=no \
      --enable-doxygen=no \
      --enable-dot=no \
      --enable-shared=no \
      --prefix=${INSTALL_DIR}
    CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" make 
    CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" make install
    ;;

  expat)
# build expat
    mkdir -p ${BUILD_DIR}/expat
    cd ${BUILD_DIR}/expat
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        $DIRECTORY/src/expat
    $MAKE -j 4
    $MAKE install
    # delete shared library just in case
    [ -e ${INSTALL_DIR}/lib/libexpat*dylib ] && rm ${INSTALL_DIR}/lib/libexpat*dylib
    ;;

  raptor)
    # build raptor
    mkdir -p ${BUILD_DIR}/raptor
    cd ${BUILD_DIR}/raptor
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        -DEXPAT_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DEXPAT_LIBRARY=${INSTALL_DIR}/lib/libexpat.a \
        $DIRECTORY/src/raptor
    $MAKE -j 4
    $MAKE install
    ;;

  MML)
    #build MML
    mkdir ${BUILD_DIR}/mml 
    cd ${BUILD_DIR}/mml 
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
      $DIRECTORY/src/mml
    $MAKE -j 4
    $MAKE install
    ;;

  qwt)
    #build qwt 
    mkdir ${BUILD_DIR}/qwt 
    cd ${BUILD_DIR}/qwt
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
      $DIRECTORY/src/qwt-6
    $MAKE -j 4
    $MAKE install
    ;;

  qwtplot3d)
    #build qwtplot3d 
    mkdir ${BUILD_DIR}/qwtplot3d-qt4
    cd ${BUILD_DIR}/qwtplot3d-qt4 
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
      $DIRECTORY/src/qwtplot3d-qt4
    $MAKE -j 4
    $MAKE install
    ;;

  SBW)
    #Build SBW
    mkdir ${BUILD_DIR}/SBW
    cd ${BUILD_DIR}/SBW
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
      -DWITH_BUILD_BROKER=OFF \
      -DWITH_BUILD_SHARED=OFF \
      $DIRECTORY/src/core
    $MAKE -j 4
    $MAKE install
    ;;

  libSBML)
    # build libsbml
    mkdir ${BUILD_DIR}/libsbml
    cd ${BUILD_DIR}/libsbml
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DENABLE_LAYOUT=ON \
        -DENABLE_REQUIREDELEMENTS=OFF \
        -DENABLE_RENDER=ON \
        -DENABLE_COMP=ON \
        -DENABLE_FBC=OFF \
        -DENABLE_SPATIAL=OFF \
        -DENABLE_GROUPS=OFF \
        -DWITH_EXPAT=ON \
        -DWITH_LIBXML=OFF \
        -DLIBSBML_USE_LEGACY_MATH=ON \
        -DLIBSBML_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DLIBSBML_SKIP_SHARED_LIBRARY=ON \
        -DWITH_BZIP2=OFF \
        -DWITH_ZLIB=OFF \
        -DLIBEXPAT_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DLIBEXPAT_LIBRARY=${INSTALL_DIR}/lib/libexpat.a \
        $DIRECTORY/src/libSBML
    $MAKE -j 4
    $MAKE install
    ;;

  libnuml)
    # build libnuml
    mkdir -p ${BUILD_DIR}/libnuml
    cd ${BUILD_DIR}/libnuml
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
      -DLIBSBML_STATIC=ON \
      -DLIBNUML_SHARED_VERSION=OFF \
      -DLIBNUML_SKIP_SHARED_LIBRARY=ON \
      -DLIBNUML_DEPENDENCY_DIR=${INSTALL_DIR} \
      -DLIBSBML_INCLUDE_DIR=${INSTALL_DIR}/include \
      -DLIBSBML_LIBRARY=${INSTALL_DIR}/lib/libsbml-static.a \
      -DEXTRA_LIBS=${INSTALL_DIR}/lib/libexpat.a \
      -DWITH_ZLIB=OFF \
      $DIRECTORY/src/libnuml
    $MAKE -j 4
    $MAKE install
    [ -e ${INSTALL_DIR}/lib/libnuml*.dylib ] && rm ${INSTALL_DIR}/lib/libnuml*.dylib
    ;;

  libSEDML)
    # build libSEDML
    mkdir -p ${BUILD_DIR}/libSEDML
    cd ${BUILD_DIR}/libSEDML
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
      -DLIBSBML_STATIC=ON \
      -DLIBSEDML_SHARED_VERSION=OFF \
      -DLIBSEDML_SKIP_SHARED_LIBRARY=ON \
      -DLIBSEDML_DEPENDENCY_DIR=${INSTALL_DIR} \
      -DLIBSBML_INCLUDE_DIR=${INSTALL_DIR}/include \
      -DLIBSBML_LIBRARY=${INSTALL_DIR}/lib/libsbml-static.a \
      -DEXTRA_LIBS=${INSTALL_DIR}/lib/libexpat.a \
      -DWITH_ZLIB=OFF \
      $DIRECTORY/src/libSEDML
    $MAKE -j 4
    $MAKE install
    [ -e ${INSTALL_DIR}/lib/libsedml.dylib ] && rm ${INSTALL_DIR}/lib/libsedml*.dylib
    ;;

  zlib)
    # build zlib
    mkdir -p ${BUILD_DIR}/zlib
    cd ${BUILD_DIR}/zlib
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
      $DIRECTORY/src/zlib
    $MAKE -j 4
    $MAKE install
    ;;

  libCombine)
    # build zipper
    mkdir -p ${BUILD_DIR}/zipper
    cd ${BUILD_DIR}/zipper
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
       -DWITH_QT_FILESYSTEM=ON \
       -DZIPPER_DEPENDENCY_DIR=${INSTALL_DIR} \
        $DIRECTORY/src/zipper
    make -j 4
    make install

    # build libCombine
    mkdir -p ${BUILD_DIR}/libCombine
    cd ${BUILD_DIR}/libCombine
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DCOMBINE_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DEXTRA_LIBS=${INSTALL_DIR}/lib/libexpat.a \
        -DLIBCOMBINE_SKIP_SHARED_LIBRARY=ON \
        -DEXPAT_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DEXPAT_LIBRARY=${INSTALL_DIR}/lib/libexpat.a \
        $DIRECTORY/src/libCombine
    $MAKE -j 4
    $MAKE install
    [ -e ${INSTALL_DIR}/lib/libCombine.dylib ] && rm ${INSTALL_DIR}/lib/libCombine*.dylib
    ;;
esac
}

for b in ${ToBeBuild}; do
  build $b
done

echo ${COPASI_CMAKE_OPTIONS}
