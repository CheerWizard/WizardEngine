//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Renderer.h"
#include <ecs/Scene.h>
#include <platform/graphics/FrameBuffer.h>

namespace engine {

    class RenderSystem {

    public:
        RenderSystem(const Ref<FrameBuffer> &sceneFrame) : sceneFrame(sceneFrame) {
            create();
        }

        ~RenderSystem() = default;

    public:
        void onUpdate();

    public:
        inline void setActiveScene(const Ref<Scene> &activeScene) {
            this->activeScene = activeScene;
        }

    private:
        void create();
        void createSceneRenderer();
        void createLineRenderers();
        void createQuadRenderer();
        void createCircleRenderer();

    private:
        Ref<Scene> activeScene = nullptr;
        Ref<FrameBuffer> sceneFrame;
        // scene renderer
        Ref<MultiRenderer> sceneRenderer;
        // line renderers
        Ref<MultiRenderer> lineRenderer;
        Ref<MultiRenderer> stripLineRenderer;
        Ref<MultiRenderer> loopLineRenderer;
        // quad renderer
        Ref<MultiRenderer> quadRenderer;
        // circle renderer
        Ref<MultiRenderer> circleRenderer;
    };

}