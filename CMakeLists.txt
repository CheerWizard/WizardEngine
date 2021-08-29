cmake_minimum_required(VERSION 3.20)

project(Wizard)
set(CMAKE_CXX_STANDARD 23)

add_library(WizardEngine "")
include(engine/CMakeLists.txt)

add_executable(WizardTest "")
include(test/CMakeLists.txt)