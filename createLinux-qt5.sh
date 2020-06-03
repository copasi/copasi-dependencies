#!/usr/bin/env bash
#

DIRECTORY=$(cd `dirname $0` && pwd)

# Build Directory
BUILD_DIR=${BUILD_DIR:=${DIRECTORY}/tmp}
[ -d "${BUILD_DIR}" ] || mkdir -p "${BUILD_DIR}"

CC=${CC:=cc}
TARGET="${COPASI_TARGET:-$($CC -v 2>&1 | awk -- '$1 ~ "Target:" {print $2}')}"

case ${TARGET} in
  x86_64-*)
    LIB_DIR="lib64"
    ;;

  *)
    LIB_DIR="lib"
    ;;
esac

# Install Directory
INSTALL_DIR=${INSTALL_DIR:=${DIRECTORY}/bin}
[ -d ${INSTALL_DIR} ] || mkdir -p ${INSTALL_DIR}
[ -d ${INSTALL_DIR}/include ] || mkdir ${INSTALL_DIR}/include
[ -d ${INSTALL_DIR}/${LIB_DIR} ] || mkdir ${INSTALL_DIR}/${LIB_DIR}

if [ $# = 0 ]; then
  ToBeBuild="expat raptor crossguid clapack SBW libSBML libnuml libSEDML zlib zipper libCombine MML qwt-6 qwtplot3d"
elif [ _${1} = _--rebuild -a -e "${BUILD_DIR}/.packages" ]; then
  . "${BUILD_DIR}/.packages"
else
  while [ _${1} != _ ]; do
    ToBeBuild="$ToBeBuild ${1}"
    shift
  done;
fi

echo ToBeBuild=\"${ToBeBuild}\" > "${BUILD_DIR}/.packages"

#Default Values:
BUILD_TYPE=${BUILD_TYPE:="Release"}
SELECT_QT=${SELECT_QT:="Qt5"}
CMAKE=${CMAKE:="cmake"}

MAKE=${MAKE:="gmake"}
command -v $MAKE >/dev/null 2>&1 || { MAKE=make; }

# echo ${BUILD_TYPE}
# echo ${CMAKE}
# echo ${MAKE}

COPASI_COMMON_CMAKE_OPTIONS="${COPASI_COMMON_CMAKE_OPTIONS} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"
COPASI_COMMON_CMAKE_OPTIONS="${COPASI_COMMON_CMAKE_OPTIONS} -DEXTRA_INCLUDE_DIRS=${INSTALL_DIR}/include"

COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DSELECT_QT=${SELECT_QT}"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} ${COPASI_COMMON_CMAKE_OPTIONS}"

COPASI_CXXFLAGS="${CXXFLAGS} ${COPASI_CXXFLAGS} -I${INSTALL_DIR}/include"
COPASI_CFLAGS="${CFLAGS} ${COPASI_CFLAGS} -I${INSTALL_DIR}/include"
COPASI_LDFLAGS="${LDFLAGS} ${COPASI_LDFLAGS} -L${INSTALL_DIR}/lib"

function build () {
case $1 in
  cppunit)
    # Build cppunit
    cd $DIRECTORY/src/cppunit
    chmod +x configure
    CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" ./configure \
        --enable-html-docs=no \
        --with-pic \
        --enable-doxygen=no \
        --enable-dot=no \
        --enable-shared=no \
        --prefix=${INSTALL_DIR}
    CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" $MAKE -j 4 
    CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" $MAKE install
    ;;

  expat)
    # build expat
    [ -e ${BUILD_DIR}/expat ] || mkdir -p ${BUILD_DIR}/expat
    cd ${BUILD_DIR}/expat
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        $DIRECTORY/src/expat
    $MAKE -j 4
    $MAKE install
    ;;

  raptor)
    # build raptor
    [ -e ${BUILD_DIR}/raptor ] || mkdir -p ${BUILD_DIR}/raptor
    cd ${BUILD_DIR}/raptor
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        -DEXPAT_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DEXPAT_LIBRARY=${INSTALL_DIR}/${LIB_DIR}/libexpat.a \
        $DIRECTORY/src/raptor
    $MAKE -j 4
    $MAKE install
    ;;

  crossguid)
    # build crossguid
    [ -e ${BUILD_DIR}/crossguid ] || mkdir -p ${BUILD_DIR}/crossguid
    cd ${BUILD_DIR}/crossguid
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        $DIRECTORY/src/crossguid
    $MAKE -j 4
    $MAKE install
    ;;

  clapack)
    # Build Clapack
    [ -e ${BUILD_DIR}/clapack ] || mkdir -p ${BUILD_DIR}/clapack
    cd ${BUILD_DIR}/clapack
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_TESTING=OFF \
        $DIRECTORY/src/clapack
    $MAKE -j 4
    $MAKE install
    ;;

  MML)
    #build MML
    [ -e ${BUILD_DIR}/MML ] || mkdir -p ${BUILD_DIR}/MML 
    cd ${BUILD_DIR}/MML
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
           $DIRECTORY/src/mml
    $MAKE -j 4
    $MAKE install
    ;;

  qwt)
    if [ _${SELECT_QT} == _Qt5 ]; then
      QWT=qwt-6
    else
      QWT=qwt
    fi
    
    #build qwt 
    [ -e ${BUILD_DIR}/${QWT} ] || mkdir "${BUILD_DIR}/${QWT}"
    cd "${BUILD_DIR}/${QWT}"
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        "$DIRECTORY/src/${QWT}"
    make -j 4
    make install
    ;;

  qwt-6)
    #build qwt 
    [ -e ${BUILD_DIR}/qwt-6 ] || mkdir ${BUILD_DIR}/qwt-6 
    cd ${BUILD_DIR}/qwt-6
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/qwt-6
    make -j 4
    make install
    ;;

  qwtplot3d)
    #build qwtplot3d 
    [ -e ${BUILD_DIR}/qwtplot3d ] || mkdir ${BUILD_DIR}/qwtplot3d
    cd ${BUILD_DIR}/qwtplot3d 
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/qwtplot3d-qt4
    make -j 4
    make install
    ;;

  SBW)
    #Build SBW
    [ -e ${BUILD_DIR}/SBW ] || mkdir -p ${BUILD_DIR}/SBW
    cd ${BUILD_DIR}/SBW
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DWITH_BUILD_BROKER=OFF \
        -DWITH_BUILD_SHARED=OFF \
        $DIRECTORY/src/core
    $MAKE -j 4
    $MAKE install
    ;;

  libSBML)
    # build libSBML
    [ -e ${BUILD_DIR}/libSBML ] || mkdir -p ${BUILD_DIR}/libSBML
    cd ${BUILD_DIR}/libSBML
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
        -DLIBEXPAT_LIBRARY=${INSTALL_DIR}/${LIB_DIR}/libexpat.a \
        $DIRECTORY/src/libSBML
    $MAKE -j 4
    $MAKE install
    ;;

  libnuml)
    # build libnuml
    [ -e ${BUILD_DIR}/libnuml ] || mkdir -p ${BUILD_DIR}/libnuml
    cd ${BUILD_DIR}/libnuml
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DLIBSBML_STATIC=ON \
        -DLIBNUML_SHARED_VERSION=OFF \
        -DLIBNUML_SKIP_SHARED_LIBRARY=ON \
        -DLIBSBML_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DLIBSBML_LIBRARY=${INSTALL_DIR}/${LIB_DIR}/libsbml-static.a \
        -DLIBNUML_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DEXTRA_LIBS=${INSTALL_DIR}/${LIB_DIR}/libexpat.a \
        -DWITH_ZLIB=OFF \
        $DIRECTORY/src/libnuml
    $MAKE -j 4
    $MAKE install
    [ -e ${INSTALL_DIR}/${LIB_DIR}/libnuml*.so ] && rm ${INSTALL_DIR}/${LIB_DIR}/libnuml*.so
    ;;

  libSEDML)
    # build libSEDML
    [ -e ${BUILD_DIR}/libSEDML ] || mkdir -p ${BUILD_DIR}/libSEDML
    cd ${BUILD_DIR}/libSEDML
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DLIBSBML_STATIC=ON \
        -DLIBSEDML_SHARED_VERSION=OFF \
        -DLIBSEDML_SKIP_SHARED_LIBRARY=ON \
        -DLIBSBML_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DLIBSBML_LIBRARY=${INSTALL_DIR}/${LIB_DIR}/libsbml-static.a \
        -DLIBSEDML_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DEXTRA_LIBS=${INSTALL_DIR}/${LIB_DIR}/libexpat.a \
        -DWITH_ZLIB=OFF \
        $DIRECTORY/src/libSEDML
    $MAKE -j 4
    $MAKE install
    [ -e ${INSTALL_DIR}/${LIB_DIR}/libsedml*.so ] && rm ${INSTALL_DIR}/${LIB_DIR}/libsedml*.so
    ;;

  zlib)
    # build zlib
    [ -e ${BUILD_DIR}/zlib ] || mkdir -p ${BUILD_DIR}/zlib
    cd ${BUILD_DIR}/zlib
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/zlib
    $MAKE -j 4
    $MAKE install
    ;;

  zipper)
    # build zipper
    [ -e ${BUILD_DIR}/zipper ] || mkdir -p ${BUILD_DIR}/zipper
    cd ${BUILD_DIR}/zipper
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
       -DWITH_QT_FILESYSTEM=ON \
       -DZIPPER_DEPENDENCY_DIR=${INSTALL_DIR} \
        $DIRECTORY/src/zipper
    make -j 4
    make install
    ;;

  libCombine)
    # build libCombine
    [ -e ${BUILD_DIR}/libCombine ] || mkdir -p ${BUILD_DIR}/libCombine
    cd ${BUILD_DIR}/libCombine
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DCOMBINE_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DEXTRA_LIBS=${INSTALL_DIR}/${LIB_DIR}/libexpat.a \
        -DLIBCOMBINE_SKIP_SHARED_LIBRARY=ON \
        -DEXPAT_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DEXPAT_LIBRARY=${INSTALL_DIR}/${LIB_DIR}/libexpat.a \
        $DIRECTORY/src/libCombine
    $MAKE -j 4
    $MAKE install
    ;;
esac
}

for b in ${ToBeBuild}; do
  build $b
done

cd ${INSTALL_DIR}
