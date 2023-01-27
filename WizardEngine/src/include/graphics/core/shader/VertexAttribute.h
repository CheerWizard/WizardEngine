//
// Created by mecha on 12.09.2021.
//

#pragma once

#include <core/primitives.h>
#include <core/String.h>

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

    struct ENGINE_API AttributeCategory final {
        static u8 VERTEX;
        static u8 INSTANCE;
    };

    struct ENGINE_API AttributeBool final {
        static u8 FALSE;
        static u8 TRUE;
    };

    struct ENGINE_API VertexAttribute {
        std::string name;
        u32 location;
        u32 offset = 0;
        ElementCount elementCount = SINGLE;
        u8 normalized = AttributeBool::FALSE;
        u8 category = AttributeCategory::VERTEX;
    };

    inline size_t elementSize(const VertexAttribute &vertexAttribute) {
        return vertexAttribute.elementCount * sizeof(float);
    }

}