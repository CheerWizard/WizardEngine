//
// Created by mecha on 27.03.2022.
//

#pragma once

#include "graphics/core/Renderer.h"

namespace engine {

    class TextRenderer : public Renderer {

    public:
        TextRenderer(const Ref<BaseShaderProgram>& shaderProgram)
        : Renderer(shaderProgram, QUAD, VERTEX) {
            init();
        }

    public:
        void render(entt::registry& registry);

    private:
        void init();
    };

}
