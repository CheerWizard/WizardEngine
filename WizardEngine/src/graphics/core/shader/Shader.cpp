//
// Created by mecha on 05.09.2021.
//

#include "Shader.h"

#include "fstream"

namespace engine {

    const VertexAttribute& Shader::getAttribute(const uint32_t &index) const {
        return vertexFormat->get(index);
    }

    void Shader::removeAttribute(const uint32_t &index) {
        vertexFormat->remove(index);
    }

    uint32_t Shader::addAttribute(const VertexAttribute &attribute) {
        return vertexFormat->add(attribute);
    }

    void Shader::replaceAttribute(const uint32_t &index, const VertexAttribute &attribute) {
        vertexFormat->replace(index, attribute);
    }

    uint32_t Shader::addUniformBlockAttr(const UniformAttribute &uniformAttribute) {
        return uniformBlockFormat->add(uniformAttribute);
    }

    const UniformAttribute& Shader::getUniformBlockAttr(const uint32_t &index) {
        return uniformBlockFormat->get(index);
    }

    void Shader::updateUniformBuffer(Mat4fUniform &uniform) {
        if (uniform.isUpdated) {
            uniform.isUpdated = false;

            auto uniformData = UniformData {
                uniform.toFloatPtr(),
                0
            };
            uniformBuffer->setUniformBlockPointer();
            uniformBuffer->bind();
            uniformBuffer->load(uniformData);
            uniformBuffer->unbind();
        }
    }

    void Shader::tryUpdateUniformBuffer(Mat4fUniform &uniform) {
        if (state == ShaderState::NO_UNIFORM_BLOCKS) {
            setUniform(uniform);
        } else {
            updateUniformBuffer(uniform);
        }
    }
}