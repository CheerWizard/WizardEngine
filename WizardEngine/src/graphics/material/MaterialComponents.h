//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../core/shader/Uniform.h"

#include "string"

using namespace engine::shader;

namespace engine {

    struct TextureComponent {
        std::string fileName;
        IntUniform sampler;
    };

    struct MaterialComponent {
        const char* name = "material";
        Vec4fUniform color = { "color", {0.25, 0.5, 0.75, 1} };
        FloatUniform ambient = { "ambient", 0.1f };
        FloatUniform diffuse = { "diffuse", 0.5f };
        FloatUniform specular = { "specular", 0.5f };
        FloatUniform shiny = { "shiny", 32.0f };
        BoolUniform diffuseMapEnabled = { "diffuseMap", false };
        BoolUniform specularMapEnabled = { "specularMap", false };
    };

    struct MaterialMapsComponent {
        const char* name = "materialMaps";
        std::string diffuseFileName;
        std::string specularFileName;
        IntUniform diffuse = { "diffuse", 0 };
        IntUniform specular = { "specular", 1 };
    };
}