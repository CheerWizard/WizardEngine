cmake_minimum_required(VERSION 3.20)

project(FairyLab)

file(GLOB_RECURSE FAIRY_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
include_directories(
        src
        ../WizardEngine/src
        ../WizardEngine/vendor
)
add_executable(FairyLab ${FAIRY_SRC})
target_link_libraries(FairyLab PUBLIC WizardEngine)
replace_dirs(FairyLab/assets FairyLab/assets)