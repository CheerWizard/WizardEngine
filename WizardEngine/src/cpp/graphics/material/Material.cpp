//
// Created by mecha on 28.08.2022.
//

#include <graphics/materials/Material.h>
#include <platform/graphics/TextureBuffer.h>

namespace engine::graphics {

    void Material::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Material";
        out << YAML::BeginMap;
        yaml::serialize(out, "color", color);
        yaml::serialize(out, "gamma", gamma);
        yaml::serialize(out, "brightness", brightness);
        yaml::serialize(out, "heightScale", heightScale);
        yaml::serialize(out, "minLayers", minLayers);
        yaml::serialize(out, "maxLayers", maxLayers);
        yaml::serialize(out, "metallic", metallic);
        yaml::serialize(out, "roughness", roughness);
        yaml::serialize(out, "ao", ao);
        yaml::serialize(out, "enableAlbedoMap", enableAlbedoMap);
        yaml::serialize(out, "enableNormalMap", enableNormalMap);
        yaml::serialize(out, "enableParallaxMap", enableParallaxMap);
        yaml::serialize(out, "enableMetallicMap", enableMetallicMap);
        yaml::serialize(out, "enableRoughnessMap", enableRoughnessMap);
        yaml::serialize(out, "enableAOMap", enableAOMap);
        albedoSlot.serialize(out);
        normalSlot.serialize(out);
        depthSlot.serialize(out);
        metallicSlot.serialize(out);
        roughnessSlot.serialize(out);
        aoSlot.serialize(out);
        out << YAML::EndMap;
    }

    void Material::deserialize(const YAML::Node &parent) {
        auto root = parent["Material"];
        if (root) {
            yaml::deserialize(root, "color", color);
            yaml::deserialize(root, "gamma", gamma);
            yaml::deserialize(root, "brightness", brightness);
            yaml::deserialize(root, "heightScale", heightScale);
            yaml::deserialize(root, "minLayers", minLayers);
            yaml::deserialize(root, "maxLayers", maxLayers);
            yaml::deserialize(root, "metallic", metallic);
            yaml::deserialize(root, "roughness", roughness);
            yaml::deserialize(root, "ao", ao);
            yaml::deserialize(root, "enableAlbedoMap", enableAlbedoMap);
            yaml::deserialize(root, "enableNormalMap", enableNormalMap);
            yaml::deserialize(root, "enableParallaxMap", enableParallaxMap);
            yaml::deserialize(root, "enableMetallicMap", enableMetallicMap);
            yaml::deserialize(root, "enableRoughnessMap", enableRoughnessMap);
            yaml::deserialize(root, "enableAOMap", enableAOMap);
            albedoSlot.deserialize(root);
            normalSlot.deserialize(root);
            depthSlot.deserialize(root);
            metallicSlot.deserialize(root);
            roughnessSlot.deserialize(root);
            aoSlot.deserialize(root);
        }
    }

    void MaterialShader::setMaterial(u32 index, Material *material) {
        shaderProgram.setUniformArrayStructField(index, material->name, material->color);
        shaderProgram.setUniformArrayStructField(index, material->name, material->gamma);
        shaderProgram.setUniformArrayStructField(index, material->name, material->brightness);
        shaderProgram.setUniformArrayStructField(index, material->name, material->minLayers);
        shaderProgram.setUniformArrayStructField(index, material->name, material->maxLayers);
        shaderProgram.setUniformArrayStructField(index, material->name, material->heightScale);
        shaderProgram.setUniformArrayStructField(index, material->name, material->roughness);
        shaderProgram.setUniformArrayStructField(index, material->name, material->metallic);
        shaderProgram.setUniformArrayStructField(index, material->name, material->ao);

        shaderProgram.setUniformArrayStructField(index, material->name, material->enableAlbedoMap);
        shaderProgram.setUniformArrayStructField(index, material->name, material->enableNormalMap);
        shaderProgram.setUniformArrayStructField(index, material->name, material->enableParallaxMap);
        shaderProgram.setUniformArrayStructField(index, material->name, material->enableMetallicMap);
        shaderProgram.setUniformArrayStructField(index, material->name, material->enableRoughnessMap);
        shaderProgram.setUniformArrayStructField(index, material->name, material->enableAOMap);

        if (material->enableAlbedoMap.value) {
            setMaterialTexture(index, material->name, material->albedoSlot);
        }

        if (material->enableNormalMap.value) {
            setMaterialTexture(index, material->name, material->normalSlot);
        }

        if (material->enableParallaxMap.value) {
            setMaterialTexture(index, material->name, material->depthSlot);
        }

        if (material->enableMetallicMap.value) {
            setMaterialTexture(index, material->name, material->metallicSlot);
        }

        if (material->enableRoughnessMap.value) {
            setMaterialTexture(index, material->name, material->roughnessSlot);
        }

        if (material->enableAOMap.value) {
            setMaterialTexture(index, material->name, material->aoSlot);
        }
    }

    void MaterialShader::setMaterialTexture(u32 index, const char* materialName, const TextureComponent& texture) {
        if (texture.textureId != invalidTextureId) {
            ENGINE_INFO("MaterialShader::setMaterialTexture()");
            IntUniform sampler = {
                    texture.sampler.name,
                    static_cast<int>(texture.sampler.value + index * MATERIAL_TEXTURES_COUNT)
            };
            shaderProgram.setUniformArrayStructField(index, materialName, sampler);
            TextureBuffer::activate(texture.sampler.value + index * MATERIAL_TEXTURES_COUNT);
            TextureBuffer::bind(texture.textureId, texture.typeId);
        }
    }

}