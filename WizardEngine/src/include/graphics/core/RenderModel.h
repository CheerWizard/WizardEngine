//
// Created by mecha on 09.01.2022.
//

#pragma once

#include <platform/graphics/VertexArray.h>
#include <platform/graphics/VertexBuffer.h>
#include <platform/graphics/IndexBuffer.h>

#include <graphics/core/geometry/Mesh.h>

namespace engine::graphics {

    struct ENGINE_API VRenderModel {
        u8 id = 0;
        VertexArray vao;
        VertexBuffer vbo;
        vector<ecs::entity_id> entities;
        ecs::entity_id geometry = invalid_entity_id;

        VRenderModel() = default;

        VRenderModel(
                const u8 &id,
                const u32& vertexCount
        ): id(id), vbo(vertexCount) {
            vao.create();
            vbo.create();
        }

        void release();
        void resetCounts();

        template<typename T>
        void upload(VertexDataComponent<T> &vertexDataComponent);
        template<typename T>
        void uploadStatic(const VertexDataComponent<T> &vertexDataComponent);

        template<typename T>
        void tryUpload(VertexDataComponent<T> &vertexDataComponent, u32 &previousVertexCount);

        template<typename T>
        void tryUploadBatch(
                u32 batchId,
                VertexDataComponent<BatchVertex<T>> &vertexDataComponent,
                uint32_t &previousVertexCount
        );

        template<typename T>
        bool hasCapacity(const VertexDataComponent<T> &vertexDataComponent) const;

        template<typename T>
        void increaseCounts(const VertexDataComponent<T> &vertexDataComponent);
    };

    template<typename T>
    void VRenderModel::uploadStatic(const VertexDataComponent<T> &vertexDataComponent) {
        vao.bind();
        vbo.bind();
        vbo.loadStatic(vertexDataComponent.vertexData);
        VertexArray::unbind();
    }

    template<typename T>
    void VRenderModel::upload(VertexDataComponent<T> &vertexDataComponent) {
        vao.bind();
        vbo.bind();
        vbo.load(vertexDataComponent.vertexData);
    }

    template<typename T>
    bool VRenderModel::hasCapacity(const VertexDataComponent<T> &vertexDataComponent) const {
        return vbo.hasCapacity(vertexDataComponent.vertexData.size);
    }

    template<typename T>
    void VRenderModel::increaseCounts(const VertexDataComponent<T> &vertexDataComponent) {
        vbo.increaseCount(vertexDataComponent.vertexData.size);
    }

    template<typename T>
    void VRenderModel::tryUpload(VertexDataComponent<T> &vertexDataComponent, u32 &previousVertexCount) {
        if (vertexDataComponent.isUpdated) {
            vertexDataComponent.isUpdated = false;
            vertexDataComponent.updateStart(previousVertexCount);
            upload(vertexDataComponent);
        }
        previousVertexCount += vertexDataComponent.vertexData.size;
    }

    template<typename T>
    void VRenderModel::tryUploadBatch(
            u32 batchId,
            VertexDataComponent<BatchVertex<T>> &vertexDataComponent,
            uint32_t &previousVertexCount
    ) {
        if (vertexDataComponent.isUpdated) {
            vertexDataComponent.setBatchId(batchId);
        }
        tryUpload(vertexDataComponent, previousVertexCount);
    }

    struct ENGINE_API VIRenderModel {
        u8 id = 0;
        VertexArray vao;
        VertexBuffer vbo;
        IndexBuffer ibo;
        vector<ecs::entity_id> entities;
        ecs::entity_id mesh = invalid_entity_id;

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

        void release();
        void resetCounts();

        template<typename T>
        void upload(BaseMeshComponent<T> &meshComponent);

        template<typename T>
        void tryUpload(
                BaseMeshComponent<T> &meshComponent,
                u32 &previousVertexCount,
                u32 &previousIndexCount
        );

        template<typename T>
        void tryUploadBatchMesh(
                u32 batchId,
                BaseMeshComponent<BatchVertex<T>> &baseMeshComponent,
                u32 &previousVertexCount,
                u32 &previousIndexCount
        );

        template<typename T>
        bool hasCapacity(const BaseMeshComponent<T> &meshComponent) const;

        template<typename T>
        void increaseCounts(const BaseMeshComponent<T> &meshComponent);
    };

    template<typename T>
    void VIRenderModel::upload(BaseMeshComponent<T> &meshComponent) {
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

    template<typename T>
    bool VIRenderModel::hasCapacity(const BaseMeshComponent<T> &meshComponent) const {
        bool hasVertexCapacity = vbo.hasCapacity(meshComponent.totalVertexCount);
        bool hasIndexCapacity = ibo.hasCapacity(meshComponent.totalIndexCount);
        return hasVertexCapacity && hasIndexCapacity;
    }

    template<typename T>
    void VIRenderModel::increaseCounts(const BaseMeshComponent<T> &meshComponent) {
        vbo.increaseCount(meshComponent.totalVertexCount);
        ibo.increaseCount(meshComponent.totalIndexCount);
    }

    template<typename T>
    void VIRenderModel::tryUpload(BaseMeshComponent<T> &meshComponent,
                                  u32 &previousVertexCount, u32 &previousIndexCount) {
        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            meshComponent.invalidateMeshes(previousVertexCount, previousIndexCount);
            upload(meshComponent);
        }
        previousIndexCount += meshComponent.totalIndexCount;
        previousVertexCount += meshComponent.totalVertexCount;
    }

    template<typename T>
    void VIRenderModel::tryUploadBatchMesh(
            u32 batchId,
            BaseMeshComponent<BatchVertex<T>> &baseMeshComponent,
            u32 &previousVertexCount, u32 &previousIndexCount
    ) {
        if (baseMeshComponent.isUpdated) {
            baseMeshComponent.setId(batchId);
        }
        tryUpload<BatchVertex<T>>(baseMeshComponent, previousVertexCount, previousIndexCount);
    }
}