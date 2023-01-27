//
// Created by mecha on 13.11.2021.
//

#pragma once

#include <core/filesystem.h>
#include <platform/graphics/tools/ShaderPath.h>

#include "string"

namespace engine::io {

    typedef std::unordered_map<std::string, std::string> ShadersStorage;

    class ENGINE_API ShaderFile final {

    private:
        ShaderFile() = default;
        ~ShaderFile();

    public:
        static ShaderFile& get() {
            static ShaderFile instance;
            return instance;
        }

        const std::string& readShader(const std::string& filepath);
        void clear();

    private:
        ShadersStorage s_ShadersStorage;
    };

}
