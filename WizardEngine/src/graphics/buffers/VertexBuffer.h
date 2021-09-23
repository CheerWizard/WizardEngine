//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Buffer.h"

#include "../geometry/Vertex.h"

#include "vector"

namespace engine {

    class VertexBuffer : public Buffer {

    public:
        explicit VertexBuffer(Vertex* vertex) : vertex(vertex) {}

    public:
        virtual void allocate() = 0;
        virtual void prepare() = 0;
        virtual void load(const uint32_t &vertexStart, float* vertices) = 0;
        virtual void enableAttributes() = 0;
        virtual void disableAttributes() = 0;

    public:
        inline void setVertex(Vertex* vertex) {
            this->vertex = vertex;
        }

        inline Vertex *getVertex() const {
            return vertex;
        }

    protected:
        Vertex* vertex;

    };

}