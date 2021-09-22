cmake_minimum_required(VERSION 3.20)

project(WizardTest)

# creating .exe from sources.
file(GLOB_RECURSE TEST_SRC src/*.cpp)
file(GLOB_RECURSE ASSETS assets/shaders/shape2d.glsl)
add_executable(WizardTest ${TEST_SRC} ${ASSETS})

target_link_libraries(WizardTest WizardEngine)