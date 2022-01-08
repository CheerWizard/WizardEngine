//
// Created by mecha on 02.10.2021.
//

#pragma once

#include "../../../core/String.h"

#include "ElementCount.h"

namespace engine::shader {

    struct UniformAttribute {
        std::string name;
        ElementCount elementCount = SINGLE;
        uint16_t count = 1;
        size_t offset = 0;
    };

    inline size_t elementSize(const UniformAttribute &uniformAttribute) {
        return uniformAttribute.elementCount * sizeof(float);
    }

    inline size_t size_of(const UniformAttribute &uniformAttribute) {
        return uniformAttribute.count * elementSize(uniformAttribute);
    }

    struct UniformStructAttribute {
        std::string name;
        std::vector<UniformAttribute> uniformAttributes;

        void add(const UniformAttribute &uniformAttribute) {
            uniformAttributes.emplace_back(uniformAttribute);
        }

        void add(const std::vector<UniformAttribute> &uniformAttributes) {
            for (const auto &uniformAttribute : uniformAttributes) {
                add(uniformAttribute);
            }
        }

        void clear() {
            uniformAttributes.clear();
        }
    };

}