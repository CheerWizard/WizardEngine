//
// Created by mecha on 26.09.2021.
//

#include <graphics/core/buffer_data/VertexData.h>

namespace engine {

    VertexData copy(const VertexData &vertexData) {
        auto* copyVertices = new Vertex[vertexData.vertexCount];
        std::copy(vertexData.vertices, vertexData.vertices + vertexData.vertexCount, copyVertices);

        return VertexData {
                copyVertices,
                vertexData.vertexStart,
                vertexData.vertexCount
        };
    }

}
