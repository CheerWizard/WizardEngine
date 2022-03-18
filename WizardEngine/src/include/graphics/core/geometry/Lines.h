//
// Created by mecha on 18.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <glm/glm.hpp>

namespace engine {

    struct LineVertex {
        glm::vec3 position = { 0.5f, 0.5f, 0.5f };
        glm::vec4 color = { 0, 1, 0, 1 };
    };

    struct LinesComponent {
        VertexData<LineVertex> vertexData;
    };

    struct Line : LinesComponent {
        Line(const LineVertex& v1 = { { -0.5, 0.5, 0.5 } },
             const LineVertex& v2 = { { 0.5, 0.5, 0.5 } }
         ) : LinesComponent() {
            vertexData = VertexData<LineVertex> {
                new LineVertex[2] { v1, v2 },
                0,
                2
            };
        }
    };

}
