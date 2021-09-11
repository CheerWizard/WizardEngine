//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Buffer.h"
#include "Vertex.h"
#include "vector"

namespace engine {

    class VertexBuffer : public Buffer {

    public:
        VertexBuffer(const std::vector<Vertex>& vertices, const uint32_t& size) : vertices(vertices), size(size) {}
        virtual ~VertexBuffer() = default;

    protected:
        std::vector<Vertex> vertices;
        uint32_t size;

    };

}