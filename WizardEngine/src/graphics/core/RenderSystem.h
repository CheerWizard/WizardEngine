//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Renderer.h"
#include "../../ecs/Scene.h"
#include "frame/FrameController.h"

namespace engine {

    class RenderSystem {

    public:
        RenderSystem(
                const Ref<FrameController> &frameController,
                const Ref<Renderer> &batchRenderer,
                const Ref<Renderer> &instanceRenderer
        ) : sceneFrameController(frameController), batchRenderer(batchRenderer), instanceRenderer(instanceRenderer) {}

        ~RenderSystem() = default;

    public:
        void onUpdate();

    public:
        inline void setActiveScene(const Ref<Scene> &activeScene) {
            this->activeScene = activeScene;
        }

    private:
        Ref<Scene> activeScene = nullptr;
        Ref<FrameController> sceneFrameController;
        Ref<Renderer> batchRenderer;
        Ref<Renderer> instanceRenderer;

    };

}