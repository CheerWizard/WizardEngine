//
// Created by mecha on 05.09.2021.
//

#include <graphics/core/RenderSystem.h>

namespace engine {

    void RenderSystem::onUpdate() {
        sceneFrameController->bind();

        setClearColor({0.2, 0.2, 0.2, 1});
        setDepthTest(true);
        clearDepthBuffer();

//        setStencilTest(true);
//        setStencilTestActions({ KEEP, KEEP, REPLACE });
//        clearStencilBuffer();

//        setStencilTestOperator(ALWAYS, 1, false);
//        stencilMask(false);

        if (!activeScene->batchEmpty()) {
            batchRenderer->renderBatched(activeScene->getBatchRegistry());
        }
        if (!activeScene->instanceEmpty()) {
            instanceRenderer->renderInstanced(activeScene->getInstanceRegistry());
        }

        sceneFrameController->unbind();

//        setStencilTestOperator(NOT_EQUAL, 1, false);
//        stencilMask(true);

        setDepthTest(false);
        clearColorBuffer();
    }
}
