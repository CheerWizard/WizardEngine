//
// Created by mecha on 01.04.2022.
//

#pragma once

#include <graphics/core/Renderer.h>

namespace engine::graphics {

    class ScreenRenderer : public PrimitiveRenderer {
    public:
        ScreenRenderer() = default;

    public:
        void init();
    };

}