//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "Shader.h"

#include "string"
#include "unordered_map"

namespace engine {

    class ShaderCache {

    public:
        ShaderCache() = default;
        ~ShaderCache() = default;

    public:
        void add(const std::string& name, const Ref<Shader>& shader);
        void add(const Ref<Shader>& shader);
        Ref<Shader> load(const std::string& filepath);
        Ref<Shader> load(const std::string& name, const std::string& filepath);
        Ref<Shader> get(const std::string& name);
        bool exists(const std::string& name) const;
        void clear();

    private:
        std::unordered_map<std::string, Ref<Shader>> _shaders;

    };

}