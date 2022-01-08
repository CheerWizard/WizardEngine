//
// Created by mecha on 13.11.2021.
//

#include "ShaderFile.h"

#include "sstream"

namespace engine::shader {

    ShaderSources ShaderFile::readAssetWithIncludes(const ShaderProps& props) {
        return {
            readAssetWithIncludes(props.assetPath, props.vFileName, props.includeToken),
            readAssetWithIncludes(props.assetPath, props.fFileName, props.includeToken)
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
        return FileSystem::readWithIncludes(fullPath, includeToken);
    }

}