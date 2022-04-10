//
// Created by mecha on 01.04.2022.
//

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/shader/Uniform.h>

namespace engine::graphics {

    struct ScreenVertex {
        glm::vec2 position = { -1, -1 };
        glm::vec2 uv = { 0, 0 };
    };

    struct Screen : VertexDataComponent<ScreenVertex> {
        Screen();
    };

}