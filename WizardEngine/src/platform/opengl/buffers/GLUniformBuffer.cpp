//
// Created by mecha on 02.10.2021.
//

#include "GLUniformBuffer.h"

#include <glad/glad.h>

namespace engine {

    void GLUniformBuffer::create() {
        glGenBuffers(1, &id);
    }

    void GLUniformBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }

    void GLUniformBuffer::recreate() {
        destroy();
        create();
    }

    void GLUniformBuffer::bind() {
        glBindBuffer(GL_UNIFORM_BUFFER, id);
    }

    void GLUniformBuffer::unbind() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void GLUniformBuffer::allocate() {
        capacity = uniformBlockFormat->size();

        if (!hasCapacity()) return;
        glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr) capacity, nullptr, GL_DYNAMIC_DRAW);
    }

    void GLUniformBuffer::setUniformBlockPointer() {
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, id, 0, (GLsizeiptr) capacity);
    }

    void GLUniformBuffer::load(const UniformData &uniformData) {
        if (!hasCapacity()) return;

        auto subDataSize = uniformBlockFormat[uniformData.index].size();
        auto subDataOffset = uniformData.index * subDataSize;
        glBufferSubData(
                GL_UNIFORM_BUFFER,
                (GLsizeiptr)subDataOffset,
                (GLsizeiptr)subDataSize,
                uniformData.uniforms
        );
    }

}