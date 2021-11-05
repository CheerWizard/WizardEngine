//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../core/shader/ShaderController.h"

namespace engine {

    class Camera2dShaderController : public ShaderController {

    public:
        Camera2dShaderController(): ShaderController() {}
        ~Camera2dShaderController() override = default;

        void updateShader(const Ref<Shader> &shader, const Entity &entity) override;
        void updateShader(const Ref<Shader> &shader, const Entity &entity, const uint32_t &instanceId) override;

    };

    class Camera3dShaderController : public ShaderController {

    public:
        Camera3dShaderController(): ShaderController() {}
        ~Camera3dShaderController() override = default;

        void updateShader(const Ref<Shader> &shader, const Entity &entity) override;
        void updateShader(const Ref<Shader> &shader, const Entity &entity, const uint32_t &instanceId) override;

    };

}