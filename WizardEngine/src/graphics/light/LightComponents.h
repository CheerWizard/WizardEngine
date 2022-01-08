//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../core/shader/Uniform.h"

using namespace engine::shader;

namespace engine {

    struct PhongLightComponent {
        const char* name = "phongLight";
        Vec4fUniform position = { "position", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform ambient = { "ambient", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform diffuse = { "diffuse", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform specular = { "specular", { 0.5f, 0.5f, 0.5f, 0 } };
    };

    struct DirectLightComponent {
        const char* name = "directLight";
        Vec4fUniform direction = { "direction", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform ambient = { "ambient", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform diffuse = { "diffuse", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform specular = { "specular", { 0.5f, 0.5f, 0.5f, 0 } };
    };

    struct PointLightComponent {
        const char* name = "pointLight";
        Vec4fUniform position = { "position", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform ambient = { "ambient", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform diffuse = { "diffuse", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform specular = { "specular", { 0.5f, 0.5f, 0.5f, 0 } };
        FloatUniform constant = { "constant", 1};
        FloatUniform linear = { "linear", 0.045f};
        FloatUniform quadratic = { "quadratic", 0.0075f};
    };

    struct FlashLightComponent {
        const char* name = "flashLight";
        Vec4fUniform position = { "position", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform direction = { "direction", { 0.5f, 0.5f, 0.5f, 0 } };
        FloatUniform cutoff = { "cutoff", 0.5};
        FloatUniform outerCutoff = { "outerCutoff", 0.5};
        Vec4fUniform ambient = { "ambient", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform diffuse = { "diffuse", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform specular = { "specular", { 0.5f, 0.5f, 0.5f, 0 } };
        FloatUniform constant = { "constant", 1};
        FloatUniform linear = { "linear", 0.045f};
        FloatUniform quadratic = { "quadratic", 0.0075f};
    };
}