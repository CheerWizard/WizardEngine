//
// Created by mecha on 24.08.2022.
//

#pragma once

#include <ecs/ecs.h>
#include <graphics/core/shader/BaseShader.h>

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    component(ColorMaterial) {
        Vec4fUniform color = { "color", { 0.5, 0.5, 0.5, 1 } };

        ColorMaterial() = default;
        ColorMaterial(const vec4f& colorValue) {
            color = colorValue;
        }
    };

    ShaderScript colorMaterialScript();

}