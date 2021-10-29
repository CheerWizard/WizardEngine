//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../buffers/VertexData.h"
#include "../buffers/IndexData.h"

#define MIN_MESH_COUNT 1

namespace engine {

    struct Mesh {
        VertexData vertexData;
        IndexData indexData;

    public:
        Mesh copy() const {
            return Mesh {
                vertexData.copy(),
                indexData.copy()
            };
        }
    };

    struct MeshComponent {
        Mesh* meshes;
        uint32_t meshCount;
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        uint32_t vertexStart = 0;
        uint32_t indexStart = 0;

        bool isUpdated = false;

        MeshComponent(Mesh* meshes = nullptr, const uint32_t &meshCount = MIN_MESH_COUNT) :
        meshes(meshes), meshCount(meshCount)  {
            applyChanges();
        }

    public:
        void applyChanges() {
            isUpdated = true;
        }

        MeshComponent copy() const {
            auto* copyMeshes = new Mesh[meshCount];

            for (auto i = 0 ; i < meshCount ; i++) {
                copyMeshes[i] = meshes[i].copy();
            }

            return MeshComponent {
                copyMeshes,
                meshCount
            };
        }

        void updateCounts() {
            totalVertexCount = 0;
            totalIndexCount = 0;
            for (auto i = 0 ; i < meshCount ; i++) {
                totalVertexCount += meshes[i].vertexData.vertexCount;
                totalIndexCount += meshes[i].indexData.indexCount;
            }
        }

        void updateStart(const uint32_t &prevVertexCount, const uint32_t &prevIndexCount) {
            if (vertexStart != prevVertexCount || indexStart != prevIndexCount) {
                vertexStart = prevVertexCount;
                indexStart = prevIndexCount;
                for (auto i = 0; i < meshCount ; i++) {
                    auto& vertexData = meshes[i].vertexData;
                    auto& indexData = meshes[i].indexData;

                    vertexData.vertexStart += vertexStart;
                    indexData.indexStart += indexStart;
                    for (auto j = 0 ; j < indexData.indexCount ; j++) {
                        auto& index = indexData.indices[j];
                        index += vertexStart;
                    }
                }
            }
        }

        void invalidateMeshes() const {
            uint32_t totalVertexCount = 0;
            uint32_t totalIndexCount = 0;
            for (auto i = 0 ; i < meshCount ; i++) {
                auto& vertexData = meshes[i].vertexData;
                auto& indexData = meshes[i].indexData;

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

        void setInstanceId(const uint32_t &instanceId) {
            if (_instanceId == instanceId) return;
            _instanceId = instanceId;
            // update instanceId for each vertex in each mesh.
            for (auto i = 0 ; i < meshCount ; i++) {
                const auto& vertexData = meshes[i].vertexData;
                for (auto j = 0; j < vertexData.vertexCount; j++) {
                    auto& vertex = vertexData.vertices[j];
                    vertex.instanceId = (float) instanceId;
                }
            }
        }

    private:
        uint32_t _instanceId = 0;

    };

}