//
// Created by mecha on 11.11.2021.
//

#include <graphics/core/geometry/MeshComponent.h>

namespace engine {

    Mesh copy(const Mesh &mesh) {
        auto copyIndexData = copy(mesh.indexData);
        auto copyVertexData = copy(mesh.vertexData);
        return {
            copyVertexData,
            copyIndexData
        };
    }

    MeshComponent copy(const MeshComponent &meshComponent) {
        auto* copyMeshes = new Mesh[meshComponent.meshCount];

        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            const auto& mesh = meshComponent.meshes[i];
            copyMeshes[i] = copy(mesh);
        }

        return {
            copyMeshes,
            meshComponent.meshCount
        };
    }

    void updateCounts(MeshComponent &meshComponent) {
        meshComponent.totalVertexCount = 0;
        meshComponent.totalIndexCount = 0;
        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            const auto& vertexCount = meshComponent.meshes[i].vertexData.vertexCount;
            const auto& indexCount = meshComponent.meshes[i].indexData.indexCount;
            meshComponent.totalVertexCount += vertexCount;
            meshComponent.totalIndexCount += indexCount;
        }
    }

    void updateStart(MeshComponent &meshComponent, const uint32_t &prevVertexCount, const uint32_t &prevIndexCount) {
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

    void invalidate(MeshComponent &meshComponent) {
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

    void setInstanceId(MeshComponent &meshComponent, const uint32_t &instanceId) {
        const auto& meshInstanceId = meshComponent.meshes[0].vertexData.vertices[0].instanceId;
        if (meshInstanceId == instanceId) return;
        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            const auto& vertexData = meshComponent.meshes[i].vertexData;
            for (auto j = 0; j < vertexData.vertexCount; j++) {
                auto& vertex = vertexData.vertices[j];
                vertex.instanceId = (float) instanceId;
            }
        }
    }

}