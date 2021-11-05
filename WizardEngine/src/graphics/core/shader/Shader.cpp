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

    void Shader::bindVUniformBlock() {
        bindUniformBlock(vUniformBlockFormat);
    }

    void Shader::bindFUniformBlock() {
        bindUniformBlock(fUniformBlockFormat);
    }

    void Shader::updateUniformBuffer(
            const Ref<UniformBuffer> &uniformBuffer,
            const UniformData &uniformData
    ) {
        uniformBuffer->setUniformBlockPointer();
        uniformBuffer->bind();
        uniformBuffer->load(uniformData);
        uniformBuffer->unbind();
    }

    void Shader::updateVUniformBuffer(const UniformData &uniformData) {
        updateUniformBuffer(vUniformBuffer, uniformData);
    }

    void Shader::updateFUniformBuffer(const UniformData &uniformData) {
        updateUniformBuffer(fUniformBuffer, uniformData);
    }
}