## @file    setup.py
## @brief   Python distutils code for libNUML Python module (including dependencies)
## @author  Michael Hucka
## @author  Ben Bornstein
## @author  Ben Kovitz
## @author  Frank Bergmann (fbergman@caltech.edu)
##
##<!---------------------------------------------------------------------------
## This file is part of libNuML.  Please visit http://sbml.org for more
## information about SBML, and the latest version of libNuML.
##
## Copyright (C) 2013-2018 jointly by the following organizations:
##     1. California Institute of Technology, Pasadena, CA, USA
##     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
##     3. University of Heidelberg, Heidelberg, Germany
##
## Copyright 2005-2010 California Institute of Technology.
## Copyright 2002-2005 California Institute of Technology and
##                     Japan Science and Technology Corporation.
##
## This library is free software; you can redistribute it and/or modify it
## under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation.  A copy of the license agreement is provided
## in the file named "LICENSE.txt" included with this software distribution
## and also available online as http://sbml.org/software/libnuml/license.html
##----------------------------------------------------------------------- -->*/

import errno
import os
import sys
import shutil
import platform
import sysconfig 
from os.path import abspath, exists, join, split

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

def get_python_include():
  path = sysconfig.get_paths()['include']
  if exists(path): 
    return path
  # for whatever reason 2.7 on centos returns a wrong path here 
  return sysconfig.get_config_vars()['INCLUDEPY']

def prepend_variables(args, variables):
  for var in variables: 
    temp = os.getenv(var)
    if temp:
      if var == 'CMAKE_GENERATOR':
        args = ['-G', temp] + args
      else:
        args.insert(0, '-D' + var + '=' +temp)
  return args


def get_lib_full_path(path, partial): 
  for file in os.listdir(path): 
    if partial in file: 
      return os.path.join(path, file)
  return None

def makedirs(folder, *args, **kwargs):
  try:
    return os.makedirs(folder, exist_ok=True, *args, **kwargs)
  except TypeError: 
    # Unexpected arguments encountered 
    pass

  try:
    # Should work is TypeError was caused by exist_ok, eg., Py2
    return os.makedirs(folder, *args, **kwargs)
  except OSError as e:
    if e.errno != errno.EEXIST:
      raise

    if os.path.isfile(folder):
      # folder is a file, raise OSError just like os.makedirs() in Py3
      raise

def get_dir_if_exists(variable, default):
  value = os.getenv(variable, default)
  value = abspath(value)
  if not exists(value):
    return None
  return value

global SRC_DIR
SRC_DIR = get_dir_if_exists('LIBNUML_SRC_DIR', '.')
global DEP_DIR
DEP_DIR = get_dir_if_exists('LIBNUML_DEP_DIR', '../libnuml_dependencies/')
DEP_SRC_DIR = get_dir_if_exists('LIBNUML_DEP_SRC', './submodules/libSBML-dependencies/')
DEP_SBML_SRC_DIR = get_dir_if_exists('LIBNUML_SBML_DEP_SRC', './submodules/libSBML/')
DEP_DIR32 = get_dir_if_exists('LIBNUML_DEP_DIR_32', '../win_libnuml_dependencies_32/')
DEP_DIR64 = get_dir_if_exists('LIBNUML_DEP_DIR_64', '../win_libnuml_dependencies_64/')


if not SRC_DIR:
  src_defined = os.getenv('LIBNUML_SRC_DIR')
  if src_defined:
    raise ValueError("LibNuML Source defined as: {0}, but coun't be found".format(src_defined))
  else:
    raise ValueError("LibNuML Source not specified or not present, define LIBNUML_SRC_DIR.")

print ("Using libNuML from: {0}".format(SRC_DIR))

version_file_name = join(SRC_DIR, 'VERSION.txt')
print ("Using VERSION.txt: {0}".format(version_file_name))

if not exists(version_file_name):
  print(os.listdir(SRC_DIR))
  raise ValueError("Invalid libNuML Source directory, no VERSION.txt file")

with open(version_file_name, 'r') as version_file:
  VERSION = version_file.readline().strip()

print ("Version is: {0}".format(VERSION))
print ("building for python: {0}".format(sys.version))

if not exists('libnuml'):
  makedirs('libnuml')


class CMakeExtension(Extension):
    """Override the default setuptools extension building."""

    def __init__(self, name, sources=(), **kwargs):
        """Initialize by passing on arguments."""
        # Don't invoke the original `build_ext` for this special extension.
        try: 
          super(CMakeExtension, self).__init__(name=name, sources=list(sources), **kwargs)
        except:
          Extension.__init__(self, name, list(sources), **kwargs)


class CMakeBuild(build_ext):
    """Override `build_ext` to then register it in the command classes."""

    def run(self):
        """
        Call Cmake and build every extension.

        Overrides parent's method.

        """
        for ext in self.extensions:
            self.build_cmake(ext)
        try: 
            super(CMakeBuild, self).run()
        except:
            build_ext.run(self)

    def build_cmake(self, extension):
        """Configure `extension` with CMake and build modules."""
        cwd = os.getcwd()
        build_temp = self.build_temp
        suffix = build_temp[build_temp.find('temp.') + 5:]
        if '/' in suffix:
          suffix = suffix[:suffix.rfind('/')]
        if '\\' in suffix:
          suffix = suffix[:suffix.rfind('\\')]
          
        ext_dir = self.get_ext_fullpath(extension.name)
        makedirs(build_temp)
        target_lib_path = abspath(ext_dir)
        target_dir_path, name = split(target_lib_path)
        makedirs(target_dir_path)
        makedirs(join(cwd, 'libnuml'))

        print ('name: {0}'.format(name))
        print ('build temp: {0}'.format(build_temp))
        print ('extension name: {0}'.format(extension.name))
        print ('extension dir: {0}'.format(ext_dir))
        print ('target_dir_path: {0}'.format(target_dir_path))
        print ('target_lib_path: {0}'.format(target_lib_path))
        print ('suffix: {0}'.format(suffix))
        print ('cwd: {0}'.format(cwd))

        # example of cmake args
        config = 'Debug' if self.debug else 'Release'
        print ('name: {0}, tmp: {1}'.format(name, build_temp))
        is_osx = platform.system() == 'Darwin'
        is_win = platform.system() == 'Windows'
        is_win_32 = is_win and ('win32' in name or 'win32' in build_temp)

        cmake_args = [
            '-DCMAKE_BUILD_TYPE=' + config
        ]
        
        os.environ["CMAKE_BUILD_PARALLEL_LEVEL"] = "4"

        cmake_args = prepend_variables(cmake_args, [
          'CMAKE_CXX_COMPILER', 
          'CMAKE_C_COMPILER', 
          'CMAKE_LINKER', 
          'CMAKE_GENERATOR'
        ])

        if is_win_32:
          if not '-G' in str(cmake_args):
            cmake_args.append('-A')
            cmake_args.append('win32')
        if is_osx: 
          if 'arm64' in suffix: 
            cmake_args.append('-DCMAKE_OSX_ARCHITECTURES=arm64')
          else:
            cmake_args.append('-DCMAKE_OSX_ARCHITECTURES=x86_64')
        if is_win:
          cmake_args.append('-DWITH_STATIC_RUNTIME=ON')

        # example of build args
        build_args = [
            '--config', config,
            '--'
        ]

        global DEP_DIR
        if not DEP_DIR and not self.dry_run:
            print("compiling dependencies")
            dep_suffix = sysconfig.get_platform()
            dep_build_dir = os.path.join(cwd, 'build_dependencies_' + dep_suffix)
            dep_inst_dir = os.path.join(cwd, 'install_dependencies_' + dep_suffix)
            if not exists(dep_inst_dir):
              dep_src_dir = DEP_SRC_DIR
              makedirs(dep_build_dir)
              os.chdir(dep_build_dir)
              print ('compiling libSBML dependencies from: {0}'.format(dep_src_dir))
              self.spawn(['cmake', dep_src_dir] + cmake_args
                         + [
                             '-DCMAKE_INSTALL_PREFIX=' + dep_inst_dir,
                             '-DWITH_BZIP2=ON',
                             '-DWITH_CHECK=OFF',
                             '-DWITH_EXPAT=ON',
                             '-DWITH_XERCES=OFF',
                             '-DWITH_ICONV=OFF',
                             '-DWITH_LIBXML=OFF',
                         ]
                         )
              self.spawn(['cmake', '--build', '.', '--target', 'install'] + build_args)
              
              os.chdir(cwd)
            
              dep_build_dir = os.path.join(cwd, 'build_libSBML_' + dep_suffix)
              makedirs(dep_build_dir)
              os.chdir(dep_build_dir)
              zlib = get_lib_full_path(os.path.join(dep_inst_dir, 'lib'), 'zlib')
              if not zlib: 
                zlib = get_lib_full_path(os.path.join(dep_inst_dir, 'lib'), 'zdll')
              self.spawn(['cmake', DEP_SBML_SRC_DIR] + cmake_args
                         + [
                             '-DCMAKE_INSTALL_PREFIX=' + dep_inst_dir,
                             '-DLIBSBML_DEPENDENCY_DIR=' + dep_inst_dir,
                             '-DLIBEXPAT_INCLUDE_DIR=' + os.path.join(dep_inst_dir, 'include'),
                             '-DLIBEXPAT_LIBRARY=' + get_lib_full_path(os.path.join(dep_inst_dir, 'lib'), 'expat'),
                             #'-DLIBBZ_INCLUDE_DIR=' + os.path.join(dep_inst_dir, 'include'),
                             #'-DLIBBZ_LIBRARY=' + get_lib_full_path(os.path.join(dep_inst_dir, 'lib'), 'bz2'),
                             #'-DLIBZ_INCLUDE_DIR=' + os.path.join(dep_inst_dir, 'include'),
                             #'-DLIBZ_LIBRARY=' + zlib,
                             '-DWITH_ZLIB=OFF',
                             '-DWITH_BZIP2=OFF',
                             '-DWITH_EXPAT=ON',
                             '-DWITH_LIBXML=OFF',
                             '-DLIBSBML_SKIP_SHARED_LIBRARY=ON',
                         ]
                         )
              self.spawn(['cmake', '--build', '.', '--target', 'install'] + build_args)
              
              os.chdir(cwd)
            
            DEP_DIR = dep_inst_dir

        libnuml_args = [
            '-DWITH_SWIG=ON',
            '-DWITH_ZLIB=ON',
            '-DWITH_PYTHON=ON',
            '-DPYTHON_EXECUTABLE=' + sys.executable,
            '-DPYTHON_INCLUDE_DIR=' + get_python_include()
        ]

        libnuml_args = prepend_variables(libnuml_args, [
          'SWIG_DIR',
          'SWIG_EXECUTABLE'
        ])

        if not is_win:
          libnuml_args.append('-DPYTHON_USE_DYNAMIC_LOOKUP=ON')

        cmake_args = cmake_args + libnuml_args
        
        if DEP_DIR:
          cmake_args.append('-DLIBNUML_DEPENDENCY_DIR=' + DEP_DIR)

        if is_win_32:
          if DEP_DIR32:
            cmake_args.append('-DLIBNUML_DEPENDENCY_DIR=' + DEP_DIR32)
        elif is_win:
          if DEP_DIR64:
            cmake_args.append('-DLIBNUML_DEPENDENCY_DIR=' + DEP_DIR64)

        os.chdir(build_temp)
        
        self.spawn(['cmake', SRC_DIR] + cmake_args)
        if not self.dry_run:
            self.spawn(['cmake', '--build', '.', '--target', 'binding_python_lib'] + build_args)
        
            # at this point the build should be complete, and we have all the files 
            # neeed in the temp build_folder
            
            init_py2 = None
            init_py3 = None
            dst_file = join(target_dir_path, '__init__.py')
            
            for root, dirs, files in os.walk(".", topdown=False):
              for name in files:
                # 1. find pyd and copy to target_lib_path
                if name.endswith('.pyd') or name == '_libnuml.so' or name == '_libnuml.dylib':
                  pyd_file = join(root, name)
                  print('copying pyd file to output file')
                  shutil.copyfile(pyd_file, target_lib_path)
                # 2. get scripts and copy to target_lib_path.parent.__init__.py corresponding to version 
                if name == 'libnuml.py':
                  src_file = join(root, name)
                  shutil.copyfile(src_file, dst_file)
                if name == 'libnuml2.py':
                  init_py2 = join(root, name)
                if name == 'libnuml3.py':
                  init_py3 = join(root, name)

            if init_py2 and exists(init_py2) and sys.version_info.major == 2:
                  shutil.copyfile(init_py2, dst_file)
            
            if init_py3 and exists(init_py3) and sys.version_info.major == 3:
                  shutil.copyfile(init_py3, dst_file)
        
        os.chdir(cwd)


setup(name             = "python-libnuml",
      version          = VERSION,
      description      = "LibNuML Python API",
      long_description = ("libNuML is a library for reading, writing and "+
                          "manipulating NuML.  It is written in ISO C and C++, supports "+
                          "NuML Level 1, Version 1, and runs on Linux, Microsoft "+
                          "Windows, and Apple MacOS X.  For more information "+
                          "about SEDML, please see http://github.com/numl/numl/."),
      license          = "BSD",
      author           = "Frank T. Bergmann",
      author_email     = "fbergman@caltech.edu",
      url              = "https://github.com/NuML/NuML/tree/master/libnuml",
      packages         = ["libnuml"],
      package_dir      = {'libnuml': 'libnuml'},
      ext_package      = "libnuml",
      ext_modules=[CMakeExtension('_libnuml')],
      cmdclass={
        'build_ext': CMakeBuild,
      }
)
