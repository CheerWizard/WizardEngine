//
// Created by mecha on 07.09.2021.
//

#include "GLIndexBuffer.h"

#include "glad/glad.h"

namespace engine {

    void GLIndexBuffer::create() {
        glCreateBuffers(1, &id);
    }

    void GLIndexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }

    void GLIndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void GLIndexBuffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void GLIndexBuffer::allocate() {
        if (!hasCapacity()) return;
        auto data = new uint32_t[capacity];
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
    }

    void GLIndexBuffer::load(const uint32_t &indexStart, uint32_t* subData) {
        if (!hasCapacity()) return;
        auto subDataOffset = indexStart * sizeof(uint32_t);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, subDataOffset, sizeof(subData), subData);
    }

}