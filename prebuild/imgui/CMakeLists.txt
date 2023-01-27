cmake_minimum_required(VERSION 3.20)
project(imgui)
file(GLOB_RECURSE PROJECT_SRC
        backends/imgui_impl_opengl3.cpp
        backends/imgui_impl_glfw.cpp
        misc/cpp/imgui_stdlib.cpp
        imgui.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        # imgui node editor
        imnodes.h
        imnodes.cpp
        imnodes_internal.h
)
add_library(${PROJECT_NAME} ${PROJECT_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

include_directories(../imgui)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad)