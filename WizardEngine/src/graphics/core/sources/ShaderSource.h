//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "../shader/Shader.h"
#include "../../GraphicsFactory.h"

#include "string"
#include "unordered_map"

namespace engine {

    typedef std::unordered_map<std::string, Ref<Shader>> Shaders;
    typedef Shaders::iterator ShaderIterator;

    class ShaderSource {

    public:
        ShaderSource(const Ref<GraphicsFactory> &factory) : _factory(factory) {}
        ~ShaderSource() {
            clear();
        }

    public:
        inline ShaderIterator begin() {
            return _shaders.begin();
        }

        inline ShaderIterator end() {
            return _shaders.end();
        }

    public:
        const Ref<Shader>& create(const ShaderProps& props);
        void add(const Ref<Shader>& shader);
        const Ref<Shader>& get(const std::string& name);
        bool exists(const std::string& name) const;
        void remove(const std::string& name);
        void clear();

    public:
        const Ref<Shader>& operator[](const std::string &name) {
            return get(name);
        }

    private:
        Shaders _shaders;
        Ref<GraphicsFactory> _factory;

    };

}