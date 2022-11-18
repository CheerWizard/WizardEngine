cmake_minimum_required(VERSION 3.20)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_tools/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/../export_cmake/CMakeLists.txt)
project(ScriptEngine)
add_definitions(-DSCRIPT_DLL=1)

file(GLOB_RECURSE SRC
    ../assets/scripts/*.c
    ../assets/scripts/*.cpp
    ../assets/scripts/*.h
    ../assets/scripts/*.hpp
        Template.cpp
)
include_directories(src/include)
add_library(${PROJECT_NAME} SHARED ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

include_engine(../../../../WizardEngine/includes)
link_to_engine(${PROJECT_NAME} ../../../../WizardEngine)