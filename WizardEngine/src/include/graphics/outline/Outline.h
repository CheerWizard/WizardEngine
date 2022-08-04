//
// Created by mecha on 23.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/geometry/Mesh.h>
#include <ecs/ecs.h>

namespace engine::graphics {

    using namespace shader;
    using namespace math;

    struct OutlineVertex {
        vec3f position = { 0.5, 0.5, 0.5 };
        vec3f normal = { 0.5, 0.5, 0.5 };
    };

    serialize_component(OutlineComponent) {
        const char* name = "outline";
        Vec4fUniform color = { "color", { 0, 1, 0, 1 } };
        FloatUniform thickness = { "thickness", 0.05 };

        OutlineComponent() = default;
        OutlineComponent(const vec4f& color, const float& thickness)
        : color({ "color", color }), thickness({ "thickness", thickness }) {}
        OutlineComponent(const char* name, const Vec4fUniform& color, const FloatUniform& thickness)
        : name(name), color(color), thickness(thickness) {}

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

    typedef BaseMeshComponent<InstanceVertex<OutlineVertex>> OutlineInstanceMesh;
    typedef BaseMeshComponent<BatchVertex<OutlineVertex>> OutlineBatchMesh;
    typedef VertexDataComponent<InstanceVertex<OutlineVertex>> OutlineInstanceVertexData;
    typedef VertexDataComponent<BatchVertex<OutlineVertex>> OutlineBatchVertexData;

    template<typename T>
    OutlineVertex map(const T& vertex) {
        return { vertex.position, vertex.normal };
    }

    template<typename T>
    BaseMeshComponent<InstanceVertex<OutlineVertex>> toOutlineInstanceMesh(const BaseMeshComponent<InstanceVertex<T>>& baseMeshComponent) {
        return toMeshComponent<InstanceVertex<T>, InstanceVertex<OutlineVertex>>(
                baseMeshComponent,
                [](const InstanceVertex<T>& vertex) { return InstanceVertex<OutlineVertex> { map(vertex.vertex) }; }
        );
    }

    template<typename T>
    BaseMeshComponent<BatchVertex<OutlineVertex>> toOutlineBatchMesh(const BaseMeshComponent<BatchVertex<T>>& baseMeshComponent) {
        return toMeshComponent<BatchVertex<T>, BatchVertex<OutlineVertex>>(
                baseMeshComponent,
                [](const BatchVertex<T>& vertex) { return BatchVertex<OutlineVertex> { map(vertex.vertex) }; }
        );
    }

    template<typename T>
    VertexDataComponent<InstanceVertex<OutlineVertex>> toOutlineInstanceVertexData(const VertexDataComponent<InstanceVertex<T>>& vertexDataComponent) {
        return toVertexDataComponent<InstanceVertex<T>, InstanceVertex<OutlineVertex>>(
                vertexDataComponent,
                [](const InstanceVertex<T>& vertex) { return InstanceVertex<OutlineVertex> { map(vertex.vertex) }; }
        );
    }

    template<typename T>
    VertexDataComponent<BatchVertex<OutlineVertex>> toOutlineBatchVertexData(const VertexDataComponent<BatchVertex<T>>& vertexDataComponent) {
        return toVertexDataComponent<BatchVertex<T>, BatchVertex<OutlineVertex>>(
                vertexDataComponent,
                [](const BatchVertex<T>& vertex) { return BatchVertex<OutlineVertex> { map(vertex.vertex) }; }
        );
    }
}
