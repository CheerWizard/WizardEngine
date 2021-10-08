//
// Created by mecha on 07.09.2021.
//

#include "VertexBuffer.h"

namespace engine {

    void VertexBuffer::prepare(VertexFormat *vertexFormat) {
        setVertexFormat(vertexFormat);
        bind();
        allocate();
        setAttributesPointer();
    }

}