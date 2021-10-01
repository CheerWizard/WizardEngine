//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../geometry/Vertex.h"

#define MIN_VERTEX_COUNT 3

namespace engine {

    struct VertexData {
        Vertex* vertices;
        uint32_t vertexCount;
        uint32_t vertexStart;

        VertexData() = default;

        VertexData(Vertex* vertices,
                   const uint32_t &vertexStart = 0,
                   const uint32_t &vertexCount = MIN_VERTEX_COUNT) :
                   vertices(vertices),
                   vertexCount(vertexCount),
                   vertexStart(vertexStart) {}
    };

}
