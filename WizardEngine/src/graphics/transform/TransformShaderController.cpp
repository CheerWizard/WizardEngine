//
// Created by mecha on 01.11.2021.
//

#include "TransformShaderController.h"
#include "TransformComponents.h"
#include "../core/shader/InstanceID.h"

namespace engine {

    void TransformShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (entity.has<InstanceID>()) {
            auto instanceId = entity.get<InstanceID>().value;
            if (entity.has<Transform2dComponent>()) {
                shader->setUniformArrayElement(instanceId, entity.get<Transform2dComponent>());
            }
            if (entity.has<Transform3dComponent>()) {
                shader->setUniformArrayElement(instanceId, entity.get<Transform3dComponent>());
            }
        } else {
            if (entity.has<Transform2dComponent>()) {
                shader->setUniform(entity.get<Transform2dComponent>());
            }
            if (entity.has<Transform3dComponent>()) {
                shader->setUniform(entity.get<Transform3dComponent>());
            }
        }
    }

}