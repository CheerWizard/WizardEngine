cmake_minimum_required(VERSION 3.20)

project(Game)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_tools/CMakeLists.txt)

file(GLOB_RECURSE GAME_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
include_directories(
        src/include
        ../src/include
        ../vendor/spdlog/include
        ../vendor/entt/src
        ../vendor/glm
        ../vendor/glad/include
        ../vendor/glfw/include
        ../vendor/imgui
        ../vendor/stb
        ../cpp_scripting/src
)
add_executable(${PROJECT_NAME} ${GAME_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PUBLIC ScriptDLL WizardEngine)