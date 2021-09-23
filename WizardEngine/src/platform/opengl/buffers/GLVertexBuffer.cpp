//
// Created by mecha on 07.09.2021.
//

#include "GLVertexBuffer.h"

#include "glad/glad.h"

namespace engine {

    void GLVertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void GLVertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLVertexBuffer::create() {
        glCreateBuffers(1, &id);
    }

    void GLVertexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }

    void GLVertexBuffer::allocate() {
        capacity = vertex->getElementCount() * vertex->getCount();
        auto data = new float[capacity];
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
    }

    void GLVertexBuffer::prepare() {
        auto stride = vertex->getElementCount() * sizeof(float);
        uint32_t offset = 0;

        for (const Attribute &attribute : vertex->getAttributes()) {
            glVertexAttribPointer(attribute.location,
                                  attribute.elementCount,
                                  GL_FLOAT,
                                  attribute.normalized,
                                  stride,
                                  (GLvoid*) offset);
            offset += attribute.elementCount * sizeof(float);
            glVertexAttribDivisor(attribute.location, attribute.category);
        }
    }

    void GLVertexBuffer::load(const uint32_t &vertexStart, float *subData) {
        auto subDataOffset = vertexStart * vertex->getElementCount() * sizeof(float);
        glBufferSubData(GL_ARRAY_BUFFER, subDataOffset, sizeof(subData), subData);
    }

    void GLVertexBuffer::enableAttributes() {
        for (const Attribute& attribute : vertex->getAttributes()) {
            glEnableVertexAttribArray(attribute.location);
        }
    }

    void GLVertexBuffer::disableAttributes() {
        for (const Attribute& attribute : vertex->getAttributes()) {
            glDisableVertexAttribArray(attribute.location);
        }
    }
}