//
// Created by mecha on 07.09.2021.
//

#include <platform/graphics/VertexBuffer.h>
#include <glad/glad.h>

namespace engine::graphics {

    void VertexBuffer::create() {
        glGenBuffers(1, &id);
    }

    void VertexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::mallocDynamic(const size_t &memorySize) {
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) memorySize, nullptr, GL_DYNAMIC_DRAW);
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
            glEnableVertexAttribArray(attribute.location);
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

    void VertexBuffer::load(const void* vertices, const size_t& subDataOffset, const size_t& memorySize) {
        glBufferSubData(GL_ARRAY_BUFFER, (GLsizeiptr)subDataOffset, memorySize, vertices);
    }

    void VertexBuffer::loadStatic(const void* vertices, const size_t& memorySize) {
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) memorySize, vertices, GL_STATIC_DRAW);
    }

}