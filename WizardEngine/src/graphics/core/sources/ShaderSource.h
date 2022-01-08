//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "../shader/BaseShader.h"

#include "string"
#include "unordered_map"

namespace engine::shader {

    typedef std::unordered_map<std::string, BaseShader> Shaders;
    typedef Shaders::iterator ShaderIterator;

    class ShaderSource {

    public:
        ShaderSource() = default;
        ~ShaderSource() {
            clear();
        }

    public:
        static const Ref<ShaderSource>& get() {
            return instance;
        }

    public:
        inline ShaderIterator begin() {
            return _shaders.begin();
        }

        inline ShaderIterator end() {
            return _shaders.end();
        }

    public:
        void add(const std::string& name, const BaseShader& shader);
        const BaseShader& get(const std::string& name);
        [[nodiscard]] bool exists(const std::string& name) const;
        void remove(const std::string& name);
        void clear();

    public:
        const BaseShader& operator[](const std::string &name) {
            return get(name);
        }

    private:
        Shaders _shaders;

    private:
        static Ref<ShaderSource> instance;
    };
}