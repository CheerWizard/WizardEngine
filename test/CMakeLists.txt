cmake_minimum_required(VERSION 3.20)

file(GLOB TEST_SRC test/*.cpp)
target_sources(WizardTest PRIVATE ${TEST_SRC})

find_library(WIZARD_ENGINE WizardEngine cmake-build-debug)
target_link_libraries(WizardTest LINK_PUBLIC ${WIZARD_ENGINE})