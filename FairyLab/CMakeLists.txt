cmake_minimum_required(VERSION 3.20)

project(FairyLab)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_tools/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/../WizardEngine/export_cmake/CMakeLists.txt)

file(GLOB_RECURSE FAIRY_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
include_directories(src/include)
set(ICON_RC "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")

add_executable(${PROJECT_NAME} ${FAIRY_SRC} ${ICON_RC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(
        ${PROJECT_NAME} PUBLIC
        WizardEngine
        spdlog
        glfw
        glad
        imgui
        glm::glm
        stb
        EnTT
)
# copy assets, editor res and imgui init file.
replace_dirs(FairyLab/assets FairyLab/assets)
replace_dirs(FairyLab/editor FairyLab/editor)
replace_dirs(FairyLab/projects FairyLab/projects)
configure_file(imgui.ini imgui.ini COPYONLY)
configure_file(FairyLab.png FairyLab.png COPYONLY)