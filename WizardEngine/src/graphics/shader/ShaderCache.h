//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "Shader.h"

#include "string"
#include "unordered_map"

namespace engine {

    typedef std::unordered_map<std::string, Ref<Shader>> Shaders;
    typedef Shaders::iterator ShaderIterator;

    class ShaderCache {

    public:
        ShaderCache() = default;
        ~ShaderCache() = default;

    public:
        void add(const std::string& name, const Ref<Shader>& shader);
        void add(const Ref<Shader>& shader);
        Ref<Shader> load(const ShaderProps& shaderProps, VertexFormat* vertexFormat);
        Ref<Shader> get(const std::string& name);
        bool exists(const std::string& name) const;
        void clear();

    public:
        ShaderIterator begin() {
            return _shaders.begin();
        }

        ShaderIterator end() {
            return _shaders.end();
        }

    private:
         Shaders _shaders;

    };

}