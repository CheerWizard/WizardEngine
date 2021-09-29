//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Buffer.h"

#include "VertexData.h"
#include "../shader/VertexFormat.h"

#define DEFAULT_VERTEX_COUNT 100

namespace engine {

    class VertexBuffer : public Buffer {

    public:
        explicit VertexBuffer(VertexFormat* vertexFormat) : vertexFormat(vertexFormat) {}

    public:
        virtual void allocate() = 0;
        virtual void setAttributesPointer() = 0;
        virtual void load(const VertexData &vertexData) = 0;
        virtual void enableAttributes() = 0;
        virtual void disableAttributes() = 0;

    public:
        inline void setVertex(VertexFormat* vertexFormat) {
            this->vertexFormat = vertexFormat;
        }

        inline VertexFormat *getVertex() const {
            return vertexFormat;
        }

    protected:
        VertexFormat* vertexFormat;

    };

}