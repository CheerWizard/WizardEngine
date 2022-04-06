//
// Created by mecha on 07.09.2021.
//

#include <platform/graphics/IndexBuffer.h>

#include <glad/glad.h>

namespace engine::graphics {

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

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void IndexBuffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::alloc() {
        alloc(totalCount);
    }

    void IndexBuffer::alloc(const uint32_t &indexCount) {
        this->totalCount = indexCount;
        malloc(indexCount * sizeof(indexCount));
    }

    void IndexBuffer::malloc(const size_t &memorySize) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, memorySize, nullptr, GL_DYNAMIC_DRAW);
    }

    void IndexBuffer::load(const IndexData &indexData) {
        auto subDataOffset = indexData.indexStart * sizeof(u32);
        glBufferSubData(
                GL_ELEMENT_ARRAY_BUFFER,
                (GLintptr)subDataOffset,
                indexData.indexCount * sizeof(u32),
                indexData.indices
        );
    }

}