//
// Created by mecha on 05.09.2021.
//

#pragma once

#include <graphics/core/Renderer.h>
#include <ecs/Scene.h>
#include <platform/graphics/FrameBuffer.h>
#include <graphics/text/TextRenderer.h>
#include <graphics/core/geometry/Screen.h>

namespace engine::graphics {

    class RenderSystem {

    public:
        RenderSystem(const core::Ref<FrameBuffer> &sceneFrame, const core::Ref<FrameBuffer>& screenFrame)
        : sceneFrame(sceneFrame), screenFrame(screenFrame) {
            create();
        }

        ~RenderSystem();

    public:
        void onPrepare();
        void onUpdate();
        void uploadSkybox();

    public:
        inline void setActiveScene(const core::Ref<ecs::Scene> &activeScene) {
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
        void createPointRenderer();

    private:
        // we must share this data with front-end, so that's why they are pointers
        core::Ref<ecs::Scene> activeScene = nullptr;
        core::Ref<FrameBuffer> sceneFrame;
        core::Ref<FrameBuffer> screenFrame;
        // these data are mostly for back-end, they are mostly just u32 numbers
        // screen
        VRenderer screenRenderer;
        // scene
        DefaultRenderer sceneRenderer;
        // line
        DefaultRenderer lineRenderer;
        DefaultRenderer stripLineRenderer;
        DefaultRenderer loopLineRenderer;
        // quad
        DefaultRenderer quadRenderer;
        // circle
        DefaultRenderer circleRenderer;
        // outlining everything
        // scene
        DefaultRenderer outlineSceneRenderer;
        // quad
        DefaultRenderer outlineQuadRenderer;
        // skybox
        VRenderer skyboxRenderer;
        // text
        TextRenderer text2dRenderer;
        TextRenderer text3dRenderer;
        // points renderer
        VRenderer pointRenderer;
    };

}