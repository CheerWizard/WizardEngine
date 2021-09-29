//
// Created by mecha on 12.09.2021.
//

#pragma once

#include "glm/glm.hpp"

#include <string>

namespace engine {

    enum AttributeCategory : unsigned char {
        VERTEX = 0,
        INSTANCE = 1
    };

    enum AttributeBool : unsigned char {
        FALSE = 0,
        TRUE = 1
    };

    enum ElementCount : unsigned char {
        SINGLE = 1,
        VEC2 = 2,
        VEC3 = 3,
        VEC4 = 4,
        MAT2 = 4,
        MAT3 = 9,
        MAT4 = 16
    };

    struct Attribute {
        std::string name;
        uint32_t location;
        uint32_t offset;
        ElementCount elementCount;
        AttributeBool normalized;
        AttributeCategory category;

        Attribute(const std::string& name,
                  const uint32_t &location,
                  const ElementCount &elementCount = SINGLE,
                  const uint32_t &offset = 0,
                  const AttributeCategory &category = VERTEX,
                  const AttributeBool &normalized = FALSE) :
                  name(name), location(location),
                  offset(offset), elementCount(elementCount),
                  normalized(normalized), category(category)
                  {}

    };

}