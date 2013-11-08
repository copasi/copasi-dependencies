#!/bin/bash
#

DIRECTORY=$(cd `dirname $0` && pwd)

# export QTDIR=/Users/fbergmann/QtSDK/Desktop/Qt/4.8.1/gcc/
export QMAKESPEC=macx-g++
CMAKE=cmake
QMAKE=$QTDIR/bin/qmake
command -v $QMAKE >/dev/null 2>&1 || { QMAKE=qmake-qt4; }
command -v $QMAKE >/dev/null 2>&1 || { QMAKE=qmake; }

[ -d $DIRECTORY/tmp ] || mkdir $DIRECTORY/tmp
[ -d $DIRECTORY/bin ] || mkdir $DIRECTORY/bin
[ -d $DIRECTORY/bin/include ] || mkdir $DIRECTORY/bin/include
[ -d $DIRECTORY/bin/lib ] || mkdir $DIRECTORY/bin/lib

COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=$BUILD_TYPE"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DENABLE_UNIVERSAL=ON"
COPASI_CMAKE_OPTIONS="${COPASI_CMAKE_OPTIONS} -DCMAKE_OSX_ARCHITECTURES=i386;x86_64"

COPASI_CXXFLAGS="${COPASI_CXXFLAGS} -arch x86_64 -arch i386 -gdwarf-2 -O2 -I$DIRECTORY/bin/include"
COPASI_CFLAGS="${COPASI_CFLAGS} -arch x86_64 -arch i386 -gdwarf-2 -O2 -I$DIRECTORY/bin/include"
COPASI_LDFLAGS="${COPASI_LDFLAGS}  -arch x86_64 -arch i386 -L$DIRECTORY/bin/lib"

# Build cppunit
cd $DIRECTORY/src/cppunit
chmod +x configure
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" ./configure \
  --disable-dependency-tracking \
  --enable-html-docs=no \
  --enable-doxygen=no \
  --enable-dot=no \
  --enable-shared=no \
  --prefix=$DIRECTORY/bin
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" make 
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" make install

# build expat
cd $DIRECTORY/src/expat
chmod +x configure 
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" ./configure \
  --disable-dependency-tracking \
  --enable-shared=no \
  --prefix=$DIRECTORY/bin
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" make 
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" make install
# delete shared library just in case
[ -e $DIRECTORY/bin/lib/libexpat*dylib ] && rm $DIRECTORY/bin/lib/libexpat*dylib

# build raptor
cd $DIRECTORY/src/raptor
chmod +x configure
chmod +x install-sh
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" ./configure \
  --disable-dependency-tracking \
  --with-xml-parser=expat \
  --with-www=none \
  --enable-shared=no \
  --prefix=$DIRECTORY/bin
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" make 
CXXFLAGS="${COPASI_CXXFLAGS}" CFLAGS="${COPASI_CFLAGS}" LDFLAGS="${COPASI_LDFLAGS}" make install

#build MML
mkdir $DIRECTORY/tmp/mml 
cd $DIRECTORY/tmp/mml 
$CMAKE ${COPASI_CMAKE_OPTIONS} \
  $DIRECTORY/src/mml
make -j 4
make install

#build qwt 
mkdir $DIRECTORY/tmp/qwt 
cd $DIRECTORY/tmp/qwt 
$CMAKE ${COPASI_CMAKE_OPTIONS} \
  $DIRECTORY/src/qwt
make -j 4
make install

#build qwtplot3d 
mkdir $DIRECTORY/tmp/qwtplot3d-qt4
cd $DIRECTORY/tmp/qwtplot3d-qt4 
$CMAKE ${COPASI_CMAKE_OPTIONS} \
  $DIRECTORY/src/qwtplot3d-qt4
make -j 4
make install

#Build SBW
mkdir $DIRECTORY/tmp/SBW
cd $DIRECTORY/tmp/SBW
$CMAKE ${COPASI_CMAKE_OPTIONS} \
  -DWITH_BUILD_BROKER=OFF \
  -DWITH_BUILD_SHARED=OFF \
  $DIRECTORY/src/core
make
make install

# build libsbml
mkdir $DIRECTORY/tmp/libsbml
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
  -DLIBSBML_DEPENDENCY_DIR=$DIRECTORY/bin \
  -DLIBSBML_SKIP_SHARED_LIBRARY=ON \
  -DWITH_BZIP2=OFF \
  -DWITH_ZLIB=OFF \
  $DIRECTORY/src/libSBML
make -j 4
make install

# build libSEDML
mkdir -p $DIRECTORY/tmp/libSEDML
cd $DIRECTORY/tmp/libSEDML
$CMAKE ${COPASI_CMAKE_OPTIONS} \
  -DLIBSBML_STATIC=ON \
  -DLIBSEDML_SHARED_VERSION=OFF \
  -DLIBSEDML_SKIP_SHARED_LIBRARY=ON \
  -DLIBSEDML_DEPENDENCY_DIR=$BASE_DIR/bin \
  -DEXTRA_LIBS=%BASE_DIR%/lib/libexpat.a \
  $DIRECTORY/src/libSEDML
make -j
make install
[ -e $DIRECTORY/bin/lib/libsedml*.dylib ] && rm $DIRECTORY/bin/lib/libsedml*.dylib

echo ${COPASI_CMAKE_OPTIONS}
