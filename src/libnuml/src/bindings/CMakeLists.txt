###############################################################################
#
# Description       : CCMake build script for libNUML bindings
# Original author(s): Joseph O. Dada <joseph.dada@manchester.ac.uk>
# Organization      : University of Manchester
#
# This file is part of libNUML.  Please visit http://code.google.com/p/numl/ for more
# information about NUML, and the latest version of libNUML.
#
# Copyright (C) 2013: 
#	The University of Manchester, UK
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation.  A copy of the license agreement is provided
# in the file named "LICENSE.txt" included with this software distribution
#
###############################################################################


#
# When checked removes all generated SWIG wrappers upon configure
#
option(LIBNUML_REMOVE_WRAPPERS   "Remove generated SWIG wrappers." OFF)
mark_as_advanced(LIBNUML_REMOVE_WRAPPERS)

include_directories(BEFORE ${CMAKE_BINARY_DIR}/include)

#
# and build selected language bindings
#

#if(WITH_CSHARP)
#add_subdirectory(csharp)
#endif()

if(WITH_JAVA)
add_subdirectory(java)
endif()

if(WITH_PYTHON)
add_subdirectory(python)
endif()


#if(WITH_PERL)
#add_subdirectory(perl)
#endif()

#if(WITH_RUBY)
#add_subdirectory(ruby)
#endif()

#if(WITH_R)
#add_subdirectory(r)
#endif()

