#!/usr/bin/env bash
#

DIRECTORY=$(cd `dirname $0` && pwd)

# Build Directory
BUILD_DIR=${BUILD_DIR:=${DIRECTORY}/tmp}
[ -d "${BUILD_DIR}" ] || mkdir -p "${BUILD_DIR}"

# Install Directory
INSTALL_DIR=${INSTALL_DIR:=${DIRECTORY}/bin}
[ -d ${INSTALL_DIR} ] || mkdir -p ${INSTALL_DIR}
[ -d ${INSTALL_DIR}/include ] || mkdir ${INSTALL_DIR}/include
[ -d ${INSTALL_DIR}/lib ] || mkdir ${INSTALL_DIR}/lib

if [ $# = 0 ]; then
  ToBeBuild="expat raptor clapack SBW libSBML libnuml libSEDML zlib libCombine MML qwt qwt-6 qwtplot3d"
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
SELECT_QT=${SELECT_QT:="Any"}
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
    mkdir -p ${BUILD_DIR}/expat
    cd ${BUILD_DIR}/expat
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        $DIRECTORY/src/expat
    $MAKE -j 4
    $MAKE install
    ;;

  raptor)
    # build raptor
    mkdir -p ${BUILD_DIR}/raptor
    cd ${BUILD_DIR}/raptor
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        $DIRECTORY/src/raptor
    $MAKE -j 4
    $MAKE install
    ;;

  libuuid)
    # build libuuid
    mkdir -p $DIRECTORY/tmp/libuuid
    cd $DIRECTORY/tmp/libuuid
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        $DIRECTORY/src/libuuid
    $MAKE -j 4
    $MAKE install
    ;;

  clapack)
    # Build Clapack
    mkdir -p ${BUILD_DIR}/clapack
    cd ${BUILD_DIR}/clapack
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_TESTING=OFF \
        $DIRECTORY/src/clapack
    $MAKE -j 4
    $MAKE install
    ;;

  MML)
    #build MML
    mkdir -p ${BUILD_DIR}/mml 
    cd ${BUILD_DIR}/mml
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
           $DIRECTORY/src/mml
    $MAKE -j 4
    $MAKE install
    ;;

  qwt)
    if [ _${SELECT_QT} == _Qt5 ]; then
      QWT=qwt-6
    else
      QWT=qwt-6
    fi
    
    #build qwt 
    mkdir "${BUILD_DIR}/${QWT}"
    cd "${BUILD_DIR}/${QWT}"
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        "$DIRECTORY/src/${QWT}"
    make -j 4
    make install
    ;;

  qwt-6)
    #build qwt 
    mkdir ${BUILD_DIR}/qwt-6 
    cd ${BUILD_DIR}/qwt-6
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/qwt-6
    make -j 4
    make install
    ;;

  qwtplot3d)
    #build qwtplot3d 
    mkdir ${BUILD_DIR}/qwtplot3d-qt4
    cd ${BUILD_DIR}/qwtplot3d-qt4 
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/qwtplot3d-qt4
    make -j 4
    make install
    ;;

  SBW)
    #Build SBW
    mkdir -p ${BUILD_DIR}/SBW
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
    mkdir -p ${BUILD_DIR}/libsbml
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
        -DLIBSBML_USE_LEGACY_MATH=ON \
        -DLIBSBML_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DLIBSBML_SKIP_SHARED_LIBRARY=ON \
        -DWITH_BZIP2=OFF \
        -DWITH_ZLIB=OFF \
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
        -DLIBSBML_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DLIBSBML_LIBRARY=${INSTALL_DIR}/lib/libsbml-static.a \
        -DLIBNUML_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DEXTRA_LIBS=${INSTALL_DIR}/lib/libexpat.a \
        -DWITH_ZLIB=OFF \
        $DIRECTORY/src/libnuml
    $MAKE -j 4
    $MAKE install
    [ -e ${INSTALL_DIR}/lib/libnuml*.so ] && rm ${INSTALL_DIR}/lib/libnuml*.so
    ;;

  libSEDML)
    # build libSEDML
    mkdir -p ${BUILD_DIR}/libSEDML
    cd ${BUILD_DIR}/libSEDML
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DLIBSBML_STATIC=ON \
        -DLIBSEDML_SHARED_VERSION=OFF \
        -DLIBSEDML_SKIP_SHARED_LIBRARY=ON \
        -DLIBSBML_INCLUDE_DIR=${INSTALL_DIR}/include \
        -DLIBSBML_LIBRARY=${INSTALL_DIR}/lib/libsbml-static.a \
        -DLIBSEDML_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DEXTRA_LIBS=${INSTALL_DIR}/lib/libexpat.a \
        -DWITH_ZLIB=OFF \
        $DIRECTORY/src/libSEDML
    $MAKE -j 4
    $MAKE install
    [ -e ${INSTALL_DIR}/lib/libsedml*.so ] && rm ${INSTALL_DIR}/lib/libsedml*.so
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
     # build libCombine
    mkdir -p ${BUILD_DIR}/libCombine
    cd ${BUILD_DIR}/libCombine
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DCOMBINE_DEPENDENCY_DIR=${INSTALL_DIR} \
        -DLIBCOMBINE_SKIP_SHARED_LIBRARY=ON \
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

case "$(uname -m)" in
  'x86_64')
    SUFFIX="-64"
    ;;

  'i686')
    SUFFIX="-32"
    ;;
esac

# tar -czvf dependencies${SUFFIX}.tar.gz *
