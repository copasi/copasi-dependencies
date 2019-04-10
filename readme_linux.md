# Building COPASI on Linux
This document describes how to build COPASI and its dependencies on Linux.

## General Remarks

### Building the Graphical User Interface

We recently upgraded the graphical user interface to use Qt5 whenever we can. COPASI still builds with Qt4, but you should select one or the other before compiling. You choose the Qt version by specifying the `SELECT_QT` parameter and set it to either `Qt5` or `Qt4`. This parameter defaults to `Any` at which point we try and select the first one. 

Our release builds of CopasiUI use Qt5, where we also make use of the `Qt5DataVisualization` module. Unfortunately as of April, 2019, this is still not available on many linux distributions. So your choice would be to either download a pre-built qt version from <https://download.qt.io/archive/qt/>, and use that one, or you specify another configuration parameter called `QT5_USE_DATAVISUALIZATION` and set it to `OFF` so it will not be used. 

In case you have several Qt5 versions installed, you can specify the one to be used by supplying the parameter `Qt5_DIR` with a full path to the `<qt install prefix>/lib/cmake/Qt5` directory.  

For example: 

```
  cmake -G Ninja -DCOPASI_DEPENDENCY_DIR=../../copasi-dependencies/bin -DSELECT_QT=Qt5 -DQT5_USE_DATAVISUALIZATION=OFF -DBUILD_GUI=ON ../COPASI
```

would compile COPASI without the data visualization widget, which would work with the default Qt5 version available on Ubuntu 18.04. Alternatively: 

```
cmake -G Ninja -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin -DSELECT_QT=Qt5  -DBUILD_GUI=ON  -DQt5_DIR=~/Qt5.12.2/5.12.2/gcc_64/lib/cmake/Qt5 ../COPASI/
```

would compile against a custom installed Qt 5.12 version. 

### C++ compiler

At this point we do require any C++ compiler capable of supporting C++11.

## Various linux distributions checked in the past

(we are in the process of updating the following section)

In order to ensure compilation in the majority of cases, we list the instructions for the [mostly used](http://www.zdnet.com/the-5-most-popular-linux-distributions-7000003183/) linux distributions. If your linux distibution is not listed here, and the instructions don't work for you, please let us know and we will try to include yours. This document lists:

* [Ubuntu](#ubuntu-1210)
* [Mint Linux](#mint-linux-15) 
* [Debian](#debian-70)
* [Fedora](#fedora-19-beta)
* [Mageia](#mageia-3) (a Mandriva derivate)
* [Slackware](#slackware-14)
* [openSUSE](#opensuse-123)

## Ubuntu 12.10
Starting from a vanilla / updated copy of Ubuntu 12.10, the dependencies are built as follows. 
### Prerequisites 
First some required packages need to be installed: 

	apt-get install git cmake g++ libgl1-mesa-dev libglu1-mesa-dev

If you plan on compiling the **graphical frontend** to COPASI you also need: 

	apt-get install libqt4-dev libxml2-dev zlib1g-dev 

Optionally for a graphical frontend to CMake you install `cmake-qt-gui`. This is only needed if you like to finely tweak what is built. 

### Compiling the dependencies
Simply run
	
	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	./createLinux.sh
	cd ..

You can verify that all needed files are there, by listing the files in the `./bin/lib` directory. You ought to see: 

	fbergmann@ubuntu:~/Development$ ls copasi-dependencies/bin/lib/
	libblas.a      libexpat.a   liblapack.a   libsbml-static.a
	libcppunit.a   libexpat.la  libraptor.a   pkgconfig
	libcppunit.la  libf2c.a     libraptor.la


if you also like to build the COPASI graphical frontend (and this have `libqt4-dev` and `libxml2-dev` installed) you ought to also find: 

	fbergmann@ubuntu:~/Development/copasi-dependencies$ ls bin/lib/
	libblas.a      libexpat.la  libqwt.a        libsbml-static.a  libSBW-static.a
	libcppunit.a   libf2c.a     libqwtplot3d.a  libSBW.so         pkgconfig
	libcppunit.la  liblapack.a  libraptor.a     libSBW.so.2.10
	libexpat.a     libmml.a     libraptor.la    libSBW.so.2.10.0
	

Once verified that those files is there you are ready to compile COPASI.

### Building COPASI
continuing from the command line before you would type the following (**Note** the `-DBUILD_GUI=OFF` parameter, this one is only needed if you do not want to build the graphical frontend. The parameter `-DCMAKE_INSTALL_PREFIX=~/copasi` indicates, that the installation target is the `copasi` directory in the home directory. ) 

	git clone https://github.com/copasi/COPASI
	mkdir build_copasi
	cd build_copasi
	cmake -DBUILD_GUI=OFF -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

### Testing COPASI
To verify that the installation worked as intended you could run the following tests. For `CopasiSE` it ought to be sufficient to run: 

	fbergmann@ubuntu:~/Development/build_copasi$ ~/copasi/bin/CopasiSE 
	COPASI 4.9.43 (Source)
	The use of this software indicates the acceptance of the attached license.
	To view the license please use the option: --license
	
	Usage: CopasiSE [options] [file]
	  --SBMLSchema schema           The Schema of the SBML file to export.
	  --configdir dir               The configuration directory for copasi. The
	                                default is .copasi in the home directory.
	  --configfile file             The configuration file for copasi. The
	                                default is copasi in the ConfigDir.
	  --exportBerkeleyMadonna file  The Berkeley Madonna file to export.
	  --exportC file                The C code file to export.
	  --exportXPPAUT file           The XPPAUT file to export.
	  --home dir                    Your home directory.
	  --license                     Display the license.
	  --maxTime seconds             The maximal time CopasiSE may run in
	                                seconds.
	  --nologo                      Surpresses the startup message.
	  --validate                    Only validate the given input file (COPASI,
	                                Gepasi, or SBML) without performing any
	                                calculations.
	  --verbose                     Enable output of messages during runtime to
	                                std::error.
	  -c, --copasidir dir           The COPASI installation directory.
	  -e, --exportSBML file         The SBML file to export.
	  -i, --importSBML file         A SBML file to import.
	  -s, --save file               The file the model is saved to after work.
  	  -t, --tmp dir                 The temp directory used for autosave.

If you also built the graphical frontend, you want to run

	~/copasi/bin/CopasiUI

. This will bring up the UI, and you could get started by looking at `File\Examples` and open one of the included models. In both cases `~/copasi` refers to the install prefix that was provided to CMake. 

## Mint Linux 15
Mint Linux actually seems to use the same package repositories as Ubuntu, so the same instructions as above can be used to build the COPASI dependencies and COPASI. 

## Debian 7.0
In this case Debian 7 was installed from a network image, choosing all standard settings. Overall the same instructions as the ones for Ubuntu ought to hold. So mainly the prerequisites and the compile commands (for COPASI with GUI) are listed below. 

As for prerequisites, they are precisely the same, except that for whatever reason `make` was not installed, and had to be added to the list. So all dependencies were: 

	apt-get install git cmake g++ libgl1-mesa-dev libglu1-mesa-dev make libqt4-dev libxml2-dev zlib1g-dev 

Compiling then completed as before with: 

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	./createLinux.sh
	cd ..
	git clone https://github.com/copasi/COPASI
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install


A run of `~/copasi/bin/CopasiUI` verifies that it works. 

## Fedora 19 (Beta)
Fedora 18 wouldn't start in my VM, so I took 19 beta from a netinstall.
This has been tested for releases up to Fedora 27 

### Prerequisites
At the bare minimum the following dependencies are needed: 
	
	yum install git cmake gcc-c++ mesa-libGL-devel mesa-libGLU-devel byacc

for CopasiUI additionally needed are: 

	yum install qt4-devel libxml2-devel qt-webkit-devel

### Compiling
Now again as before, first checking out the copasi-dependencies project, then compiling the dependencies:

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	./createLinux.sh
	cd ..

Verifying that the following files are available in the `copasi-dependencies/bin/lib` directory. 

	[fbergmann@localhost copasi-dependencies]$ ls bin/lib
	libblas.a     libcppunit.la  libexpat.la  liblapack.a  libqwt.a        libraptor.a   libsbml-static.a  libSBW.so.2.10    libSBW-static.a
	libcppunit.a  libexpat.a     libf2c.a     libmml.a     libqwtplot3d.a  libraptor.la  libSBW.so         libSBW.so.2.10.0  pkgconfig

(Again if you are not compiling the GUI you won't need mml, qwt, qwtplot3d and SBW). If all these files are present next is COPASI: 


	git clone https://github.com/copasi/COPASI
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

Once installed, again a start of `~/copasi/bin/CopasiUI` verifies that everything works as planned. 

##Mageia 3
The following prerequisites are used on Mageia: 

	urpmi git cmake gcc-c++ lib64mesagl1-devel lib64mesaglu1-devel lib64qt4-devel lib64xml2-devel lib64qtwebkit2.2-devel

(obviously on 32bit Mageia you'd choose the `libqt4-devel` and `libxml2-devel` package). And again, qt and xml2 are not needed when building only CopasiSE. 

Next we build the dependencies: 

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	./createLinux.sh
	cd ..

and verify that all needed files are there: 

	[fbergmann@localhost copasi-dependencies]$ ls bin/lib
	libblas.a       libf2c.a        libraptor.a       libSBW.so.2.10.0*
	libcppunit.a    liblapack.a     libraptor.la*     libSBW-static.a
	libcppunit.la*  libmml.a        libsbml-static.a  pkgconfig/
	libexpat.a      libqwt.a        libSBW.so@
	libexpat.la*    libqwtplot3d.a  libSBW.so.2.10@

before checking out and building COPASI:

	git clone https://github.com/copasi/COPASI
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

and testing it again by running `~/copasi/bin/CopasiUI`.


## Slackware 14

After a full install (all 8GB worth of packages) of Slackware 14 the good news is, that there are already all needed packages installed. So once installed, the dependencies are again simply built using: 

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	./createLinux.sh
	cd ..

after which the `bin/lib` directory contains the files: 

	bash-4.2# ls bin/lib
	libSBW-static.a   libblas.a      libexpat.la  libqwt.a        libsbml-static.a
	libSBW.so         libcppunit.a   libf2c.a     libqwtplot3d.a  pkgconfig
	libSBW.so.2.10    libcppunit.la  liblapack.a  libraptor.a
	libSBW.so.2.10.0  libexpat.a     libmml.a     libraptor.la

and so building COPASI is again as easy as: 

	git clone https://github.com/copasi/COPASI
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

which can be confirmed by running `~/copasi/bin/CopasiUI`. 


## openSUSE 12.3
Starting from the a fresh installation (choosing only the GNOME desktop environment) the following packages need to be installed: 

	zypper install git cmake gcc-c++ glu-devel bison

for the graphical frontend you need to additionally install: 

	zypper install libqt4-devel libxml2-devel  libQtWebKit-devel

(in versions prior to 12.3 the `glu.h` is provided by `Mesa-libGLU-devel`). From here the compilation of the dependencies proceeds as before with: 

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	./createLinux.sh
	cd ..

after which the `copasi-dependencies/bin/lib` directory contains the files: 

	fbergmann@linux-251g:~/Development> ls copasi-dependencies/bin/lib
	libblas.a      libexpat.a   liblapack.a  libqwtplot3d.a  libsbml-static.a  libSBW.so.2.10.0
	libcppunit.a   libexpat.la  libmml.a     libraptor.a     libSBW.so         libSBW-static.a
	libcppunit.la  libf2c.a     libqwt.a     libraptor.la    libSBW.so.2.10    pkgconfig


and and then COPASI is again built with: 

	git clone https://github.com/copasi/COPASI
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

which can be confirmed by running `~/copasi/bin/CopasiUI`. 
