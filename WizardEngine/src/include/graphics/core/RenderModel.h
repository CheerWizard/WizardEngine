//
// Created by mecha on 09.01.2022.
//

#pragma once

#include <platform/graphics/VertexArray.h>
#include <platform/graphics/VertexBuffer.h>
#include <platform/graphics/IndexBuffer.h>

#include <graphics/core/geometry/Mesh.h>

namespace engine::graphics {

    struct VRenderModel {
        u8 id = 0;
        VertexArray vao;
        VertexBuffer vbo;

        VRenderModel() = default;

        VRenderModel(
                const u8 &id,
                const u32& vertexCount
        ): id(id), vbo(vertexCount) {
            vao.create();
            vbo.create();
        }

        template<typename T>
        void upload(VertexDataComponent<T> &vertexDataComponent) {
            vao.bind();
            vbo.bind();
            vbo.load(vertexDataComponent.vertexData);
        }

        template<typename T>
        void uploadStatic(const VertexDataComponent<T> &vertexDataComponent) {
            vao.bind();
            vbo.bind();
            vbo.loadStatic(vertexDataComponent.vertexData);
            VertexArray::unbind();
        }
    };

    struct VIRenderModel {
        u8 id = 0;
        VertexArray vao;
        VertexBuffer vbo;
        IndexBuffer ibo;

        VIRenderModel() = default;

        VIRenderModel(
                const u8 &id,
                const u32& vertexCount,
                const u32& indexCount
        ): id(id), vbo(vertexCount), ibo(indexCount) {
            vao.create();
            vbo.create();
            ibo.create();
        }

        template<typename T>
        void upload(BaseMeshComponent<T> &meshComponent) {
            vao.bind();
            vbo.bind();
            ibo.bind();

            const auto& meshes = meshComponent.meshes;
            for (auto i = 0; i < meshComponent.meshCount ; i++) {
                const auto& vertexData = meshes[i].vertexData;
                vbo.load(vertexData);
                const auto& indexData = meshes[i].indexData;
                ibo.load(indexData);
            }
        }
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

    template<typename T>
    void upload(VertexDataComponent<T> &vertexDataComponent, VRenderModel& renderModel) {
        renderModel.vao.bind();
        renderModel.vbo.bind();
        renderModel.vbo.load(vertexDataComponent.vertexData);
    }

    template<typename T>
    void upload(BaseMeshComponent<T> &meshComponent, VIRenderModel& renderModel) {
        auto& vbo = renderModel.vbo;
        auto& ibo = renderModel.ibo;

        renderModel.vao.bind();
        vbo.bind();
        ibo.bind();
        const auto& meshes = meshComponent.meshes;
        for (auto i = 0; i < meshComponent.meshCount ; i++) {
            const auto& vertexData = meshes[i].vertexData;
            vbo.load(vertexData);
            const auto& indexData = meshes[i].indexData;
            ibo.load(indexData);
        }
    }

    template<typename T>
    void tryUpload(
            VertexDataComponent<T> &vertexDataComponent,
            uint32_t &previousVertexCount,
            VRenderModel& renderModel
    ) {
        if (vertexDataComponent.isUpdated) {
            vertexDataComponent.isUpdated = false;
            updateStart(vertexDataComponent, previousVertexCount);
            upload(vertexDataComponent, renderModel);
        }
        previousVertexCount += vertexDataComponent.vertexData.vertexCount;
    }

    template<typename T>
    void tryUploadBatch(
            const uint32_t &id,
            VertexDataComponent<BatchVertex<T>> &vertexDataComponent,
            uint32_t &previousVertexCount,
            VRenderModel& renderModel
    ) {
        if (vertexDataComponent.isUpdated) {
            setBatchId(vertexDataComponent, id);
        }
        tryUpload<BatchVertex<T>>(vertexDataComponent, previousVertexCount, renderModel);
    }

    template<typename T>
    void tryUpload(
            BaseMeshComponent<T> &meshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount,
            VIRenderModel& renderModel
    ) {
        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            upload(meshComponent, renderModel);
        }
        previousIndexCount += meshComponent.totalIndexCount;
        previousVertexCount += meshComponent.totalVertexCount;
    }

    template<typename T>
    void tryUploadBatchMesh(
            const uint32_t &id,
            BaseMeshComponent<BatchVertex<T>> &baseMeshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount,
            VIRenderModel& renderModel
    ) {
        if (baseMeshComponent.isUpdated) {
            setBatchId(baseMeshComponent, id);
        }
        tryUpload<BatchVertex<T>>(baseMeshComponent, previousVertexCount, previousIndexCount, renderModel);
    }

    template<typename T>
    void uploadStatic(const VertexDataComponent<T> &vertexDataComponent, VRenderModel& renderModel) {
        renderModel.vao.bind();
        renderModel.vbo.bind();
        renderModel.vbo.loadStatic(vertexDataComponent.vertexData);
        VertexArray::unbind();
    }
}