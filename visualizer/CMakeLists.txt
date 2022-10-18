cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES 
	visualizer/AIPlayer.cpp
	visualizer/Evaluator.cpp
	visualizer/Game.cpp
	visualizer/GameWindow.cpp
	visualizer/main.cpp
	visualizer/WindowPlayer.cpp
)
set(RESOURCES
	visualizer/res/font.ttf
	visualizer/res/black.model
	visualizer/res/white.model
)

project(visualizer VERSION 1.0)
add_executable(visualizer ${SOURCES})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(visualizer PRIVATE Threads::Threads)

	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		find_library(SFML_GRAPHICS NAMES sfml-graphics-d REQUIRED)
		find_library(SFML_WINDOW NAMES sfml-window-d REQUIRED)
		find_library(SFML_SYSTEM NAMES sfml-system-d REQUIRED)
	else()
		find_library(SFML_GRAPHICS NAMES sfml-graphics REQUIRED)
		find_library(SFML_WINDOW NAMES sfml-window REQUIRED)
		find_library(SFML_SYSTEM NAMES sfml-system REQUIRED)
	endif()
else()
	find_library(FREETYPE NAMES freetype REQUIRED)
	find_library(OPENGL32 NAMES opengl32 REQUIRED)
	find_library(WINMM NAMES winmm REQUIRED)
	find_library(GDI32 NAMES gdi32 REQUIRED)
	
	target_link_libraries(visualizer PRIVATE ${FREETYPE})
	target_link_libraries(visualizer PRIVATE ${OPENGL32})
	target_link_libraries(visualizer PRIVATE ${WINMM})
	target_link_libraries(visualizer PRIVATE ${GDI32})

	target_compile_definitions(visualizer PRIVATE SFML_STATIC)

	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		find_library(SFML_GRAPHICS NAMES sfml-graphics-s-d REQUIRED)
		find_library(SFML_WINDOW NAMES sfml-window-s-d REQUIRED)
		find_library(SFML_SYSTEM NAMES sfml-system-s-d REQUIRED)
	else()
		find_library(SFML_GRAPHICS NAMES sfml-graphics-s REQUIRED)
		find_library(SFML_WINDOW NAMES sfml-window-s REQUIRED)
		find_library(SFML_SYSTEM NAMES sfml-system-s REQUIRED)
	endif()
endif()

target_link_libraries(visualizer PRIVATE ${SFML_GRAPHICS})
target_link_libraries(visualizer PRIVATE ${SFML_WINDOW})
target_link_libraries(visualizer PRIVATE ${SFML_SYSTEM})

find_package(Torch REQUIRED)
target_link_libraries(visualizer PRIVATE ${TORCH_LIBRARIES})
target_include_directories(visualizer PRIVATE ${TORCH_INCLUDE_DIRS})
target_compile_options(visualizer PRIVATE ${TORCH_CXX_FLAGS})

target_link_libraries(visualizer PRIVATE common_modules)
target_include_directories(visualizer PRIVATE ${CMAKE_SOURCE_DIR})

install(TARGETS visualizer DESTINATION .)
install(FILES ${RESOURCES} DESTINATION ./res)
