//
// Created by mecha on 07.01.2022.
//

#pragma once

#include "../core/Renderer.h"

namespace engine {

    class PostEffect : public Renderer {

    public:
        PostEffect(const shader::BaseShaderProgram& shaderProgram) : Renderer(shaderProgram) {}
        ~PostEffect() {
            release();
        }

    public:
        void render();

    };

}
