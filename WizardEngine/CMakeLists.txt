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

#/ Include static dependencies.

#/ Add WizardEngine headers
include_directories(src/include)

#/ Logger
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

#/ Math library
add_subdirectory(vendor/glm/glm)
include_directories(vendor/glm/glm)

#/ ImGuizmo
add_subdirectory(vendor/ImGuizmo)
include_directories(vendor/ImGuizmo)

#/ .zip compress library
add_subdirectory(vendor/zip)
include_directories(vendor/zip/src)

#/ Dirent API support for Windows
add_subdirectory(vendor/dirent)
include_directories(vendor/dirent/include)

#/ Link static dependencies.
target_link_libraries(
        WizardEngine PUBLIC
        spdlog
        glfw
        glad
        imgui
        stb
        freetype
        glm
        imguizmo
        zip
)

#/ Include dynamic dependencies

#/ Assets importer/exporter. Builds as DLL
include_directories(vendor/assimp/include)
#/ Audio core. Builds as DLL
include_directories(vendor/openal-soft/include)
#/ YAML parser. Builds as DLL
include_directories(vendor/yaml-cpp/include)

#/ Link dynamic dependencies
dynamic_link(WizardEngine assimp-vc143-mtd)
dynamic_link(WizardEngine OpenAL32)
dynamic_link(WizardEngine yaml-cppd)

#/ move engine binaries into output dir
replace_dirs(WizardEngine/assets WizardEngine/assets)
replace_dirs(WizardEngine/export_cmake WizardEngine/export_cmake)
#/ move project template resources
replace_dirs(WizardEngine/project_template WizardEngine/project_template)
replace_dirs(cmake_tools WizardEngine/project_template/cmake_tools)
replace_dirs(WizardEngine/export_cmake WizardEngine/project_template/export_cmake)
replace_dirs(WizardEngine/core_shaders WizardEngine/project_template/core_shaders)
#/ move headers as well for game projects linking at runtime
replace_dirs(WizardEngine/src/include WizardEngine/includes/src/include)
replace_dirs(WizardEngine/vendor/assimp/include WizardEngine/includes/vendor/assimp/include)
replace_dirs(WizardEngine/vendor/freetype/include WizardEngine/includes/vendor/freetype/include)
replace_dirs(WizardEngine/vendor/glad/include WizardEngine/includes/vendor/glad/include)
replace_dirs(WizardEngine/vendor/glfw/include WizardEngine/includes/vendor/glfw/include)
replace_dirs(WizardEngine/vendor/glm WizardEngine/includes/vendor/glm)
replace_dirs(WizardEngine/vendor/imgui WizardEngine/includes/vendor/imgui)
replace_dirs(WizardEngine/vendor/ImGuizmo WizardEngine/includes/vendor/ImGuizmo)
replace_dirs(WizardEngine/vendor/openal-soft/include WizardEngine/includes/vendor/openal-soft/include)
replace_dirs(WizardEngine/vendor/spdlog/include WizardEngine/includes/vendor/spdlog/include)
replace_dirs(WizardEngine/vendor/stb WizardEngine/includes/vendor/stb)
replace_dirs(WizardEngine/vendor/yaml-cpp/include WizardEngine/includes/vendor/yaml-cpp/include)