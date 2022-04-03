# use this macro to include all dirs of WizardEngine, including it's dependencies!
macro(include_engine path)
    include_directories(
            ${path}/src/include
            ${path}/vendor/spdlog/include
            ${path}/vendor/entt/src
            ${path}/vendor/glm
            ${path}/vendor/glad/include
            ${path}/vendor/glfw/include
            ${path}/vendor/imgui
            ${path}/vendor/stb
            ${path}/vendor/freetype
            ${path}/vendor/assimp/include
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
            glm::glm
            stb
            EnTT
            freetype
    )
endmacro()