####################################################################
#
# SBML req package - include files to build req
#
# $Author$
# $Id$
# $HeadURL$
#

if(ENABLE_REQUIREDELEMENTS)


include(${CMAKE_SOURCE_DIR}/req-package.cmake)

#build up sources
set(REQ_SOURCES)

# go through all directtories: common, extension and sbml
foreach(dir common extension sbml)
	
	# file sources
	file(GLOB current ${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/req/${dir}/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/req/${dir}/*.h)
	
	# add sources 
	set(REQ_SOURCES ${REQ_SOURCES} ${current})
	
	# mark header files for installation 
	file(GLOB req_headers ${CMAKE_CURRENT_SOURCE_DIR}/sbml/packages/req/${dir}/*.h)
    install(FILES ${req_headers} DESTINATION include/sbml/packages/req/${dir})	
	
endforeach()

# create source group for IDEs
source_group(req_package FILES ${REQ_SOURCES})

# add req sources to SBML sources
SET(LIBSBML_SOURCES ${LIBSBML_SOURCES} ${REQ_SOURCES})

#####################################################################
##
## add test scripts
##
#
#if(WITH_CHECK)
#
#	add_subdirectory(packages/req/extension/test)
#
#endif()

endif()