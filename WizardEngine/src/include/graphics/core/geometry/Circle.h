//
// Created by mecha on 22.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/shader/BaseShader.h>

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    struct CircleVertex {
        serializable()
        vec3f position = { 0.5, 0.5, 0.5 };
        vec2f uv = { 0, 0 };
    };

    struct InstanceCircle : VertexDataComponent<InstanceVertex<CircleVertex>> {
        InstanceCircle() : VertexDataComponent<InstanceVertex<CircleVertex>>() {
            auto vertices = new InstanceVertex<CircleVertex>[4] {
                    InstanceVertex<CircleVertex> { CircleVertex { { -0.5, -0.5, 0.5 } } },
                    InstanceVertex<CircleVertex> { CircleVertex { { 0.5, -0.5, 0.5 } } },
                    InstanceVertex<CircleVertex> { CircleVertex { { 0.5, 0.5, 0.5 } } },
                    InstanceVertex<CircleVertex> { CircleVertex { { -0.5, 0.5, 0.5 } } },
            };
            vertexData = array<InstanceVertex<CircleVertex>> { vertices, 0, 4 };
            this->drawType = DrawType::QUAD;
        }
    };

    struct BatchCircle : VertexDataComponent<BatchVertex<CircleVertex>> {
        BatchCircle() : VertexDataComponent<BatchVertex<CircleVertex>>() {
            auto vertices = new BatchVertex<CircleVertex>[4] {
                    BatchVertex<CircleVertex> { CircleVertex { { -0.5, -0.5, 0.5 } } },
                    BatchVertex<CircleVertex> { CircleVertex { { 0.5, -0.5, 0.5 } } },
                    BatchVertex<CircleVertex> { CircleVertex { { 0.5, 0.5, 0.5 } } },
                    BatchVertex<CircleVertex> { CircleVertex { { -0.5, 0.5, 0.5 } } },
            };
            vertexData = array<BatchVertex<CircleVertex>> { vertices, 0, 4 };
            this->drawType = DrawType::QUAD;
        }
    };

    component(CircleComponent) {
        serializable()
        const char* name = "circle";
        Vec4fUniform color = { "color", { 0.5, 0, 0.5, 1 } };
        FloatUniform thickness = { "thickness", 0.5 };
        FloatUniform fade = { "fade", 0.005 };

        CircleComponent() = default;
        CircleComponent(const vec4f& color, const float& thickness, const float& fade)
        : color({ "color", color }), thickness({ "thickness", thickness }), fade({ "fade", fade }) {}
        CircleComponent(const Vec4fUniform& color, const FloatUniform& thickness, const FloatUniform& fade)
        : color(color), thickness(thickness), fade(fade) {}
    };
}
