//
// Created by mecha on 09.01.2022.
//

#pragma once

#include <platform/graphics/VertexArray.h>
#include <platform/graphics/VertexBuffer.h>
#include <platform/graphics/IndexBuffer.h>

#include <graphics/core/geometry/MeshComponent.h>

namespace engine {

    struct RenderModel {
        uint32_t id = 0;
        VertexArray vao;
        VertexBuffer vbo;
        IndexBuffer ibo;

        RenderModel(
                const uint32_t &id,
                const uint32_t& vertexCount,
                const uint32_t& indexCount
        ): id(id), vbo(vertexCount), ibo(indexCount) {}
    };

    void release(RenderModel& renderModel);
    void resetCounts(RenderModel& renderModel);

    template<typename T>
    bool hasCapacity(const RenderModel& renderModel, const BaseMeshComponent<T> &meshComponent) {
        bool hasVertexCapacity = renderModel.vbo.hasCapacity(meshComponent.totalVertexCount);
        bool hasIndexCapacity = renderModel.ibo.hasCapacity(meshComponent.totalIndexCount);
        return hasVertexCapacity && hasIndexCapacity;
    }

    template<typename T>
    void increaseCounts(RenderModel& renderModel, const BaseMeshComponent<T> &meshComponent) {
        renderModel.vbo.increaseCount(meshComponent.totalVertexCount);
        renderModel.ibo.increaseCount(meshComponent.totalIndexCount);
    }
}