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

    typedef InstanceVertex<LineVertex> InstanceLineVertex;
    typedef VertexDataComponent<LineVertex> LineComponent;
    typedef VertexDataComponent<InstanceLineVertex> InstanceLineComponent;

    struct Line : LineComponent {
        Line(const std::initializer_list<LineVertex>& linesVertices, const DrawType& drawType = LINE) : LineComponent() {
            vertexData = VertexData<LineVertex> {
                    const_cast<LineVertex*>(data(linesVertices)),
                    0,
                    static_cast<uint32_t>(linesVertices.size())
            };
            this->drawType = drawType;
        }
    };

    struct StripLine : Line {
        StripLine(const std::initializer_list<LineVertex>& linesVertices) : Line(linesVertices, LINE_STRIP) {}
    };

    struct LoopLine : Line {
        LoopLine(const std::initializer_list<LineVertex>& linesVertices) : Line(linesVertices, LINE_LOOP) {}
    };

    struct Lines : InstanceLineComponent {
        Lines(const std::initializer_list<LineVertex>& linesVertices, const DrawType& drawType = LINE) : InstanceLineComponent() {
            vertexData = VertexData<InstanceLineVertex> {
                    const_cast<InstanceLineVertex*>(toInstanceLineVertex(data(linesVertices), linesVertices.size())),
                    0,
                    static_cast<uint32_t>(linesVertices.size())
            };
            this->drawType = drawType;
        }

        static InstanceLineVertex* toInstanceLineVertex(const LineVertex* lineVertices, const size_t& size) {
            auto* instanceLineVertex = new InstanceLineVertex[size];
            for (auto i = 0; i < size; i++) {
                instanceLineVertex[i] = { lineVertices[i] };
            }
            return instanceLineVertex;
        }
    };

    struct StripLines : Lines {
        StripLines(const std::initializer_list<LineVertex>& linesVertices) : Lines(linesVertices, LINE_STRIP) {}
    };

    struct LoopLines : Lines {
        LoopLines(const std::initializer_list<LineVertex>& linesVertices) : Lines(linesVertices, LINE_LOOP) {}
    };

}
