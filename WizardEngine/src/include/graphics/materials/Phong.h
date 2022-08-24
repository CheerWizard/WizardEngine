//
// Created by mecha on 24.08.2022.
//

#pragma once

#include <ecs/ecs.h>
#include <graphics/core/shader/BaseShader.h>
#include <graphics/core/texture/Texture.h>

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    component(Phong) {
        const char* name = "phong";
        Vec4fUniform color = { "color", { 0.5, 0.5, 0.5, 1 } };

        TextureComponent albedo = IntUniform { "albedo", 0 };
        FloatUniform ambient = { "ambient", 1 };

        TextureComponent diffuseMap = IntUniform { "diffuseMap", 1 };
        FloatUniform diffuse = { "diffuse", 1 };

        TextureComponent specularMap = IntUniform { "specularMap", 2 };
        FloatUniform specular = { "specular", 1 };
        FloatUniform shiny = { "shiny", 1 };

        Phong() = default;
        Phong(const vec4f& colorValue) {
            color.value = colorValue;
        }

        u32 slotCount() { return 3; }
    };

    ShaderScript solidPhongScript();

}