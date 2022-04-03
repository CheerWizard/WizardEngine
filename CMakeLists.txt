cmake_minimum_required(VERSION 3.20)

project(Wizard)
# include CMake tools
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_tools/CMakeLists.txt)
replace_dirs(cmake_tools cmake_tools)

add_subdirectory(WizardEngine)
add_subdirectory(WizardStudio)
add_subdirectory(WizardTest)