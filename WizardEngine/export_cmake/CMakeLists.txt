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
            ${path}/vendor/rcc/RuntimeCompiler
            ${path}/vendor/rcc/RuntimeObjectSystem
    )
endmacro()
# use this macro to link WizardEngine and all it's dependencies!
macro(link_to_engine project)
    add_definitions(-DENGINE_DLL=1)
    target_link_libraries(${project} PUBLIC WizardEngine)
    replace_file(../WizardEngine/vendor/dlls/assimp-vc143-mtd.dll assimp-vc143-mtd.dll)
    replace_file(../WizardEngine/vendor/dlls/OpenAL32.dll OpenAL32.dll)
    replace_file(../WizardEngine/vendor/dlls/yaml-cppd.dll yaml-cppd.dll)
    replace_file(${PROJECT_BINARY_DIR}/../WizardEngine/WizardEngine.dll WizardEngine.dll)
endmacro()