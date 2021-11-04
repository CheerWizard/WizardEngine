//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../core/shader/ShaderController.h"

namespace engine {

    class CameraShaderController : public ShaderController {

    public:
        CameraShaderController(): ShaderController() {}
        ~CameraShaderController() override = default;

        void updateShader(const Ref<Shader> &shader, const Entity &entity) override;

        void updateShader(const Ref<Shader> &shader, const Entity &entity, const uint32_t &instanceId) override;
    };

}