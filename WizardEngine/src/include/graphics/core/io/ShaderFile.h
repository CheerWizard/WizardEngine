//
// Created by mecha on 13.11.2021.
//

#pragma once

#include <core/FileSystem.h>
#include <platform/graphics/tools/ShaderPath.h>

#include "string"

#define DEFAULT_INCLUDE_TOKEN "#include"

namespace engine::shader {

    struct ShaderProps {
        std::string name;
        std::string vFileName;
        std::string fFileName;
        std::string assetPath = getRuntimeShadersPath();
        std::string includeToken = DEFAULT_INCLUDE_TOKEN;
    };

    struct ShaderSources {
        std::string vSrc;
        std::string fSrc;
    };

    typedef std::unordered_map<std::string, ShaderProps> ShaderPropsMap;

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
        static ShaderSources readAssetWithIncludes(const std::string& name);
        static const ShaderProps& getShaderProps(const std::string& name);
        static void clear();

    private:
        static ShaderPropsMap shaderPropsMap;

    };

}
