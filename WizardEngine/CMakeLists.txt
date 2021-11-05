cmake_minimum_required(VERSION 3.20)

project(WizardEngine)

#/ creating a library from sources.
file(GLOB_RECURSE ENGINE_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
add_library(WizardEngine ${ENGINE_SRC} src/graphics/light/LightShaderController.h src/graphics/light/LightShaderController.cpp src/graphics/light/Light.h src/graphics/light/Light.cpp src/graphics/core/shader/ShaderController.cpp)

#/ adding dependencies.

#/ Logger.
add_subdirectory(vendor/spdlog)

#/ Window managing for OpenGL/Vulkan/OpenGLES context.
add_subdirectory(vendor/glfw)

#/ Graphics API, which uses OpenGL and Vulkan.
add_subdirectory(vendor/glad)
include_directories(vendor/glad/include)

# Immediate UI API.
add_subdirectory(vendor/imgui)
include_directories(vendor/imgui/imgui)
include_directories(vendor/imgui/imgui/misc/cpp)

#/ Networking and async IO.
add_subdirectory(vendor/asio)
include_directories(vendor/asio/asio/include)

#/ Math.
add_subdirectory(vendor/glm)

#/ Picture API
add_subdirectory(vendor/stb)
include_directories(vendor/stb/stb)

#/ ECS lib.
add_subdirectory(vendor/entt)

#/ linking dependencies.
target_link_libraries(
        WizardEngine PUBLIC
        spdlog
        glfw
        glad
        imgui
        glm::glm
        stb
        EnTT
)

replace_dirs(WizardEngine/assets WizardEngine/assets)