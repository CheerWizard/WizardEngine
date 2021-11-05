//
// Created by mecha on 02.10.2021.
//

#pragma once

#include "../../../core/String.h"

#include "ElementCount.h"

namespace engine {

    struct UniformAttribute {
        std::string name;
        ElementCount elementCount = SINGLE;
        uint32_t count = 1;

        size_t size() const {
            return count * elementCount * sizeof(float);
        }

        size_t elementSize() const {
            return elementCount * sizeof(float);
        }
    };

    struct UniformStructAttribute {
        std::string name;
        std::vector<UniformAttribute> uniformAttributes;
    };

}