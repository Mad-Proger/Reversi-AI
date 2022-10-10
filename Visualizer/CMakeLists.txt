cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES 
	AIPlayer.cpp
	Desk.cpp
	Evaluator.cpp
	Game.cpp
	GameWindow.cpp
	main.cpp
	WindowPlayer.cpp
)
set(RESOURCES
	res/font.ttf
)

project(Visualizer)
add_executable(Visualizer ${SOURCES})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(Visualizer PRIVATE Threads::Threads)

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
	
	target_link_libraries(Visualizer PRIVATE ${FREETYPE})
	target_link_libraries(Visualizer PRIVATE ${OPENGL32})
	target_link_libraries(Visualizer PRIVATE ${WINMM})
	target_link_libraries(Visualizer PRIVATE ${GDI32})

	add_compile_definitions(SFML_STATIC)

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

target_link_libraries(Visualizer PRIVATE ${SFML_GRAPHICS})
target_link_libraries(Visualizer PRIVATE ${SFML_WINDOW})
target_link_libraries(Visualizer PRIVATE ${SFML_SYSTEM})

install(TARGETS Visualizer DESTINATION .)
install(FILES ${RESOURCES} DESTINATION ./res)
