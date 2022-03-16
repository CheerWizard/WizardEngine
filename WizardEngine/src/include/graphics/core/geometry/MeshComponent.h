//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/buffer_data/IndexData.h>

#define MIN_MESH_COUNT 1

namespace engine {

    struct Mesh {
        VertexData vertexData;
        IndexData indexData;
    };

    Mesh copy(const Mesh &mesh);

    struct MeshComponent {
        Mesh *meshes = nullptr;
        uint32_t meshCount = MIN_MESH_COUNT;
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        uint32_t vertexStart = 0;
        uint32_t indexStart = 0;
        bool isUpdated = true;
        uint32_t renderModelId = 0;
    };

    MeshComponent copy(const MeshComponent &meshComponent);
    void updateCounts(MeshComponent &meshComponent);
    void updateStart(MeshComponent &meshComponent, const uint32_t &prevVertexCount, const uint32_t &prevIndexCount);
    void invalidate(MeshComponent &meshComponent);
    void setInstanceId(MeshComponent &meshComponent, const uint32_t &instanceId);
}