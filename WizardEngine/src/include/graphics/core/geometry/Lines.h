//
// Created by mecha on 18.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <glm/glm.hpp>
#include <initializer_list>

namespace engine {

    struct LineVertex {
        glm::vec3 position = { 0.5, 0.5, 0.5 };
        glm::vec4 color = { 0, 1, 0, 1 };
    };

    typedef BatchVertex<LineVertex> BatchLineVertex;
    typedef VertexDataComponent<BatchLineVertex> BatchLineComponent;
    typedef InstanceVertex<LineVertex> InstanceLineVertex;
    typedef VertexDataComponent<InstanceLineVertex> InstanceLineComponent;

    struct Line : InstanceLineComponent {
        Line(const std::vector<InstanceLineVertex>& linesVertices, const DrawType& drawType = LINE) : InstanceLineComponent() {
            vertexData = toVertexData(linesVertices);
            this->drawType = drawType;
        }
    };

    struct Lines : BatchLineComponent {
        Lines(const std::vector<LineVertex>& linesVertices, const DrawType& drawType = LINE) : BatchLineComponent() {
            vertexData = toVertexData<LineVertex>(linesVertices);
            this->drawType = drawType;
        }
    };
}
