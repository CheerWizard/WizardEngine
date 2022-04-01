//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Renderer.h"
#include <ecs/Scene.h>
#include <platform/graphics/FrameBuffer.h>
#include <graphics/text/TextRenderer.h>
#include <graphics/core/geometry/Screen.h>

namespace engine {

    class RenderSystem {

    public:
        RenderSystem(const Ref<FrameBuffer> &sceneFrame, const Ref<FrameBuffer>& screenFrame)
        : sceneFrame(sceneFrame), screenFrame(screenFrame) {
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
        void createScreenRenderer();
        void createSceneRenderer();
        void createLineRenderers();
        void createQuadRenderer();
        void createCircleRenderer();
        void createOutlineRenderer();
        void createSkyboxRenderer();
        void createTextRenderers();

    private:
        Ref<Scene> activeScene = nullptr;

        Ref<FrameBuffer> sceneFrame;
        Ref<FrameBuffer> screenFrame;
        // screen
        Ref<VRenderer> screenRenderer;
        Screen screen;
        // scene
        Ref<MultiRenderer> sceneRenderer;
        // line
        Ref<MultiRenderer> lineRenderer;
        Ref<MultiRenderer> stripLineRenderer;
        Ref<MultiRenderer> loopLineRenderer;
        // quad
        Ref<MultiRenderer> quadRenderer;
        // circle
        Ref<MultiRenderer> circleRenderer;
        // outlining everything
        // scene
        Ref<MultiRenderer> outlineSceneRenderer;
        // quad
        Ref<MultiRenderer> outlineQuadRenderer;
        // skybox
        Ref<VRenderer> skyboxRenderer;
        // text
        Ref<TextRenderer> text2dRenderer;
        Ref<TextRenderer> text3dRenderer;
    };

}