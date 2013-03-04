####################################################################
#
# SBML Required Elements package 
#
# $Author$
# $Id$
# $HeadURL$
#

option(ENABLE_REQUIREDELEMENTS     "Enable required elements package"    OFF )

if(ENABLE_REQUIREDELEMENTS)
	add_definitions(-DUSE_REQUIREDELEMENTS)
	set(LIBSBML_PACKAGE_INCLUDES ${LIBSBML_PACKAGE_INCLUDES} "LIBSBML_HAS_PACKAGE_REQ")
	list(APPEND SWIG_EXTRA_ARGS -DUSE_REQUIREDELEMENTS)	
endif()
