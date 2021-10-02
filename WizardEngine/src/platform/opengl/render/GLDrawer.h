//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../../graphics/render/Drawer.h"

namespace engine {

    class GLDrawer : public Drawer {

    public:
        GLDrawer() = default;
        ~GLDrawer() = default;

    public:
        void setViewPort(const uint32_t &width, const uint32_t &height) override;
        void enableDepth() override;
        void disableDepth() override;
        void clearDepth(const Color &color) override;
        void clearColor(const Color &color) override;
        void drawIndicesInstanced(const uint32_t &indexCount, const uint32_t &instanceCount) override;

        void multiDrawIndices(const uint32_t &totalIndexCount, const uint32_t &drawCount) override;

        void enableCulling() override;

        void drawQuadsIndices(const uint32_t &indexCount) override;

        void drawTrianglesIndices(const uint32_t &indexCount) override;

    };

}