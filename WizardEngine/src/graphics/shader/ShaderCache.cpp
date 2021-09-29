//
// Created by mecha on 11.09.2021.
//

#include "ShaderCache.h"

#include "../../core/Application.h"

namespace engine {

    void ShaderCache::add(const std::string &name, const Ref<Shader>& shader) {
        ENGINE_ASSERT(!exists(name), "Shader already exists!");
        _shaders[name] = shader;
    }

    void ShaderCache::add(const Ref<Shader> &shader) {
        auto name = shader->getName();
        add(name, shader);
    }

    Ref<Shader> ShaderCache::get(const std::string &name) {
        ENGINE_ASSERT(exists(name), "Shader not found!");
        return _shaders[name];
    }

    bool ShaderCache::exists(const std::string &name) const {
        return _shaders.find(name) != _shaders.end();
    }

    void ShaderCache::clear() {
        _shaders.clear();
    }

    ShaderError ShaderCache::load(const ShaderProps &shaderProps, VertexFormat* vertexFormat) {
        auto shader = GRAPHICS_CONTEXT->newShader(shaderProps, vertexFormat);
        return handleShaderError(shader);
    }

    ShaderCache::~ShaderCache() {
        clear();
    }

    ShaderError ShaderCache::load(const ShaderProps &shaderProps) {
        auto shader = GRAPHICS_CONTEXT->newShader(shaderProps);
        return handleShaderError(shader);
    }

    ShaderError ShaderCache::handleShaderError(Ref<Shader> shader) {
        auto shaderError = shader->getShaderError();
        if (shaderError == ShaderError::NONE) {
            add(shader);
        }
        return shaderError;
    }

}