//
// Created by mecha on 12.09.2021.
//

#include "VertexArray.h"

#include "../../core/Application.h"

namespace engine {

    void VertexArray::destroy() {
        indexBuffer.reset();
        vertexBufferCache->clear();
        textureBuffer.reset();
    }

    void VertexArray::createVertexBuffer(VertexFormat *vertexFormat, const std::string &shaderName) {
        if (vertexBufferCache->isEmpty(shaderName)) {
            vertexBufferCache->add(shaderName, GRAPHICS_CONTEXT->newVertexBuffer(vertexFormat));
        }
        vertexBufferCache->last(shaderName)->setVertex(vertexFormat);
    }

    void VertexArray::createIndexBuffer() {
        indexBuffer = GRAPHICS_CONTEXT->newIndexBuffer();
    }

    void VertexArray::createIndexBuffer(const uint32_t &indexCount) {
        indexBuffer = GRAPHICS_CONTEXT->newIndexBuffer(indexCount);
    }

    void VertexArray::createTextureBuffer() {
        textureBuffer = GRAPHICS_CONTEXT->newTextureBuffer();
    }

    void VertexArray::loadVertexBuffer(const std::string &shaderName, const VertexData &vertexData) {
        vertexBufferCache->last(shaderName)->load(vertexData);
    }

    void VertexArray::loadIndexBuffer(const IndexData &indexData) {
        indexBuffer->load(indexData);
    }

    void VertexArray::bindLastVertexBuffer(const std::string &shaderName) {
        vertexBufferCache->last(shaderName)->bind();
    }

    void VertexArray::bindIndexBuffer() {
        indexBuffer->bind();
    }

    void VertexArray::loadTextureBufferData(const void *data) {
        textureBuffer->loadData(data);
    }

    void VertexArray::loadTextureBuffer(const std::string &filePath) {
        textureBuffer->load(filePath);
    }

    void VertexArray::bindTextureBuffer() {
        textureBuffer->bind();
    }

    void VertexArray::activateTextureBuffer(const int &slot) {
        textureBuffer->activate(slot);
    }

}