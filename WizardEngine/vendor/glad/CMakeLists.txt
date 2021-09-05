cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE GLAD_SRC src/*.c)
add_library(glad ${GLAD_SRC})

include_directories(include)