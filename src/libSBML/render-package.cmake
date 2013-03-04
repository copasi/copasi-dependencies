####################################################################
#
# SBML RENDER package 
#

option(ENABLE_RENDER     "Enable RENDER package"    OFF )

if(ENABLE_RENDER)	
	if (NOT ENABLE_LAYOUT)
		# instead simply force layout to be true, as it will have to be present!
		message(WARNING " The layout package is required to build the render package, setting ENABLE_LAYOUT to ON")
		set(ENABLE_LAYOUT ON CACHE BOOL "Enable Layout package" FORCE)
	endif()
    SET(USE_RENDER TRUE)
	set(LIBSBML_PACKAGE_INCLUDES ${LIBSBML_PACKAGE_INCLUDES} "LIBSBML_HAS_PACKAGE_RENDER")
	mark_as_advanced(USE_RENDER)
	add_definitions(-DUSE_RENDER)
	list(APPEND SWIG_EXTRA_ARGS -DUSE_RENDER)
endif()
