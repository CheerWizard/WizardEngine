//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/buffer_data/IndexData.h>

#define MIN_MESH_COUNT 1

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
        BaseMesh<T> *meshes = nullptr;
        u32 meshCount = MIN_MESH_COUNT;
        u32 totalVertexCount = 0;
        u32 totalIndexCount = 0;
        u32 vertexStart = 0;
        u32 indexStart = 0;
        bool isUpdated = true;
        u32 renderModelId = 0;
        DrawType drawType = DrawType::TRIANGLE;

        BaseMeshComponent() = default;
        BaseMeshComponent(BaseMesh<T>* meshes, u32 meshCount) : meshes(meshes), meshCount(meshCount) {}

        [[nodiscard]] u32 getId() const {
            return meshes[0].vertexData.values[0].id;
        }

        template<typename TO>
        BaseMeshComponent<TO> toMeshComponent(const std::function<TO(const T&)>& vertexMapper);

        void setId(u32 id);

        BaseMeshComponent<T> copy() const;

        void invalidateMeshes(u32 prevVertexCount, u32 prevIndexCount);
        void invalidateSize();
    };

    template<typename T>
    template<typename TO>
    BaseMeshComponent<TO> BaseMeshComponent<T>::toMeshComponent(const std::function<TO(const T&)> &vertexMapper) {
        auto toMeshes = new BaseMesh<TO>[meshCount];

        for (auto j = 0; j < meshCount; j++) {
            toMeshes[j] = meshes[j].template toMesh<TO>(vertexMapper);
        }

        auto meshComponent = BaseMeshComponent<TO>();
        meshComponent.meshes = toMeshes;
        meshComponent.meshCount = meshCount;
        meshComponent.totalVertexCount = totalVertexCount;
        meshComponent.totalIndexCount = totalIndexCount;
        meshComponent.vertexStart = vertexStart;
        meshComponent.indexStart = indexStart;
        meshComponent.isUpdated = isUpdated;
        meshComponent.renderModelId = renderModelId;
        return meshComponent;
    }

    template<typename T>
    void BaseMeshComponent<T>::setId(u32 id) {
        u32 meshInstanceId = getId();
        if (meshInstanceId == id) return;
        for (auto i = 0 ; i < meshCount ; i++) {
            const auto& vertexData = meshes[i].vertexData;
            for (auto j = 0; j < vertexData.size; j++) {
                auto& vertex = vertexData.values[j];
                vertex.id = (float) id;
            }
        }
    }

    template<typename T>
    BaseMeshComponent<T> BaseMeshComponent<T>::copy() const {
        auto* copyMeshes = new BaseMesh<T>[meshCount];

        for (auto i = 0 ; i < meshCount ; i++) {
            copyMeshes[i] = meshes[i].copy();
        }

        BaseMeshComponent<T> copyMeshComponent;
        copyMeshComponent.renderModelId = renderModelId;
        copyMeshComponent.isUpdated = isUpdated;
        copyMeshComponent.drawType = drawType;
        copyMeshComponent.indexStart = indexStart;
        copyMeshComponent.vertexStart = vertexStart;
        copyMeshComponent.totalIndexCount = totalIndexCount;
        copyMeshComponent.totalVertexCount = totalVertexCount;
        copyMeshComponent.meshCount = meshCount;
        copyMeshComponent.meshes = copyMeshes;

        return copyMeshComponent;
    }

    template<typename T>
    void BaseMeshComponent<T>::invalidateMeshes(u32 prevVertexCount, u32 prevIndexCount) {
        vertexStart = prevVertexCount;
        indexStart = prevIndexCount;
        totalVertexCount = 0;
        totalIndexCount = 0;

        for (auto i = 0; i < meshCount ; i++) {
            array<T>& vertexData = meshes[i].vertexData;
            IndexData& indexData = meshes[i].indexData;

            vertexData.offset = vertexStart + totalVertexCount;
            indexData.offset = indexStart + totalIndexCount;

            for (auto j = 0 ; j < indexData.size ; j++) {
                auto& index = indexData.values[j];
                index += vertexData.offset;
            }

            totalVertexCount += vertexData.size;
            totalIndexCount += indexData.size;
        }
    }

    template<typename T>
    void BaseMeshComponent<T>::invalidateSize() {
        totalVertexCount = 0;
        totalIndexCount = 0;
        for (auto i = 0; i < meshCount ; i++) {
            totalVertexCount += meshes[i].vertexData.size;
            totalIndexCount += meshes[i].indexData.size;
        }
    }
}
