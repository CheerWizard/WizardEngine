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
add_library(${PROJECT_NAME} ${ENGINE_SRC})
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

#/ Audio core. Builds as DLL
include_directories(vendor/openal-soft/include)

#/ YAML parser. Builds as DLL
include_directories(vendor/yaml-cpp/include)

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
        ../WizardEngine/vendor/dlls/OpenAL32
        ../WizardEngine/vendor/dlls/yaml-cppd
)

replace_dirs(WizardEngine/assets WizardEngine/assets)
replace_dirs(WizardEngine/vendor/dlls WizardEngine/vendor/dlls)
replace_dirs(WizardEngine/project_template WizardEngine/project_template)
replace_dirs(WizardEngine/export_cmake WizardEngine/export_cmake)

replace_file(vendor/dlls/assimp-vc143-mtd.dll ../WizardSamples/assimp-vc143-mtd.dll)
replace_file(vendor/dlls/assimp-vc143-mtd.dll ../WizardStudio/assimp-vc143-mtd.dll)
replace_file(vendor/dlls/assimp-vc143-mtd.dll ../WizardTest/assimp-vc143-mtd.dll)
replace_file(vendor/dlls/assimp-vc143-mtd.dll ../WizardServer/assimp-vc143-mtd.dll)

replace_file(vendor/dlls/OpenAL32.dll ../WizardSamples/OpenAL32.dll)
replace_file(vendor/dlls/OpenAL32.dll ../WizardStudio/OpenAL32.dll)
replace_file(vendor/dlls/OpenAL32.dll ../WizardTest/OpenAL32.dll)
replace_file(vendor/dlls/OpenAL32.dll ../WizardServer/OpenAL32.dll)

replace_file(vendor/dlls/yaml-cppd.dll ../WizardSamples/yaml-cppd.dll)
replace_file(vendor/dlls/yaml-cppd.dll ../WizardStudio/yaml-cppd.dll)
replace_file(vendor/dlls/yaml-cppd.dll ../WizardTest/yaml-cppd.dll)
replace_file(vendor/dlls/yaml-cppd.dll ../WizardServer/yaml-cppd.dll)
