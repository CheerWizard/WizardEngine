//
// Created by mecha on 02.10.2021.
//

#pragma once

#include "../../../core/String.h"

#include "ElementCount.h"

namespace engine {

    struct UniformAttribute {
        std::string name;
        ElementCount elementCount;
        uint32_t count;

        UniformAttribute(
            const std::string& name,
            const ElementCount &elementCount = SINGLE,
            const uint32_t &count = 1
        ) : name(name), elementCount(elementCount), count(count) {}

        size_t size() const {
            return count * elementCount * sizeof(float);
        }

        size_t elementSize() const {
            return elementCount * sizeof(float);
        }
    };

}