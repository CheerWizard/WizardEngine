//
// Created by mecha on 13.11.2021.
//

#include <graphics/core/io/ShaderFile.h>

#include <sstream>

namespace engine::io {

    ShaderFile::~ShaderFile() {
        clear();
    }

    const std::string& ShaderFile::readShader(const std::string &filepath) {
        if (s_ShadersStorage.find(filepath) == s_ShadersStorage.end()) {
            s_ShadersStorage[filepath] = filesystem::readWithIncludes(filepath, "#include");
        }
        return s_ShadersStorage.at(filepath);
    }

    void ShaderFile::clear() {
        s_ShadersStorage.clear();
    }

}