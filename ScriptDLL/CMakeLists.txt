cmake_minimum_required(VERSION 3.20)

project(ScriptDLL)

#/ creating a library from sources.
file(GLOB_RECURSE SCRIPT_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
include_directories(
        ../WizardEngine/src
        ../WizardEngine/vendor
)
add_library(ScriptDLL SHARED ${SCRIPT_SRC})

#/ linking dependencies.
target_link_libraries(ScriptDLL PUBLIC WizardEngine)