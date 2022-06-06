//
// Created by mecha on 01.04.2022.
//

#include <graphics/core/geometry/Screen.h>

namespace engine::graphics {

    Screen::Screen() {
        auto vertices = new ScreenVertex[4] {
                ScreenVertex { { -1, -1 }, { 0, 0 } },
                ScreenVertex { { 1, -1 }, { 1, 0 } },
                ScreenVertex { { 1, 1 }, { 1, 1 } },
                ScreenVertex { { -1, 1 }, { 0, 1 } }
        };
        vertexData = array<ScreenVertex> { vertices, 0, 4 };
        drawType = DrawType::QUAD;
    }

}