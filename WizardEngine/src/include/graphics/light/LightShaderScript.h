//
// Created by mecha on 03.11.2021.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>
#include <graphics/light/LightComponents.h>

namespace engine::graphics {
    using namespace shader;
    ShaderScript pointLightScript();
    ShaderScript pointLightArrayScript();
    ShaderScript pointLightUboScript();
    ShaderScript directLightScript();
    ShaderScript directLightUboScript();
    ShaderScript flashLightScript();
    ShaderScript flashLightUboScript();
    void updateLight(const BaseShaderProgram& shader, PhongLightComponent& light);
    void updateLight(const BaseShaderProgram& shader, PointLightComponent& light);
    void updateLight(const BaseShaderProgram& shader, PointLightComponent& light, const u32& index);
    void updateLight(const BaseShaderProgram& shader, DirectLightComponent& light);
    void updateLight(const BaseShaderProgram& shader, FlashLightComponent& light);
    void updateUboLight(const BaseShaderProgram& shader, PhongLightComponent& light);
    void updateUboLight(const BaseShaderProgram& shader, PointLightComponent& light);
    void updateUboLight(const BaseShaderProgram& shader, DirectLightComponent& light);
    void updateUboLight(const BaseShaderProgram& shader, FlashLightComponent& light);
}