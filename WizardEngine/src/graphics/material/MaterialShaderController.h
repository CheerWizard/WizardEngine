//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../core/shader/ShaderController.h"
#include "../core/sources/TextureSource.h"

namespace engine {

    class MaterialShaderController : public ShaderController {

    public:
        MaterialShaderController(const Ref<TextureSource> &textureSource)
        : _textureSource(textureSource), ShaderController() {}
        ~MaterialShaderController();

        void updateShader(const Ref<Shader> &shader, const Entity &entity) override;
        void updateShader(const Ref<Shader> &shader, const Entity &entity, const uint32_t &instanceId) override;

    private:
        void updateTexture(const Ref<Shader> &shader, const Entity &entity);

    private:
        Ref<TextureSource> _textureSource;

    };

}
