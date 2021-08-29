cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE ENGINE_SRC engine/*.cpp)
target_sources(WizardEngine PUBLIC ${ENGINE_SRC})