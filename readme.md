# COPASI Dependencies
This project contains a number of open source libraries, that make the building of [COPASI](http://copasi.org "COPASI") with [CMAKE](http://www.cmake.org/) as easy as to configure with: 

	cmake -DCOPASI_DEPENDENCY_DIR=<dir> <copasi source dir>  
	make  
	make install

## Building the dependencies
For the time being, I am using shell scripts for the individual operating systems to create the build. All of them install into the `./bin` directory of the project. From there I usually either archive, rename, or refer to it directly from CMAKE. 

When building the dependencies, you have only to be aware of one thing. The scripts are pretty much dumb, and just build whatever they can, and they certainly won't worry whether the build was successful. So if you happen to not have Qt present, then building all the Qt dependencies will pretty much fail (those being the MML widget, QWT and QWT3dPlot). Now that will be fine, if all you want to build is the language bindings / CopasiSE, but might not be otherwise. 

Should the build fail unexpectedly for any other reason, please feel free to file an issue and let me know!
### Windows
Truth be told, building the dependencies on windows was the hardest, and so there is a while bunch of batch scripts for the case. There are ones for debug builds and release builds. And while they have directories hard coded in them that suit me, those directories might not exist on your machine. But don't despair, you can override them easily. The environment variables to watch out for: 

- `INCLUDE`: if not set variants of vcvars will be called to set up MSVC
- `QTDIR`: if not set my Qt directories will be used, otherwise just set to your Qt root
- `QMAKESPEC`: set to win32-msvc2010 by default

The batch files so far: 

	createX86_vs11_x64_debug.bat  
	createX86_vs11_x64_release.bat  
	createX86_vs11_x86_debug.bat  
	createX86_vs11_x86_release.bat  

These files mainly differ in whether they do a release or debug build, and in what defaults they use. And again, even though those files have a 'vs11' in the name, if your environment is initialized for example with a 'vs9' environment, that will be used instead. 
#### Intel Compiler
Perhaps a word to compile with the intel compiler. I've added variants of the above batch files, that will use the Intel compiler. You would only need to open the intel compiler shell for the architecture and then run either: 

	create_icc_x64_release.bat  
	create_icc_x86_release.bat

### OS X
On OSX the `QTDIR` amd and `QMAKESPEC` (to `macx-g++`) are hardcoded to my location. Since I use Lion / Mountainlion, I build with universal binaries for `i386` and `x86_64`, if that is not sufficient, you might need to adapt the: 

	./createOSX.sh
 
**Note:** when I tried runing some builds with cmake 2.8.10 supplied by macports, the builds failed (due to missing `CMAKE_OSX_SYSROOT` variable, and later also in a test for endianess). So for the time being I recommend building with the downloadable version from cmake. 

**Note:** when using the gnu compiler from macports, you want to make sure, that it supports universal builds. If it does not, you will need to adapt the `createOSX.sh` to not build universal binaries. Additionally set the `CMAKE_OSX_ARCHITECTURES` to only the systems arch. 
  
### Linux
To build the dependencies on Linux, be sure to have your operating systems qt4 or qt5 libraries  installed (if building with GUI) along with the mesa gl libraries, and xml2 as well ass uuid-dev. From there simply run:

	./createLinux.sh

For more step-by-step instructions on building COPASI for various distributions see this [separate document](./readme_linux.md). 

## Resetting the build
Simly remove the `bin` and `tmp` folder to start over. 


	rm -rf bin  
	rm -rf tmp  


## Libraries
The following open source libraries are included in this project: 

- [clapack 3.2.1](http://www.netlib.org/clapack/)
- [cppunit 1.12.1](http://sourceforge.net/projects/cppunit/)
- [expat 2.1.0](http://expat.sourceforge.net/)
- Qt MML widget (LGPL, Qt Solutions)
- [QWT 5.2.0](http://qwt.sourceforge.net/)
- [QWT 3D plot](http://qwtplot3d.sourceforge.net/)
- [libSBML 5.11.1](http://sbml.org/Software/libSBML)
- [SBW Core](http://sbw.sourceforge.net/)
- [raptor](http://librdf.org/raptor/)

## License
Just in case it needs saying, each of the libraries in the `src` folder are released under their own respective licenses. Otherwise this project (i.e.: the build scripts) are released under the BSD License: 

```
Copyright (c) 2013-2014, Frank T. Bergmann  
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
