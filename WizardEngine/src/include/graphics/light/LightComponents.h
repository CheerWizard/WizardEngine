//
// Created by mecha on 01.11.2021.
//

#pragma once

#include <ecs/ecs.h>
#include <graphics/core/shader/BaseShader.h>

namespace engine::graphics {

    using namespace shader;

    component(LightComponent) {
        serializable()
        const char* name = "light";
        Vec3fUniform position = { "position", { 0.5f, 0.5f, 0.5f } };
        Vec4fUniform color = { "color", { 1, 1, 1, 1 } };
    };

    ENGINE_API ShaderScript lightScript();

    component(PhongLightComponent) {
        serializable()
        const char* name = "phongLight";
        Vec3fUniform position = { "position", { 0.5f, 0.5f, 0.5f } };
        Vec4fUniform color = { "color", { 1, 1, 1, 1 } };
        FloatUniform ambient = { "ambient", 1 };
        FloatUniform diffuse = { "diffuse", 1 };
        FloatUniform specular = { "specular", 1 };
    };

    ENGINE_API ShaderScript phongLightScript();

    component(DirectLightComponent) {
        serializable()
        const char* name = "directLight";
        Vec4fUniform direction = { "direction", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform ambient = { "ambient", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform diffuse = { "diffuse", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform specular = { "specular", { 0.5f, 0.5f, 0.5f, 0 } };
    };

    component(PointLightComponent) {
        serializable()
        const char* name = "pointLight";
        Vec4fUniform position = { "position", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform ambient = { "ambient", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform diffuse = { "diffuse", { 0.5f, 0.5f, 0.5f, 0 } };
        Vec4fUniform specular = { "specular", { 0.5f, 0.5f, 0.5f, 0 } };
        FloatUniform constant = { "constant", 1};
        FloatUniform linear = { "linear", 0.045f};
        FloatUniform quadratic = { "quadratic", 0.0075f};
    };

    component(FlashLightComponent) {
        serializable()
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