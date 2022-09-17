//
// Created by mecha on 28.08.2022.
//

#include <graphics/materials/Material.h>
#include <platform/graphics/TextureBuffer.h>

namespace engine::graphics {

    void MaterialShader::setMaterial(u32 index, Material *material) {
        // upload primitives
        for (const auto& primitive : material->floats) {
            shaderProgram.setUniformArrayStructField(index, material->name, primitive);
        }
        for (const auto& primitive : material->doubles) {
            shaderProgram.setUniformArrayStructField(index, material->name, primitive);
        }
        for (const auto& primitive : material->ints) {
            shaderProgram.setUniformArrayStructField(index, material->name, primitive);
        }
        for (const auto& primitive : material->flags) {
            shaderProgram.setUniformArrayStructField(index, material->name, primitive);
        }
        // upload vectors
        for (auto& vec : material->vec4) {
            shaderProgram.setUniformArrayStructField(index, material->name, vec);
        }
        // upload primitive arrays
        for (const auto& primitiveArray : material->intArrays) {
            shaderProgram.setUniformArray(primitiveArray);
        }
    }

}