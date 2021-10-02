//
// Created by mecha on 27.09.2021.
//

#pragma once

#include <cstdint>

#define DEFAULT_COLOR {0.5, 0.5, 0.5, 1}

namespace engine {

    struct Color {
        float r,g,b,a;
    };

    // interface to implement for specific platform.
    // used to make a GPU calls and control the screen.
    class Drawer {

    public:
        virtual void enableDepth() = 0;
        virtual void disableDepth() = 0;
        virtual void enableCulling() = 0;
        virtual void clearDepth(const Color &color) = 0;
        virtual void clearColor(const Color &color) = 0;
        virtual void setViewPort(const uint32_t &width, const uint32_t &height) = 0;
        virtual void drawIndices(const uint32_t &indexCount) = 0;
        virtual void drawIndicesInstanced(const uint32_t &indexCount, const uint32_t &instanceCount) = 0;
        virtual void multiDrawIndices(const uint32_t &totalIndexCount, const uint32_t &drawCount) = 0;
    };

}