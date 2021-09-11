//
// Created by mecha on 11.09.2021.
//

#include "Shaders.h"

namespace engine {

    void Shaders::add(const std::string &name, const Ref<Shader>& shader) {
        ENGINE_ASSERT(!exists(name), "Shader already exists!");
        _shaders[name] = shader;
    }

    void Shaders::add(const Ref<Shader> &shader) {
        auto& name = shader->getName();
        add(name, shader);
    }

    Ref<Shader> Shaders::load(const std::string &name, const std::string &filepath) {
        auto shader = GRAPHICS_CONTEXT.newShader(filepath);
        add(name, shader);
        return shader;
    }

    Ref<Shader> Shaders::load(const std::string &filepath) {
        auto shader = GRAPHICS_CONTEXT.newShader(filepath);
        add(shader);
        return shader;
    }

    Ref<Shader> Shaders::get(const std::string &name) {
        ENGINE_ASSERT(exists(name), "Shader not found!");
        return _shaders[name];
    }

    bool Shaders::exists(const std::string &name) const {
        return _shaders.find(name) != _shaders.end();
    }

}