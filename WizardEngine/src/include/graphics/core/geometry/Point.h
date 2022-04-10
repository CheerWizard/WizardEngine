//
// Created by mecha on 01.04.2022.
//

#include <graphics/core/buffer_data/VertexData.h>

#include <glm/glm.hpp>

namespace engine::graphics {

    struct PointVertex {
        glm::vec2 position = { 0.5f, 0.5f };
        glm::vec3 color = { 1, 1, 1 };
    };

    struct Points : VertexDataComponent<PointVertex> {
        Points(PointVertex* points, const uint32_t& count) : VertexDataComponent<PointVertex>() {
            vertexData = { points, 0, count };
            this->drawType = DrawType::POINTS;
        }
    };

}