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
set(ICON_RC "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")

add_executable(FairyLab ${FAIRY_SRC} ${ICON_RC})

target_link_libraries(FairyLab PUBLIC WizardEngine)
# copy assets, editor res and imgui init file.
replace_dirs(FairyLab/assets FairyLab/assets)
replace_dirs(FairyLab/editorRes FairyLab/editorRes)
configure_file(imgui.ini imgui.ini COPYONLY)
configure_file(FairyLab.png FairyLab.png COPYONLY)