//
// Created by mecha on 07.09.2021.
//

#include "IndexBuffer.h"

#include "glad/glad.h"

namespace engine {

    void IndexBuffer::create() {
        glGenBuffers(1, &id);
    }

    void IndexBuffer::destroy() {
        glDeleteBuffers(1, &id);
    }

    void IndexBuffer::recreate() {
        destroy();
        create();
    }

    void IndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void IndexBuffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::alloc() {
        alloc(DEFAULT_INDEX_COUNT);
    }

    void IndexBuffer::alloc(const uint32_t &indexCount) {
        malloc(indexCount * sizeof(indexCount));
    }

    void IndexBuffer::malloc(const size_t &memorySize) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, memorySize, nullptr, GL_DYNAMIC_DRAW);
    }

    void IndexBuffer::load(const IndexData &indexData) {
        auto subDataOffset = indexData.indexStart * sizeof(uint32_t);
        glBufferSubData(
                GL_ELEMENT_ARRAY_BUFFER,
                (GLintptr)subDataOffset,
                indexData.indexCount * sizeof(uint32_t),
                indexData.indices
        );
    }

}