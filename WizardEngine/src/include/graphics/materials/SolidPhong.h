//
// Created by mecha on 24.08.2022.
//

#pragma once

#include <graphics/materials/Material.h>

namespace engine::graphics {

    struct SolidPhong : Material {

        SolidPhong(const char* name = "solidPhong") : Material(name) {
            floats.emplace_back("ambient", 1);
            floats.emplace_back("diffuse", 1);
            floats.emplace_back("specular", 1);
            floats.emplace_back("shiny", 1);
            vec4.emplace_back("color", vec4f { 0.5, 0.5, 0.5, 1 });
        }

        SolidPhong(const vec4f& colorValue) : Material("solidPhong") {
            floats.emplace_back("ambient", 1);
            floats.emplace_back("diffuse", 1);
            floats.emplace_back("specular", 1);
            floats.emplace_back("shiny", 1);
            vec4.emplace_back("color", colorValue);
        }

        inline FloatUniform& ambient() {
            return floats[0];
        }

        inline FloatUniform& diffuse() {
            return floats[1];
        }

        inline FloatUniform& specular() {
            return floats[2];
        }

        inline FloatUniform& shiny() {
            return floats[3];
        }

        inline Vec4fUniform& color() {
            return vec4[0];
        }
    };

}
