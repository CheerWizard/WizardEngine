//
// Created by mecha on 13.11.2021.
//

#include <graphics/core/io/ShaderFile.h>

#include <sstream>

namespace engine::io {

    ShaderPropsMap ShaderFile::shaderPropsMap = {};

    ShaderSources ShaderFile::readAssetWithIncludes(const ShaderProps& props) {
        shaderPropsMap[props.name] = props;
        return {
            readAssetWithIncludes(props.assetPath, props.vFileName, props.includeToken),
            readAssetWithIncludes(props.assetPath, props.fFileName, props.includeToken),
            readAssetWithIncludes(props.assetPath, props.gFileName, props.includeToken)
        };
    }

    std::string ShaderFile::readAssetWithIncludes(
            const std::string &assetPath,
            const std::string &fileName,
            const std::string &includeToken
    ) {
        // creating a full path to shader
        std::stringstream ss;
        ss << assetPath << "/" << fileName;
        auto fullPath = ss.str();
        return filesystem::readWithIncludes(fullPath, includeToken);
    }

    void ShaderFile::clear() {
        shaderPropsMap.clear();
    }

    ShaderSources ShaderFile::readAssetWithIncludes(const std::string &name) {
        return readAssetWithIncludes(shaderPropsMap[name]);
    }

    const ShaderProps &ShaderFile::getShaderProps(const std::string &name) {
        return shaderPropsMap[name];
    }

}