//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <algorithm>

#define MIN_VERTEX_COUNT 3

namespace engine {

    template<typename T>
    struct VertexData {
        T* vertices = nullptr;
        uint32_t vertexStart = 0;
        uint32_t vertexCount = MIN_VERTEX_COUNT;
    };

    template<typename T>
    VertexData<T> copy(const VertexData<T> &vertexData) {
        auto* copyVertices = new T[vertexData.vertexCount];
        std::copy(vertexData.vertices, vertexData.vertices + vertexData.vertexCount, copyVertices);

        return VertexData<T> {
                copyVertices,
                vertexData.vertexStart,
                vertexData.vertexCount
        };
    }
}
