cmake_minimum_required(VERSION 3.18)

set(SOURCES
    common/Desk.cpp
)

project(common_modules)
add_library(common_modules ${SOURCES})
