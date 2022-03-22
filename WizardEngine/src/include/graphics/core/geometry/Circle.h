//
// Created by mecha on 22.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/shader/BaseShader.h>
#include <glm/glm.hpp>

namespace engine {

    struct CircleVertex {
        glm::vec3 position = { 0.5, 0.5, 0.5 };
        glm::vec2 uv = { 0, 0 };
        glm::vec4 color = { 0, 1, 0, 1 };
    };

    struct InstanceCircle : VertexDataComponent<InstanceVertex<CircleVertex>> {
        InstanceCircle(const std::array<CircleVertex, 4>& circleVertices) : VertexDataComponent<InstanceVertex<CircleVertex>>() {
            vertexData = toVertexData<CircleVertex, InstanceVertex<CircleVertex>>(circleVertices);
            this->drawType = TRIANGLE_STRIP;
        }
    };

    struct BatchCircle : VertexDataComponent<BatchVertex<CircleVertex>> {
        BatchCircle(const std::array<CircleVertex, 4>& circleVertices) : VertexDataComponent<BatchVertex<CircleVertex>>() {
            vertexData = toVertexData<CircleVertex, BatchVertex<CircleVertex>>(circleVertices);
            this->drawType = TRIANGLE_STRIP;
        }
    };

    struct CircleComponent {
        const char* name = "circle";
        FloatUniform thickness = { "thickness", 0.5 };
        FloatUniform fade = { "fade", 0.005 };

        CircleComponent() = default;
        CircleComponent(const float& thickness, const float& fade) {
            this->thickness = { "thickness", thickness };
            this->fade = { "fade", fade };
        }
    };
}
