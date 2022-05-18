cmake_minimum_required(VERSION 3.20)
project(imgui)
file(GLOB_RECURSE PROJECT_SRC
        imgui/*.h
        imgui/*.c
        imgui/*.hpp
        imgui/*.cpp
)
add_library(${PROJECT_NAME} ${PROJECT_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)