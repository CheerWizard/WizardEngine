//
// Created by mecha on 13.11.2021.
//

#pragma once

#include <core/filesystem.h>
#include <platform/graphics/tools/ShaderPath.h>

#include "string"

#define DEFAULT_INCLUDE_TOKEN "#include"

namespace engine::io {

    struct ENGINE_API ShaderProps {
        std::string name;
        std::string vFileName;
        std::string fFileName;
        std::string gFileName;
        std::string assetPath = getRuntimeShadersPath();
        std::string includeToken = DEFAULT_INCLUDE_TOKEN;

        ShaderProps() = default;

        ShaderProps(
                const std::string& name,
                const std::string& vFileName,
                const std::string& fFileName,
                const std::string& assetPath
        ) : name(name), vFileName(vFileName), fFileName(fFileName),
        assetPath(assetPath) {}

        ShaderProps(
                const std::string& name,
                const std::string& vFileName,
                const std::string& fFileName,
                const std::string& gFileName,
                const std::string& assetPath
        ) : name(name), vFileName(vFileName), fFileName(fFileName), gFileName(gFileName),
        assetPath(assetPath) {}
    };

    struct ENGINE_API ShaderSources {
        std::string vSrc;
        std::string fSrc;
        std::string gSrc;
    };

    typedef std::unordered_map<std::string, ShaderProps> ShaderPropsMap;

    class ENGINE_API ShaderFile final {

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
