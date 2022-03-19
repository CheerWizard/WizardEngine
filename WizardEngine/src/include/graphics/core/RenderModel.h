//
// Created by mecha on 09.01.2022.
//

#pragma once

#include <platform/graphics/VertexArray.h>
#include <platform/graphics/VertexBuffer.h>
#include <platform/graphics/IndexBuffer.h>

#include <graphics/core/geometry/MeshComponent.h>

namespace engine {

    struct VRenderModel {
        uint32_t id = 0;
        VertexArray vao;
        VertexBuffer vbo;

        VRenderModel(
                const uint32_t &id,
                const uint32_t& vertexCount
        ): id(id), vbo(vertexCount) {}
    };

    struct VIRenderModel {
        uint32_t id = 0;
        VertexArray vao;
        VertexBuffer vbo;
        IndexBuffer ibo;

        VIRenderModel(
                const uint32_t &id,
                const uint32_t& vertexCount,
                const uint32_t& indexCount
        ): id(id), vbo(vertexCount), ibo(indexCount) {}
    };

    void release(VRenderModel& renderModel);
    void resetCounts(VRenderModel& renderModel);

    template<typename T>
    bool hasCapacity(const VRenderModel& renderModel, const VertexDataComponent<T> &vertexDataComponent) {
        return renderModel.vbo.hasCapacity(vertexDataComponent.vertexData.vertexCount);
    }

    template<typename T>
    void increaseCounts(VRenderModel& renderModel, const VertexDataComponent<T> &vertexDataComponent) {
        renderModel.vbo.increaseCount(vertexDataComponent.vertexData.vertexCount);
    }

    void release(VIRenderModel& renderModel);
    void resetCounts(VIRenderModel& renderModel);

    template<typename T>
    bool hasCapacity(const VIRenderModel& renderModel, const BaseMeshComponent<T> &meshComponent) {
        bool hasVertexCapacity = renderModel.vbo.hasCapacity(meshComponent.totalVertexCount);
        bool hasIndexCapacity = renderModel.ibo.hasCapacity(meshComponent.totalIndexCount);
        return hasVertexCapacity && hasIndexCapacity;
    }

    template<typename T>
    void increaseCounts(VIRenderModel& renderModel, const BaseMeshComponent<T> &meshComponent) {
        renderModel.vbo.increaseCount(meshComponent.totalVertexCount);
        renderModel.ibo.increaseCount(meshComponent.totalIndexCount);
    }
}