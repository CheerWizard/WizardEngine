//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/buffer_data/IndexData.h>

namespace engine::graphics {

    using namespace math;

    struct Vertex3d {
        vec3f position = { 0.5f, 0.5f, 0.5f };
        vec2f uv = {0.25f, -0.25f };
        vec3f normal = { 0, 0, 0 };
        vec3f tangent = { 0, 0, 0 };
        vec3f bitangent = { 0, 0, 0 };
    };

    template<typename T>
    struct BaseMesh {
        array<T> vertexData;
        IndexData indexData;

        BaseMesh<T> copy();

        template<typename TO>
        BaseMesh<TO> toMesh(const std::function<TO(const T&)>& vertexMapper);
    };

    template<typename T>
    BaseMesh<T> BaseMesh<T>::copy() {
        return BaseMesh<T>{ vertexData.copy(), indexData.copy() };
    }

    template<typename T>
    template<typename TO>
    BaseMesh<TO> BaseMesh<T>::toMesh(const std::function<TO(const T &)> &vertexMapper) {
        auto toVertexData = array<TO> {
                new TO[vertexData.size],
                vertexData.offset,
                vertexData.size
        };
        for (auto j = 0; j < vertexData.size; j++) {
            toVertexData.values[j] = vertexMapper(vertexData.values[j]);
        }
        return { toVertexData, indexData };
    }

    template_component(BaseMeshComponent, T) {
        BaseMesh<T> mesh;
        u32 vertexStart = 0;
        u32 indexStart = 0;
        bool isUpdated = true;
        u32 drawType = DrawType::TRIANGLE;

        BaseMeshComponent() = default;
        BaseMeshComponent(const BaseMesh<T>& mesh) : mesh(mesh) {}

        [[nodiscard]] u32 getId() const {
            return mesh.vertexData.values[0].id;
        }

        template<typename TO>
        BaseMeshComponent<TO> toMeshComponent(const std::function<TO(const T&)>& vertexMapper);

        void setId(u32 id);

        BaseMeshComponent<T> copy() const;

        void invalidateMeshes(u32 prevVertexCount, u32 prevIndexCount);

        [[nodiscard]] inline u32 totalVertexCount() const { return mesh.vertexData.size; }
        [[nodiscard]] inline u32 totalIndexCount() const { return mesh.indexData.size; }
    };

    template<typename T>
    template<typename TO>
    BaseMeshComponent<TO> BaseMeshComponent<T>::toMeshComponent(const std::function<TO(const T&)> &vertexMapper) {
        BaseMeshComponent<TO> meshComponent;
        meshComponent.mesh = mesh.template toMesh<TO>(vertexMapper);
        meshComponent.vertexStart = vertexStart;
        meshComponent.indexStart = indexStart;
        meshComponent.isUpdated = isUpdated;
        return meshComponent;
    }

    template<typename T>
    void BaseMeshComponent<T>::setId(u32 id) {
        u32 meshInstanceId = getId();
        if (meshInstanceId == id) return;

        const auto& vertexData = mesh.vertexData;
        for (auto j = 0; j < vertexData.size; j++) {
            auto& vertex = vertexData.values[j];
            vertex.id = (float) id;
        }
    }

    template<typename T>
    BaseMeshComponent<T> BaseMeshComponent<T>::copy() const {
        BaseMeshComponent<T> copyMeshComponent;
        copyMeshComponent.isUpdated = isUpdated;
        copyMeshComponent.drawType = drawType;
        copyMeshComponent.indexStart = indexStart;
        copyMeshComponent.vertexStart = vertexStart;
        copyMeshComponent.meshes = mesh.copy();
        return copyMeshComponent;
    }

    template<typename T>
    void BaseMeshComponent<T>::invalidateMeshes(u32 prevVertexCount, u32 prevIndexCount) {
        vertexStart = prevVertexCount;
        indexStart = prevIndexCount;

        array<T>& vertexData = mesh.vertexData;
        IndexData& indexData = mesh.indexData;

        vertexData.offset = vertexStart;
        indexData.offset = indexStart;

        for (auto j = 0 ; j < indexData.size ; j++) {
            auto& index = indexData.values[j];
            index += vertexData.offset;
        }
    }
}
