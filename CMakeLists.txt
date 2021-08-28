cmake_minimum_required(VERSION 3.20)
project(Wizard)

set(CMAKE_CXX_STANDARD 23)

add_library(WizardEngine STATIC engine/Core.h engine/Application.cpp)
add_executable(Wizard Launcher.cpp test/TestApplication.cpp)

include_directories(engine)
include_directories(test)

find_library(WIZARD_ENGINE WizardEngine engine)
target_link_libraries(Wizard LINK_PUBLIC ${WIZARD_ENGINE})