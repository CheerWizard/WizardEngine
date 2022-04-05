//
// Created by mecha on 03.11.2021.
//

#include <graphics/light/LightShaderScript.h>

namespace engine::graphics {

    ShaderScript phongLightScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto phongLights = registry.view<PhongLightComponent>();
            for (auto [entity, phongLight] : phongLights.each()) {
                updateLight(shader, phongLight);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto phongLight = entity.getPtr<PhongLightComponent>();
            if (phongLight) {
                updateLight(shader, *phongLight);
            }
        };
        return script;
    }

    ShaderScript phongLightUboScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto phongLights = registry.view<PhongLightComponent>();
            for (auto [entity, phongLight] : phongLights.each()) {
                updateUboLight(shader, phongLight);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto phongLight = entity.getPtr<PhongLightComponent>();
            if (phongLight) {
                updateUboLight(shader, *phongLight);
            }
        };
        return script;
    }

    ShaderScript pointLightScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto pointLights = registry.view<PointLightComponent>();
            for (auto [entity, pointLight] : pointLights.each()) {
                updateLight(shader, pointLight);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto pointLight = entity.getPtr<PointLightComponent>();
            if (pointLight) {
                updateLight(shader, *pointLight);
            }
        };
        return script;
    }

    ShaderScript pointLightArrayScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto pointLights = registry.view<PointLightComponent>();
            auto i = 0;
            for (auto [entity, pointLight] : pointLights.each()) {
                updateLight(shader, pointLight, i++);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto pointLight = entity.getPtr<PointLightComponent>();
            if (pointLight) {
                updateLight(shader, *pointLight, 0);
            }
        };
        return script;
    }

    ShaderScript pointLightUboScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto pointLights = registry.view<PointLightComponent>();
            for (auto [entity, pointLight] : pointLights.each()) {
                updateUboLight(shader, pointLight);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto pointLight = entity.getPtr<PointLightComponent>();
            if (pointLight) {
                updateUboLight(shader, *pointLight);
            }
        };
        return script;
    }

    ShaderScript directLightScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto directLights = registry.view<DirectLightComponent>();
            for (auto [entity, directLight] : directLights.each()) {
                updateLight(shader, directLight);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto directLight = entity.getPtr<DirectLightComponent>();
            if (directLight) {
                updateLight(shader, *directLight);
            }
        };
        return script;
    }

    ShaderScript directLightUboScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto directLights = registry.view<DirectLightComponent>();
            for (auto [entity, directLight] : directLights.each()) {
                updateUboLight(shader, directLight);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto directLight = entity.getPtr<DirectLightComponent>();
            if (directLight) {
                updateUboLight(shader, *directLight);
            }
        };
        return script;
    }

    ShaderScript flashLightScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto flashLights = registry.view<FlashLightComponent>();
            for (auto [entity, flashLight] : flashLights.each()) {
                updateLight(shader, flashLight);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto flashLight = entity.getPtr<FlashLightComponent>();
            if (flashLight) {
                updateLight(shader, *flashLight);
            }
        };
        return script;
    }

    ShaderScript flashLightUboScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto flashLights = registry.view<FlashLightComponent>();
            for (auto [entity, flashLight] : flashLights.each()) {
                updateUboLight(shader, flashLight);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto flashLight = entity.getPtr<FlashLightComponent>();
            if (flashLight) {
                updateUboLight(shader, *flashLight);
            }
        };
        return script;
    }

    void updateLight(const BaseShader& shader, PhongLightComponent& light) {
        shader.setUniformStructField(light.name, light.position);
        shader.setUniformStructField(light.name, light.ambient);
        shader.setUniformStructField(light.name, light.diffuse);
        shader.setUniformStructField(light.name, light.specular);
    }

    void updateLight(const BaseShader& shader, PointLightComponent& light) {
        shader.setUniformStructField(light.name, light.position);
        shader.setUniformStructField(light.name, light.ambient);
        shader.setUniformStructField(light.name, light.diffuse);
        shader.setUniformStructField(light.name, light.specular);
        shader.setUniformStructField(light.name, light.constant);
        shader.setUniformStructField(light.name, light.linear);
        shader.setUniformStructField(light.name, light.quadratic);
    }

    void updateLight(const BaseShader& shader, PointLightComponent& light, const uint32_t& index) {
        shader.setUniformArrayStructField(index, light.name, light.position);
        shader.setUniformArrayStructField(index, light.name, light.ambient);
        shader.setUniformArrayStructField(index, light.name, light.diffuse);
        shader.setUniformArrayStructField(index, light.name, light.specular);
        shader.setUniformArrayStructField(index, light.name, light.constant);
        shader.setUniformArrayStructField(index, light.name, light.linear);
        shader.setUniformArrayStructField(index, light.name, light.quadratic);
    }

    void updateLight(const BaseShader& shader, DirectLightComponent& light) {
        shader.setUniformStructField(light.name, light.direction);
        shader.setUniformStructField(light.name, light.ambient);
        shader.setUniformStructField(light.name, light.diffuse);
        shader.setUniformStructField(light.name, light.specular);
    }

    void updateLight(const BaseShader& shader, FlashLightComponent& light) {
        shader.setUniformStructField(light.name, light.position);
        shader.setUniformStructField(light.name, light.direction);
        shader.setUniformStructField(light.name, light.cutoff);
        shader.setUniformStructField(light.name, light.outerCutoff);
        shader.setUniformStructField(light.name, light.ambient);
        shader.setUniformStructField(light.name, light.diffuse);
        shader.setUniformStructField(light.name, light.specular);
        shader.setUniformStructField(light.name, light.constant);
        shader.setUniformStructField(light.name, light.linear);
        shader.setUniformStructField(light.name, light.quadratic);
    }

    void updateUboLight(const BaseShader& shader, PhongLightComponent& light) {
        uint32_t i = 0;
        shader.updateUniformBuffer(light.position, i++);
        shader.updateUniformBuffer(light.ambient, i++);
        shader.updateUniformBuffer(light.diffuse, i++);
        shader.updateUniformBuffer(light.specular, i++);
    }

    void updateUboLight(const BaseShader& shader, PointLightComponent& light) {
        uint32_t i = 4;
        shader.updateUniformBuffer(light.position, i++);
        shader.updateUniformBuffer(light.ambient, i++);
        shader.updateUniformBuffer(light.diffuse, i++);
        shader.updateUniformBuffer(light.specular, i++);
        shader.updateUniformBuffer(light.constant, i++);
        shader.updateUniformBuffer(light.linear, i++);
        shader.updateUniformBuffer(light.quadratic, i++);
    }

    void updateUboLight(const BaseShader& shader, DirectLightComponent& light) {
        uint32_t i = 0;
        shader.updateUniformBuffer(light.direction, i++);
        shader.updateUniformBuffer(light.ambient, i++);
        shader.updateUniformBuffer(light.diffuse, i++);
        shader.updateUniformBuffer(light.specular, i++);
    }

    void updateUboLight(const BaseShader& shader, FlashLightComponent& light) {
        uint32_t i = 0;
        shader.updateUniformBuffer(light.position, i++);
        shader.updateUniformBuffer(light.direction, i++);
        shader.updateUniformBuffer(light.cutoff, i++);
        shader.updateUniformBuffer(light.outerCutoff, i++);
        shader.updateUniformBuffer(light.ambient, i++);
        shader.updateUniformBuffer(light.diffuse, i++);
        shader.updateUniformBuffer(light.specular, i++);
        shader.updateUniformBuffer(light.constant, i++);
        shader.updateUniformBuffer(light.linear, i++);
        shader.updateUniformBuffer(light.quadratic, i++);
    }

}