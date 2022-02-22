cmake_minimum_required(VERSION 3.20)

project(ScriptDLL)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_tools/CMakeLists.txt)

file(GLOB_RECURSE SCRIPT_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
include_directories(
        src
        ../src/include
        ../vendor/spdlog/include
        ../vendor/entt/src
        ../vendor/glm
)
add_library(${PROJECT_NAME} SHARED ${SCRIPT_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(ScriptDLL PUBLIC WizardEngine)