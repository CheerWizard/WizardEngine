//
// Created by mecha on 12.09.2021.
//

#pragma once

#include <core/String.h>
#include "ElementCount.h"

#include <glm/glm.hpp>

namespace engine::shader {

    enum AttributeCategory : uint8_t {
        VERTEX = 0,
        INSTANCE = 1
    };

    enum AttributeBool : uint8_t {
        A_FALSE = 0,
        A_TRUE = 1
    };

    struct VertexAttribute {
        std::string name;
        uint32_t location;
        uint32_t offset = 0;
        ElementCount elementCount = SINGLE;
        AttributeBool normalized = A_FALSE;
        AttributeCategory category = VERTEX;
    };

    inline size_t elementSize(const VertexAttribute &vertexAttribute) {
        return vertexAttribute.elementCount * sizeof(float);
    }

}