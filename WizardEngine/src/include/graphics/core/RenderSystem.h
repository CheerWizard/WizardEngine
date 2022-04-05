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

        ~RenderSystem() = default;

    public:
        void onUpdate();

    public:
        inline void setActiveScene(const core::Ref<Scene> &activeScene) {
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
        core::Ref<Scene> activeScene = nullptr;

        core::Ref<FrameBuffer> sceneFrame;
        core::Ref<FrameBuffer> screenFrame;
        // screen
        core::Ref<VRenderer> screenRenderer;
        Screen screen;
        // scene
        core::Ref<MultiRenderer> sceneRenderer;
        // line
        core::Ref<MultiRenderer> lineRenderer;
        core::Ref<MultiRenderer> stripLineRenderer;
        core::Ref<MultiRenderer> loopLineRenderer;
        // quad
        core::Ref<MultiRenderer> quadRenderer;
        // circle
        core::Ref<MultiRenderer> circleRenderer;
        // outlining everything
        // scene
        core::Ref<MultiRenderer> outlineSceneRenderer;
        // quad
        core::Ref<MultiRenderer> outlineQuadRenderer;
        // skybox
        core::Ref<VRenderer> skyboxRenderer;
        // text
        core::Ref<TextRenderer> text2dRenderer;
        core::Ref<TextRenderer> text3dRenderer;
        // points renderer
        core::Ref<VRenderer> pointRenderer;
    };

}