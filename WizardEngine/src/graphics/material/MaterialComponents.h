//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../core/shader/Uniform.h"

#include "string"

namespace engine {

    struct TextureComponent {
        std::string fileName;
        IntUniform sampler;
    };

    struct MaterialComponent {
        const char* name = "material";
        Vec4fUniform color = { "color", {0.85, 0.85, 0.85, 1} };
        Vec3fUniform ambient = { "ambient", {1.0f, 0.5f, 0.31f} };
        Vec3fUniform diffuse = { "diffuse", {1.0f, 0.5f, 0.31f} };
        Vec3fUniform specular = { "specular", {0.5f, 0.5f, 0.5f} };
        FloatUniform shiny = { "shiny", 32.0f };
    };

}