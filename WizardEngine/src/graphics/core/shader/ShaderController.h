//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "Shader.h"
#include "../../../ecs/Entity.h"

namespace engine {

    class ShaderController {

    public:
        ShaderController() = default;
        virtual ~ShaderController() = default;

    public:
        virtual void updateShader(const Ref<Shader> &shader, const Entity &entity) = 0;

    };

}
