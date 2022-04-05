//
// Created by mecha on 01.04.2022.
//

#include <graphics/core/buffer_data/VertexData.h>
#include <glm/glm.hpp>

namespace engine::graphics {

    struct ScreenVertex {
        glm::vec2 position = { -0.5, -0.5 };
        glm::vec2 uv = { 0, 0 };
    };

    struct Screen : VertexDataComponent<ScreenVertex> {
        Screen();
    };

}