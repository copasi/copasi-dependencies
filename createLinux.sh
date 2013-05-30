#!/bin/bash
#

DIRECTORY=$(cd `dirname $0` && pwd)
BUILD_TYPE=Release
CMAKE=cmake
QMAKE=qmake
MAKE=gmake
command -v $MAKE >/dev/null 2>&1 || { MAKE=make; }
command -v $QMAKE >/dev/null 2>&1 || { QMAKE=qmake-qt4; }
command -v $QMAKE >/dev/null 2>&1 || { echo >&2 "qmake cannot be found, please update the qmake variable."; }

[ -d $DIRECTORY/tmp ] || mkdir $DIRECTORY/tmp
[ -d $DIRECTORY/bin ] || mkdir $DIRECTORY/bin
[ -d $DIRECTORY/bin/include ] || mkdir $DIRECTORY/bin/include
[ -d $DIRECTORY/bin/lib ] || mkdir $DIRECTORY/bin/lib

# Build Clapack
mkdir -p $DIRECTORY/tmp/clapack
cd $DIRECTORY/tmp/clapack
$CMAKE -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin -DBUILD_TESTING=OFF $DIRECTORY/src/clapack
$MAKE 
$MAKE install

#build MML
mkdir -p $DIRECTORY/tmp/mml 
cd $DIRECTORY/tmp/mml 
$CMAKE  -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin $DIRECTORY/src/mml
make
$MAKE install

#build qwt 
cd $DIRECTORY/src/qwt
$QMAKE qwt.pro -o Makefile
$MAKE -j 4
cp include/*.h $DIRECTORY/bin/include
cp lib/*.a $DIRECTORY/bin/lib

#build qwtplot3d 
cd $DIRECTORY/src/qwtplot3d-qt4
$QMAKE qwtplot3d.pro -o Makefile
$MAKE -j 4
cp include/*.h $DIRECTORY/bin/include
cp lib/*.a $DIRECTORY/bin/lib

#Build SBW
mkdir -p $DIRECTORY/tmp/SBW
cd $DIRECTORY/tmp/SBW
$CMAKE -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin $DIRECTORY/src/core
make
$MAKE install

# Build cppunit
cd $DIRECTORY/src/cppunit
chmod +x configure
./configure --enable-html-docs=no --enable-doxygen=no --enable-dot=no --enable-shared=no --prefix=$DIRECTORY/bin
$MAKE 
$MAKE install

# build expat
cd $DIRECTORY/src/expat
chmod +x configure 
./configure  --enable-shared=no --prefix=$DIRECTORY/bin
$MAKE 
$MAKE install
# delete shared library just in case
rm bin/lib/libexpat*so

# build libsbml
mkdir -p $DIRECTORY/tmp/libsbml
cd $DIRECTORY/tmp/libsbml
$CMAKE -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin -DENABLE_LAYOUT=ON -DENABLE_REQUIREDELEMENTS=OFF -DENABLE_RENDER=ON -DENABLE_COMP=ON -DENABLE_FBC=OFF -DENABLE_SPATIAL=OFF -DENABLE_GROUPS=OFF -DWITH_EXPAT=ON -DWITH_LIBXML=OFF -DLIBSBML_DEPENDENCY_DIR=$DIRECTORY/bin -DLIBSBML_SKIP_SHARED_LIBRARY=ON -DWITH_BZIP2=OFF -DWITH_ZLIB=OFF $DIRECTORY/src/libSBML
$MAKE -j 4
$MAKE install

# build raptor
cd $DIRECTORY/src/raptor
chmod +x configure
chmod +x install-sh
./configure  LDFLAGS="-L$DIRECTORY/bin/lib" CXXFLAGS="-I$DIRECTORY/bin/include" CFLAGS="-I$DIRECTORY/bin/include" --with-xml-parser=expat --with-www=none --enable-shared=no --prefix=$DIRECTORY/bin
$MAKE -i 
$MAKE -i install

