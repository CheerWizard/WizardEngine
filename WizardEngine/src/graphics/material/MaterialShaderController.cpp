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
        updateTexture(shader, entity);

        if (entity.has<MaterialComponent>()) {
            auto& material = entity.get<MaterialComponent>();
            shader->setUniformStructField(material.name, material.color);
            shader->setUniformStructField(material.name, material.ambient);
            shader->setUniformStructField(material.name, material.diffuse);
            shader->setUniformStructField(material.name, material.specular);
            shader->setUniformStructField(material.name, material.shiny);
        }
    }

    void MaterialShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity, const uint32_t &instanceId) {
        updateTexture(shader, entity);

        if (entity.has<MaterialComponent>()) {
            auto& material = entity.get<MaterialComponent>();
            shader->setUniformArrayStructField(instanceId, material.name, material.color);
            shader->setUniformArrayStructField(instanceId, material.name, material.ambient);
            shader->setUniformArrayStructField(instanceId, material.name, material.diffuse);
            shader->setUniformArrayStructField(instanceId, material.name, material.specular);
            shader->setUniformArrayStructField(instanceId, material.name, material.shiny);
        }
    }

    void MaterialShaderController::updateTexture(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<TextureComponent>()) {
            auto& texture = entity.get<TextureComponent>();
            shader->setUniformArrayElement(texture.sampler.value, texture.sampler);
            // get/load TBO and activate his slot!
            auto& tbo = _textureSource->getTextureBuffer(texture.fileName);
            tbo->activate(texture.sampler.value);
            tbo->bind();
        }
    }
}