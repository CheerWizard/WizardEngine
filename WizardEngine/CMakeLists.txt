cmake_minimum_required(VERSION 3.20)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_tools/CMakeLists.txt)

project(WizardEngine)

add_definitions(-DENGINE_DLL_BUILD=1)

#/ creating a library from sources.
file(GLOB_RECURSE ENGINE_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
add_library(${PROJECT_NAME} SHARED ${ENGINE_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

#/ Add WizardEngine headers
include_directories(src/include)

#/ Logger.
add_subdirectory(vendor/spdlog)
include_directories(vendor/spdlog/include)

#/ Window managing for OpenGL/Vulkan/OpenGLES context.
add_subdirectory(vendor/glfw)

#/ Graphics API, which uses OpenGL and Vulkan.
add_subdirectory(vendor/glad)
include_directories(vendor/glad/include)

# Immediate UI API.
add_subdirectory(vendor/imgui)
include_directories(vendor/imgui)
include_directories(vendor/imgui/misc/cpp)

#/ Image file processing
add_subdirectory(vendor/stb)
include_directories(vendor/stb/stb)

#/ Freetype text font rendering
add_subdirectory(vendor/freetype)
include_directories(vendor/freetype)

#/ Assets importer/exporter. Builds as DLL
include_directories(vendor/assimp/include)

#/ Audio core. Builds as DLL
include_directories(vendor/openal-soft/include)

#/ YAML parser. Builds as DLL
include_directories(vendor/yaml-cpp/include)

add_subdirectory(vendor/glm/glm)
include_directories(vendor/glm/glm)

#/ ImGuizmo
add_subdirectory(vendor/ImGuizmo)
include_directories(vendor/ImGuizmo)

#/ RCC (Runtime Compiled C++)
add_subdirectory(vendor/rcc)
include_directories(vendor/rcc/RuntimeCompiler)
include_directories(vendor/rcc/RuntimeObjectSystem)

#/ linking dependencies.
target_link_libraries(
        WizardEngine PUBLIC
        spdlog
        glfw
        glad
        imgui
        stb
        freetype
        glm
        ../WizardEngine/vendor/dlls/assimp-vc143-mtd
        ../WizardEngine/vendor/dlls/OpenAL32
        ../WizardEngine/vendor/dlls/yaml-cppd
        imguizmo
        RuntimeCompiler
        RuntimeObjectSystem
)

replace_dirs(WizardEngine/assets WizardEngine/assets)
replace_dirs(WizardEngine/vendor/dlls WizardEngine/vendor/dlls)
#replace_dirs(WizardEngine/project_template WizardEngine/project_template)
replace_dirs(WizardEngine/export_cmake WizardEngine/export_cmake)

#add_custom_command(
#        TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMAND replace_file(vendor/dlls/yaml-cppd.dll ../WizardServer/yaml-cppd.dll)
#        COMMENT "Replace files"
#)