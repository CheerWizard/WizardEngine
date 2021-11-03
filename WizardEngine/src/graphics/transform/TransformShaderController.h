//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../core/shader/ShaderController.h"

namespace engine {

    class TransformShaderController : public ShaderController {

    public:
        TransformShaderController() : ShaderController() {}
        ~TransformShaderController() override = default;

        void updateShader(const Ref<Shader> &shader, const Entity &entity) override;
    };

}