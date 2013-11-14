####################################################################
#
# SBML spatial package - include files to build spatial
#
# $Author$
# $Id$
# $HeadURL$
#

if(ENABLE_SPATIAL)

include(${CMAKE_SOURCE_DIR}/spatial-package.cmake)

#build up sources
set(SPATIAL_SOURCES)

# go through all directtories: common, extension and sbml
foreach(dir common extension sbml)

	# add to include directory
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/spatial/${dir})
	
	# file sources
	file(GLOB current ${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/spatial/${dir}/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/spatial/${dir}/*.h
	)
	
        # Set the *Constraints.cpp files to be 'header' files so they won't be compiled--
        #  they are #included directly, instead.
        if ("${dir}" STREQUAL "validator/constraints")
            foreach(tempFile ${current})
                if ("${tempFile}" MATCHES ".*Constraints.cpp")
                    set_source_files_properties(
                        ${tempFile}
                        PROPERTIES HEADER_FILE_ONLY true
                        )
                endif()
            endforeach()
        endif()

	# add sources 
	set(SPATIAL_SOURCES ${SPATIAL_SOURCES} ${current})
	
	# mark header files for installation 
	file(GLOB spatial_headers ${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/spatial/${dir}/*.h)
    install(FILES ${spatial_headers} DESTINATION include/sbml/packages/spatial/${dir})	
		
endforeach()


# create source group for IDEs
source_group(spatial_package FILES ${SPATIAL_SOURCES})

# add spatial sources to SBML sources
SET(LIBSBML_SOURCES ${LIBSBML_SOURCES} ${SPATIAL_SOURCES})

####################################################################
#
# add test scripts
#
if(WITH_CHECK)

	add_subdirectory(sbml/packages/spatial/extension/test)

endif()

endif()
