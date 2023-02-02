#pragma once

#include <graphics/core/buffer_data/VertexData.h>

namespace engine::graphics {

    using namespace math;

    struct LineVertex {
        serializable()
        vec3f position = { 0.5, 0.5, 0.5 };
        vec4f color = { 0, 1, 0, 1 };
    };

    struct InstanceLine : VertexDataComponent<InstanceVertex<LineVertex>> {
        InstanceLine(const std::vector<LineVertex>& linesVertices)
        : VertexDataComponent<InstanceVertex<LineVertex>>() {
            vertexData = toVertexData<LineVertex, InstanceVertex<LineVertex>>(linesVertices);
            this->drawType = DrawType::LINE;
        }
    };

    struct BatchLine : VertexDataComponent<BatchVertex<LineVertex>> {
        BatchLine(const std::vector<LineVertex>& linesVertices)
        : VertexDataComponent<BatchVertex<LineVertex>>() {
            vertexData = toVertexData<LineVertex, BatchVertex<LineVertex>>(linesVertices);
            this->drawType = DrawType::LINE;
        }
    };

}