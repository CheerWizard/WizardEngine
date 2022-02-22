//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <graphics/core/geometry/Vertex.h>

#include <algorithm>

#define MIN_VERTEX_COUNT 3

namespace engine {

    struct VertexData {
        Vertex* vertices = nullptr;
        uint32_t vertexStart = 0;
        uint32_t vertexCount = MIN_VERTEX_COUNT;
    };

    VertexData copy(const VertexData &vertexData);

}
