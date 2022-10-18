cmake_minimum_required(VERSION 3.18)

set(SOURCES
    train/EpsilonGreedy.cpp
    train/main.cpp
    train/CompressedDesk.cpp
)

project(train)

add_executable(train ${SOURCES})
target_link_libraries(train PRIVATE common_modules)
target_include_directories(train PRIVATE ${CMAKE_SOURCE_DIR})

find_package(Torch REQUIRED)
target_link_libraries(train PRIVATE ${TORCH_LIBRARIES})
target_include_directories(train PRIVATE ${TORCH_INCLUDE_DIRS})
target_compile_options(train PRIVATE ${TORCH_CXX_FLAGS})
