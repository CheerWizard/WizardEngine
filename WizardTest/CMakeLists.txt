cmake_minimum_required(VERSION 3.20)

project(WizardTest)

# creating .exe from sources.
file(GLOB_RECURSE TEST_SRC src/*.cpp)
add_executable(WizardTest ${TEST_SRC})

target_link_libraries(WizardTest WizardEngine)