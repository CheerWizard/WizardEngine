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
    }

}