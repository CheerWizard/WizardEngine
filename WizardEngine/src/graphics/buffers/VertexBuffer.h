//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Buffer.h"

#include "VertexData.h"
#include "../shader/VertexFormat.h"

#define DEFAULT_VERTEX_COUNT 200000

namespace engine {

    class VertexBuffer : public Buffer {

    public:
        VertexBuffer() : Buffer() {}
        VertexBuffer(VertexFormat* vertexFormat) : Buffer(), vertexFormat(vertexFormat) {}

    public:
        void prepare(VertexFormat* vertexFormat);

    public:
        virtual void allocate() = 0;
        virtual void setAttributesPointer() = 0;
        virtual void load(const VertexData &vertexData) = 0;
        virtual void enableAttributes() = 0;
        virtual void disableAttributes() = 0;

    public:
        inline void setVertexFormat(VertexFormat* vertexFormat) {
            this->vertexFormat = vertexFormat;
        }

        inline VertexFormat *getVertex() const {
            return vertexFormat;
        }

    protected:
        VertexFormat* vertexFormat = nullptr;

    };

}