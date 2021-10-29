//
// Created by mecha on 07.09.2021.
//

#include "VertexBuffer.h"

namespace engine {

    void VertexBuffer::prepare(VertexFormat* vertexFormat) {
        prepare(vertexFormat, DEFAULT_VERTEX_COUNT);
    }

    void VertexBuffer::prepare(VertexFormat* vertexFormat, const uint32_t &vertexCount) {
        setVertexFormat(vertexFormat);
        bind();
        allocate(vertexCount);
        setAttributesPointer();
    }

}