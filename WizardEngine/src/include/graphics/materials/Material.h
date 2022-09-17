//
// Created by mecha on 26.08.2022.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>
#include <graphics/core/texture/Texture.h>
#include <platform/graphics/tools/VideoStats.h>

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    component(Material) {
        const char* name;
        // primitives
        vector<FloatUniform> floats;
        vector<BoolUniform> flags;
        vector<IntUniform> ints;
        vector<DoubleUniform> doubles;
        // vectors
        vector<Vec4fUniform> vec4;
        // primitive arrays
        vector<IntUniformArray> intArrays;

        Material(const char* name) : name(name) {}
    };

    component(MaterialList) {
        vector<Material> materials;
    };

    class ENGINE_API MaterialShader final {

    public:
        MaterialShader() = default;
        MaterialShader(const BaseShaderProgram& shaderProgram) : shaderProgram(shaderProgram) {}

    public:
        void setMaterial(u32 index, Material* material);

    private:
        BaseShaderProgram shaderProgram;
    };

}
