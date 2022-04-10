//
// Created by mecha on 01.11.2021.
//

#include <graphics/camera/CameraShaderScript.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::shader {

    using namespace graphics;

    ShaderScript camera2dUboScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto camera = entity.get<Camera2dComponent>();
            if (camera) {
                shader.updateUniformBuffer(camera->viewProjection, 0);
            }
        };
        script.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            registry.each<Camera2dComponent>([&shader](Camera2dComponent* camera) {
                shader.updateUniformBuffer(camera->viewProjection, 0);
            });
        };
        return script;
    }

    ShaderScript camera3dUboScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto camera = entity.get<Camera3dComponent>();
            if (camera) {
                auto& viewPosition = camera->viewProjection.viewMatrix.position;
                shader.setUniform(viewPosition);
                viewPosition.isUpdated = false;
                shader.updateUniformBuffer(camera->viewProjection, 0);
            }
        };
        script.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            registry.each<Camera3dComponent>([&shader](Camera3dComponent* camera) {
                auto& viewPosition = camera->viewProjection.viewMatrix.position;
                shader.setUniform(viewPosition);
                viewPosition.isUpdated = false;
                shader.updateUniformBuffer(camera->viewProjection, 0);
            });
        };
        return script;
    }

    ShaderScript camera2dScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto camera = entity.get<Camera2dComponent>();
            if (camera) {
                shader.setUniform(camera->viewProjection);
                camera->viewProjection.isUpdated = false;
            }
        };
        script.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            registry.each<Camera2dComponent>([&shader](Camera2dComponent* camera) {
                shader.setUniform(camera->viewProjection);
                camera->viewProjection.isUpdated = false;
            });
        };
        return script;
    }

    ShaderScript camera3dScript() {
        auto script = ShaderScript();

        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto camera = entity.get<Camera3dComponent>();
            if (camera) {
                auto& viewPosition = camera->viewProjection.viewMatrix.position;
                shader.setUniform(viewPosition);
                shader.setUniform(camera->viewProjection);
                viewPosition.isUpdated = false;
                camera->viewProjection.isUpdated = false;
            }
        };

        script.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            registry.each<Camera3dComponent>([&shader](Camera3dComponent* camera) {
                auto& viewPosition = camera->viewProjection.viewMatrix.position;
                shader.setUniform(viewPosition);
                shader.setUniform(camera->viewProjection);
                viewPosition.isUpdated = false;
                camera->viewProjection.isUpdated = false;
            });
        };

        return script;
    }

}