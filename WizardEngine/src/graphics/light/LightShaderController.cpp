//
// Created by mecha on 03.11.2021.
//

#include "LightShaderController.h"
#include "LightComponents.h"

namespace engine {

    void LightShaderController::updateShader(const Ref<Shader> &shader, const Entity &entity) {
        if (shader->hasFUniformBlocks()) {
            if (entity.has<AmbientLightComponent>()) {
                auto ambientLight = entity.get<AmbientLightComponent>();
                if (ambientLight.color.isUpdated) {
                    ambientLight.color.isUpdated = false;
                    shader->updateFUniformBuffer({ ambientLight.color.toFloatPtr(), 0 });
                }
                if (ambientLight.strength.isUpdated) {
                    ambientLight.strength.isUpdated = false;
                    shader->updateFUniformBuffer({ new float { ambientLight.strength.value }, 1 });
                }
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

            if (entity.has<PhongLightComponent>()) {
                auto phongLight = entity.get<PhongLightComponent>();
                shader->setUniformStructField(phongLight.name, phongLight.position);
                shader->setUniformStructField(phongLight.name, phongLight.color);
                shader->setUniformStructField(phongLight.name, phongLight.ambientStrength);
                shader->setUniformStructField(phongLight.name, phongLight.specularStrength);
            }
        } else {
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

            if (entity.has<PhongLightComponent>()) {
                auto phongLight = entity.get<PhongLightComponent>();
                shader->setUniformStructField(phongLight.name, phongLight.position);
                shader->setUniformStructField(phongLight.name, phongLight.color);
                shader->setUniformStructField(phongLight.name, phongLight.ambientStrength);
                shader->setUniformStructField(phongLight.name, phongLight.specularStrength);
            }
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