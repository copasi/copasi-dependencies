###############################################################################
#
# Description       : CMake build script for libNUML C++ examples
#
###############################################################################


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src)
include_directories(BEFORE ${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

foreach(example 

	createNUML
	echoNUML
	printNUML
	
)
	add_executable(example_cpp_${example} ${example}.cpp)
	set_target_properties(example_cpp_${example} PROPERTIES  OUTPUT_NAME ${example})
	if (WIN32 AND NOT CYGWIN)
	set_target_properties(example_cpp_${example} PROPERTIES COMPILE_DEFINITIONS "LIBNUML_STATIC=1")
	endif()
	target_link_libraries(example_cpp_${example} ${LIBNUML_LIBRARY}-static)

endforeach()
