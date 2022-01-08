//
// Created by mecha on 07.09.2021.
//

#include "VertexBuffer.h"

#include "glad/glad.h"

namespace engine {

    void VertexBuffer::create() {
        glGenBuffers(1, &id);
    }

    void VertexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }

    void VertexBuffer::recreate() {
        destroy();
        create();
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::alloc(const uint32_t &vertexCount) {
        size_t size = vertexFormat.getSize() * vertexCount;
        malloc(size);
    }

    void VertexBuffer::malloc(const size_t &memorySize) {
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) memorySize, nullptr, GL_DYNAMIC_DRAW);
    }

    void VertexBuffer::setFormat(const shader::VertexFormat &vertexFormat, const uint32_t &vertexCount) {
        this->vertexFormat = vertexFormat;
        bind();
        alloc(vertexCount);
        bindAttributes();
    }

    void VertexBuffer::bindAttributes() {
        auto stride = vertexFormat.getElementCount() * sizeof(float);
        uint32_t offset = 0;

        for (const shader::VertexAttribute &attribute : vertexFormat.getAttributes()) {
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

    void VertexBuffer::enableAttributes() {
        for (const shader::VertexAttribute& attribute : vertexFormat.getAttributes()) {
            glEnableVertexAttribArray(attribute.location);
        }
    }

    void VertexBuffer::disableAttributes() {
        for (const shader::VertexAttribute& attribute : vertexFormat.getAttributes()) {
            glDisableVertexAttribArray(attribute.location);
        }
    }

    void VertexBuffer::load(const VertexData &vertexData) {
        auto vertexSize = vertexFormat.getSize();
        auto subDataOffset = vertexData.vertexStart * vertexSize;
        glBufferSubData(GL_ARRAY_BUFFER,
                        (GLsizeiptr)subDataOffset,
                        vertexData.vertexCount * vertexSize,
                        vertexData.vertices);
    }

}