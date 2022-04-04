cmake_minimum_required(VERSION 3.20)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_tools/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/../WizardEngine/export_cmake/CMakeLists.txt)

project(WizardTest)

file(GLOB_RECURSE TEST_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
include_directories(src/include)

add_executable(${PROJECT_NAME} ${TEST_SRC} src/include/core.h)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

include_engine(../WizardEngine)
link_to_engine(${PROJECT_NAME})