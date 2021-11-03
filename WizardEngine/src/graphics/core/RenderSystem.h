//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../ecs/System.h"
#include "Renderer.h"
#include "frame/FrameController.h"

namespace engine {

    class RenderSystem : public System {

    public:
        RenderSystem(
                const Ref<FrameController> &frameController,
                const Ref<Renderer> &batchRenderer,
                const Ref<Renderer> &instanceRenderer
        ) : sceneFrameController(frameController), batchRenderer(batchRenderer), instanceRenderer(instanceRenderer) {}
        ~RenderSystem() override = default;

    public:
        void onUpdate() override;

    private:
        Ref<FrameController> sceneFrameController;
        Ref<Renderer> batchRenderer;
        Ref<Renderer> instanceRenderer;

    };

}