//
// Created by mecha on 23.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/shader/Uniform.h>
#include <graphics/core/geometry/Mesh.h>
#include <glm/glm.hpp>

namespace engine {

    struct OutlineVertex {
        glm::vec3 position = { 0.5, 0.5, 0.5 };
    };

    struct OutlineComponent {
        shader::Vec4fUniform color = { "outlineColor", { 0, 1, 0, 1 } };
    };

    typedef BaseMeshComponent<InstanceVertex<OutlineVertex>> OutlineInstanceMesh;
    typedef BaseMeshComponent<BatchVertex<OutlineVertex>> OutlineBatchMesh;

    template<typename T>
    OutlineVertex mapPos(const T& vertex) {
        return { vertex.position };
    }

    template<typename T>
    BaseMeshComponent<InstanceVertex<OutlineVertex>> toOutlineMesh(const BaseMeshComponent<InstanceVertex<T>>& baseMeshComponent) {
        return toMeshComponent<InstanceVertex<T>, InstanceVertex<OutlineVertex>>(
                baseMeshComponent,
                [](const T& vertex) { return mapPos(vertex); }
        );
    }

    template<typename T>
    BaseMeshComponent<BatchVertex<OutlineVertex>> toOutlineMesh(const BaseMeshComponent<BatchVertex<T>>& baseMeshComponent) {
        return toMeshComponent<BatchVertex<T>, BatchVertex<OutlineVertex>>(
                baseMeshComponent,
                [](const T& vertex) { return mapPos(vertex); }
        );
    }

    template<typename T>
    VertexDataComponent<InstanceVertex<OutlineVertex>> toOutlineVertexData(const VertexDataComponent<InstanceVertex<T>>& vertexDataComponent) {
        return toVertexDataComponent<InstanceVertex<T>, InstanceVertex<OutlineVertex>>(
                vertexDataComponent,
                [](const T& vertex) { return mapPos(vertex); }
        );
    }

    template<typename T>
    VertexDataComponent<BatchVertex<OutlineVertex>> toOutlineVertexData(const VertexDataComponent<BatchVertex<T>>& vertexDataComponent) {
        return toVertexDataComponent<BatchVertex<T>, BatchVertex<OutlineVertex>>(
                vertexDataComponent,
                [](const T& vertex) { return mapPos(vertex); }
        );
    }

}