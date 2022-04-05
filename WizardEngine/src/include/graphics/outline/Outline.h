//
// Created by mecha on 23.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/shader/Uniform.h>
#include <graphics/core/geometry/Mesh.h>
#include <glm/glm.hpp>

namespace engine::graphics {

    struct OutlineVertex {
        glm::vec3 position = { 0.5, 0.5, 0.5 };
        glm::vec3 normal = { 0.5, 0.5, 0.5 };
    };

    struct OutlineComponent {
        const char* name = "outline";
        shader::Vec4fUniform color = { "color", { 0, 1, 0, 1 } };
        FloatUniform thickness = { "thickness", 0.05 };

        OutlineComponent() = default;
        OutlineComponent(const glm::vec4& color, const float& thickness)
        : color({ "color", color }), thickness({ "thickness", thickness }) {}
        OutlineComponent(const char* name, const Vec4fUniform& color, const FloatUniform& thickness)
        : name(name), color(color), thickness(thickness) {}
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
