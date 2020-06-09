# Building COPASI on Linux
This document describes how to build COPASI and its dependencies on Linux.

## General Remarks

### Building the Graphical User Interface

We recently upgraded the graphical user interface to use Qt5 whenever we can. COPASI still builds with Qt4, but you should select one or the other before compiling. You choose the Qt version by specifying the `SELECT_QT` parameter and set it to either `Qt5` or `Qt4`. This parameter defaults to `Any` at which point we try and select the first one. 

Our release builds of CopasiUI use Qt5, where we also make use of the `Qt5DataVisualization` module. Which is still not yet available on many linux distributions. So your choice would be to either download a pre-built qt version from <https://download.qt.io/archive/qt/>, and use that one, or you specify another configuration parameter called `QT5_USE_DATAVISUALIZATION` and set it to `OFF` so it will not be used. 

Since the graphical user interface displays HTML notes, there are various ways of displaying them. The default now is to simply display the text in the Qt Textbrowser, as that requires only a small amount of resources. It is however possible to use WebKit or WebEngine to display those notes. This is controlled by the following variables:

* `QT_USE_TEXTBROWSER`: default `ON`
* if `QT_USE_TEXTBROWSER` is `OFF` then we will attempt to discover webkit, unless `QT5_USE_WEBENGINE` is set to `ON`.  

In case you have several Qt5 versions installed, you can specify the one to be used by supplying the parameter `Qt5_DIR` with a full path to the `<qt install prefix>/lib/cmake/Qt5` directory.  

For example: 

```
  cmake -G Ninja -DCOPASI_DEPENDENCY_DIR=../../copasi-dependencies/bin -DSELECT_QT=Qt5 -DQT5_USE_DATAVISUALIZATION=OFF -DBUILD_GUI=ON ../COPASI
```

would compile COPASI without the data visualization widget, which would work with the default Qt5 version available on Ubuntu 18.04 (the data visualization is working with ubuntu 20.04). Alternatively: 

```
cmake -G Ninja -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin -DSELECT_QT=Qt5  -DBUILD_GUI=ON  -DQt5_DIR=~/Qt5.12.2/5.12.2/gcc_64/lib/cmake/Qt5 ../COPASI/
```

would compile against a custom installed Qt 5.12 version. 

### C++ compiler

At this point we do require any C++ compiler capable of supporting C++11.

## Compilation on various linux distributions

(we are in the process of updating the following section)

In order to ensure compilation in the majority of cases, we list the instructions for some of the most used linux distributions. If your linux distibution is not listed here, and the instructions don't work for you, please let us know and we will try to include yours. This document lists:

* [Ubuntu](#ubuntu-2004)
* [Mint Linux](#mint-linux-193) 
* [Debian](#debian-100)
* [Fedora](#fedora-31)
* [Mageia](#mageia-7) (a Mandriva derivate)
* [Slackware](#slackware-142)
* [openSUSE](#opensuse-tumbleweed)

## Ubuntu 20.04
Starting from a vanilla / updated copy of Ubuntu 20.04, the dependencies are built as follows. 
### Prerequisites 
First some required packages need to be installed: 

	apt install git cmake g++ libgl1-mesa-dev libglu1-mesa-dev uuid-dev wget gawk 

If you plan on compiling the **graphical frontend** to COPASI you also need: 

	apt install qtbase5-dev qtbase5-dev-tools libqt5datavisualization5-dev libqt5svg5-dev

Optionally for a graphical frontend to CMake you install `cmake-qt-gui`. This is only needed if you like to finely tweak what is built. 

### Compiling the dependencies
Simply run
	
	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	mkdir build
	cd build 
	cmake -DBUILD_UI_DEPS=OFF -DCMAKE_INSTALL_PREFIX=../bin .. 
	make
	cd ..

You can verify that all needed files are there, by listing the files in the `./bin/lib` directory. You ought to see: 

	fbergmann@ubuntu:~/Development$ ls copasi-dependencies/bin/lib/
	cmake                libcrossguid.a  libnuml-static.a   libzlib.a
	libCombine-static.a  libexpat.a      libraptor.a        pkgconfig
	libZipper-static.a   libf2c.a        libsbml-static.a
	libblas.a            liblapack.a     libsedml-static.a


if you also like to build the COPASI graphical frontend (and thus have Qt5 installed installed) you ought to also find: 

	fbergmann@ubuntu:~/Development/copasi-dependencies$ ls bin/lib/
	cmake                libexpat.a        libqwt.a           libzlib.a
	libCombine-static.a  libf2c.a          libqwtplot3d.a     pkgconfig
	libZipper-static.a   liblapack.a       libraptor.a
	libblas.a            libmml.a          libsbml-static.a
	libcrossguid.a       libnuml-static.a  libsedml-static.a
	

Once verified that those files is there you are ready to compile COPASI.

### Building COPASI
continuing from the command line before you would type the following (**Note** the `-DBUILD_GUI=OFF` parameter, this one is only needed if you do not want to build the graphical frontend. The parameter `-DCMAKE_INSTALL_PREFIX=~/copasi` indicates, that the installation target is the `copasi` directory in the home directory. ) 

	git clone https://github.com/copasi/COPASI
	cd COPASI
	./gitTools/UpdateCopasiVersion
	cd ..
	mkdir build_copasi
	cd build_copasi
	cmake -DBUILD_GUI=OFF -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

### Testing COPASI
To verify that the installation worked as intended you could run the following tests. For `CopasiSE` it ought to be sufficient to run: 

	fbergmann@ubuntu:~/Development/build_copasi$ ~/copasi/bin/CopasiSE 
	COPASI 4.26.224+ (Source)
	The use of this software indicates the acceptance of the attached license.
	To view the license please use the option: --license
	
	Usage: CopasiSE [options] [file]
	  --SBMLSchema schema           The Schema of the SBML file to export.
	  --configdir dir               The configuration directory for copasi. The
	                                default is .copasi in the home directory.
	  --configfile file             The configuration file for copasi. The
	                                default is copasi in the ConfigDir.
	  --convert-to-irreversible     Converts reversible reactions to irreversibl-
	                                e ones before running Task.
	  --exportBerkeleyMadonna file  The Berkeley Madonna file to export.
	  --exportC file                The C code file to export.
	  --exportCA file               The COMBINE archive file to export.
	  --exportIni file              export the parameterization of the model as
	                                INI file for use with the --reparameterize
	                                option.
	  --exportSEDML file            The SEDML file to export.
	  --exportXPPAUT file           The XPPAUT file to export.
	  --home dir                    Your home directory.
	  --importCA file               A COMBINE archive file to import.
	  --importSEDML file            A SEDML file to import.
	  --license                     Display the license.
	  --maxTime seconds             The maximal time CopasiSE may run in
	                                seconds.
	  --nologo                      Surpresses the startup message.
	  --report-file file            Override report file name to be used except
	                                for the one defined in the scheduled task.
	  --scheduled-task taskName     Override the task marked as executable.
	  --validate                    Only validate the given input file (COPASI,
	                                Gepasi, or SBML) without performing any
	                                calculations.
	  --verbose                     Enable output of messages during runtime to
	                                std::error.
	  -c, --copasidir dir           The COPASI installation directory.
	  -e, --exportSBML file         The SBML file to export.
	  -i, --importSBML file         A SBML file to import.
	  -r, --reparameterize file     Before any task is run, the model is
	                                reparameterized with the values specified
	                                in the provided INI file.
	  -s, --save file               The file the model is saved to after work.
	  -t, --tmp dir                 The temp directory used for autosave.

If you also built the graphical frontend, you want to run

	~/copasi/bin/CopasiUI

. This will bring up the UI, and you could get started by looking at `File\Examples` and open one of the included models. In both cases `~/copasi` refers to the install prefix that was provided to CMake. 

## Mint Linux 19.3
Mint Linux actually seems to use the same package repositories as Ubuntu, so the same instructions as above can be used to build the COPASI dependencies and COPASI. The only exception is that the `libqt5datavisualization5-dev` package was not available, so in this case you will have to disable the use of the data visualization as [mentioned above](#building-the-graphical-user-interface).

## Debian 10.0

So all dependencies for the command line versions are: 

	apt-get install git cmake g++ libgl1-mesa-dev libglu1-mesa-dev uuid-dev make qtbase5-dev qtbase5-dev-tools libqt5datavisualization5-dev libqt5svg5-dev

Compiling then completed as before with: 

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	mkdir build
	cd build 
	cmake -DBUILD_UI_DEPS=ON -DCMAKE_INSTALL_PREFIX=../bin .. 
	make
	cd ../..
	git clone https://github.com/copasi/COPASI
	cd COPASI
	./gitTools/UpdateCopasiVersion
	cd ..
	mkdir build_copasi
	cd build_copasi
	cmake -DSELECT_QT=Qt5 -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install


A run of `~/copasi/bin/CopasiUI` verifies that it works. 

## Fedora 31

### Prerequisites
At the bare minimum the following dependencies are needed: 
	
	yum install git cmake gcc-c++ mesa-libGL-devel mesa-libGLU-devel byacc libuuid-devel make

for CopasiUI additionally needed are: 

	yum install qt5-qtbase-devel qt5-qtdatavis3d-devel qt5-qtsvg-devel

### Compiling
Now again as before, first checking out the copasi-dependencies project, then compiling the dependencies:

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	mkdir build
	cd build 
	cmake -DBUILD_UI_DEPS=OFF -DCMAKE_INSTALL_PREFIX=../bin .. 
	make
	cd ..

Verifying that the following files are available in the `copasi-dependencies/bin/lib64` directory. 

	[fbergmann@localhost copasi-dependencies]$ ls bin/lib64
	cmake                libZipper-static.a  libcrossguid.a  libf2c.a     libnuml-static.a  libsbml-static.a   libzlib.a
	libCombine-static.a  libblas.a           libexpat.a      liblapack.a  libraptor.a       libsedml-static.a  pkgconfig

or

	[fbergmann@localhost copasi-dependencies]$ ls bin/lib64
	cmake                libblas.a       libf2c.a     libnuml-static.a  libraptor.a        libzlib.a
	libCombine-static.a  libcrossguid.a  liblapack.a  libqwt.a          libsbml-static.a   pkgconfig
	libZipper-static.a   libexpat.a      libmml.a     libqwtplot3d.a    libsedml-static.a

(Again if you are not compiling the GUI you won't need mml, qwt, qwtplot3d and SBW). If all these files are present next is COPASI: 


	git clone https://github.com/copasi/COPASI
	cd COPASI
	./gitTools/UpdateCopasiVersion
	cd ..
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin -DSELECT_QT=Qt5 -DQt5_DIR=/usr/lib64/cmake/Qt5 ../COPASI
	make
	make install

Once installed, again a start of `~/copasi/bin/CopasiUI` verifies that everything works as planned. 


## Mageia 7

The following prerequisites are used on Mageia: 

	urpmi --auto-select
	urpmi git cmake gcc-c++ lib64mesagl1-devel lib64mesaglu1-devel lib64uuid-devel qtdatavis3d5-devel qtsvg5-devel lib64qt5opengl-devel lib64qt5xml-devel lib64qt5printsupport-devel lib64qt5concurrent-devel lib64qt5network-devel wget make

And again, qt is not needed when building only CopasiSE. 

Next we build the dependencies: 

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	mkdir build
	cd build 
	cmake -DBUILD_UI_DEPS=ON -DCMAKE_INSTALL_PREFIX=../bin .. 
	make
	cd ..

and building COPASI:

	git clone https://github.com/copasi/COPASI
	cd COPASI
	./gitTools/UpdateCopasiVersion
	cd ..
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

and testing it again by running `~/copasi/bin/CopasiUI`.


## Slackware 14.2
If you are on Slackware-current you can skip to the last step and build the COPASI dependencies right away. If you are on 14.2 then you need two additions: upgrade CMake to a newer version, and install Qt5. Strictly speaking, you can use the native Qt4 but we think you will be better served with Qt5, so we advise installing it (side by side with Qt4, following the instructions below).

Upgrading CMake can be done using a package in Pedro Mendes' [slackware-stuff repository](https://github.com/pmendes/slackware-stuff): 

	wget https://github.com/pmendes/slackware-stuff/raw/master/packages/cmake-3.17.3-x86_64-1_pm.txz
	sudo upgradepkg cmake-3.17.3-x86_64-1_pm.txz

Installing Qt5 can be done using packages from Alien Bob (or alternatively build your own from [SBo](https://slackbuilds.org/); if you follow that option you should know what to do...). Follow the steps below to install Qt5 side by side with Qt4 (which you need because of KDE4; note that we use 'installpkg' rather than 'upgradepkg'!): 

	wget http://www.slackware.com/~alien/slackbuilds/qt5/pkg64/14.2/qt5-5.9.6-x86_64-1alien.txz
	wget http://www.slackware.com/~alien/slackbuilds/qt5-webkit/pkg64/14.2/qt5-webkit-5.9.1-x86_64-1alien.txz
	sudo installpkg qt5-5.9.6-x86_64-1alien.txz
	sudo installpkg qt5-webkit-5.9.1-x86_64-1alien.txz

Now you can build the COPASI dependencies using: 

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	mkdir build
	cd build 
	cmake -DBUILD_UI_DEPS=ON -DSELECT_QT=Qt5 -DQT5_DIR=/usr/lib64/cmake/Qt5 -DCMAKE_INSTALL_PREFIX=../bin .. 
	make
	cd ..

Building COPASI is then done with:

	git clone https://github.com/copasi/COPASI
	mkdir build_copasi
	cd build_copasi
	cmake -DSELECT_QT=Qt5 -DQT5_DIR=/usr/lib64/cmake/Qt5 -DENABLE_TIME_SENS=OFF -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

which can be confirmed by running `~/copasi/bin/CopasiUI`. 


## openSUSE Tumbleweed
Dependencies are installed with: 

	zypper install git cmake gcc-c++ glu-devel bison libuuid-devel

for the graphical frontend you need to additionally install: 

	zypper install libqt5-qtbase-devel libqt5-qtsvg-devel libQt5DataVisualization5-devel

From here the compilation of the dependencies proceeds as before with: 

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	mkdir build
	cd build 
	cmake -DBUILD_UI_DEPS=OFF -DCMAKE_INSTALL_PREFIX=../bin .. 
	make
	cd ..

and and then COPASI is again built with: 

	git clone https://github.com/copasi/COPASI
	cd COPASI
	./gitTools/UpdateCopasiVersion
	cd ..
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin ../COPASI
	make
	make install

which can be confirmed by running `~/copasi/bin/CopasiUI`. 


##Centos 8.1

### Prerequisites
At the bare minimum the following dependencies are needed: 
	
	yum install git cmake gcc-c++ mesa-libGL-devel mesa-libGLU-devel byacc libuuid-devel make wget which

for CopasiUI additionally needed are: 

	yum install epel-release
	yum install qt5-qtbase-devel qt5-qtdatavis3d-devel qt5-qtsvg-devel

### Compiling
Now again as before, first checking out the copasi-dependencies project, then compiling the dependencies:

	git clone https://github.com/copasi/copasi-dependencies
	cd copasi-dependencies
	mkdir build
	cd build 
	cmake -DBUILD_UI_DEPS=ON -DCMAKE_INSTALL_PREFIX=../bin .. 
	make
	cd ..

and then to compile COPASI: 

	git clone https://github.com/copasi/COPASI
	cd COPASI
	./gitTools/UpdateCopasiVersion
	cd ..
	mkdir build_copasi
	cd build_copasi
	cmake -DCMAKE_INSTALL_PREFIX=~/copasi -DCOPASI_DEPENDENCY_DIR=../copasi-dependencies/bin -DSELECT_QT=Qt5 -DQt5_DIR=/usr/lib64/cmake/Qt5 ../COPASI
	make
	make install

Once installed, again a start of `~/copasi/bin/CopasiUI` verifies that everything works as planned. 
