//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/buffer_data/IndexData.h>
#include <glm/glm.hpp>

#define MIN_MESH_COUNT 1

namespace engine {

    struct Vertex3d {
        glm::vec3 position = { 0.5f, 0.5f, 0.5f };
        glm::vec2 textureCoords = { 0.25f, -0.25f };
        glm::vec3 normal = { 0, 0, 0 };
        float textureSlot = 0;
    };

    template<typename T>
    struct BaseMesh {
        VertexData<T> vertexData;
        IndexData indexData;
    };

    template<typename T>
    BaseMesh<T> copy(const BaseMesh<T> &mesh) {
        return BaseMesh<T>{ copy<T>(mesh.vertexData), copy(mesh.indexData) };
    }

    template<typename T>
    struct BaseMeshComponent {
        BaseMesh<T> *meshes = nullptr;
        uint32_t meshCount = MIN_MESH_COUNT;
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        uint32_t vertexStart = 0;
        uint32_t indexStart = 0;
        bool isUpdated = true;
        uint32_t renderModelId = 0;
        DrawType drawType = TRIANGLE;
    };

    template<typename T>
    void setBatchId(BaseMeshComponent<BatchVertex<T>> &meshComponent, const uint32_t &batchId) {
        const auto& meshInstanceId = meshComponent.meshes[0].vertexData.vertices[0].id;
        if (meshInstanceId == batchId) return;
        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            const auto& vertexData = meshComponent.meshes[i].vertexData;
            for (auto j = 0; j < vertexData.vertexCount; j++) {
                auto& vertex = vertexData.vertices[j];
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

        return {
                copyMeshes,
                meshComponent.meshCount
        };
    }

    template<typename T>
    void updateCounts(BaseMeshComponent<T> &meshComponent) {
        meshComponent.totalVertexCount = 0;
        meshComponent.totalIndexCount = 0;
        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            const auto& vertexCount = meshComponent.meshes[i].vertexData.vertexCount;
            const auto& indexCount = meshComponent.meshes[i].indexData.indexCount;
            meshComponent.totalVertexCount += vertexCount;
            meshComponent.totalIndexCount += indexCount;
        }
    }

    template<typename T>
    void updateStart(BaseMeshComponent<T> &meshComponent, const uint32_t &prevVertexCount, const uint32_t &prevIndexCount) {
        if (meshComponent.vertexStart != prevVertexCount || meshComponent.indexStart != prevIndexCount) {
            meshComponent.vertexStart = prevVertexCount;
            meshComponent.indexStart = prevIndexCount;
            for (auto i = 0; i < meshComponent.meshCount ; i++) {
                auto& vertexData = meshComponent.meshes[i].vertexData;
                auto& indexData = meshComponent.meshes[i].indexData;

                vertexData.vertexStart += meshComponent.vertexStart;
                indexData.indexStart += meshComponent.indexStart;
                for (auto j = 0 ; j < indexData.indexCount ; j++) {
                    auto& index = indexData.indices[j];
                    index += meshComponent.vertexStart;
                }
            }
        }
    }

    template<typename T>
    void invalidate(BaseMeshComponent<T> &meshComponent) {
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            auto& vertexData = meshComponent.meshes[i].vertexData;
            auto& indexData = meshComponent.meshes[i].indexData;

            if (vertexData.vertexStart != totalVertexCount || indexData.indexStart != totalIndexCount) {
                vertexData.vertexStart = totalVertexCount;
                indexData.indexStart = totalIndexCount;
                for (auto j = 0 ; j < indexData.indexCount ; j++) {
                    auto& index = indexData.indices[j];
                    index += vertexData.vertexStart;
                }
            }

            totalVertexCount += vertexData.vertexCount;
            totalIndexCount += indexData.indexCount;
        }
    }
}