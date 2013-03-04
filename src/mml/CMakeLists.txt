cmake_minimum_required (VERSION 2.6)
project (mml)

include_directories("${PROJECT_SOURCE_DIR}")

set(SOURCES qtmmlwidget.cpp)
set(HEADERS qtmmlwidget.h)

find_package(Qt4 4.7.0 REQUIRED QtCore QtOpenGL QtXml)
include_directories(BEFORE ${QT_QTGUI_INCLUDE_DIR})
include_directories(BEFORE ${QT_QTGUI_INCLUDE_DIR}/..)

add_library(mml STATIC ${SOURCES} ${HEADERS})

target_link_libraries(mml ${QT_LIBRARIES} )

install(FILES ${HEADERS} DESTINATION include)
INSTALL(TARGETS mml 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )
