cmake_minimum_required(VERSION 3.20)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_tools/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/../WizardEngine/export_cmake/CMakeLists.txt)

project(WizardStudio)

file(GLOB_RECURSE STUDIO_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
include_directories(src/include)
set(ICON_RC "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")

add_executable(${PROJECT_NAME} ${STUDIO_SRC} ${ICON_RC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

include_engine(../WizardEngine)
link_to_engine(${PROJECT_NAME} ../WizardEngine)
# copy assets, editor res and imgui init file.
replace_dirs(WizardStudio/assets WizardStudio/assets)
replace_dirs(WizardStudio/editor WizardStudio/editor)
replace_dirs(WizardStudio/projects WizardStudio/projects)
replace_dirs(WizardStudio/assets/bitmaps WizardStudio/assets/bitmaps)
replace_dirs(WizardStudio/assets/scripts WizardStudio/assets/scripts)
configure_file(Visual.ini Visual.ini COPYONLY)
configure_file(WizardStudio.png WizardStudio.png COPYONLY)