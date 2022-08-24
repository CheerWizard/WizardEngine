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
    void updateLight(const BaseShader& shader, PhongLightComponent& light);
    void updateLight(const BaseShader& shader, PointLightComponent& light);
    void updateLight(const BaseShader& shader, PointLightComponent& light, const u32& index);
    void updateLight(const BaseShader& shader, DirectLightComponent& light);
    void updateLight(const BaseShader& shader, FlashLightComponent& light);
    void updateUboLight(const BaseShader& shader, PhongLightComponent& light);
    void updateUboLight(const BaseShader& shader, PointLightComponent& light);
    void updateUboLight(const BaseShader& shader, DirectLightComponent& light);
    void updateUboLight(const BaseShader& shader, FlashLightComponent& light);
}