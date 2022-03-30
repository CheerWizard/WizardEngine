//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Renderer.h"
#include <ecs/Scene.h>
#include <platform/graphics/FrameBuffer.h>
#include <graphics/text/TextRenderer.h>

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

        inline void setSkybox(const Entity& skybox) {
            this->skybox = skybox;
        }

    private:
        void create();
        void createSceneRenderer();
        void createLineRenderers();
        void createQuadRenderer();
        void createCircleRenderer();
        void createOutlineRenderer();
        void createSkyboxRenderer();
        void createTextRenderers();

    private:
        Ref<Scene> activeScene = nullptr;
        Entity skybox;

        Ref<FrameBuffer> sceneFrame;
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