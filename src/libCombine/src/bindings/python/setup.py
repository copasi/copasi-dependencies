## @file    setup.py
## @brief   Python distutils code for libCombine Python module (including dependencies)
## @author  Michael Hucka
## @author  Ben Bornstein
## @author  Ben Kovitz
## @author  Frank Bergmann (fbergman@caltech.edu)

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
  include_dir = sysconfig.get_paths()['include']
  if exists(include_dir):
    return include_dir
  include_dir = include_dir.replace('/local', '/')
  if exists(include_dir):
    return include_dir
  return ''

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
SRC_DIR = get_dir_if_exists('LIBCOMBINE_SRC_DIR', '.')
global DEP_DIR
cwd = os.getcwd()
dep_suffix = sysconfig.get_platform()
DEP_DIR = get_dir_if_exists('LIBCOMBINE_DEP_DIR', os.path.join(cwd, 'install_dependencies_' + dep_suffix))
DEP_SRC_DIR = get_dir_if_exists('LIBCOMBINE_DEP_SRC', './submodules/libSBML-dependencies/')
DEP_SBML_SRC_DIR = get_dir_if_exists('LIBCOMBINE_SBML_DEP_SRC', './submodules/libSBML/')
DEP_ZIP_SRC_DIR = get_dir_if_exists('LIBCOMBINE_NUML_DEP_SRC', './submodules/zipper/')
DEP_DIR32 = get_dir_if_exists('LIBCOMBINE_DEP_DIR_32', '../win_libcombine_dependencies_32/')
DEP_DIR64 = get_dir_if_exists('LIBCOMBINE_DEP_DIR_64', '../win_libcombine_dependencies_64/')


if not SRC_DIR:
  src_defined = os.getenv('LIBCOMBINE_SRC_DIR')
  if src_defined:
    raise ValueError("LibCombine Source defined as: {0}, but coun't be found".format(src_defined))
  else:
    raise ValueError("LibCombine Source not specified or not present, define LIBCOMBINE_SRC_DIR.")

print ("Using libCombine from: {0}".format(SRC_DIR))

version_file_name = join(SRC_DIR, 'VERSION.txt')
print ("Using VERSION.txt: {0}".format(version_file_name))

if not exists(version_file_name):
  print(os.listdir(SRC_DIR))
  raise ValueError("Invalid libCombine Source directory, no VERSION.txt file")

with open(version_file_name, 'r') as version_file:
  VERSION = version_file.readline().strip()

print ("Version is: {0}".format(VERSION))
print ("building for python: {0}".format(sys.version))

if not exists('libcombine'):
  makedirs('libcombine')


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
        makedirs(join(cwd, 'libcombine'))

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
  
        os.environ["CMAKE_BUILD_PARALLEL_LEVEL"] = "4"

        cmake_args = [
            '-DCMAKE_BUILD_TYPE=' + config
        ]

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
            dep_build_dir = os.path.join(cwd, 'build_dependencies_' + dep_suffix)
            dep_inst_dir = os.path.join(cwd, 'install_dependencies_' + dep_suffix)
            dep_src_dir = DEP_SRC_DIR
            makedirs(dep_build_dir)
            os.chdir(dep_build_dir)
            print ('compiling libSBML dependencies from: {0}'.format(dep_src_dir))
            self.spawn(['cmake', dep_src_dir] + cmake_args
                       + [
                           '-DCMAKE_INSTALL_PREFIX=' + dep_inst_dir,
                           '-DWITH_BZIP2=OFF',
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
                           '-DWITH_ZLIB=ON',
                           '-DWITH_BZIP2=OFF',
                           '-DWITH_EXPAT=ON',
                           '-DWITH_LIBXML=OFF',
                           '-DLIBSBML_SKIP_SHARED_LIBRARY=ON',
                       ]
                       )
            self.spawn(['cmake', '--build', '.', '--target', 'install'] + build_args)
            
            os.chdir(cwd)
            
            dep_build_dir = os.path.join(cwd, 'build_zipper_' + dep_suffix)
            makedirs(dep_build_dir)
            os.chdir(dep_build_dir)
            zlib = get_lib_full_path(os.path.join(dep_inst_dir, 'lib'), 'zlib')
            if not zlib: 
              zlib = get_lib_full_path(os.path.join(dep_inst_dir, 'lib'), 'zdll')
            self.spawn(['cmake', DEP_ZIP_SRC_DIR] + cmake_args
                       + [
                           '-DCMAKE_INSTALL_PREFIX=' + dep_inst_dir,
                           '-DZIPPER_DEPENDENCY_DIR=' + dep_inst_dir,
                           '-DLIBZ_INCLUDE_DIR=' + os.path.join(dep_inst_dir, 'include'),
                           '-DLIBZ_LIBRARY=' + zlib,
                           '-DBUILD_SHARED_VERSION=OFF',
                           '-DBUILD_STATIC_VERSION=ON',
                           '-DBUILD_TEST=OFF',
                       ]
                       )
            self.spawn(['cmake', '--build', '.', '--target', 'install'] + build_args)
            
            os.chdir(cwd)
            
            DEP_DIR = dep_inst_dir

        libcombine_args = [
            '-DWITH_SWIG=ON',
            '-DWITH_PYTHON=ON',
            '-DPYTHON_EXECUTABLE=' + sys.executable,
            '-DPYTHON_INCLUDE_DIR=' + get_python_include()
        ]

        libcombine_args = prepend_variables(libcombine_args, [
          'SWIG_DIR',
          'SWIG_EXECUTABLE'
        ])

        if not is_win:
          libcombine_args.append('-DPYTHON_USE_DYNAMIC_LOOKUP=ON')

        cmake_args = cmake_args + libcombine_args
        
        if DEP_DIR:
          cmake_args.append('-DLIBCOMBINE_DEPENDENCY_DIR=' + DEP_DIR)

        if is_win_32:
          if DEP_DIR32:
            cmake_args.append('-DLIBCOMBINE_DEPENDENCY_DIR=' + DEP_DIR32)
        elif is_win:
          if DEP_DIR64:
            cmake_args.append('-DLIBCOMBINE_DEPENDENCY_DIR=' + DEP_DIR64)

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
                if name.endswith('.pyd') or name == '_libcombine.so' or name == '_libcombine.dylib':
                  pyd_file = join(root, name)
                  print('copying pyd file to output file')
                  shutil.copyfile(pyd_file, target_lib_path)
                # 2. get scripts and copy to target_lib_path.parent.__init__.py corresponding to version 
                if name == 'libcombine.py':
                  src_file = join(root, name)
                  shutil.copyfile(src_file, dst_file)
                if name == 'libcombine2.py':
                  init_py2 = join(root, name)
                if name == 'libcombine3.py':
                  init_py3 = join(root, name)

            if init_py2 and exists(init_py2) and sys.version_info.major == 2:
                  shutil.copyfile(init_py2, dst_file)
            
            if init_py3 and exists(init_py3) and sys.version_info.major == 3:
                  shutil.copyfile(init_py3, dst_file)
        
        os.chdir(cwd)


setup(name             = "python-libcombine",
      version          = VERSION,
      description      = "LibCombine Python API",
      long_description = ("LibCombine implements a C++ API library providing support for the Combine Archive.  For more information about COMBINE, please see https://github.com/sbmlteam/libcombine."),
      license          = "BSD",
      author           = "Frank T. Bergmann",
      author_email     = "fbergman@caltech.edu",
      url              = "https://github.com/sbmlteam/libcombine",
      packages         = ["libcombine"],
      package_dir      = {'libcombine': 'libcombine'},
      ext_package      = "libcombine",
      ext_modules=[CMakeExtension('_libcombine')],
      cmdclass={
        'build_ext': CMakeBuild,
      }
)
