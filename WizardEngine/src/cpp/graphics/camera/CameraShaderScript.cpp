//
// Created by mecha on 01.11.2021.
//

#include <graphics/camera/CameraShaderScript.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::shader {

    using namespace graphics;

    ShaderScript camera2dUboScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto camera = entity.get<Camera2dComponent>();
            if (camera) {
                shader.getVShader().updateUniformBuffer(camera->viewProjection, 0);
            }
        };
        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<Camera2dComponent>([&shader](Camera2dComponent* camera) {
                shader.getVShader().updateUniformBuffer(camera->viewProjection, 0);
            });
        };
        return script;
    }

    ShaderScript camera3dUboScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto camera = entity.get<Camera3dComponent>();
            if (camera) {
                auto& viewPosition = camera->viewProjection.viewMatrix.position;
                shader.setUniform(viewPosition);
                shader.getVShader().updateUniformBuffer(camera->viewProjection, 0);
            }
        };
        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<Camera3dComponent>([&shader](Camera3dComponent* camera) {
                auto& viewPosition = camera->viewProjection.viewMatrix.position;
                shader.setUniform(viewPosition);
                shader.getVShader().updateUniformBuffer(camera->viewProjection, 0);
            });
        };
        return script;
    }

    ShaderScript camera2dScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto camera = entity.get<Camera2dComponent>();
            if (camera) {
                shader.setUniform(camera->viewProjection);
            }
        };
        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<Camera2dComponent>([&shader](Camera2dComponent* camera) {
                shader.setUniform(camera->viewProjection);
            });
        };
        return script;
    }

    ShaderScript camera3dScript() {
        auto script = ShaderScript();

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto camera = entity.get<Camera3dComponent>();
            if (camera) {
                auto& viewPosition = camera->viewProjection.viewMatrix.position;
                shader.setUniform(viewPosition);
                shader.setUniform(camera->viewProjection);
            }
        };

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            registry.each<Camera3dComponent>([&shader](Camera3dComponent* camera) {
                auto& viewPosition = camera->viewProjection.viewMatrix.position;
                shader.setUniform(viewPosition);
                shader.setUniform(camera->viewProjection);
            });
        };

        return script;
    }

}