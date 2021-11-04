//
// Created by mecha on 01.11.2021.
//

#include "TransformShaderController.h"
#include "TransformComponents.h"

namespace engine {

    void TransformShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<Transform2dComponent>()) {
            shader->setUniform(entity.get<Transform2dComponent>());
        }
        if (entity.has<Transform3dComponent>()) {
            shader->setUniform(entity.get<Transform3dComponent>());
        }
    }

    void TransformShaderController::updateShader(
            const Ref<Shader> &shader,
            const Entity &entity,
            const uint32_t &instanceId
    ) {
        if (entity.has<Transform2dComponent>()) {
            shader->setUniformArrayElement(instanceId, entity.get<Transform2dComponent>());
        }
        if (entity.has<Transform3dComponent>()) {
            shader->setUniformArrayElement(instanceId, entity.get<Transform3dComponent>());
        }
    }

}