# use this macro to include all dirs of WizardEngine, including it's dependencies!
macro(include_engine path)
    include_directories(
            ${path}/src/include
            ${path}/vendor/spdlog/include
            ${path}/vendor/glad/include
            ${path}/vendor/glfw/include
            ${path}/vendor/imgui
            ${path}/vendor/stb
            ${path}/vendor/freetype/include
            ${path}/vendor/assimp/include
            ${path}/vendor/openal-soft/include
            ${path}/vendor/yaml-cpp/include
            ${path}/vendor/glm/glm
            ${path}/vendor/ImGuizmo
    )
endmacro()
# use this macro to link WizardEngine and all it's dependencies!
macro(link_to_engine project)
    target_link_libraries(
            ${project} PUBLIC
            WizardEngine
            spdlog
            glfw
            glad
            imgui
            stb
            freetype
            glm
            imguizmo
    )
endmacro()

macro(link_to_engine_dll project)
    target_link_libraries(
            ${project} PUBLIC
            WizardEngine
            spdlog
            glfw
            glad
            imgui
            stb
            freetype
            assimp-vc143-mtd
            OpenAL32
            yaml-cppd
            imguizmo
    )
endmacro()