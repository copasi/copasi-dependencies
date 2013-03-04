cmake_minimum_required (VERSION 2.6)
project (raptor)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/librdfa")

file(GLOB SOURCES 
			${PROJECT_SOURCE_DIR}/librdfa/*.c
			${PROJECT_SOURCE_DIR}/src/*.c
) 

list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/raptor_grddl.c)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/raptor_librdfa.c)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/raptor_nfc_test.c)

find_package(EXPAT REQUIRED)
include_directories(BEFORE ${EXPAT_INCLUDE_DIR})

add_definitions(-DXML_STATIC -DRAPTOR_STATIC -DRAPTOR_V2_EXPERIMENTAL -DHAVE_ERRNO_H -DRAPTOR_INTERNAL -DWIN32 )

add_library(raptor ${SOURCES} ${HEADERS})
target_link_libraries(raptor ${EXPAT_LIBRARY} )

install(TARGETS raptor
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )

file(GLOB HEADERS src/*.h ) 
  
install(FILES ${HEADERS} DESTINATION include)


