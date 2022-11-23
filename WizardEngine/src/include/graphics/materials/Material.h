//
// Created by mecha on 26.08.2022.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>
#include <graphics/core/texture/Texture.h>
#include <platform/graphics/TextureBuffer.h>
#include <platform/graphics/tools/VideoStats.h>

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    serialize_component(Material) {
        const char* name = "material";
        Vec4fUniform color = { "color", { 0.5, 0.5, 0.5, 1 } };
        FloatUniform ambient = { "ambient", 1 };
        FloatUniform diffuse = { "diffuse", 1 };
        FloatUniform specular = { "specular", 1 };
        FloatUniform shiny = { "specular", 1 };
        FloatUniform gamma = { "specular", 2.2 };
        FloatUniform heightScale = { "specular", 0.1 };
        FloatUniform minLayers = { "specular", 8 };
        FloatUniform maxLayers = { "specular", 32 };
        FloatUniform brightness = { "specular", 1 };
        BoolUniform enableAlbedoMap = { "enableAlbedoMap", false };
        BoolUniform enableDiffuseMap = { "enableAlbedoMap", false };
        BoolUniform enableSpecularMap = { "enableAlbedoMap", false };
        BoolUniform enableBlinn = { "enableAlbedoMap", false };
        BoolUniform enableNormalMap = { "enableAlbedoMap", false };
        BoolUniform enableParallaxMap = { "enableAlbedoMap", false };
        TextureComponent albedoSlot = {
                TextureBuffer::getTypeId(TextureType::TEXTURE_2D),
                IntUniform { "albedoSlot", 0 }
        };
        TextureComponent diffuseSlot = {
                TextureBuffer::getTypeId(TextureType::TEXTURE_2D),
                IntUniform { "diffuseSlot", 1 }
        };
        TextureComponent specularSlot = {
                TextureBuffer::getTypeId(TextureType::TEXTURE_2D),
                IntUniform { "specularSlot", 2 }
        };
        TextureComponent normalSlot = {
                TextureBuffer::getTypeId(TextureType::TEXTURE_2D),
                IntUniform { "normalSlot", 3 }
        };
        TextureComponent depthSlot = {
                TextureBuffer::getTypeId(TextureType::TEXTURE_2D),
                IntUniform { "depthSlot", 4 }
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
