//
// Created by mecha on 21.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <glm/glm.hpp>

namespace engine {

    struct QuadVertex {
        glm::vec3 position = { 0.5, 0.5, 0.5 };
        glm::vec4 color = { 0, 1, 0, 1 };
    };

    struct InstanceQuad : VertexDataComponent<InstanceVertex<QuadVertex>> {
        InstanceQuad(const std::array<QuadVertex, 4>& quadVertices) : VertexDataComponent<InstanceVertex<QuadVertex>>() {
            vertexData = toVertexData<QuadVertex, InstanceVertex<QuadVertex>>(quadVertices);
            this->drawType = DrawType::QUAD;
        }
    };

    struct BatchQuad : VertexDataComponent<BatchVertex<QuadVertex>> {
        BatchQuad(const std::array<QuadVertex, 4>& quadVertices) : VertexDataComponent<BatchVertex<QuadVertex>>() {
            vertexData = toVertexData<QuadVertex, BatchVertex<QuadVertex>>(quadVertices);
            this->drawType = DrawType::QUAD;
        }
    };
}
