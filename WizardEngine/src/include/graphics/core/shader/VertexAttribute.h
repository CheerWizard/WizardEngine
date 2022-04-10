//
// Created by mecha on 12.09.2021.
//

#pragma once

#include <core/primitives.h>
#include <core/String.h>

#include <glm/glm.hpp>

namespace engine::shader {

    using namespace core;

    enum ElementCount : u8 {
        NONE = 0,
        SINGLE = 1,
        DOUBLE_ = 2,
        VEC2 = 2,
        D_VEC2 = 4,
        VEC3 = 3,
        D_VEC3 = 6, // double = 2 floats!
        VEC4 = 4,
        D_VEC4 = 8, // double = 2 floats!
        MAT2_ = 4,
        MAT3 = 9,
        MAT4 = 16
    };

    enum AttributeCategory : u8 {
        VERTEX = 0,
        INSTANCE = 1
    };

    enum AttributeBool : u8 {
        A_FALSE = 0,
        A_TRUE = 1
    };

    struct VertexAttribute {
        std::string name;
        u32 location;
        u32 offset = 0;
        ElementCount elementCount = SINGLE;
        AttributeBool normalized = A_FALSE;
        AttributeCategory category = VERTEX;
    };

    inline size_t elementSize(const VertexAttribute &vertexAttribute) {
        return vertexAttribute.elementCount * sizeof(float);
    }

}