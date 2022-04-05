//
// Created by mecha on 01.11.2021.
//

#include <graphics/camera/CameraShaderScript.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::shader {

    using namespace graphics;

    ShaderScript camera2dUboScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto camera = entity.getPtr<Camera2dComponent>();
            if (camera) {
                shader.updateUniformBuffer(*camera, 0);
            }
        };
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto cameras = registry.view<Camera2dComponent>();
            for (auto [entity, camera] : cameras.each()) {
                shader.updateUniformBuffer(camera, 0);
            }
        };
        return script;
    }

    ShaderScript camera3dUboScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto camera = entity.getPtr<Camera3dComponent>();
            if (camera) {
                auto& viewPosition = camera->viewMatrix.position;
                shader.setUniform(viewPosition);
                viewPosition.isUpdated = false;
                shader.updateUniformBuffer(*camera, 0);
            }
        };
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto cameras = registry.view<Camera3dComponent>();
            for (auto [entity, camera] : cameras.each()) {
                auto& viewPosition = camera.viewMatrix.position;
                shader.setUniform(viewPosition);
                viewPosition.isUpdated = false;
                shader.updateUniformBuffer(camera, 0);
            }
        };
        return script;
    }

    ShaderScript camera2dScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto camera = entity.getPtr<Camera2dComponent>();
            if (camera) {
                shader.setUniform(*camera);
                camera->isUpdated = false;
            }
        };
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto cameras = registry.view<Camera2dComponent>();
            for (auto [entity, camera] : cameras.each()) {
                shader.setUniform(camera);
                camera.isUpdated = false;
            }
        };
        return script;
    }

    ShaderScript camera3dScript() {
        auto script = ShaderScript();
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto camera = entity.getPtr<Camera3dComponent>();
            if (camera) {
                auto& viewPosition = camera->viewMatrix.position;
                shader.setUniform(viewPosition);
                shader.setUniform(*camera);
                viewPosition.isUpdated = false;
                camera->isUpdated = false;
            }
        };
        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto cameras = registry.view<Camera3dComponent>();
            for (auto [entity, camera] : cameras.each()) {
                auto& viewPosition = camera.viewMatrix.position;
                shader.setUniform(viewPosition);
                shader.setUniform(camera);
                viewPosition.isUpdated = false;
                camera.isUpdated = false;
            }
        };
        return script;
    }

}