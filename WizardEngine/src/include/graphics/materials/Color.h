//
// Created by mecha on 26.08.2022.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    component(ColorComponent) {
        serializable()
        Vec4fUniform color = { "color", { 0.5, 0.5, 0.5, 1 } };

        ColorComponent() = default;
        ColorComponent(const vec4f& colorValue) {
            color = colorValue;
        }
    };

    ShaderScript colorScript();
}