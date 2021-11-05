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
        virtual void updateShader(const Ref<Shader> &shader, const Entity &entity, const uint32_t &instanceId) = 0;

    protected:
        void updateVUniform(const Ref<Shader> &shader, Mat4fUniform &uniform, const uint32_t &uniformDataIndex = 0);
        void updateFUniform(const Ref<Shader> &shader, Mat4fUniform &uniform, const uint32_t &uniformDataIndex = 0);

    };

}
