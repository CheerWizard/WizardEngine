//
// Created by mecha on 03.11.2021.
//

#include "LightShaderController.h"
#include "LightComponents.h"

namespace engine {

    void LightShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<AmbientLightComponent>()) {
            auto ambientLight = entity.get<AmbientLightComponent>();
            shader->setUniform(ambientLight.strength);
            shader->setUniform(ambientLight.color);
        }

        if (entity.has<DiffuseLightComponent>()) {
            auto diffuseLight = entity.get<DiffuseLightComponent>();
            shader->setUniform(diffuseLight.color);
            shader->setUniform(diffuseLight.position);
        }

        if (entity.has<SpecularLightComponent>()) {
            auto specularLight = entity.get<SpecularLightComponent>();
            shader->setUniform(specularLight.color);
            shader->setUniform(specularLight.position);
            shader->setUniform(specularLight.strength);
        }
    }

}