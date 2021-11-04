//
// Created by mecha on 03.11.2021.
//

#include "LightShaderController.h"
#include "LightComponents.h"

namespace engine {

    void LightShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<AmbientLightComponent>()) {
            auto ambientLight = entity.get<AmbientLightComponent>();
            shader->setUniformStructField(ambientLight.name, ambientLight.strength);
            shader->setUniformStructField(ambientLight.name, ambientLight.color);
        }

        if (entity.has<DiffuseLightComponent>()) {
            auto diffuseLight = entity.get<DiffuseLightComponent>();
            shader->setUniformStructField(diffuseLight.name, diffuseLight.position);
            shader->setUniformStructField(diffuseLight.name, diffuseLight.color);
        }

        if (entity.has<SpecularLightComponent>()) {
            auto specularLight = entity.get<SpecularLightComponent>();
            shader->setUniformStructField(specularLight.name, specularLight.position);
            shader->setUniformStructField(specularLight.name, specularLight.color);
            shader->setUniformStructField(specularLight.name, specularLight.strength);
        }
    }

    void LightShaderController::updateShader(
            const Ref<engine::Shader> &shader,
            const Entity &entity,
            const uint32_t &instanceId
    ) {
        updateShader(shader, entity);
    }

}