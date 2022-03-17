//
// Created by mecha on 05.09.2021.
//

#include <graphics/core/RenderSystem.h>

namespace engine {

    void RenderSystem::onUpdate() {
        sceneFrameController->begin();

        if (!activeScene->batchEmpty()) {
            batchRenderer->renderBatched(activeScene->getBatchRegistry());
        }
        if (!activeScene->instanceEmpty()) {
            instanceRenderer->renderInstanced(activeScene->getInstanceRegistry());
        }

        sceneFrameController->end();
    }

}