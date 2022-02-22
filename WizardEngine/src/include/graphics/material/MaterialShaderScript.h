//
// Created by mecha on 01.11.2021.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>
#include <graphics/core/sources/TextureSource.h>
#include "MaterialComponents.h"

namespace engine::shader {
    ShaderScript textureScript();
    ShaderScript textureUboScript();
    ShaderScript materialScript();
    ShaderScript materialUboScript();
    ShaderScript materialArrayScript();
    ShaderScript materialMapScript();
    ShaderScript materialMapUboScript();
    ShaderScript materialMapArrayScript();
    void activateTexture(const std::string &fileName, const uint32_t &slot);
    void updateMaterial(const BaseShader& shader, MaterialComponent& material);
    void updateMaterial(const BaseShader& shader, MaterialComponent& material, const uint32_t& index);
    void updateMaterialUbo(const BaseShader& shader, MaterialComponent& material, const uint32_t& index);
    void updateMaterialMaps(const BaseShader& shader, MaterialMapsComponent& materialMaps);
    void updateMaterialMaps(const BaseShader& shader, MaterialMapsComponent& materialMaps, const uint32_t& index);
    void updateMaterialMapsUbo(const BaseShader& shader, MaterialMapsComponent& materialMaps);
}
