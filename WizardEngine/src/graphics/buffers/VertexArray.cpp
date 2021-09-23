//
// Created by mecha on 12.09.2021.
//

#include "VertexArray.h"

#include "../../core/Application.h"

namespace engine {

    void VertexArray::destroy() {
        indexBuffer.reset();
        vertexBufferCache->clear();
    }

    void VertexArray::createVertexBuffer(Vertex *vertex, const std::string &shaderName) {
        if (vertexBufferCache->isEmpty(shaderName)) {
            vertexBufferCache->add(shaderName, GRAPHICS_CONTEXT->newVertexBuffer(vertex));
        }
        vertexBufferCache->last(shaderName)->setVertex(vertex);
    }

    void VertexArray::createIndexBuffer() {
        indexBuffer = GRAPHICS_CONTEXT->newIndexBuffer();
    }

    void VertexArray::createIndexBuffer(const uint32_t &indexCount) {
        indexBuffer = GRAPHICS_CONTEXT->newIndexBuffer(indexCount);
    }

    void VertexArray::loadVertexBuffer(const std::string &shaderName, const uint32_t &vertexStart, float *vertices) {
        vertexBufferCache->last(shaderName)->load(vertexStart, vertices);
    }

    void VertexArray::loadIndexBuffer(const uint32_t &indexStart, uint32_t *indices) {
        indexBuffer->load(indexStart, indices);
    }

}