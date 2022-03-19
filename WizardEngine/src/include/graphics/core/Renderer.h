//
// Created by mecha on 06.10.2021.
//

#pragma once

#include "RenderModel.h"

#include <ecs/Components.h>

#include "geometry/MeshComponent.h"
#include "geometry/Lines.h"

#include "shader/BaseShader.h"

#include <graphics/transform/TransformComponents.h>
#include <platform/graphics/RenderCommands.h>

#define INSTANCE_COUNT_LIMIT 128

using namespace engine::shader;

namespace engine {

    class Renderer {

    public:
        Renderer(const Ref<BaseShaderProgram>& shaderProgram, const DrawType& drawType = TRIANGLE)
        : shaderProgram(shaderProgram), drawType(drawType) {
            create();
        }

        ~Renderer() {
            release();
        }

    public:
        template<typename T1, typename T2>
        void render(const Entity& entity);
        template<typename T1, typename T2>
        void renderInstanced(entt::registry &registry);
        template<typename T1, typename T2>
        void renderBatched(entt::registry &registry);

        template<typename T>
        void renderV(const Entity& entity);
        template<typename T>
        void renderInstancedV(entt::registry &registry);
        template<typename T>
        void renderBatchedV(entt::registry &registry);

        template<typename T>
        void renderVI(const Entity& entity);
        template<typename T>
        void renderInstancedVI(entt::registry &registry);
        template<typename T>
        void renderBatchedVI(entt::registry &registry);

    public:
        template<typename T>
        void tryUploadInstance(
                const uint32_t &instanceId,
                VertexDataComponent<InstanceVertex<T>>& vertexDataComponent,
                uint32_t &previousVertexCount,
                VRenderModel& renderModel
        );
        template<typename T>
        void tryUpload(
                VertexDataComponent<T>& vertexDataComponent,
                uint32_t &previousVertexCount,
                VRenderModel& renderModel
        );
        template<typename T>
        void upload(VertexDataComponent<T> &vertexDataComponent, VRenderModel& renderModel);

        template<typename T>
        void tryUploadMeshInstance(
                const uint32_t &instanceId,
                BaseMeshComponent<InstanceVertex<T>>& meshComponent,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount,
                VIRenderModel& renderModel
        );
        template<typename T>
        void tryUpload(
                BaseMeshComponent<T>& meshComponent,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount,
                VIRenderModel& renderModel
        );
        template<typename T>
        void upload(BaseMeshComponent<T> &meshComponent, VIRenderModel& renderModel);

    protected:
        void create();
        VRenderModel& createRenderModel(const uint32_t& vertexCount);
        VIRenderModel& createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount);
        void release();

        template<typename T>
        bool validate(VRenderModel& renderModel, VertexDataComponent<T>& vertexDataComponent);
        template<typename T>
        bool validate(VIRenderModel& renderModel, BaseMeshComponent<T>& meshComponent);

    protected:
        std::vector<VRenderModel> vRenderModels;
        std::vector<VIRenderModel> viRenderModels;
        Ref<BaseShaderProgram> shaderProgram;
        DrawType drawType;
    };

    template<typename T1, typename T2>
    void Renderer::render(const Entity& entity) {
//        renderV<T1>(entity);
        renderVI<T2>(entity);
    }

    template<typename T1, typename T2>
    void Renderer::renderInstanced(entt::registry &registry) {
//        renderInstancedV<T1>(registry);
        renderInstancedVI<T2>(registry);
    }

    template<typename T1, typename T2>
    void Renderer::renderBatched(entt::registry &registry) {
//        renderBatchedV<T1>(registry);
        renderBatchedVI<T2>(registry);
    }

    template<typename T>
    void Renderer::renderInstancedV(entt::registry& registry) {
        if (!shaderProgram->isReady()) return;

        shaderProgram->start();
        shaderProgram->update(registry);

        auto entities = registry.group<Transform3dComponent, VertexDataComponent<T>>();
        uint32_t totalVertexCount = 0;
        uint32_t i = 0;
        uint32_t renderModelId = 0;
        bool renderModelReady = false;
        for (auto [entity, transform, vertexDataComponent] : entities.each()) {
            if (drawType != vertexDataComponent.drawType) {
                i++; // shift instanceId
                continue;
            }

            if (!renderModelReady) {
                auto& oldRenderModel = vRenderModels[vertexDataComponent.renderModelId];
                validate(oldRenderModel, vertexDataComponent);
                renderModelId = vertexDataComponent.renderModelId;

                auto& renderModel = vRenderModels[vertexDataComponent.renderModelId];
                tryUpload(vertexDataComponent, totalVertexCount, renderModel);

                renderModelReady = true;
            }

            shaderProgram->getVShader().setUniformArrayElement(i++, transform);
            // if transform count is out of limit, then draw current instances and repeat iteration!
            auto& renderModel = vRenderModels[renderModelId];
            if (i > INSTANCE_COUNT_LIMIT) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount, i);
                i = 0;
            }
        }

        auto& renderModel = viRenderModels[renderModelId];
        if (i > 0 && totalVertexCount > 0) {
            renderModel.vao.bind();
            drawV(drawType, totalVertexCount, i);
        }

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer::renderBatchedV(entt::registry &registry) {
        if (!shaderProgram->isReady()) return;

        shaderProgram->start();
        shaderProgram->update(registry);

        auto entities = registry.group<Transform3dComponent, VertexDataComponent<InstanceVertex<T>>>();

        uint32_t nextRenderModelId = 0;
        for (auto [entity, transform, vertexDataComponent] : entities.each()) {
            vertexDataComponent.renderModelId += nextRenderModelId;
            auto& renderModel = vRenderModels[vertexDataComponent.renderModelId];
            if (!validate<InstanceVertex<T>>(renderModel, vertexDataComponent)) {
                nextRenderModelId++;
            }
        }

        for (auto& renderModel : vRenderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t i = 0;
            for (auto [entity, transform, vertexDataComponent] : entities.each()) {
                if (renderModel.id != vertexDataComponent.renderModelId || drawType != vertexDataComponent.drawType) {
                    i++; // shift instance id
                    continue;
                }

                tryUploadInstance<>(i, vertexDataComponent, totalVertexCount, renderModel);
                shaderProgram->getVShader().setUniformArrayElement(i++, transform);
                if (i > INSTANCE_COUNT_LIMIT) {
                    renderModel.vao.bind();
                    drawV(drawType, totalVertexCount);
                    i = 0;
                    totalVertexCount = 0;
                }
            }

            if (i > 0 && totalVertexCount > 0) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount);
            }
        }

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer::renderV(const Entity &entity) {
        auto vertexDataComponent = entity.getPtr<VertexDataComponent<T>>();
        if (!shaderProgram->isReady() || !vertexDataComponent || drawType != vertexDataComponent->drawType) return;

        auto& oldRenderModel = vRenderModels[vertexDataComponent->renderModelId];
        validate(oldRenderModel, *vertexDataComponent);
        auto& renderModel = vRenderModels[vertexDataComponent->renderModelId];

        shaderProgram->start();
        shaderProgram->update(entity);

        uint32_t totalVertexCount = 0;
        tryUpload(*vertexDataComponent, totalVertexCount, renderModel);
        // try to upload transform
        auto transform = entity.getPtr<Transform3dComponent>();
        if (transform) {
            shaderProgram->getVShader().setUniform(*transform);
        }

        renderModel.vao.bind();
        drawV(drawType, totalVertexCount);

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer::renderInstancedVI(entt::registry& registry) {
        if (!shaderProgram->isReady()) return;

        shaderProgram->start();
        shaderProgram->update(registry);

        auto entities = registry.group<Transform3dComponent, BaseMeshComponent<T>>();
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        uint32_t i = 0;
        uint32_t renderModelId = 0;
        bool renderModelReady = false;
        for (auto [entity, transform, mesh] : entities.each()) {
            if (drawType != mesh.drawType) {
                i++; // next instanceId
                continue;
            }

            if (!renderModelReady) {
                auto& oldRenderModel = viRenderModels[mesh.renderModelId];
                resetCounts(oldRenderModel);
                validate(oldRenderModel, mesh);
                renderModelId = mesh.renderModelId;

                auto& renderModel = viRenderModels[mesh.renderModelId];
                tryUpload(mesh, totalVertexCount, totalIndexCount, renderModel);

                renderModelReady = true;
            }

            shaderProgram->getVShader().setUniformArrayElement(i++, transform);
            // if transform count is out of limit, then draw current instances and repeat iteration!
            auto& renderModel = viRenderModels[renderModelId];
            if (i > INSTANCE_COUNT_LIMIT) {
                renderModel.vao.bind();
                drawVI(drawType, totalIndexCount, i);
                i = 0;
            }
        }

        auto& renderModel = viRenderModels[renderModelId];
        if (i > 0 && totalIndexCount > 0) {
            renderModel.vao.bind();
            drawVI(drawType, totalIndexCount, i);
        }

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer::renderBatchedVI(entt::registry &registry) {
        if (!shaderProgram->isReady()) return;

        shaderProgram->start();
        shaderProgram->update(registry);

        auto entities = registry.group<Transform3dComponent, BaseMeshComponent<InstanceVertex<T>>>();

        uint32_t nextRenderModelId = 0;
        for (auto [entity, transform, mesh] : entities.each()) {
            mesh.renderModelId += nextRenderModelId;
            auto& renderModel = viRenderModels[mesh.renderModelId];
            if (!validate<InstanceVertex<T>>(renderModel, mesh)) {
                nextRenderModelId++;
            }
        }

        for (auto& renderModel : viRenderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t totalIndexCount = 0;
            uint32_t i = 0;
            for (auto [entity, transform, mesh] : entities.each()) {
                if (renderModel.id != mesh.renderModelId || drawType != mesh.drawType) {
                    i++; // shift instance id
                    continue;
                }

                tryUploadMeshInstance(i, mesh, totalVertexCount, totalIndexCount, renderModel);
                shaderProgram->getVShader().setUniformArrayElement(i++, transform);
                if (i > INSTANCE_COUNT_LIMIT) {
                    renderModel.vao.bind();
                    drawVI(drawType, totalIndexCount);
                    i = 0;
                    totalVertexCount = 0;
                    totalIndexCount = 0;
                }
            }

            if (i > 0 && totalIndexCount > 0) {
                renderModel.vao.bind();
                drawVI(drawType, totalIndexCount);
            }
            resetCounts(renderModel);
        }

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer::renderVI(const Entity &entity) {
        auto mesh = entity.getPtr<BaseMeshComponent<T>>();
        if (!shaderProgram->isReady() || !mesh || drawType != mesh->drawType) return;

        auto& oldRenderModel = viRenderModels[mesh->renderModelId];
        resetCounts(oldRenderModel);
        validate(oldRenderModel, *mesh);
        auto& renderModel = viRenderModels[mesh->renderModelId];

        shaderProgram->start();
        shaderProgram->update(entity);

        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        tryUpload(*mesh, totalVertexCount, totalIndexCount, renderModel);
        // try to upload transform
        auto transform = entity.getPtr<Transform3dComponent>();
        if (transform) {
            shaderProgram->getVShader().setUniform(*transform);
        }

        renderModel.vao.bind();
        drawVI(drawType, totalIndexCount);

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer::tryUploadInstance(
            const uint32_t &instanceId,
            VertexDataComponent<InstanceVertex<T>> &vertexDataComponent,
            uint32_t &previousVertexCount,
            VRenderModel& renderModel
    ) {
        if (vertexDataComponent.isUpdated) {
            setInstanceId(vertexDataComponent, instanceId);
        }
        tryUpload<InstanceVertex<T>>(vertexDataComponent, previousVertexCount, renderModel);
    }

    template<typename T>
    void Renderer::tryUpload(
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
    void Renderer::upload(VertexDataComponent<T> &vertexDataComponent, VRenderModel& renderModel) {
        renderModel.vao.bind();
        renderModel.vbo.bind();
        renderModel.vbo.load(vertexDataComponent.vertexData);
    }

    template<typename T>
    void Renderer::tryUploadMeshInstance(
            const uint32_t &instanceId,
            BaseMeshComponent<InstanceVertex<T>> &meshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount,
            VIRenderModel& renderModel
    ) {
        if (meshComponent.isUpdated) {
            setInstanceId(meshComponent, instanceId);
        }
        tryUpload<InstanceVertex<T>>(meshComponent, previousVertexCount, previousIndexCount, renderModel);
    }

    template<typename T>
    void Renderer::tryUpload(
            BaseMeshComponent<T> &meshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount,
            VIRenderModel& renderModel
    ) {
        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            updateStart(meshComponent, previousVertexCount, previousIndexCount);
            updateCounts(meshComponent);
            upload(meshComponent, renderModel);
        }
        previousIndexCount += meshComponent.totalIndexCount;
        previousVertexCount += meshComponent.totalVertexCount;
    }

    template<typename T>
    void Renderer::upload(BaseMeshComponent<T> &meshComponent, VIRenderModel& renderModel) {
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
    bool Renderer::validate(VRenderModel &renderModel, VertexDataComponent<T>& vertexDataComponent) {
        if (!hasCapacity(renderModel, vertexDataComponent)) {
            auto vertexCount = vertexDataComponent.vertexData.vertexCount > DEFAULT_VERTEX_COUNT
                    ? vertexDataComponent.vertexData.vertexCount * 3 : DEFAULT_VERTEX_COUNT;
            auto& newRenderModel = createRenderModel(vertexCount);
            vertexDataComponent.renderModelId = newRenderModel.id;
            increaseCounts(newRenderModel, vertexDataComponent);
            return false;
        }
        increaseCounts(renderModel, vertexDataComponent);
        return true;
    }

    template<typename T>
    bool Renderer::validate(VIRenderModel &renderModel, BaseMeshComponent<T>& meshComponent) {
        if ((meshComponent.totalIndexCount == 0 || meshComponent.totalVertexCount == 0) && meshComponent.meshCount > 0) {
            updateCounts(meshComponent);
        }
        if (!hasCapacity(renderModel, meshComponent)) {
            auto vertexCount = meshComponent.totalVertexCount > DEFAULT_VERTEX_COUNT ? meshComponent.totalVertexCount * 3 : DEFAULT_VERTEX_COUNT;
            auto indexCount = meshComponent.totalIndexCount > DEFAULT_INDEX_COUNT ? meshComponent.totalIndexCount * 3 : DEFAULT_INDEX_COUNT;
            auto& newRenderModel = createRenderModel(vertexCount, indexCount);
            meshComponent.renderModelId = newRenderModel.id;
            increaseCounts(newRenderModel, meshComponent);
            return false;
        }
        increaseCounts(renderModel, meshComponent);
        return true;
    }

}
