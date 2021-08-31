cmake_minimum_required(VERSION 3.20)

project(WizardEngine)

#/ creating a library from sources.
file(GLOB_RECURSE ENGINE_SRC src/*.cpp src/*.h)
add_library(WizardEngine ${ENGINE_SRC})

#/ adding dependencies.
add_subdirectory(vendor/spdlog) #/ Logging.
add_subdirectory(vendor/glfw) #/ Win managing for OpenGL/Vulkan/OpenGLES context.
#/ adding sources explicitly.
include_directories(vendor/asio/asio/include) #/ Networking and async IO.
include_directories(vendor/glad/include) #/ Graphics modern API, extended from OpenGL.
include_directories(vendor/imgui) # Immediate UI API , for debugging, profiling, creating editor, etc.

#/ linking dependencies.
target_link_libraries(
        WizardEngine PUBLIC
        spdlog
        glfw
)

#/ creating network executable for testing.
add_executable(WizardNetwork src/network/Client.cpp)
target_link_libraries(WizardNetwork PRIVATE WizardEngine)

# need to fix all linking errors for WizardNetwork target.