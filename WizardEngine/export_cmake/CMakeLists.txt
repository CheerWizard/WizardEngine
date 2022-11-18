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
            ${path}/vendor/zip/src
            ${path}/vendor/dirent/include
    )
endmacro()

macro(copy_engine_dll engine_path)

    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/WizardEngine.dll WizardEngine.dll)
    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/WizardEngine.lib WizardEngine.lib)

    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/assimp-vc143-mtd.dll assimp-vc143-mtd.dll)
    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/assimp-vc143-mtd.lib assimp-vc143-mtd.lib)

    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/OpenAL32.dll OpenAL32.dll)
    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/OpenAL32.lib OpenAL32.lib)

    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/yaml-cppd.dll yaml-cppd.dll)
    replace_file(${PROJECT_BINARY_DIR}/${engine_path}/yaml-cppd.lib yaml-cppd.lib)

endmacro()

# use this macro to link WizardEngine and all it's dependencies!
macro(link_to_engine project engine_path)
    add_definitions(-DENGINE_DLL=1)
    target_link_libraries(${project} PUBLIC
            WizardEngine
            yaml-cppd
            assimp-vc143-mtd
            OpenAL32
    )
    copy_engine_dll(${engine_path})
endmacro()