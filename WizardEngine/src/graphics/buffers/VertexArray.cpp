//
// Created by mecha on 12.09.2021.
//

#include "VertexArray.h"

namespace engine {

    void VertexArray::loadVertices(const std::string &shaderName, const uint32_t &vertexStart, float *vertices) {
        if (vertexBufferCache->isEmpty(shaderName)) {
            vertexBufferCache->add(shaderName, nullptr);
        }
        vertexBufferCache->last(shaderName)->load(vertexStart, vertices);
    }

    void VertexArray::loadIndices(const std::string &shaderName, const uint32_t &indexStart, uint32_t *indices) {

    }

    void VertexArray::destroy() {
        indexBuffer.reset();
        vertexBufferCache->clear();
    }

}