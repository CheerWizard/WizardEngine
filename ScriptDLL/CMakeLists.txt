cmake_minimum_required(VERSION 3.20)

project(ScriptDLL)

file(GLOB_RECURSE SCRIPT_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
include_directories(
        src
        include
        vendor/spdlog/include
        vendor/entt/src
)
add_library(ScriptDLL SHARED ${SCRIPT_SRC})

target_link_libraries(ScriptDLL PUBLIC WizardEngine)