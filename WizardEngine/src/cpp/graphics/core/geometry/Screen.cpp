//
// Created by mecha on 01.04.2022.
//

#include <graphics/core/geometry/Screen.h>

namespace engine {

    Screen::Screen() {
        auto vertices = new ScreenVertex[4] {
                ScreenVertex { { -0.5, -0.5 }, { 0, 0 } },
                ScreenVertex { { 0.5, -0.5 }, { 1, 0 } },
                ScreenVertex { { 0.5, 0.5 }, { 1, 1 } },
                ScreenVertex { { -0.5, 0.5 }, { 0, 1 } }
        };
        vertexData = VertexData<ScreenVertex> { vertices, 0, 4 };
        drawType = DrawType::QUAD;
    }

}