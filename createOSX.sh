#!/bin/bash
#

DIRECTORY=$(cd `dirname $0` && pwd)

export QTDIR=/Users/fbergmann/QtSDK/Desktop/Qt/4.8.1/gcc/
export QMAKESPEC=macx-g++


[ -d $DIRECTORY/tmp ] || mkdir $DIRECTORY/tmp
[ -d $DIRECTORY/bin ] || mkdir $DIRECTORY/bin
[ -d $DIRECTORY/bin/include ] || mkdir $DIRECTORY/bin/include
[ -d $DIRECTORY/bin/lib ] || mkdir $DIRECTORY/bin/lib

#build MML
mkdir $DIRECTORY/tmp/mml 
cd $DIRECTORY/tmp/mml 
cmake -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin -DENABLE_UNIVERSAL=ON -DCMAKE_OSX_ARCHITECTURES="i386;x86_64" $DIRECTORY/src/mml
make
make install

#build qwt 
cd $DIRECTORY/src/qwt
$QTDIR/bin/qmake qwt.pro -o Makefile
make -j 4
cp include/*.h $DIRECTORY/bin/include
cp lib/*.a $DIRECTORY/bin/lib

#build qwtplot3d 
cd $DIRECTORY/src/qwtplot3d-qt4
$QTDIR/bin/qmake qwtplot3d.pro -o Makefile
make -j 4
cp include/*.h $DIRECTORY/bin/include
cp lib/*.a $DIRECTORY/bin/lib

#Build SBW
mkdir $DIRECTORY/tmp/SBW
cd $DIRECTORY/tmp/SBW
cmake -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin -DENABLE_UNIVERSAL=ON $DIRECTORY/src/core
make
make install

# Build cppunit
cd $DIRECTORY/src/cppunit
chmod +x configure
./configure --disable-dependency-tracking CXXFLAGS="-arch x86_64 -arch i386 -gdwarf-2 -O2" --enable-html-docs=no --enable-doxygen=no --enable-dot=no --enable-shared=no --prefix=$DIRECTORY/bin
make AM_LDFLAGS="-XCClinker -arch -XCClinker x86_64 -XCClinker -arch -XCClinker i386"
make install

# build expat
cd $DIRECTORY/src/expat
chmod +x configure 
./configure --disable-dependency-tracking CXXFLAGS="-arch x86_64 -arch i386 -gdwarf-2 -O2" CFLAGS="-arch x86_64 -arch i386 -gdwarf-2 -O2"  --enable-shared=no --prefix=$DIRECTORY/bin
make AM_LDFLAGS="-XCClinker -arch -XCClinker x86_64 -XCClinker -arch -XCClinker i386"
make install
rm bin/lib/libexpat*dylib

# build libsbml
mkdir $DIRECTORY/tmp/libsbml
cd $DIRECTORY/tmp/libsbml
cmake -DCMAKE_INSTALL_PREFIX=$DIRECTORY/bin -DENABLE_COMP=ON -DENABLE_LAYOUT=ON -DENABLE_REQUIREDELEMENTS=OFF -DENABLE_RENDER=ON -DENABLE_COMP=ON -DENABLE_FBC=OFF -DENABLE_SPATIAL=OFF -DENABLE_GROUPS=OFF -DWITH_EXPAT=ON -DWITH_LIBXML=OFF -DLIBSBML_DEPENDENCY_DIR=$DIRECTORY/bin -DLIBSBML_SKIP_SHARED_LIBRARY=ON -DWITH_BZIP2=OFF -DWITH_ZLIB=OFF -DENABLE_UNIVERSAL=ON $DIRECTORY/src/libSBML
make -j 4
make install

# build raptor
cd $DIRECTORY/src/raptor
chmod +x configure
./configure --disable-dependency-tracking CXXFLAGS="-arch x86_64 -arch i386 -gdwarf-2 -O2" CFLAGS="-arch x86_64 -arch i386 -gdwarf-2 -O2"   --with-xml-parser=expat --with-www=none  --with-expat-source=$DIRECTORY/src/expat --enable-shared=no --prefix=$DIRECTORY/bin
make AM_LDFLAGS="-XCClinker -arch -XCClinker x86_64 -XCClinker -arch -XCClinker i386"
make install

