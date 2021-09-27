cmake_minimum_required(VERSION 3.20)

include_directories(stb)

file(GLOB_RECURSE STB_SRC stb/deprecated/*.c)
add_library(stb ${STB_SRC})

target_link_libraries(stb PUBLIC glfw)