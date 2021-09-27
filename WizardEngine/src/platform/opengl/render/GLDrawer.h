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
        void drawIndices(const uint32_t &indexCount) override;

    };

}