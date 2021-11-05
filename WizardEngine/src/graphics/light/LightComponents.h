//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../core/shader/Uniform.h"

namespace engine {

    struct AmbientLightComponent {
        const char* name = "ambientLight";
        Vec3fUniform color = { "color", { 1, 1, 1 } };
        FloatUniform strength = { "strength", 0.5f};
    };

    struct DiffuseLightComponent {
        const char* name = "diffuseLight";
        Vec3fUniform color = { "color", { 1, 1, 1 } };
        Vec3fUniform position = { "position", { 0.5f, 0.5f, 0.5f } };
    };

    struct SpecularLightComponent {
        const char* name = "specularLight";
        Vec3fUniform color = { "color", { 1, 1, 1 } };
        Vec3fUniform position = { "position", { 0.5f, 0.5f, 0.5f } };
        FloatUniform strength = { "strength", 0.5f};
    };

    struct PhongLightComponent {
        const char* name = "phongLight";
        Vec3fUniform color = { "color", { 1, 1, 1 } };
        Vec3fUniform position = { "position", { 0.5f, 0.5f, 0.5f } };
        FloatUniform ambientStrength = { "ambientStrength", 0.5f};
        FloatUniform specularStrength = { "specularStrength", 0.5f};
    };

    struct DirectionLightComponent {
        const char* name = "directionLight";
        Vec3fUniform color = { "color", { 1, 1, 1 } };
        Vec3fUniform direction = { "direction", { 0.5f, 0.5f, 0.5f } };
        Vec3fUniform ambient = { "ambient", { 0.5f, 0.5f, 0.5f } };
        Vec3fUniform diffuse = { "diffuse", { 0.5f, 0.5f, 0.5f } };
        Vec3fUniform specular = { "specular", { 0.5f, 0.5f, 0.5f } };
    };

    struct PointLightComponent {
        const char* name = "pointLight";
        Vec3fUniform color = { "color", { 1, 1, 1 } };
        Vec3fUniform position = { "position", { 0.5f, 0.5f, 0.5f } };
        Vec3fUniform ambient = { "ambient", { 0.5f, 0.5f, 0.5f } };
        Vec3fUniform diffuse = { "diffuse", { 0.5f, 0.5f, 0.5f } };
        Vec3fUniform specular = { "specular", { 0.5f, 0.5f, 0.5f } };
        FloatUniform constant = { "constant", 0.5f};
        FloatUniform linear = { "linear", 0.5f};
        FloatUniform quadratic = { "quadratic", 0.5f};
    };
}