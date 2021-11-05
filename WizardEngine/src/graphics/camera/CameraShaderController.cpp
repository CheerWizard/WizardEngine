//
// Created by mecha on 01.11.2021.
//

#include "CameraShaderController.h"
#include "CameraComponents.h"

namespace engine {

    void Camera2dShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<Camera2dComponent>()) {
            auto camera = entity.get<Camera2dComponent>();
            updateVUniform(shader, camera);
        }
    }

    void Camera2dShaderController::updateShader(
            const Ref<Shader> &shader,
            const Entity &entity,
            const uint32_t &instanceId
    ) {
        updateShader(shader, entity);
    }

    void Camera3dShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<Camera3dComponent>()) {
            auto camera = entity.get<Camera3dComponent>();
            updateVUniform(shader, camera);
            shader->setUniform(camera.viewMatrix.position);
        }
    }

    void Camera3dShaderController::updateShader(
            const Ref<Shader> &shader,
            const Entity &entity,
            const uint32_t &instanceId
    ) {
        updateShader(shader, entity);
    }
}