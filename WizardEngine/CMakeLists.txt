cmake_minimum_required(VERSION 3.20)

project(WizardEngine)

#/ creating a library from sources
file(GLOB ENGINE_SRC src/*.cpp)
add_library(WizardEngine ${ENGINE_SRC})

#/ adding dependencies.
add_subdirectory(vendor/spdlog) #/ Logging.
add_subdirectory(vendor/glfw) #/ Win managing for OpenGL/Vulkan/OpenGLES context.

#/ linking dependencies.
target_link_libraries(WizardEngine spdlog)
target_link_libraries(WizardEngine glfw)