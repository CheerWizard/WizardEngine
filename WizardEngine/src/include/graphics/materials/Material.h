//
// Created by mecha on 26.08.2022.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>
#include <graphics/core/texture/Texture.h>
#include <platform/graphics/TextureBuffer.h>
#include <platform/graphics/tools/VideoStats.h>

#define MATERIAL_TEXTURES_COUNT 6

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    serialize_component(Material) {
        const char* name = "material";
        const char* title = "Untitled";
        Vec4fUniform color = { "color", { 1, 1, 1, 1 } };
        FloatUniform brightness = { "brightness", 1 };
        FloatUniform gamma = { "gamma", 2.2 };
        FloatUniform heightScale = { "heightScale", 0.1 };
        FloatUniform minLayers = { "minLayers", 8 };
        FloatUniform maxLayers = { "maxLayers", 32 };
        FloatUniform metallic = { "metallic", 0.5 };
        FloatUniform roughness = { "roughness", 0.5 };
        FloatUniform ao = { "ao", 0.5 };
        BoolUniform enableAlbedoMap = { "enableAlbedoMap", false };
        BoolUniform enableNormalMap = { "enableNormalMap", false };
        BoolUniform enableParallaxMap = { "enableParallaxMap", false };
        BoolUniform enableMetallicMap = { "enableMetallicMap", false };
        BoolUniform enableRoughnessMap = { "enableRoughnessMap", false };
        BoolUniform enableAOMap = { "enableAOMap", false };
        TextureComponent albedoSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "albedoSlot", 0 }
        };
        TextureComponent normalSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "normalSlot", 1 }
        };
        TextureComponent depthSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "depthSlot", 2 }
        };
        TextureComponent metallicSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "metallicSlot", 3 }
        };
        TextureComponent roughnessSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "roughnessSlot", 4 }
        };
        TextureComponent aoSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "aoSlot", 5 }
        };

        ENGINE_API void serialize(YAML::Emitter &out) override;
        ENGINE_API void deserialize(const YAML::Node &parent) override;
    };

    class ENGINE_API MaterialShader final {

    public:
        MaterialShader() = default;
        MaterialShader(const BaseShaderProgram& shaderProgram) : shaderProgram(shaderProgram) {}

    public:
        void setMaterial(u32 index, Material* material);

    private:
        void setMaterialTexture(u32 index, const char* materialName, const TextureComponent& texture);

    private:
        BaseShaderProgram shaderProgram;
    };

}
