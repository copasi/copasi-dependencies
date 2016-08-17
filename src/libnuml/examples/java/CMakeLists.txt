###############################################################################
#
# Description       : CMake build script for libNUML Java examples
#
###############################################################################

find_package(Java COMPONENTS Development REQUIRED)


foreach( example
	
	createNUML
	echoNUML
	printNUML
	
)

	file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR} CURRENT_BIN)
	file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/../../src/bindings/java/libnumlj.jar CURRENT_JAR )
	file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${example}.java CURRENT_FILE)

	add_custom_command(	
		OUTPUT ${example}.class
		COMMAND "${Java_JAVAC_EXECUTABLE}"
		ARGS -cp ".${FILE_SEP}\"${CURRENT_JAR}\"${FILE_SEP}${CMAKE_CURRENT_SOURCE_DIR}"
			 ${CURRENT_FILE}
			 -source 1.5
			 -target 1.5
			 -d ${CMAKE_CURRENT_BINARY_DIR}
		MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/../../src/bindings/java/local.i"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${example}.java
		COMMENT "Build java example ${example}"
	) 
	
	add_custom_target(example_java_${example} ALL 
		DEPENDS 
		${CMAKE_CURRENT_BINARY_DIR}/${example}.class 
		SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/${example}.java 
	)
	
	add_dependencies(example_java_${example} binding_java_jar)

	
endforeach()
