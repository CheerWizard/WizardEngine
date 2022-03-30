//
// Created by mecha on 11.09.2021.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>

#include "string"
#include "unordered_map"

#define GET_SHADER_PROGRAM(name) engine::shader::ShaderSource::get().get(name)
#define RECOMPILE_SHADER_PROGRAM(name) engine::shader::ShaderSource::get().recompile(name)

namespace engine::shader {

    typedef std::unordered_map<std::string, Ref<BaseShaderProgram>> ShaderPrograms;
    typedef ShaderPrograms::iterator ShaderIterator;

    // Singleton class. Stores and provides ShaderProgram instances from memory.
    class ShaderSource {

    private:
        ShaderSource() = default;
        ~ShaderSource() {
            clear();
        }

    public:
        ShaderSource(const ShaderSource&) = delete;
        ShaderSource& operator=(const ShaderSource &) = delete;
        ShaderSource(ShaderSource &&) = delete;
        ShaderSource& operator=(ShaderSource &&) = delete;

    public:
        static auto& get() {
            static ShaderSource instance;
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
        void add(const std::string& name, const Ref<BaseShaderProgram>& shaderProgram);
        const Ref<BaseShaderProgram>& get(const std::string& name);
        [[nodiscard]] bool exists(const std::string& name) const;
        void remove(const std::string& name);
        void clear();
        void recompile(const std::string& name);

    public:
        const Ref<BaseShaderProgram>& operator[](const std::string &name) {
            return get(name);
        }

    private:
        ShaderPrograms _shaders;
    };
}