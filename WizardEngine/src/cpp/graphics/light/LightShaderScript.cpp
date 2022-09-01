//
// Created by mecha on 03.11.2021.
//

#include <graphics/light/LightShaderScript.h>

namespace engine::graphics {

    ShaderScript pointLightScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<PointLightComponent>([&shader](PointLightComponent* pointLight) {
                updateLight(shader, *pointLight);
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto pointLight = entity.get<PointLightComponent>();
            if (pointLight) {
                updateLight(shader, *pointLight);
            }
        };

        return script;
    }

    ShaderScript pointLightArrayScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            auto i = 0;
            registry.each<PointLightComponent>([&shader, &i](PointLightComponent* pointLight) {
                updateLight(shader, *pointLight, i++);
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto pointLight = entity.get<PointLightComponent>();
            if (pointLight) {
                updateLight(shader, *pointLight, 0);
            }
        };

        return script;
    }

    ShaderScript pointLightUboScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<PointLightComponent>([&shader](PointLightComponent* pointLight) {
                updateUboLight(shader, *pointLight);
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto pointLight = entity.get<PointLightComponent>();
            if (pointLight) {
                updateUboLight(shader, *pointLight);
            }
        };

        return script;
    }

    ShaderScript directLightScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<DirectLightComponent>([&shader](DirectLightComponent* directLight) {
                updateLight(shader, *directLight);
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto directLight = entity.get<DirectLightComponent>();
            if (directLight) {
                updateLight(shader, *directLight);
            }
        };

        return script;
    }

    ShaderScript directLightUboScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<DirectLightComponent>([&shader](DirectLightComponent* directLight) {
                updateUboLight(shader, *directLight);
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto directLight = entity.get<DirectLightComponent>();
            if (directLight) {
                updateUboLight(shader, *directLight);
            }
        };

        return script;
    }

    ShaderScript flashLightScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<FlashLightComponent>([&shader](FlashLightComponent* flashLight) {
                updateLight(shader, *flashLight);
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto flashLight = entity.get<FlashLightComponent>();
            if (flashLight) {
                updateLight(shader, *flashLight);
            }
        };

        return script;
    }

    ShaderScript flashLightUboScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<FlashLightComponent>([&shader](FlashLightComponent* flashLight) {
                updateUboLight(shader, *flashLight);
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto flashLight = entity.get<FlashLightComponent>();
            if (flashLight) {
                updateUboLight(shader, *flashLight);
            }
        };

        return script;
    }

    void updateLight(const BaseShaderProgram& shader, PhongLightComponent& light) {
        shader.setUniformStructField(light.name, light.position);
        shader.setUniformStructField(light.name, light.ambient);
        shader.setUniformStructField(light.name, light.diffuse);
        shader.setUniformStructField(light.name, light.specular);
    }

    void updateLight(const BaseShaderProgram& shader, PointLightComponent& light) {
        shader.setUniformStructField(light.name, light.position);
        shader.setUniformStructField(light.name, light.ambient);
        shader.setUniformStructField(light.name, light.diffuse);
        shader.setUniformStructField(light.name, light.specular);
        shader.setUniformStructField(light.name, light.constant);
        shader.setUniformStructField(light.name, light.linear);
        shader.setUniformStructField(light.name, light.quadratic);
    }

    void updateLight(const BaseShaderProgram& shader, PointLightComponent& light, const uint32_t& index) {
        shader.setUniformArrayStructField(index, light.name, light.position);
        shader.setUniformArrayStructField(index, light.name, light.ambient);
        shader.setUniformArrayStructField(index, light.name, light.diffuse);
        shader.setUniformArrayStructField(index, light.name, light.specular);
        shader.setUniformArrayStructField(index, light.name, light.constant);
        shader.setUniformArrayStructField(index, light.name, light.linear);
        shader.setUniformArrayStructField(index, light.name, light.quadratic);
    }

    void updateLight(const BaseShaderProgram& shader, DirectLightComponent& light) {
        shader.setUniformStructField(light.name, light.direction);
        shader.setUniformStructField(light.name, light.ambient);
        shader.setUniformStructField(light.name, light.diffuse);
        shader.setUniformStructField(light.name, light.specular);
    }

    void updateLight(const BaseShaderProgram& shader, FlashLightComponent& light) {
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

    void updateUboLight(const BaseShaderProgram& shader, PhongLightComponent& light) {
        uint32_t i = 0;
        shader.getFShader().updateUniformBuffer(light.position, i++);
        shader.getFShader().updateUniformBuffer(light.ambient, i++);
        shader.getFShader().updateUniformBuffer(light.diffuse, i++);
        shader.getFShader().updateUniformBuffer(light.specular, i++);
    }

    void updateUboLight(const BaseShaderProgram& shader, PointLightComponent& light) {
        uint32_t i = 4;
        shader.getFShader().updateUniformBuffer(light.position, i++);
        shader.getFShader().updateUniformBuffer(light.ambient, i++);
        shader.getFShader().updateUniformBuffer(light.diffuse, i++);
        shader.getFShader().updateUniformBuffer(light.specular, i++);
        shader.getFShader().updateUniformBuffer(light.constant, i++);
        shader.getFShader().updateUniformBuffer(light.linear, i++);
        shader.getFShader().updateUniformBuffer(light.quadratic, i++);
    }

    void updateUboLight(const BaseShaderProgram& shader, DirectLightComponent& light) {
        uint32_t i = 0;
        shader.getFShader().updateUniformBuffer(light.direction, i++);
        shader.getFShader().updateUniformBuffer(light.ambient, i++);
        shader.getFShader().updateUniformBuffer(light.diffuse, i++);
        shader.getFShader().updateUniformBuffer(light.specular, i++);
    }

    void updateUboLight(const BaseShaderProgram& shader, FlashLightComponent& light) {
        uint32_t i = 0;
        shader.getFShader().updateUniformBuffer(light.position, i++);
        shader.getFShader().updateUniformBuffer(light.direction, i++);
        shader.getFShader().updateUniformBuffer(light.cutoff, i++);
        shader.getFShader().updateUniformBuffer(light.outerCutoff, i++);
        shader.getFShader().updateUniformBuffer(light.ambient, i++);
        shader.getFShader().updateUniformBuffer(light.diffuse, i++);
        shader.getFShader().updateUniformBuffer(light.specular, i++);
        shader.getFShader().updateUniformBuffer(light.constant, i++);
        shader.getFShader().updateUniformBuffer(light.linear, i++);
        shader.getFShader().updateUniformBuffer(light.quadratic, i++);
    }

}