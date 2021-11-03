//
// Created by mecha on 01.11.2021.
//

#include "CameraShaderController.h"
#include "CameraComponents.h"

namespace engine {

    void CameraShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<Camera2dComponent>()) {
            shader->tryUpdateUniformBuffer(entity.get<Camera2dComponent>());
        }

        if (entity.has<Camera3dComponent>()) {
            shader->tryUpdateUniformBuffer(entity.get<Camera3dComponent>());
        }
    }

}