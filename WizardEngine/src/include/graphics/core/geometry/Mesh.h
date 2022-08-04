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
    };

    template<typename T>
    struct BaseMesh {
        array<T> vertexData;
        IndexData indexData;
    };

    template<typename T>
    BaseMesh<T> copy(const BaseMesh<T> &mesh) {
        return BaseMesh<T>{ copy<T>(mesh.vertexData), copy(mesh.indexData) };
    }

    template_component(BaseMeshComponent, T) {
        BaseMesh<T> *meshes = nullptr;
        u32 meshCount = MIN_MESH_COUNT;
        u32 totalVertexCount = 0;
        u32 totalIndexCount = 0;
        u32 vertexStart = 0;
        u32 indexStart = 0;
        bool isUpdated = true;
        u8 renderModelId = 0;
        DrawType drawType = DrawType::TRIANGLE;

        BaseMeshComponent() = default;
        BaseMeshComponent(BaseMesh<T>* meshes, u32 meshCount) : meshes(meshes), meshCount(meshCount) {}
    };

    template<typename TO, typename FROM>
    BaseMesh<TO> toMesh(const BaseMesh<FROM>& fromBaseMesh, const std::function<TO(const FROM&)>& vertexMapper) {
        auto& fromVertexData = fromBaseMesh.vertexData;
        auto toVertexData = array<TO> {
                new TO[fromVertexData.size],
                fromVertexData.offset,
                fromVertexData.size
        };
        for (auto j = 0; j < fromVertexData.size; j++) {
            toVertexData.values[j] = vertexMapper(fromVertexData.values[j]);
        }
        return { toVertexData, fromBaseMesh.indexData };
    }

    template<typename TO, typename FROM>
    BaseMeshComponent<TO> toMeshComponent(const BaseMeshComponent<FROM>& fromBaseMeshComponent, const std::function<TO(const FROM&)>& vertexMapper) {
        auto& fromMeshes = fromBaseMeshComponent.meshes;
        auto toMeshes = new BaseMesh<TO>[fromBaseMeshComponent.meshCount];

        for (auto j = 0; j < fromBaseMeshComponent.meshCount; j++) {
            toMeshes[j] = toMesh<TO, FROM>(fromMeshes[j], vertexMapper);
        }

        auto meshComponent = BaseMeshComponent<TO>();
        meshComponent.meshes = toMeshes;
        meshComponent.meshCount = fromBaseMeshComponent.meshCount;
        meshComponent.totalVertexCount = fromBaseMeshComponent.totalVertexCount;
        meshComponent.totalIndexCount = fromBaseMeshComponent.totalIndexCount;
        meshComponent.vertexStart = fromBaseMeshComponent.vertexStart;
        meshComponent.indexStart = fromBaseMeshComponent.indexStart;
        meshComponent.isUpdated = fromBaseMeshComponent.isUpdated;
        meshComponent.renderModelId = fromBaseMeshComponent.renderModelId;
        return meshComponent;
    }

    template<typename T>
    BaseMeshComponent<InstanceVertex<Vertex3d>> toMesh3dInstance(const BaseMeshComponent<T>& fromBaseMeshComponent) {
        return toMeshComponent<T, InstanceVertex<Vertex3d>>(
                fromBaseMeshComponent,
                [](const T& vertex) {
                    return InstanceVertex<Vertex3d> {
                        Vertex3d { vertex.position, vertex.uv, vertex.normal }
                    };
                }
        );
    }

    template<typename T>
    BaseMeshComponent<BatchVertex<Vertex3d>> toMesh3dBatch(const BaseMeshComponent<T>& fromBaseMeshComponent) {
        return toMeshComponent<T, BatchVertex<Vertex3d>>(
                fromBaseMeshComponent,
                [](const T& vertex) {
                    return BatchVertex<Vertex3d> { { vertex.position, vertex.uv, vertex.normal } };
                }
        );
    }

    template<typename T>
    void setBatchId(BaseMeshComponent<BatchVertex<T>> &meshComponent, const uint32_t &batchId) {
        const auto& meshInstanceId = meshComponent.meshes[0].vertexData.values[0].id;
        if (meshInstanceId == batchId) return;
        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            const auto& vertexData = meshComponent.meshes[i].vertexData;
            for (auto j = 0; j < vertexData.size; j++) {
                auto& vertex = vertexData.values[j];
                vertex.id = (float) batchId;
            }
        }
    }

    template<typename T>
    BaseMeshComponent<T> copy(const BaseMeshComponent<T> &meshComponent) {
        auto* copyMeshes = new BaseMesh<T>[meshComponent.meshCount];

        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            const auto& mesh = meshComponent.meshes[i];
            copyMeshes[i] = copy<T>(mesh);
        }

        return BaseMeshComponent<T> { copyMeshes, meshComponent.meshCount };
    }

    template<typename T>
    void updateStartAndCounts(
            BaseMeshComponent<T> &meshComponent,
            const u32 &prevVertexCount,
            const u32 &prevIndexCount
    ) {
        meshComponent.vertexStart = prevVertexCount;
        meshComponent.indexStart = prevIndexCount;
        meshComponent.totalVertexCount = 0;
        meshComponent.totalIndexCount = 0;

        for (auto i = 0; i < meshComponent.meshCount ; i++) {
            array<T>& vertexData = meshComponent.meshes[i].vertexData;
            IndexData& indexData = meshComponent.meshes[i].indexData;

            vertexData.offset = meshComponent.vertexStart + meshComponent.totalVertexCount;
            indexData.offset = meshComponent.indexStart + meshComponent.totalIndexCount;

            for (auto j = 0 ; j < indexData.size ; j++) {
                auto& index = indexData.values[j];
                index += vertexData.offset;
            }

            meshComponent.totalVertexCount += vertexData.size;
            meshComponent.totalIndexCount += indexData.size;
        }
    }
}
