//
// Created by mecha on 01.04.2022.
//

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/shader/Uniform.h>

namespace engine::graphics {

    using namespace math;

    struct ScreenVertex {
        vec2f position = { -1, -1 };
        vec2f uv = { 0, 0 };
    };

    struct Screen : VertexDataComponent<ScreenVertex> {
        Screen();
    };

}