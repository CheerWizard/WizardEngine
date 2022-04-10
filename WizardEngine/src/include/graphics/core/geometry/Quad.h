//
// Created by mecha on 21.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <glm/glm.hpp>

namespace engine::graphics {

    struct QuadVertex {
        glm::vec3 position = { 0.5, 0.5, 0.5 };
        glm::vec4 color = { 0, 1, 0, 1 };
    };

    struct InstanceQuad : VertexDataComponent<InstanceVertex<QuadVertex>> {
        InstanceQuad() : VertexDataComponent<InstanceVertex<QuadVertex>>() {
            auto vertices = new InstanceVertex<QuadVertex>[4] {
                    InstanceVertex<QuadVertex> { QuadVertex { { -0.5, -0.5, 0.5 } } },
                    InstanceVertex<QuadVertex> { QuadVertex { { 0.5, -0.5, 0.5 } } },
                    InstanceVertex<QuadVertex> { QuadVertex { { 0.5, 0.5, 0.5 } } },
                    InstanceVertex<QuadVertex> { QuadVertex { { -0.5, 0.5, 0.5 } } },
            };
            vertexData = VertexData<InstanceVertex<QuadVertex>> { vertices, 0, 4 };
            this->drawType = DrawType::QUAD;
        }
    };

    struct BatchQuad : VertexDataComponent<BatchVertex<QuadVertex>> {
        BatchQuad() : VertexDataComponent<BatchVertex<QuadVertex>>() {
            auto vertices = new BatchVertex<QuadVertex>[4] {
                    BatchVertex<QuadVertex> { QuadVertex { { -0.5, -0.5, 0.5 } } },
                    BatchVertex<QuadVertex> { QuadVertex { { 0.5, -0.5, 0.5 } } },
                    BatchVertex<QuadVertex> { QuadVertex { { 0.5, 0.5, 0.5 } } },
                    BatchVertex<QuadVertex> { QuadVertex { { -0.5, 0.5, 0.5 } } },
            };
            vertexData = VertexData<BatchVertex<QuadVertex>> { vertices, 0, 4 };
            this->drawType = DrawType::QUAD;
        }
    };

}
