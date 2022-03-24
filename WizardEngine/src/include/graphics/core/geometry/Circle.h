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
        Vec4fUniform color = { "color", { 0.5, 0, 0.5, 1 } };
        FloatUniform thickness = { "thickness", 0.5 };
        FloatUniform fade = { "fade", 0.005 };

        CircleComponent() = default;
        CircleComponent(const glm::vec4& color, const float& thickness, const float& fade)
        : color({ "color", color }), thickness({ "thickness", thickness }), fade({ "fade", fade }) {}
        CircleComponent(const Vec4fUniform& color, const FloatUniform& thickness, const FloatUniform& fade)
        : color(color), thickness(thickness), fade(fade) {}
    };
}
