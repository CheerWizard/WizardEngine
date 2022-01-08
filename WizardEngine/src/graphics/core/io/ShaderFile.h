//
// Created by mecha on 13.11.2021.
//

#pragma once

#include "../../../core/FileSystem.h"

#include "string"

#ifdef GL
#define RUNTIME_SHADERS_PATH "assets/shaders/glsl/4.0"
#define ENGINE_SHADERS_PATH "../WizardEngine/assets/shaders/glsl/4.0"
#endif

#define DEFAULT_INCLUDE_TOKEN "#include"

namespace engine::shader {

    struct ShaderProps {
        std::string name;
        std::string vFileName;
        std::string fFileName;
        std::string assetPath = RUNTIME_SHADERS_PATH;
        std::string includeToken = DEFAULT_INCLUDE_TOKEN;
    };

    struct ShaderSources {
        std::string vSrc;
        std::string fSrc;
    };

    class ShaderFile final {

    private:
        ShaderFile() = default;
        ~ShaderFile() = default;

    public:
        static ShaderSources readAssetWithIncludes(const ShaderProps& props);
        static std::string readAssetWithIncludes(
                const std::string& assetPath,
                const std::string& fileName,
                const std::string& includeToken
        );
    };

}
