//
// Created by mecha on 11.09.2021.
//

#include "ShaderSource.h"

namespace engine {

    void ShaderSource::add(const Ref<Shader> &shader) {
        auto name = shader->getName();
        ENGINE_ASSERT(!exists(name), "Shader already exists!");
        _shaders[name] = shader;
    }

    const Ref<Shader>& ShaderSource::get(const std::string &name) {
        ENGINE_ASSERT(exists(name), "Shader not found!");
        return _shaders[name];
    }

    bool ShaderSource::exists(const std::string &name) const {
        ENGINE_INFO("ShaderSource: exists(name: {0})", name);
        return _shaders.find(name) != _shaders.end();
    }

    void ShaderSource::clear() {
        ENGINE_INFO("ShaderSource: clear()");
        _shaders.clear();
    }

    const Ref<Shader>& ShaderSource::create(const ShaderProps &props) {
        ENGINE_INFO("ShaderSource: create(name: {0})", props.name);
        add(_factory->newShader(props));
        return _shaders[props.name];
    }

    void ShaderSource::remove(const std::string &name) {
        ENGINE_INFO("ShaderSource: remove(name: {0})", name);
        _shaders.erase(name);
    }

}