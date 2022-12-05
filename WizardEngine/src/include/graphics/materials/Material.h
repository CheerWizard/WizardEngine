//
// Created by mecha on 26.08.2022.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>
#include <graphics/core/texture/Texture.h>
#include <platform/graphics/TextureBuffer.h>
#include <platform/graphics/tools/VideoStats.h>

#define MATERIAL_TEXTURES_COUNT 8

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    serialize_component(Material) {
        const char* name = "material";
        const char* title = "Untitled";
        Vec4fUniform color = { "color", { 1, 1, 1, 1 } };
        FloatUniform ambient = { "ambient", 1 };
        FloatUniform diffuse = { "diffuse", 1 };
        FloatUniform specular = { "specular", 1 };
        FloatUniform shiny = { "shiny", 1 };
        FloatUniform gamma = { "gamma", 2.2 };
        FloatUniform heightScale = { "heightScale", 0.1 };
        FloatUniform minLayers = { "minLayers", 8 };
        FloatUniform maxLayers = { "maxLayers", 32 };
        FloatUniform brightness = { "brightness", 1 };
        FloatUniform metallic = { "metallic", 0.5 };
        FloatUniform roughness = { "roughness", 0.5 };
        FloatUniform ao = { "ao", 0.5 };
        BoolUniform enableAlbedoMap = { "enableAlbedoMap", false };
        BoolUniform enableDiffuseMap = { "enableDiffuseMap", false };
        BoolUniform enableSpecularMap = { "enableSpecularMap", false };
        BoolUniform enableBlinn = { "enableBlinn", true };
        BoolUniform enableNormalMap = { "enableNormalMap", false };
        BoolUniform enableParallaxMap = { "enableParallaxMap", false };
        BoolUniform enableMetallicMap = { "enableMetallicMap", false };
        BoolUniform enableRoughnessMap = { "enableRoughnessMap", false };
        BoolUniform enableAOMap = { "enableAOMap", false };
        TextureComponent albedoSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "albedoSlot", 0 }
        };
        TextureComponent diffuseSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "diffuseSlot", 1 }
        };
        TextureComponent specularSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "specularSlot", 2 }
        };
        TextureComponent normalSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "normalSlot", 3 }
        };
        TextureComponent depthSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "depthSlot", 4 }
        };
        TextureComponent metallicSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "metallicSlot", 5 }
        };
        TextureComponent roughnessSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "roughnessSlot", 6 }
        };
        TextureComponent aoSlot = {
                TextureType::TEXTURE_2D,
                IntUniform { "aoSlot", 7 }
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
