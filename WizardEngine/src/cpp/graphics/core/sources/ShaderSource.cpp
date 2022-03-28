//
// Created by mecha on 11.09.2021.
//

#include <graphics/core/sources/ShaderSource.h>
#include <core/Assert.h>

namespace engine::shader {

    void ShaderSource::add(const std::string &name, const Ref<BaseShaderProgram> &shaderProgram) {
        ENGINE_ASSERT(!exists(name), "Shader already exists!");
        _shaders[name] = shaderProgram;
    }

    const Ref<BaseShaderProgram>& ShaderSource::get(const std::string &name) {
        ENGINE_ASSERT(exists(name), "Shader not found!");
        return _shaders[name];
    }

    bool ShaderSource::exists(const std::string &name) const {
        ENGINE_INFO("ShaderSource: exists(name: {0})", name);
        return _shaders.find(name) != _shaders.end();
    }

    void ShaderSource::clear() {
        ENGINE_INFO("ShaderSource: clear()");
        for (auto& it : *this) {
            it.second->release();
        }
        _shaders.clear();
    }

    void ShaderSource::remove(const std::string &name) {
        ENGINE_INFO("ShaderSource: remove(name: {0})", name);
        _shaders.erase(name);
    }

    void ShaderSource::recompile(const std::string &name) {
        get(name)->recompile(name);
    }

}