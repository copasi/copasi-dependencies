#!/usr/bin/env bash
#

DIRECTORY=$(cd `dirname $0` && pwd)

if [ $# = 0 ]; then
  ToBeBuild="expat raptor clapack SBW libSBML libnuml libSEDML zlib libCombine MML qwt qwt-6 qwtplot3d"
else
  while [ _${1} != _ ]; do
    ToBeBuild="$ToBeBuild ${1}"
    shift
  done;
fi
 
#Default Values:
BUILD_TYPE=${BUILD_TYPE:="Release"}
SELECT_QT=${SELECT_QT:="Any"}
CMAKE=${CMAKE:="cmake"}

MAKE=${MAKE:="gmake"}
command -v $MAKE >/dev/null 2>&1 || { MAKE=make; }


# echo ${BUILD_TYPE}
# echo ${CMAKE}
# echo ${MAKE}

[ -d $DIRECTORY/tmp ] || mkdir $DIRECTORY/tmp
[ -d $DIRECTORY/bin ] || mkdir $DIRECTORY/bin
[ -d $DIRECTORY/bin/include ] || mkdir $DIRECTORY/bin/include
[ -d $DIRECTORY/bin/lib ] || mkdir $DIRECTORY/bin/lib


COPASI_COMMON_CMAKE_OPTIONS="${COPASI_COMMON_CMAKE_OPTIONS} -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin"
COPASI_COMMON_CMAKE_OPTIONS="${COPASI_COMMON_CMAKE_OPTIONS} -DEXTRA_INCLUDE_DIRS=$DIRECTORY/bin/include"

COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DSELECT_QT=${SELECT_QT}"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} ${COPASI_COMMON_CMAKE_OPTIONS}"

COPASI_CXXFLAGS="${CXXFLAGS} ${COPASI_CXXFLAGS} -I$DIRECTORY/bin/include"
COPASI_CFLAGS="${CFLAGS} ${COPASI_CFLAGS} -I$DIRECTORY/bin/include"
COPASI_LDFLAGS="${LDFLAGS} ${COPASI_LDFLAGS} -L$DIRECTORY/bin/lib"

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
        --prefix=$DIRECTORY/bin
    CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" $MAKE -j 4 
    CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" $MAKE install
    ;;

  expat)
    # build expat
    mkdir -p $DIRECTORY/tmp/expat
    cd $DIRECTORY/tmp/expat
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        $DIRECTORY/src/expat
    $MAKE -j 4
    $MAKE install
    ;;

  raptor)
    # build raptor
    mkdir -p $DIRECTORY/tmp/raptor
    cd $DIRECTORY/tmp/raptor
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_shared=OFF \
        $DIRECTORY/src/raptor
    $MAKE -j 4
    $MAKE install
    ;;

  clapack)
    # Build Clapack
    mkdir -p $DIRECTORY/tmp/clapack
    cd $DIRECTORY/tmp/clapack
    $CMAKE ${COPASI_COMMON_CMAKE_OPTIONS} \
        -DBUILD_TESTING=OFF \
        $DIRECTORY/src/clapack
    $MAKE -j 4
    $MAKE install
    ;;

  MML)
    #build MML
    mkdir -p $DIRECTORY/tmp/mml 
    cd $DIRECTORY/tmp/mml
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
           $DIRECTORY/src/mml
    $MAKE -j 4
    $MAKE install
    ;;

  qwt)
    #build qwt 
    mkdir $DIRECTORY/tmp/qwt 
    cd $DIRECTORY/tmp/qwt 
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/qwt
    make -j 4
    make install
    ;;

  qwt-6)
    #build qwt 
    mkdir $DIRECTORY/tmp/qwt-6 
    cd $DIRECTORY/tmp/qwt-6
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/qwt-6
    make -j 4
    make install
    ;;

  qwtplot3d)
    #build qwtplot3d 
    mkdir $DIRECTORY/tmp/qwtplot3d-qt4
    cd $DIRECTORY/tmp/qwtplot3d-qt4 
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/qwtplot3d-qt4
    make -j 4
    make install
    ;;

  SBW)
    #Build SBW
    mkdir -p $DIRECTORY/tmp/SBW
    cd $DIRECTORY/tmp/SBW
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DWITH_BUILD_BROKER=OFF \
        -DWITH_BUILD_SHARED=OFF \
        $DIRECTORY/src/core
    $MAKE -j 4
    $MAKE install
    ;;

  libSBML)
    # build libsbml
    mkdir -p $DIRECTORY/tmp/libsbml
    cd $DIRECTORY/tmp/libsbml
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
        -DLIBSBML_DEPENDENCY_DIR=$DIRECTORY/bin \
        -DLIBSBML_SKIP_SHARED_LIBRARY=ON \
        -DWITH_BZIP2=OFF \
        -DWITH_ZLIB=OFF \
        $DIRECTORY/src/libSBML
    $MAKE -j 4
    $MAKE install
    ;;

  libnuml)
    # build libnuml
    mkdir -p $DIRECTORY/tmp/libnuml
    cd $DIRECTORY/tmp/libnuml
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DLIBSBML_STATIC=ON \
        -DLIBNUML_SHARED_VERSION=OFF \
        -DLIBNUML_SKIP_SHARED_LIBRARY=ON \
        -DLIBSBML_INCLUDE_DIR=$DIRECTORY/bin/include \
        -DLIBSBML_LIBRARY=$DIRECTORY/bin/lib/libsbml-static.a \
        -DLIBNUML_DEPENDENCY_DIR=$DIRECTORY/bin \
        -DEXTRA_LIBS=$DIRECTORY/bin/lib/libexpat.a \
        -DWITH_ZLIB=OFF \
        $DIRECTORY/src/libnuml
    $MAKE -j 4
    $MAKE install
    [ -e $DIRECTORY/bin/lib/libnuml*.so ] && rm $DIRECTORY/bin/lib/libnuml*.so
    ;;

  libSEDML)
    # build libSEDML
    mkdir -p $DIRECTORY/tmp/libSEDML
    cd $DIRECTORY/tmp/libSEDML
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DLIBSBML_STATIC=ON \
        -DLIBSEDML_SHARED_VERSION=OFF \
        -DLIBSEDML_SKIP_SHARED_LIBRARY=ON \
        -DLIBSBML_INCLUDE_DIR=$DIRECTORY/bin/include \
        -DLIBSBML_LIBRARY=$DIRECTORY/bin/lib/libsbml-static.a \
        -DLIBSEDML_DEPENDENCY_DIR=$DIRECTORY/bin \
        -DEXTRA_LIBS=$DIRECTORY/bin/lib/libexpat.a \
        -DWITH_ZLIB=OFF \
        $DIRECTORY/src/libSEDML
    $MAKE -j 4
    $MAKE install
    [ -e $DIRECTORY/bin/lib/libsedml*.so ] && rm $DIRECTORY/bin/lib/libsedml*.so
    ;;

  zlib)
    # build zlib
    mkdir -p $DIRECTORY/tmp/zlib
    cd $DIRECTORY/tmp/zlib
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        $DIRECTORY/src/zlib
    $MAKE -j 4
    $MAKE install
    ;;

  libCombine)
     # build libCombine
    mkdir -p $DIRECTORY/tmp/libCombine
    cd $DIRECTORY/tmp/libCombine
    $CMAKE ${COPASI_CMAKE_OPTIONS} \
        -DCOMBINE_DEPENDENCY_DIR=$DIRECTORY/bin \
        -DEXTRA_LIBS=$DIRECTORY/bin/lib/libexpat.a \
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

cd $DIRECTORY/bin

case "$(uname -m)" in
  'x86_64')
    SUFFIX="-64"
    ;;

  'i686')
    SUFFIX="-32"
    ;;
esac

# tar -czvf dependencies${SUFFIX}.tar.gz *
