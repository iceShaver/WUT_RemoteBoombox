cmake_minimum_required(VERSION 3.5)

project(RemoteBoomBox VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS PrintSupport)
find_package(Qt6 REQUIRED COMPONENTS Charts)

add_library(FFTW3 fftw/libfftw3-3.lib fftw/libfftw3f-3.lib fftw/libfftw3l-3.lib)

include_directories("${PROJECT_SOURCE_DIR}/fftw")


set(PROJECT_SOURCES
		main.cpp
		mainwindow.cpp
		mainwindow.h
		mainwindow.ui
		apploader.h apploader.cpp
		serverwindow.h serverwindow.cpp serverwindow.ui
		clientwindow.h clientwindow.cpp clientwindow.ui
		serverapp.h serverapp.cpp
		clientapp.h clientapp.cpp
		app.h app.cpp

		soundcollector.h soundcollector.cpp
		communicationmodule.h communicationmodule.cpp
		visualization.h visualization.cpp
		qcustomplot.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
	qt_add_executable(RemoteBoomBox
		MANUAL_FINALIZATION
		${PROJECT_SOURCES}
		visualizationspectogram.h visualizationspectogram.cpp
		visualizationcolor.h visualizationcolor.cpp
		visualizationvolumebar.h visualizationvolumebar.cpp
		config.h
		config.c

	)
endif()

target_link_libraries(RemoteBoomBox PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(RemoteBoomBox PRIVATE Qt6::Network)
target_link_libraries(RemoteBoomBox PRIVATE Qt6::Multimedia)
target_link_libraries(RemoteBoomBox PRIVATE Qt6::Gui)
target_link_libraries(RemoteBoomBox PRIVATE Qt6::Core)
target_link_libraries(RemoteBoomBox PUBLIC ${PROJECT_SOURCE_DIR}/fftw/libfftw3-3.lib)
target_link_libraries(RemoteBoomBox PUBLIC ${PROJECT_SOURCE_DIR}/fftw/libfftw3f-3.lib)
target_link_libraries(RemoteBoomBox PUBLIC ${PROJECT_SOURCE_DIR}/fftw/libfftw3l-3.lib)
target_link_libraries(RemoteBoomBox PRIVATE Qt6::PrintSupport)
target_link_libraries(RemoteBoomBox PRIVATE Qt6::Charts)
set_target_properties(RemoteBoomBox PROPERTIES
	${BUNDLE_ID_OPTION}
	WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RemoteBoomBox
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(RemoteBoomBox)
endif()
