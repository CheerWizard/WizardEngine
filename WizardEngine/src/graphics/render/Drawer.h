//
// Created by mecha on 27.09.2021.
//

#pragma once

#include <cstdint>

namespace engine {

    enum PolygonMode : uint16_t {
        POINT = 0,
        LINE = 1,
        FILL = 2
    };

    struct Color {
        float r,g,b,a;
    };

    // interface to implement for specific platform.
    // used to make a GPU calls and control the screen.
    class Drawer {

    public:
        virtual void drawQuadsIndices(const uint32_t &indexCount) = 0;
        virtual void drawByIndices(const uint32_t &indexCount) = 0;
        virtual void drawIndicesInstanced(const uint32_t &indexCount, const uint32_t &instanceCount) = 0;
        virtual void multiDrawIndices(const uint32_t &totalIndexCount, const uint32_t &drawCount) = 0;

        virtual void enableDepth() = 0;
        virtual void disableDepth() = 0;
        virtual void enableCulling() = 0;
        virtual void clearDepth(const Color &color) = 0;
        virtual void clearColor(const Color &color) = 0;
        virtual void setPolygonMode(const PolygonMode &polygonMode) = 0;

    };

}