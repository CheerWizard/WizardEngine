//
// Created by mecha on 07.09.2021.
//

#include "GLVertexBuffer.h"

#include "glad/glad.h"

namespace engine {

    void GLVertexBuffer::create() {
        glGenBuffers(1, &id);
    }

    void GLVertexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }

    void GLVertexBuffer::recreate() {
        destroy();
        create();
    }

    void GLVertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void GLVertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLVertexBuffer::allocate() {
        capacity = vertexFormat->getSize() * DEFAULT_VERTEX_COUNT;

        if (!hasCapacity()) return;
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) capacity, nullptr, GL_DYNAMIC_DRAW);
    }

    void GLVertexBuffer::setAttributesPointer() {
        auto stride = vertexFormat->getElementCount() * sizeof(float);
        uint32_t offset = 0;

        for (const VertexAttribute &attribute : vertexFormat->getAttributes()) {
            glVertexAttribPointer(attribute.location,
                                  attribute.elementCount,
                                  GL_FLOAT,
                                  attribute.normalized,
                                  (GLsizei) stride,
                                  (GLvoid*) offset);
            offset += attribute.elementCount * sizeof(float);
            glVertexAttribDivisor(attribute.location, attribute.category);
        }
    }

    void GLVertexBuffer::load(const VertexData &vertexData) {
        if (!hasCapacity()) return;

        auto vertexSize = vertexFormat->getSize();
        auto subDataOffset = vertexData.vertexStart * vertexSize;
        glBufferSubData(GL_ARRAY_BUFFER,
                        (GLsizeiptr)subDataOffset,
                        vertexData.vertexCount * vertexSize,
                        vertexData.vertices);
    }

    void GLVertexBuffer::enableAttributes() {
        for (const VertexAttribute& attribute : vertexFormat->getAttributes()) {
            glEnableVertexAttribArray(attribute.location);
        }
    }

    void GLVertexBuffer::disableAttributes() {
        for (const VertexAttribute& attribute : vertexFormat->getAttributes()) {
            glDisableVertexAttribArray(attribute.location);
        }
    }
}