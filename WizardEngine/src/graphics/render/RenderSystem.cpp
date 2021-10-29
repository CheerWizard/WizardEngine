//
// Created by mecha on 05.09.2021.
//

#include "RenderSystem.h"
#include "vector"

namespace engine {

    void RenderSystem::create(const Ref<GraphicsFactory> &graphicsFactory, const Ref<ShaderSource> &shaderSource) {
        auto batchShader = shaderSource->create(ShaderProps {
            "batch",
            "v_batch",
            "f_batch",
            ENGINE_SHADERS_PATH
        });
        auto instanceShader = shaderSource->create(ShaderProps {
            "instance",
            "v_instance",
            "f_instance",
            ENGINE_SHADERS_PATH
        });

        batchRenderer = createRef<Renderer>(graphicsFactory, batchShader);
        instanceRenderer = createRef<Renderer>(graphicsFactory, instanceShader);
    }

    void RenderSystem::destroy() {
        activeScene = nullptr;
    }

    void RenderSystem::onUpdate() {
        // check if it has at least 1 active scene!
        if (activeScene == nullptr) {
            ENGINE_WARN("RenderSystem : No active scene!");
            return;
        }

        // can't update empty scene!
        if (activeScene->isEmpty()) {
            ENGINE_WARN("RenderSystem : Nothing to render on scene!");
            return;
        }

        // begin frame!
        frameController->begin();

        // render families!
        const auto& families = activeScene->getFamilies();
        for (const Family& family : families) {
            bool instancingAvailable = family.has<MeshComponent>();
            ENGINE_INFO("Family {0} uses instance rendering : {1}", family.get<TagComponent>().tag, instancingAvailable);

            if (instancingAvailable) {
                renderInstancing(family);
            } else if (!family.isEmpty()) {
                renderBatching(family.getEntities());
            } else {
                // family is empty and does not have MeshComponent.
                // we can't draw it, so move to next Family!
                continue;
            }
        }

        // render other entities!
        renderBatching(activeScene->getEntities());

        // end frame!
        frameController->end();
    }

    void RenderSystem::renderBatching(const std::vector<Entity> &entities) {
        batchRenderer->begin();

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
                batchRenderer->renderCamera(entity);
                batchRenderer->renderMaterial(entity);
                batchRenderer->renderPolygonMode(entity);
                // if entity does not have "transform", then it should not increment instance count!
                if (batchRenderer->renderTransform(instanceId, entity)) {
                    batchRenderer->renderMesh(instanceId, entity, totalVertexCount, totalIndexCount);
                    instanceId++;
                }
            }

            batchRenderer->drawElements(totalIndexCount);

            entitiesCount -= INSTANCE_COUNT_LIMIT;
            if (entitiesCount < INSTANCE_COUNT_LIMIT) {
                limitedEntitiesCount = entitiesCount;
            }
            entityCounter += limitedEntitiesCount;
        }

        batchRenderer->end();
    }

    void RenderSystem::renderInstancing(const Family &family) {
        instanceRenderer->begin();
        // render family mesh!
        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        instanceRenderer->renderMesh(family, totalVertexCount, totalIndexCount);

        const auto& entities = family.getEntities();

        int instanceCount = entities.size();
        int limitedInstanceCount = instanceCount;
        if (limitedInstanceCount > INSTANCE_COUNT_LIMIT) {
            limitedInstanceCount = INSTANCE_COUNT_LIMIT;
        }

        uint32_t entityCounter = 0;
        while (limitedInstanceCount > 0) {
            uint32_t instanceId = 0;
            for (uint32_t i = entityCounter ; i < limitedInstanceCount ; i++) {
                const auto& entity = entities[i];
                instanceRenderer->renderCamera(entity);
                instanceRenderer->renderMaterial(entity);
                instanceRenderer->renderPolygonMode(entity);
                // if entity does not have "transform", then it should not increment instance count!
                if (instanceRenderer->renderTransform(instanceId, entity)) {
                    instanceId++;
                }
            }

            instanceRenderer->drawElements(totalIndexCount, limitedInstanceCount);

            instanceCount -= INSTANCE_COUNT_LIMIT;
            if (instanceCount < INSTANCE_COUNT_LIMIT) {
                limitedInstanceCount = instanceCount;
            }
            entityCounter += limitedInstanceCount;
        }

        instanceRenderer->end();
    }

}