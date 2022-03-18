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

    template<typename T>
    class Renderer {

    public:
        Renderer(const Ref<BaseShaderProgram>& shaderProgram) : shaderProgram(shaderProgram) {
            create();
        }

        ~Renderer() {
            release();
        }

    public:
        void render(const Entity& entity);
        void renderInstanced(entt::registry &registry);
        void renderBatched(entt::registry &registry);

    public:
        void tryUploadMesh(
                BaseMeshComponent<T>& meshComponent,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount,
                RenderModel& renderModel
        );
        void tryUploadMesh(
                const uint32_t &instanceId,
                MeshComponent& meshComponent,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount,
                RenderModel& renderModel
        );
        void uploadMesh(BaseMeshComponent<T> &meshComponent, RenderModel& renderModel);

    protected:
        void create();
        RenderModel& createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount);
        void release();
        bool validate(RenderModel& renderModel, BaseMeshComponent<T>& meshComponent);

    protected:
        std::vector<RenderModel> renderModels;
        Ref<BaseShaderProgram> shaderProgram;
    };

    template<typename T>
    void Renderer<T>::create() {
        shaderProgram->bindVertexFormat();
        createRenderModel(DEFAULT_VERTEX_COUNT, DEFAULT_INDEX_COUNT);
    }

    template<typename T>
    void Renderer<T>::renderInstanced(entt::registry& registry) {
        if (!shaderProgram->isReady()) return;

        shaderProgram->start();
        shaderProgram->update(registry);

        auto entities = registry.group<Transform3dComponent, MeshComponent>();
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        uint32_t i = 0;
        uint32_t renderModelId = 0;
        bool renderModelReady = false;
        for (auto [entity, transform, mesh] : entities.each()) {
            if (!renderModelReady) {
                auto& oldRenderModel = renderModels[mesh.renderModelId];
                resetCounts(oldRenderModel);
                validate(oldRenderModel, mesh);
                renderModelId = mesh.renderModelId;

                auto& renderModel = renderModels[mesh.renderModelId];
                tryUploadMesh(mesh, totalVertexCount, totalIndexCount, renderModel);

                renderModelReady = true;
            }

            shaderProgram->getVShader().setUniformArrayElement(i++, transform);
            // if transform count is out of limit, then draw current instances and repeat iteration!
            auto& renderModel = renderModels[renderModelId];
            if (i > INSTANCE_COUNT_LIMIT) {
                renderModel.vao.bind();
                drawTriangles(totalIndexCount, i);
                i = 0;
            }
        }

        auto& renderModel = renderModels[renderModelId];
        if (i > 0 && totalIndexCount > 0) {
            renderModel.vao.bind();
            drawTriangles(totalIndexCount, i);
        }

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer<T>::renderBatched(entt::registry &registry) {
        if (!shaderProgram->isReady()) return;

        shaderProgram->start();
        shaderProgram->update(registry);

        auto entities = registry.group<Transform3dComponent, MeshComponent>();

        uint32_t nextRenderModelId = 0;
        for (auto [entity, transform, mesh] : entities.each()) {
            mesh.renderModelId += nextRenderModelId;
            auto& renderModel = renderModels[mesh.renderModelId];
            if (!validate(renderModel, mesh)) {
                nextRenderModelId++;
            }
        }

        for (auto& renderModel : renderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t totalIndexCount = 0;
            uint32_t i = 0;
            for (auto [entity, transform, mesh] : entities.each()) {
                if (renderModel.id != mesh.renderModelId) {
                    i++; // shift instance id
                    continue;
                }

                tryUploadMesh(i, mesh, totalVertexCount, totalIndexCount, renderModel);
                shaderProgram->getVShader().setUniformArrayElement(i++, transform);
                if (i > INSTANCE_COUNT_LIMIT) {
                    renderModel.vao.bind();
                    drawTriangles(totalIndexCount);
                    i = 0;
                    totalVertexCount = 0;
                    totalIndexCount = 0;
                }
            }

            if (i > 0 && totalIndexCount > 0) {
                renderModel.vao.bind();
                drawTriangles(totalIndexCount);
            }
            resetCounts(renderModel);
        }

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer<T>::render(const Entity &entity) {
        auto mesh = entity.getPtr<BaseMeshComponent<T>>();
        if (!shaderProgram->isReady() || !mesh) return;

        auto& oldRenderModel = renderModels[mesh->renderModelId];
        resetCounts(oldRenderModel);
        validate(oldRenderModel, *mesh);
        auto& renderModel = renderModels[mesh->renderModelId];

        shaderProgram->start();
        shaderProgram->update(entity);

        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        tryUploadMesh(*mesh, totalVertexCount, totalIndexCount, renderModel);
        // try to upload transform
        auto transform = entity.getPtr<Transform3dComponent>();
        if (transform) {
            shaderProgram->getVShader().setUniform(*transform);
        }

        renderModel.vao.bind();
        drawTriangles(totalIndexCount);

        shaderProgram->stop();
    }

    template<typename T>
    void Renderer<T>::uploadMesh(BaseMeshComponent<T> &meshComponent, RenderModel& renderModel) {
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
    void Renderer<T>::tryUploadMesh(
            BaseMeshComponent<T> &meshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount,
            RenderModel& renderModel
    ) {
        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            updateStart(meshComponent, previousVertexCount, previousIndexCount);
            updateCounts(meshComponent);
            uploadMesh(meshComponent, renderModel);
        }

        previousIndexCount += meshComponent.totalIndexCount;
        previousVertexCount += meshComponent.totalVertexCount;
    }

    template<typename T>
    void Renderer<T>::tryUploadMesh(
            const uint32_t &instanceId,
            MeshComponent &meshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount,
            RenderModel& renderModel
    ) {
        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            updateStart(meshComponent, previousVertexCount, previousIndexCount);
            setInstanceId(meshComponent, instanceId);
            updateCounts(meshComponent);
            uploadMesh(meshComponent, renderModel);
        }

        previousIndexCount += meshComponent.totalIndexCount;
        previousVertexCount += meshComponent.totalVertexCount;
    }

    template<typename T>
    void Renderer<T>::release() {
        for (auto& renderModel : renderModels) {
            engine::release(renderModel);
        }
        renderModels.clear();
        shaderProgram->release();
    }

    template<typename T>
    RenderModel& Renderer<T>::createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount) {
        RenderModel renderModel(renderModels.size(), vertexCount, indexCount);
        renderModel.vao.bind();
        renderModel.vbo.setFormat(shaderProgram->getVertexFormat());
        renderModel.ibo.bind();
        renderModel.ibo.alloc();
        renderModel.vao.unbind();
        renderModels.emplace_back(renderModel);
        return renderModels[renderModel.id];
    }

    template<typename T>
    bool Renderer<T>::validate(RenderModel &renderModel, BaseMeshComponent<T>& meshComponent) {
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