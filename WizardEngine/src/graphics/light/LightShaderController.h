//
// Created by mecha on 03.11.2021.
//

#pragma once

#include "../core/shader/ShaderController.h"

namespace engine {

    class LightShaderController : public ShaderController {

    public:
        LightShaderController() = default;
        ~LightShaderController() override = default;

    public:
        void updateShader(const Ref<engine::Shader> &shader, const Entity &entity) override;

    };

}