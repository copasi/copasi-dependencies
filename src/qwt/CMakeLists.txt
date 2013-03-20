# CMake Build script for qwt
# 
# Frank T. Bergmann 
# 
cmake_minimum_required (VERSION 2.6)
project (qwt)

include_directories("${PROJECT_SOURCE_DIR}")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "qwt library")
set(CPACK_PACKAGE_NAME "qwt")
set(CPACK_PACKAGE_VERSION_MAJOR "5")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_CURRENT_BINARY_DIR};/.svn/;/.libs/;/.deps/;/.bzr/;.*.o$;.*.lo$;.*.la$;${CPACK_SOURCE_IGNORE_FILES};/.DS_Store;/.svnignore;blib;libsbml-dist")

if(UNIX)
    set(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
    set(CPACK_SET_DESTDIR "ON")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
else()
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
endif()

include(CPack)

# are we using the intel compiler
set(USING_INTEL)
if (WIN32 AND CMAKE_C_COMPILER AND ${CMAKE_C_COMPILER} MATCHES ".*icl.*$")
  message(STATUS "Detected Intel Compiler")
  set(USING_INTEL TRUE)
endif ()

# if using msvc or intel windows compiler allow for the use of the static runtime
if(MSVC OR USING_INTEL)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
    set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -D_CRT_SECURE_NO_WARNINGS")
    option(WITH_STATIC_RUNTIME "Compile using the static MSVC Runtime." OFF)
    if(WITH_STATIC_RUNTIME)
        foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)

            if(${flag_var} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            endif(${flag_var} MATCHES "/MD")
        endforeach(flag_var)
    add_definitions( -D_MT)
    endif(WITH_STATIC_RUNTIME)
endif(MSVC OR USING_INTEL)

option(WITH_QwtPlot "Compile with QwtPlot." ON)
option(WITH_QwtWidgets "Compile with QwtWidgets." ON)
option(WITH_QwtSVGItem "Compile with QwtSVGItem." OFF)
option(WITH_QwtMathML "Compile with QwtMathML." OFF)


set(HEADERS 
    src/qwt.h 
    src/qwt_abstract_scale_draw.h 
    src/qwt_array.h 
    src/qwt_color_map.h 
    src/qwt_clipper.h 
    src/qwt_double_interval.h 
    src/qwt_double_rect.h 
    src/qwt_dyngrid_layout.h 
    src/qwt_global.h 
    src/qwt_layout_metrics.h 
    src/qwt_math.h 
    src/qwt_magnifier.h 
    src/qwt_paint_buffer.h 
    src/qwt_painter.h 
    src/qwt_panner.h 
    src/qwt_picker.h 
    src/qwt_picker_machine.h 
    src/qwt_polygon.h 
    src/qwt_round_scale_draw.h 
    src/qwt_scale_div.h 
    src/qwt_scale_draw.h 
    src/qwt_scale_engine.h 
    src/qwt_scale_map.h 
    src/qwt_spline.h 
    src/qwt_symbol.h 
    src/qwt_text_engine.h 
    src/qwt_text_label.h 
    src/qwt_text.h 
    src/qwt_valuelist.h
)

set(SOURCES

    src/qwt_abstract_scale_draw.cpp 
    src/qwt_color_map.cpp 
    src/qwt_clipper.cpp 
    src/qwt_double_interval.cpp 
    src/qwt_double_rect.cpp 
    src/qwt_dyngrid_layout.cpp 
    src/qwt_layout_metrics.cpp 
    src/qwt_math.cpp 
    src/qwt_magnifier.cpp 
    src/qwt_paint_buffer.cpp 
    src/qwt_panner.cpp 
    src/qwt_painter.cpp 
    src/qwt_picker.cpp 
    src/qwt_round_scale_draw.cpp 
    src/qwt_scale_div.cpp 
    src/qwt_scale_draw.cpp 
    src/qwt_scale_map.cpp 
    src/qwt_spline.cpp 
    src/qwt_text_engine.cpp 
    src/qwt_text_label.cpp 
    src/qwt_text.cpp 
    src/qwt_event_pattern.cpp 
    src/qwt_picker_machine.cpp 
    src/qwt_scale_engine.cpp 
    src/qwt_symbol.cpp
)
 
if (WITH_QwtPlot)

	set(HEADERS ${HEADERS}
	
        src/qwt_curve_fitter.h 
        src/qwt_data.h 
        src/qwt_event_pattern.h 
        src/qwt_interval_data.h 
        src/qwt_legend.h 
        src/qwt_legend_item.h 
        src/qwt_legend_itemmanager.h 
        src/qwt_plot.h 
        src/qwt_plot_curve.h 
        src/qwt_plot_dict.h 
        src/qwt_plot_grid.h 
        src/qwt_plot_item.h 
        src/qwt_plot_layout.h 
        src/qwt_plot_marker.h 
        src/qwt_plot_printfilter.h 
        src/qwt_plot_rasteritem.h 
        src/qwt_plot_spectrogram.h 
        src/qwt_plot_scaleitem.h 
        src/qwt_plot_canvas.h 
        src/qwt_plot_rescaler.h 
        src/qwt_plot_panner.h 
        src/qwt_plot_picker.h 
        src/qwt_plot_zoomer.h 
        src/qwt_plot_magnifier.h 
        src/qwt_raster_data.h 
        src/qwt_scale_widget.h 
	)

    set(SOURCES ${SOURCES}
	
        src/qwt_curve_fitter.cpp 
        src/qwt_data.cpp 
        src/qwt_interval_data.cpp 
        src/qwt_legend.cpp 
        src/qwt_legend_item.cpp 
        src/qwt_plot.cpp 
        src/qwt_plot_print.cpp 
        src/qwt_plot_xml.cpp 
        src/qwt_plot_axis.cpp 
        src/qwt_plot_curve.cpp 
        src/qwt_plot_dict.cpp 
        src/qwt_plot_grid.cpp 
        src/qwt_plot_item.cpp 
        src/qwt_plot_spectrogram.cpp 
        src/qwt_plot_scaleitem.cpp 
        src/qwt_plot_marker.cpp 
        src/qwt_plot_layout.cpp 
        src/qwt_plot_printfilter.cpp 
        src/qwt_plot_rasteritem.cpp 
        src/qwt_plot_canvas.cpp 
        src/qwt_plot_rescaler.cpp 
        src/qwt_plot_panner.cpp 
        src/qwt_plot_picker.cpp 
        src/qwt_plot_zoomer.cpp 
        src/qwt_plot_magnifier.cpp 
        src/qwt_raster_data.cpp 
        src/qwt_scale_widget.cpp 
	)

endif (WITH_QwtPlot)

if (WITH_QwtSVGItem) 

	set(HEADERS ${HEADERS} src/qwt_plot_svgitem.h)
	set(SOURCES ${SOURCES} src/qwt_plot_svgitem.cpp)

endif (WITH_QwtSVGItem) 

if (WITH_QwtWidgets) 

    set(HEADERS ${HEADERS}
	
        src/qwt_abstract_slider.h 
        src/qwt_abstract_scale.h 
        src/qwt_arrow_button.h 
        src/qwt_analog_clock.h 
        src/qwt_compass.h 
        src/qwt_compass_rose.h 
        src/qwt_counter.h 
        src/qwt_dial.h 
        src/qwt_dial_needle.h 
        src/qwt_double_range.h 
        src/qwt_knob.h 
        src/qwt_slider.h 
        src/qwt_thermo.h 
        src/qwt_wheel.h
	)
    
    set(SOURCES ${SOURCES}
	
        src/qwt_abstract_slider.cpp 
        src/qwt_abstract_scale.cpp 
        src/qwt_arrow_button.cpp 
        src/qwt_analog_clock.cpp 
        src/qwt_compass.cpp 
        src/qwt_compass_rose.cpp 
        src/qwt_counter.cpp 
        src/qwt_dial.cpp 
        src/qwt_dial_needle.cpp 
        src/qwt_double_range.cpp 
        src/qwt_knob.cpp 
        src/qwt_slider.cpp 
        src/qwt_thermo.cpp 
        src/qwt_wheel.cpp
	)

endif (WITH_QwtWidgets) 


if (WITH_QwtSVGItem)
find_package(Qt4 4.7.0 REQUIRED QtCore QtOpenGL QtSvg)
else()
find_package(Qt4 4.7.0 REQUIRED QtCore QtOpenGL QtXml)
endif (WITH_QwtSVGItem)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(BEFORE ${QT_QTGUI_INCLUDE_DIR})
include_directories(BEFORE ${QT_QTGUI_INCLUDE_DIR}/..)

add_library(qwt STATIC ${SOURCES} ${HEADERS})

target_link_libraries(qwt ${QT_LIBRARIES} )

install(FILES ${HEADERS} DESTINATION include)
INSTALL(TARGETS qwt 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )
