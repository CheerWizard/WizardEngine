//
// Created by mecha on 05.09.2021.
//

#include "RenderSystem.h"

namespace engine {

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
        // begin scene pass!
        sceneFrameController->begin();
        // render scene
        // render families!
        for (const Family& family : activeScene->getFamilies()) {
            bool instancingAvailable = family.has<MeshComponent>();
            ENGINE_INFO("Family {0} uses instance rendering : {1}", family.get<TagComponent>().tag, instancingAvailable);

            if (instancingAvailable) {
                instanceRenderer->renderInstanced(family);
            } else if (!family.isEmpty()) {
                batchRenderer->renderBatched(family.getEntities());
            } else {
                return;
            }
        }
        // render other entities!
        batchRenderer->renderBatched(activeScene->getEntities());
        // end scene pass!
        sceneFrameController->end();
    }
}