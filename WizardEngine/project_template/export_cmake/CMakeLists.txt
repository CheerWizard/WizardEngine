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

macro(copy_engine_dll engine_path)

    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/WizardEngine.dll bin/WizardEngine.dll)
    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/WizardEngine.lib bin/WizardEngine.lib)

    replace_file(${engine_path}/vendor/bin/assimp-vc143-mtd.dll bin/vendor/bin/assimp-vc143-mtd.dll)
    replace_file(${engine_path}/vendor/bin/assimp-vc143-mtd.lib bin/vendor/bin/assimp-vc143-mtd.lib)

    replace_file(${engine_path}/vendor/bin/OpenAL32.dll bin/vendor/bin/OpenAL32.dll)
    replace_file(${engine_path}/vendor/bin/OpenAL32.lib bin/vendor/bin/OpenAL32.lib)

    replace_file(${engine_path}/vendor/bin/yaml-cppd.dll bin/vendor/bin/yaml-cppd.dll)
    replace_file(${engine_path}/vendor/bin/yaml-cppd.lib bin/vendor/bin/yaml-cppd.lib)

endmacro()

# use this macro to link WizardEngine and all it's dependencies!
macro(link_to_engine project engine_path)
    add_definitions(-DENGINE_DLL=1)
    target_link_libraries(${project} PUBLIC
            bin/WizardEngine
            bin/vendor/bin/yaml-cppd
            bin/vendor/bin/assimp-vc143-mtd
            bin/vendor/bin/OpenAL32
    )
    copy_engine_dll(${engine_path})
endmacro()