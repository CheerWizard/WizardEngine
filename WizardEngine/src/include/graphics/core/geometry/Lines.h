//
// Created by mecha on 18.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <glm/glm.hpp>
#include <initializer_list>

namespace engine {

    struct LineVertex {
        glm::vec3 position = { 0.5f, 0.5f, 0.5f };
        glm::vec4 color = { 0, 1, 0, 1 };
    };

    typedef InstanceVertex<LineVertex> LinesVertex;
    typedef VertexDataComponent<LineVertex> LineComponent;
    typedef VertexDataComponent<LinesVertex> LinesComponent;

    struct Line : LineComponent {
        Line(const std::initializer_list<LineVertex>& linesVertices) : LineComponent() {
            vertexData = VertexData<LineVertex> {
                    const_cast<LineVertex*>(data(linesVertices)),
                    0,
                    static_cast<uint32_t>(linesVertices.size())
            };
        }
    };

    struct Lines : LinesComponent {
        Lines(const std::initializer_list<LinesVertex>& linesVertices) : LinesComponent() {
            vertexData = VertexData<LinesVertex> {
                    const_cast<LinesVertex*>(data(linesVertices)),
                    0,
                    static_cast<uint32_t>(linesVertices.size())
            };
        }
    };
}
