//
// Created by mecha on 24.08.2022.
//

#pragma once

#include <ecs/ecs.h>
#include <graphics/core/shader/BaseShader.h>

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    component(SolidPhong) {
        const char* name = "solidPhong";
        Vec4fUniform color = { "color", { 0.5, 0.5, 0.5, 1 } };
        FloatUniform ambient = { "ambient", 1 };
        FloatUniform diffuse = { "diffuse", 1 };
        FloatUniform specular = { "specular", 1 };
        FloatUniform shiny = { "shiny", 1 };

        SolidPhong() = default;
        SolidPhong(const vec4f& colorValue) {
            color.value = colorValue;
        }
    };

    ShaderScript solidPhongScript();

}
