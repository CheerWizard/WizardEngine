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
        void drawQuadsIndices(const uint32_t &indexCount) override;
        void drawElements(const uint32_t &indexCount) override;
        void drawElements(const uint32_t &indexCount, const uint32_t &instanceCount) override;

        void setPolygonMode() override;

        void enableCulling() override;
        void disableCulling() override;
    };

}