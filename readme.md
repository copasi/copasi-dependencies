# COPASI Dependencies
This project contains a number of open source libraries, that make the building of [COPASI](http://copasi.org "COPASI") with [CMAKE](http://www.cmake.org/) as easy as to configure with: 

	cmake -DCOPASI_DEPENDENCY_DIR=<dir> <copasi source dir>  
	make  
	make install

## Building the dependencies

*We are in the process of updating the documentation. The [older instructions](./older_instructions.md) are still available.*


To build the COPASI dependencies we recommend to use the cmake build system from the main directory. This basically includes the required dependency libraries from the chosen configuration as external projects. 

#### supported cmake generators 
Currently not all cmake generators support external subprojects (building from the visual studio IDE is currently known to be not working) so we recommend to use either: 

  * `Ninja`
  * `Unix Makefiles`
  * `NMake Makefiles JOM`
  * `NMake Makefiles` 

#### overview of the build options
If you just want to build the dependencies for the language bindings or the command line version for COPASI, you can simply configure using the `BUILD_UI_DEPS=OFF` option. Then a build of those dependencies is done simply with: 

	mkdir build
	cd build
	cmake -G Ninja -DBUILD_UI_DEPS=OFF -DCMAKE_INSTALL_PREFIX=../bin ..
	ninja

where:

* `build`: is the name of the build folder in which all of the copasi-dependencies will be built.
* `-G Ninja`: specifies that the ninja generator should be used
* `-DCMAKE_INSTALL_PREFIX=../bin`:  this is also the path that you later specify as `-DCOPASI_DEPENDENCY_DIR=<dir>` when configuring COPASI
* `-DBUILD_UI_DEPS=OFF`: that no UI dependencies should be built 
* `..`: indicates that the root of the copasi-dependencies directory is one folder up


**Note:** on linux system crossguid has a dependency on the `uuid` library, as such you will have to install it manually first. This can be done using your package manager:

	apt-get install uuid-dev

or 

	yum install uuid-devel

 or

 	pacman -S libutil-linux-devel

or directly from the source archives. 

#### UI dependency libraries
The COPASI user interface uses Qt. We recommend to use Qt5, with the additional `Qt5DataVisualization` module installed. 

To specify which Qt should be used, use the 

  * `SELECT_QT`: option we recommend it to be set to `Qt5`, other valid values are `Qt4` and `Any`
  
To ensure that Qt can be found you will need to specify the `Qt5_DIR` variable to the full path of the `lib/cmake/Qt5` path of your Qt5 installation). (Or for Qt4 the `QT_DIR` variable to the full path of the root of your Qt4 installation). 

**Examples:** 

Qt5:

	-DSELECT_QT=Qt5 -DQt5_DIR=C:/Qt/Qt5.14.2/5.14.2/msvc2017/lib/cmake/Qt5
 
Qt4: 

	-DSELECT_QT=Qt4 -DQTDIR=C:\Qt\qt-everywhere-opensource-src-4.8.6_vs14

to summarize to build all dependencies (including UI dependencies for Qt5) you would use commands like: 

	mkdir build
	cd build
	cmake -G Ninja -DSELECT_QT=Qt5 -DQt5_DIR=C:/Qt/Qt5.11.1/5.11.1/msvc2015/lib/cmake/Qt5 -DCMAKE_INSTALL_PREFIX=../bin ..
    ninja

where: 

* `build`: is the name of the build folder in which all of the copasi-dependencies will be built.
* `-G Ninja`: specifies that the ninja generator should be used
* `-DSELECT_QT=Qt5`: that Qt5 from the specified `Qt5_DIR` should be used
* `-DCMAKE_INSTALL_PREFIX=../bin`:  this is also the path that you later specify as `-DCOPASI_DEPENDENCY_DIR=<dir>` when configuring COPASI
* `..`: indicates that the root of the copasi-dependencies directory is one folder up


#### all options:
The complete list of all options is: 
 
* `BUILD_UI_DEPS`: specifies whether the UI dependencies are preselected. (defaults to `ON`)
* `BUILD_expat`: specifies whether or not expat should be built (defaults to `ON`)
* `BUILD_raptor`: specifies whether raptor should be built (defaults to `ON`, forces expat to be built)
* `BUILD_crossguid`: specifies whether raptor should be built (defaults to `ON`)
* `BUILD_clapack`: specifies whether clapack should be built (defaults to `ON`)
* `BUILD_libSBML`: specifies whether libSBML should be built (defaults to `ON`, forces expat to be built)
* `BUILD_libnuml`: specifies whether libNUML should be built (defaults to `ON`, forces libSBML to be built)
* `BUILD_libSEDML`: specifies whether libSEDML should be built (defaults to `ON`, forces libnuml to be built)
* `BUILD_zlib`: specifies whether zlib should be built (defaults to `ON`)
* `BUILD_zipper`: specifies whether `zipper` should be built (defaults to `ON`, forces zlib to be built)
* `BUILD_libCombine`: specifies whether libCombine should be built (defaults to `ON`, forces zipper and libSBML to be built)
* `BUILD_SBW`: specifies whether SBW should be built (defaults to `BUILD_UI_DEPS`)
* `BUILD_mml`: specifies whether MML should be built (defaults to `BUILD_UI_DEPS`)
* `BUILD_qwt6`: specifies whether qwt-6 should be built (defaults to `BUILD_UI_DEPS`)
* `BUILD_qwt`: specifies whether qwt should be built (defaults to `BUILD_UI_DEPS`) 
* `BUILD_qwtplot3d`: specifies whether qwtplot3d should be built (defaults to `BUILD_UI_DEPS`)
* `BUILD_archive`: specifies whether an archive of the binaries should be created (defaults to `ON`)
* `WITH_STATIC_RUNTIME`: a specific option on windows to ensure that the static MSVC runtime is used rather than the dynamic one. (defaults to `OFF`) When using the dynamic runtime, the MSVC redistributable package will have to be installed on the target system. 


## Libraries
The following open source libraries are included in this project: 

- [clapack 3.2.1](http://www.netlib.org/clapack/)
- [cppunit 1.12.1](http://sourceforge.net/projects/cppunit/)
- [expat 2.1.0](http://expat.sourceforge.net/)
- [qtmmlwidget](https://code.qt.io/cgit/qt-solutions/qt-solutions.git/tree/qtmmlwidget?id=0787fe5befa2bbadf9fff1956a3e53e14c9a480f)
- [QWT 5.2.0 or 6.1.0](http://qwt.sourceforge.net/)
- [QWT 3D plot](http://qwtplot3d.sourceforge.net/)
- [libSBML 5.18.0](http://sbml.org/Software/libSBML)
- [libnuml 1.1.1](https://github.com/numl/numl)
- [libSEDML 0.4.2](https://github.com/fbergmann/libSEDML)
- [libCOMBINE 0.2.2](https://github.com/sbmlteam/libCombine)
- [SBW Core](http://sbw.sourceforge.net/)
- [raptor](http://librdf.org/raptor/)
- [zipper 0.9.1](https://github.com/fbergmann/zipper)
- [crossguid](https://github.com/graeme-hill/crossguid)

## License
Just in case it needs saying, each of the libraries in the `src` folder are released under their own respective licenses. Otherwise this project (i.e.: the build scripts) are released under the BSD License: 

```
Copyright (c) 2013-2020, Frank T. Bergmann  
All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are 
met: 

Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer. Redistributions in 
binary form must reproduce the above copyright notice, this list of 
conditions and the following disclaimer in the documentation and/or 
other materials provided with the distribution. THIS SOFTWARE IS 
PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

```
