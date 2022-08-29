//
// Created by mecha on 28.08.2022.
//

#include <graphics/materials/Material.h>
#include <platform/graphics/TextureBuffer.h>

namespace engine::graphics {

    void MaterialShader::setMaterial(u32 index, Material *material) {
        // upload primitives
        for (auto primitive : material->floats) {
            shader.setUniformArrayStructField(index, material->name, primitive);
        }
        for (auto primitive : material->doubles) {
            shader.setUniformArrayStructField(index, material->name, primitive);
        }
        for (auto primitive : material->ints) {
            shader.setUniformArrayStructField(index, material->name, primitive);
        }
        for (auto primitive : material->flags) {
            shader.setUniformArrayStructField(index, material->name, primitive);
        }
        // upload vectors
        for (auto vec : material->vec4) {
            shader.setUniformArrayStructField(index, material->name, vec);
        }
        // upload textures
        int nextSlot = index * material->textures.size();
        for (auto texture : material->textures) {
            if (texture.isValid()) {
                texture.sampler.value += nextSlot;
                shader.setUniformArrayStructField(index, material->name, texture.sampler);
                TextureBuffer::activate(texture.sampler.value);
                TextureBuffer::bind(texture.textureId, TextureBuffer::getTypeId(static_cast<TextureType>(texture.typeId)));
            }
        }
    }

}