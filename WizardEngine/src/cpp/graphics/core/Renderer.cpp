//
// Created by mecha on 06.10.2021.
//

#include <graphics/core/Renderer.h>
#include <graphics/transform/TransformComponents.h>

namespace engine {

    void Renderer::create() {
        shaderProgram->bindVertexFormat();
        createRenderModel(DEFAULT_VERTEX_COUNT, DEFAULT_INDEX_COUNT);
    }

    void Renderer::renderInstanced(MeshComponent &familyMesh, entt::registry& familyRegistry) {
        if (!shaderProgram->isReady()) return;

        auto& renderModel = renderModels[familyMesh.renderModelId];
        resetCounts(renderModel);
        validate(renderModel, familyMesh);
        renderModel = renderModels[familyMesh.renderModelId];

        shaderProgram->start();
        renderModel.vao.bind();
        shaderProgram->update(familyRegistry);

        auto transforms = familyRegistry.view<Transform3dComponent>();
        // upload family mesh!
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        tryUploadMesh(familyMesh, totalVertexCount, totalIndexCount, renderModel);
        // update each transform
        uint32_t i = 0;
        for (auto [entity, transform] : transforms.each()) {
            shaderProgram->getVShader().setUniformArrayElement(i++, transform);
            // if transform count is out of limit, then draw current instances and repeat iteration!
            if (i > INSTANCE_COUNT_LIMIT) {
                renderModel.vbo.enableAttributes();
                drawTriangles(totalIndexCount, i);
                i = 0;
            }
        }

        // draw rest of instances!
        if (i > 0) {
            renderModel.vbo.enableAttributes();
            drawTriangles(totalIndexCount, i);
        }

        renderModel.vao.unbind();
        shaderProgram->stop();
    }

    void Renderer::renderBatched(entt::registry &registry) {
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
            renderModel.vao.bind();

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
                    renderModel.vbo.enableAttributes();
                    drawTriangles(totalIndexCount);
                    i = 0;
                    totalVertexCount = 0;
                    totalIndexCount = 0;
                }
            }

            if (i > 0 && totalIndexCount > 0) {
                renderModel.vbo.enableAttributes();
                drawTriangles(totalIndexCount);
            }
            resetCounts(renderModel);
        }

        renderModels.front().vao.unbind();
        shaderProgram->stop();
    }

    void Renderer::render(const Entity &entity) {
        auto mesh = entity.getPtr<MeshComponent>();
        if (!shaderProgram->isReady() || !mesh) return;

        auto& renderModel = renderModels[mesh->renderModelId];
        resetCounts(renderModel);
        validate(renderModel, *mesh);
        renderModel = renderModels[mesh->renderModelId];

        shaderProgram->start();
        renderModel.vao.bind();
        shaderProgram->update(entity);

        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        tryUploadMesh(*mesh, totalVertexCount, totalIndexCount, renderModel);
        // try to upload transform
        auto transform = entity.getPtr<Transform3dComponent>();
        if (transform) {
            shaderProgram->getVShader().setUniform(*transform);
        }

        renderModel.vbo.enableAttributes();
        drawTriangles(totalIndexCount);

        renderModel.vao.unbind();
        shaderProgram->stop();
    }

    void Renderer::uploadMesh(MeshComponent &meshComponent, RenderModel& renderModel) {
        auto& vbo = renderModel.vbo;
        auto& ibo = renderModel.ibo;

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

    void Renderer::tryUploadMesh(
            MeshComponent& meshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount,
            RenderModel& renderModel
    ) {
        uint32_t instanceID = 0;
        tryUploadMesh(instanceID, meshComponent, previousVertexCount, previousIndexCount, renderModel);
    }

    void Renderer::tryUploadMesh(
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

    void Renderer::release() {
        for (auto& renderModel : renderModels) {
            engine::release(renderModel);
        }
        renderModels.clear();
        shaderProgram->release();
    }

    RenderModel& Renderer::createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount) {
        RenderModel renderModel(renderModels.size(), vertexCount, indexCount);
        renderModel.vao.bind();
        renderModel.vbo.setFormat(shaderProgram->getVertexFormat());
        renderModel.ibo.bind();
        renderModel.ibo.alloc();
        renderModel.vao.unbind();
        renderModels.emplace_back(renderModel);
        return renderModels[renderModel.id];
    }

    bool Renderer::validate(RenderModel &renderModel, MeshComponent& meshComponent) {
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