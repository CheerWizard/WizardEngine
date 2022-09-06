cmake_minimum_required(VERSION 3.20)
project(imguizmo)
file(GLOB_RECURSE PROJECT_SRC
        ImGuizmo.h ImGuizmo.cpp
)
add_library(${PROJECT_NAME} ${PROJECT_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

include_directories(../ImGuizmo)

target_link_libraries(${PROJECT_NAME} PUBLIC imgui)