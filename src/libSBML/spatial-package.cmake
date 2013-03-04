####################################################################
#
# SBML Spatial package 
#
# $Author$
# $Id$
# $HeadURL$
#

option(ENABLE_SPATIAL     "Enable spatial package"    OFF )

if(ENABLE_SPATIAL)
	if (NOT ENABLE_REQUIREDELEMENTS)
		message(FATAL "The spatial package requires the 'required elements package'")
	endif()
	add_definitions(-DUSE_SPATIAL)
	set(LIBSBML_PACKAGE_INCLUDES ${LIBSBML_PACKAGE_INCLUDES} "LIBSBML_HAS_PACKAGE_SPATIAL")
	list(APPEND SWIG_EXTRA_ARGS -DUSE_SPATIAL)	
endif()
