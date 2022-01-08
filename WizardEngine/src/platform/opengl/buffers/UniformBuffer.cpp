//
// Created by mecha on 02.10.2021.
//

#include "UniformBuffer.h"

#include "glad/glad.h"

namespace engine::shader {

    void UniformBuffer::create() {
        glGenBuffers(1, &id);
    }

    void UniformBuffer::destroy() const {
        glDeleteBuffers(1, &id);
    }

    void UniformBuffer::recreate() {
        destroy();
        create();
    }

    void UniformBuffer::bind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, id);
    }

    void UniformBuffer::unbind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBuffer::alloc() const {
        size_t size = uniformBlockFormat.size();
        alloc(size);
    }

    void UniformBuffer::alloc(const size_t &memorySize) const {
        glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr) memorySize, nullptr, GL_DYNAMIC_DRAW);
    }

    void UniformBuffer::bindBlock() const {
        glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockFormat.getId(), id);
    }

    void UniformBuffer::load(const UniformData &uniformData) const {
        const auto& attribute = uniformBlockFormat.get(uniformData.index);
        glBufferSubData(
                GL_UNIFORM_BUFFER,
                attribute.offset,
                size_of(attribute),
                uniformData.uniforms
        );
    }

    void UniformBuffer::applyFormat(const UniformBlockFormat &uniformBlockFormat) {
        this->uniformBlockFormat = uniformBlockFormat;
        bind();
        alloc();
        unbind();
    }

}