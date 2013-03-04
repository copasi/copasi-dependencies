cmake_minimum_required (VERSION 2.6)
project (cppunit)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include/cppunit")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/cppunit")
include_directories("${PROJECT_SOURCE_DIR}/config")

file(GLOB SOURCES 			
			${PROJECT_SOURCE_DIR}/src/cppunit/*.cpp
) 
file(GLOB_RECURSE HEADERS RELATIVE	${PROJECT_SOURCE_DIR}		
			${PROJECT_SOURCE_DIR}/include/cppunit/*.h
) 


add_library(cppunit ${SOURCES} ${HEADERS})
install(TARGETS cppunit
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib  
)

install(DIRECTORY include/cppunit DESTINATION include FILES_MATCHING PATTERN *.h)
install(FILES config/config.h DESTINATION include/cppunit/config)
