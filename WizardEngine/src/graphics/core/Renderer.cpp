//
// Created by mecha on 06.10.2021.
//

#include "Renderer.h"

namespace engine {

    void Renderer::create(const Ref<GraphicsFactory> &graphicsFactory) {
        drawer = graphicsFactory->newDrawer();
        settings = graphicsFactory->newRenderSettings();

        vertexArray = graphicsFactory->newVertexArray();
        vertexBuffer = graphicsFactory->newVertexBuffer();
        indexBuffer = graphicsFactory->newIndexBuffer();

        shader->bindAttributes();
        vertexArray->bind();

        vertexBuffer->prepare(shader->getVertexFormat());
        indexBuffer->bind();
        indexBuffer->allocate();

        vertexArray->unbind();
    }

    void Renderer::renderInstanced(const Family &family) {
        begin();
        const auto& entities = family.getEntities();

        // upload family mesh!
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        tryUploadMesh(family, totalVertexCount, totalIndexCount);

        int entitiesCount = entities.size();
        int limitedEntitiesCount = entitiesCount;
        if (limitedEntitiesCount > INSTANCE_COUNT_LIMIT) {
            limitedEntitiesCount = INSTANCE_COUNT_LIMIT;
        }

        uint32_t entityCounter = 0;
        while (limitedEntitiesCount > 0) {
            uint32_t instanceId = 0;
            for (uint32_t i = entityCounter ; i < limitedEntitiesCount ; i++) {
                const auto& entity = entities[i];
                updateShaderControllers(entity, instanceId++);
                tryUpdatePolygonMode(entity);
            }

            drawElements(totalIndexCount, limitedEntitiesCount);

            entitiesCount -= INSTANCE_COUNT_LIMIT;
            if (entitiesCount < INSTANCE_COUNT_LIMIT) {
                limitedEntitiesCount = entitiesCount;
            }
            entityCounter += limitedEntitiesCount;
        }

        end();
    }

    void Renderer::renderBatched(const std::vector<Entity> &entities) {
        begin();

        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;

        int entitiesCount = entities.size();
        int limitedEntitiesCount = entitiesCount;
        if (limitedEntitiesCount > INSTANCE_COUNT_LIMIT) {
            limitedEntitiesCount = INSTANCE_COUNT_LIMIT;
        }

        uint32_t entityCounter = 0;
        while (limitedEntitiesCount > 0) {
            uint32_t instanceId = 0;
            for (uint32_t i = entityCounter ; i < limitedEntitiesCount ; i++) {
                const auto& entity = entities[i];
                updateShaderControllers(entity, instanceId);
                // if entity has mesh - it will increment instanceID! otherwise - no!
                tryUploadMesh(instanceId, entity, totalVertexCount, totalIndexCount);
                tryUpdatePolygonMode(entity);
            }

            drawElements(totalIndexCount);

            entitiesCount -= INSTANCE_COUNT_LIMIT;
            if (entitiesCount < INSTANCE_COUNT_LIMIT) {
                limitedEntitiesCount = entitiesCount;
            }
            entityCounter += limitedEntitiesCount;
        }

        end();
    }

    void Renderer::render(const Entity &entity) {
        begin();

        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        tryUploadMesh(entity, totalVertexCount, totalIndexCount);
        tryUpdatePolygonMode(entity);
        updateShaderControllers(entity);

        drawElements(totalIndexCount);

        end();
    }

    void Renderer::uploadMesh(MeshComponent &meshComponent) {
        vertexBuffer->bind();
        indexBuffer->bind();

        const auto& meshes = meshComponent.meshes;
        for (auto i = 0; i < meshComponent.meshCount ; i++) {
            const auto& vertexData = meshes[i].vertexData;
            vertexBuffer->load(vertexData);
            const auto& indexData = meshes[i].indexData;
            indexBuffer->load(indexData);
        }
    }

    void Renderer::drawElements(const uint32_t &indexCount) {
        vertexBuffer->enableAttributes();
        drawer->drawElements(indexCount);
    }

    void Renderer::drawElements(const uint32_t &indexCount, const uint32_t &instanceCount) {
        vertexBuffer->enableAttributes();
        drawer->drawElements(indexCount, instanceCount);
    }

    void Renderer::tryUploadMesh(
            const Entity& entity,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount
    ) {
        uint32_t instanceID = 0;
        tryUploadMesh(instanceID, entity, previousVertexCount, previousIndexCount);
    }

    void Renderer::tryUploadMesh(
            uint32_t &instanceID,
            const Entity &entity,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount
    ) {
        ENGINE_INFO("renderMesh() - entityId: {0}, entityName: {1}", (uint32_t) entity.getId(), entity.get<TagComponent>().tag);

        if (!entity.has<MeshComponent>()) {
            ENGINE_INFO("No MeshComponent for Entity {0}", entity.get<TagComponent>().tag);
            return;
        }

        auto& meshComponent = entity.get<MeshComponent>();
        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            meshComponent.updateStart(previousVertexCount, previousIndexCount);
            meshComponent.setInstanceId(instanceID);
            meshComponent.updateCounts();
            uploadMesh(meshComponent);
        }

        previousIndexCount += meshComponent.totalIndexCount;
        previousVertexCount += meshComponent.totalVertexCount;
        instanceID++;
    }

    void Renderer::tryUpdatePolygonMode(const Entity &entity) {
        if (entity.has<PolygonMode>()) {
            settings->setPolygonMode(entity.get<PolygonMode>());
        }
    }

    void Renderer::add(const Ref<ShaderController> &shaderController) {
        shaderControllers.push_back(shaderController);
    }

    void Renderer::clear() {
        shaderControllers.clear();
    }

    void Renderer::updateShaderControllers(const Entity &entity) {
        for (const auto& shaderController : shaderControllers) {
            shaderController->updateShader(shader, entity);
        }
    }

    void Renderer::begin() {
        shader->start();
        vertexArray->bind();
    }

    void Renderer::end() {
        vertexArray->unbind();
        shader->stop();
    }

    void Renderer::updateShaderControllers(const Entity &entity, const uint32_t &instanceId) {
        for (const auto& shaderController : shaderControllers) {
            shaderController->updateShader(shader, entity, instanceId);
        }
    }
}