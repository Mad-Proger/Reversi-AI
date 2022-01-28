cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_definitions(SFML_STATIC)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(DEPENDENCIES_PREFIX /usr/local)
else()
	set(DEPENDENCIES_PREFIX C:/Libraries)
endif()

set(LIBDIR ${DEPENDENCIES_PREFIX}/lib)
set(INCLUDEDIR ${DEPENDENCIES_PREFIX}/include)
set(KERAS2CPP_DIR "../keras2cpp")
set(SOURCES AIPlayer.cpp Desk.cpp Evaluator.cpp Game.cpp GameWindow.cpp main.cpp Matrix.cpp Model.cpp WindowPlayer.cpp)
set(KERAS2CPP_SOURCES 
	${KERAS2CPP_DIR}/src/utils.cc
    ${KERAS2CPP_DIR}/src/baseLayer.cc
    ${KERAS2CPP_DIR}/src/layers/activation.cc
    ${KERAS2CPP_DIR}/src/layers/conv1d.cc
    ${KERAS2CPP_DIR}/src/layers/conv2d.cc
    ${KERAS2CPP_DIR}/src/layers/dense.cc
    ${KERAS2CPP_DIR}/src/layers/elu.cc
    ${KERAS2CPP_DIR}/src/layers/embedding.cc
    ${KERAS2CPP_DIR}/src/layers/flatten.cc
    ${KERAS2CPP_DIR}/src/layers/lstm.cc
    ${KERAS2CPP_DIR}/src/layers/locally1d.cc
    ${KERAS2CPP_DIR}/src/layers/locally2d.cc
    ${KERAS2CPP_DIR}/src/layers/maxPooling2d.cc
    ${KERAS2CPP_DIR}/src/layers/batchNormalization.cc
    ${KERAS2CPP_DIR}/src/model.cc
    ${KERAS2CPP_DIR}/src/tensor.cc
)

project(Visualizer)
add_executable(Visualizer ${SOURCES})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	find_library(SFML_GRAPHICS NAMES sfml-graphics PATHS ${LIBDIR} REQUIRED)
	find_library(SFML_WINDOW NAMES sfml-window PATHS ${LIBDIR} REQUIRED)
	find_library(SFML_SYSTEM NAMES sfml-system PATHS ${LIBDIR} REQUIRED)
	find_library(PTHREAD NAMES pthread PATHS ${LIBDIR} REQUIRED)

	target_link_libraries(Visualizer PRIVATE ${SFML_GRAPHICS})
	target_link_libraries(Visualizer PRIVATE ${SFML_WINDOW})
	target_link_libraries(Visualizer PRIVATE ${SFML_SYSTEM})
	target_link_libraries(Visualizer PRIVATE ${PTHREAD})
else()
	find_library(FREETYPE NAMES freetype PATHS ${LIBDIR} REQUIRED)
	find_library(OPENGL32 NAMES opengl32 PATHS ${LIBDIR} REQUIRED)
	find_library(WINMM NAMES winmm PATHS ${LIBDIR} REQUIRED)
	find_library(GDI32 NAMES gdi32 PATHS ${LIBDIR} REQUIRED)

	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		find_library(SFML_GRAPHICS NAMES sfml-graphics-s-d PATHS ${LIBDIR} REQUIRED)
		find_library(SFML_WINDOW NAMES sfml-window-s-d PATHS ${LIBDIR} REQUIRED)
		find_library(SFML_SYSTEM NAMES sfml-system-s-d PATHS ${LIBDIR} REQUIRED)
	else()
		find_library(SFML_GRAPHICS NAMES sfml-graphics-s PATHS ${LIBDIR} REQUIRED)
		find_library(SFML_WINDOW NAMES sfml-window-s PATHS ${LIBDIR} REQUIRED)
		find_library(SFML_SYSTEM NAMES sfml-system-s PATHS ${LIBDIR} REQUIRED)
	endif()

	target_link_libraries(Visualizer PRIVATE ${FREETYPE})
	target_link_libraries(Visualizer PRIVATE ${OPENGL32})
	target_link_libraries(Visualizer PRIVATE ${WINMM})
	target_link_libraries(Visualizer PRIVATE ${GDI32})
	target_link_libraries(Visualizer PRIVATE ${SFML_GRAPHICS})
	target_link_libraries(Visualizer PRIVATE ${SFML_WINDOW})
	target_link_libraries(Visualizer PRIVATE ${SFML_SYSTEM})
endif()

target_include_directories(Visualizer PRIVATE ${INCLUDEDIR})
target_include_directories(Visualizer PRIVATE ${KERAS2CPP_DIR})
target_sources(Visualizer PRIVATE ${KERAS2CPP_SOURCES})
