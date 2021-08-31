cmake_minimum_required(VERSION 3.20)

project(WizardTest)

# creating .exe from sources.
file(GLOB TEST_SRC src/*.cpp)
add_executable(WizardTest ${TEST_SRC})

# linking dependencies.
target_link_libraries(WizardTest PRIVATE WizardEngine)

# need to fix all linking errors for WizardTest project.