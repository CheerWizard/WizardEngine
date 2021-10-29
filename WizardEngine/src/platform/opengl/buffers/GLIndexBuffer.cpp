//
// Created by mecha on 07.09.2021.
//

#include "GLIndexBuffer.h"

#include "glad/glad.h"

namespace engine {

    void GLIndexBuffer::create() {
        glGenBuffers(1, &id);
    }

    void GLIndexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }

    void GLIndexBuffer::recreate() {
        destroy();
        create();
    }

    void GLIndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void GLIndexBuffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void GLIndexBuffer::allocate(const uint32_t &indexCount) {
        capacity = indexCount;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, capacity * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
    }

    void GLIndexBuffer::load(const IndexData &indexData) {
        if (!hasCapacity()) return;

        auto subDataOffset = indexData.indexStart * sizeof(uint32_t);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                        (GLintptr)subDataOffset,
                        indexData.indexCount * sizeof(uint32_t),
                        indexData.indices);
    }

}