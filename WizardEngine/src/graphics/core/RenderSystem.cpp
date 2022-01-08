//
// Created by mecha on 05.09.2021.
//

#include "RenderSystem.h"

namespace engine {

    void RenderSystem::onUpdate() {
        const auto& standaloneEntities = activeScene->getRegistry().view<MeshComponent>();
        // begin scene pass!
        sceneFrameController->begin();
        // render scene
        // render families!
        for (Family &family : activeScene->getFamilies()) {
            auto familyMesh = family.getPtr<MeshComponent>();
            bool usesInstanceRendering = familyMesh != nullptr;
            ENGINE_INFO("Family {0} uses instance rendering : {1}", family.get<TagComponent>().tag, usesInstanceRendering);

            if (usesInstanceRendering) {
                instanceRenderer->renderInstanced(*familyMesh, family.getRegistry());
            } else if (!family.isEmpty()) {
                batchRenderer->renderBatched(family.getRegistry());
            }
        }
        // render standalone entities!
        batchRenderer->renderBatched(activeScene->getRegistry());
        // end scene pass!
        sceneFrameController->end();
    }
}