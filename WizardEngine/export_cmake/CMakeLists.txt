# use this macro to include all dirs of WizardEngine, including it's dependencies!
macro(include_engine)
    include_directories(
            ../WizardEngine/src/include
            ../WizardEngine/vendor/spdlog/include
            ../WizardEngine/vendor/entt/src
            ../WizardEngine/vendor/glm
            ../WizardEngine/vendor/glad/include
            ../WizardEngine/vendor/glfw/include
            ../WizardEngine/vendor/imgui
            ../WizardEngine/vendor/stb
            ../WizardEngine/vendor/freetype
            ../WizardEngine/vendor/assimp/include
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