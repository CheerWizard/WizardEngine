cmake_minimum_required(VERSION 3.20)

project(WizardEngine)

#/ creating a library from sources.
file(GLOB_RECURSE ENGINE_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
add_library(WizardEngine ${ENGINE_SRC} src/graphics/light/LightShaderScript.h src/graphics/light/LightShaderScript.cpp src/graphics/light/Light.h src/graphics/light/Light.cpp src/graphics/core/io/TextureFile.h src/graphics/core/io/TextureFile.cpp src/graphics/core/geometry/MeshComponent.cpp src/platform/opengl/buffers/VertexArray.cpp src/graphics/core/buffer_data/FrameSpecs.h src/graphics/core/buffer_data/FrameSpecs.cpp src/graphics/core/shader/ShaderData.h src/graphics/core/io/ShaderFile.h src/graphics/core/io/ShaderFile.cpp src/graphics/core/shader/BaseShader.h src/graphics/core/shader/BaseShader.cpp src/platform/includes/graphics/tbo.h src/platform/includes/graphics/ubo.h src/platform/includes/graphics/vao.h src/platform/includes/graphics/ibo.h src/platform/includes/graphics/vbo.h src/platform/includes/graphics/fbo.h src/graphics/core/commands/RenderCommands.h src/platform/includes/graphics/commands.h src/platform/includes/graphics/shader.h src/graphics/core/commands/RenderCommands.cpp src/platform/includes/graphics/initializer.h src/scripting/ScriptComponents.h src/scripting/ScriptSystem.h src/scripting/ScriptSystem.cpp src/imgui/Colors.h src/imgui/Colors.cpp src/core/Window.cpp src/graphics/post_processing/PostEffect.h src/graphics/post_processing/PostEffect.cpp src/graphics/GraphicsObject.h src/WizardEngine.h src/scripting/ScriptCompiler.h src/scripting/ScriptCompiler.cpp src/core/Task.h)

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