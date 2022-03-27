//
// Created by mecha on 27.03.2022.
//

#pragma once

#include "graphics/core/Renderer.h"

namespace engine {

    class TextRenderer : public Renderer {

    public:
        TextRenderer(const Ref<BaseShaderProgram>& shaderProgram, const DrawType& drawType = TRIANGLE)
        : Renderer(shaderProgram, drawType) {}

    public:
        void render(entt::registry& registry);

    };

}
