//
// Created by mecha on 01.11.2021.
//

#include "MaterialShaderController.h"
#include "MaterialComponents.h"

namespace engine {

    MaterialShaderController::~MaterialShaderController() {
        _textureSource = nullptr;
    }

    void MaterialShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<TextureComponent>()) {
            auto& texture = entity.get<TextureComponent>();
            shader->setUniformArrayElement(texture.value, texture);
            // get/load TBO and activate his slot!
            auto& tbo = _textureSource->getTextureBuffer(texture.fileName);
            tbo->activate(texture.value);
            tbo->bind();
        }
    }

}