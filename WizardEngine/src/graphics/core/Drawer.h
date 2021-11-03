//
// Created by mecha on 27.09.2021.
//

#pragma once

#include <cstdint>

namespace engine {

    class Drawer {

    public:
        virtual ~Drawer() = default;

    public:
        virtual void drawQuadsIndices(const uint32_t &indexCount) = 0;
        virtual void drawElements(const uint32_t &indexCount) = 0;
        virtual void drawElements(const uint32_t &indexCount, const uint32_t &instanceCount) = 0;

    };

}