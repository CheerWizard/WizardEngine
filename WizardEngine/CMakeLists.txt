cmake_minimum_required(VERSION 3.20)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake_tools/CMakeLists.txt)

project(WizardEngine)

#/ creating a library from sources.
file(GLOB_RECURSE ENGINE_SRC
        src/*.c
        src/*.cpp
        src/*.h
        src/*.hpp
)
add_library(${PROJECT_NAME} ${ENGINE_SRC} src/cpp/io/AudioFile.cpp src/cpp/audio/MediaPlayer.cpp src/include/audio/MediaPlayer.h)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

#/ adding dependencies.
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
include_directories(vendor/imgui/imgui)
include_directories(vendor/imgui/imgui/misc/cpp)

#/ Math.
add_subdirectory(vendor/glm)

#/ Image file processing
add_subdirectory(vendor/stb)
include_directories(vendor/stb/stb)

#/ Freetype text font rendering
add_subdirectory(vendor/freetype)
include_directories(vendor/freetype)

#/ Assets importer/exporter. Builds as DLL
include_directories(vendor/assimp/include)

#/ Audio core
add_subdirectory(vendor/openal-soft)
include_directories(vendor/openal-soft/include)

#/ linking dependencies.
target_link_libraries(
        WizardEngine PUBLIC
        spdlog
        glfw
        glad
        imgui
        glm::glm
        stb
        freetype
        ../WizardEngine/vendor/dlls/assimp-vc143-mtd
        OpenAL
)

replace_dirs(WizardEngine/assets WizardEngine/assets)
replace_dirs(WizardEngine/vendor/dlls WizardEngine/vendor/dlls)
replace_file(vendor/dlls/assimp-vc143-mtd.dll ../WizardStudio/assimp-vc143-mtd.dll)
replace_file(vendor/dlls/assimp-vc143-mtd.dll ../WizardTest/assimp-vc143-mtd.dll)
#replace_dirs(WizardEngine/cpp_scripting WizardEngine/cpp_scripting)
#replace_dirs(WizardEngine/runtime WizardEngine/runtime)
#replace_dirs(WizardEngine/src/include WizardEngine/src/include)
#replace_dirs(WizardEngine/vendor/entt/src WizardEngine/vendor/entt/src)
#replace_dirs(WizardEngine/vendor/glad/include WizardEngine/vendor/glad/include)
#replace_dirs(WizardEngine/vendor/glfw/include WizardEngine/vendor/glfw/include)
#replace_dirs(WizardEngine/vendor/glm/glm WizardEngine/vendor/glm/glm)
#replace_dirs(WizardEngine/vendor/spdlog/include WizardEngine/vendor/spdlog/include)
#replace_dirs(WizardEngine/vendor/stb/stb WizardEngine/vendor/stb/stb)
#replace_dirs(WizardEngine/vendor/freetype/include WizardEngine/vendor/freetype/include)