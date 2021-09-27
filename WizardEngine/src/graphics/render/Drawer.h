//
// Created by mecha on 27.09.2021.
//

#pragma once

#include <cstdint>

namespace engine {

    // interface to implement for specific platform.
    // used to make a draw calls on the screen.
    class Drawer {

    public:
        virtual void drawIndices(const uint32_t &indexCount) = 0;

    };

}