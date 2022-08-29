//
// Created by mecha on 05.09.2021.
//

#pragma once

#include <graphics/core/Renderer.h>
#include <ecs/Scene.h>
#include <platform/graphics/FrameBuffer.h>
#include <graphics/text/TextRenderer.h>
#include <graphics/core/geometry/Screen.h>
#include <graphics/materials/Material.h>
#include <graphics/materials/Color.h>
#include <graphics/skybox/Skybox.h>

namespace engine::graphics {

    using namespace core;
    using namespace ecs;

    class RenderSystemCallback {
    public:
        virtual ~RenderSystemCallback() = default;
    public:
        virtual void onFrameBegin(const Ref<FrameBuffer>& frameBuffer) = 0;
        virtual void onFrameEnd(const Ref<FrameBuffer>& frameBuffer) = 0;
    };

    class RenderSystem final {

    public:
        static void initDefault();
        static void onPrepare();
        static void onUpdate();
        static void uploadSkybox();
        static void onDestroy();

    public:
        static void setRenderSystemCallback(RenderSystemCallback* renderSystemCallback);
        static void removeRenderSystemCallback();

    public:
        // frames
        static Ref<Scene> activeScene;
        static Ref<FrameBuffer> sceneFrame;
        static Ref<FrameBuffer> screenFrame;
        // screen
        static VRenderer<ScreenVertex> screenRenderer;
        // skybox
        static VRenderer<SkyboxVertex> skyboxRenderer;
        // scene
        static vector<Ref<Renderer>> sceneRenderers;
        // outlining
        static vector<Ref<Renderer>> outlineRenderers;
        // points
//        static VRenderer<PointVertex> pointRenderer;
        // text
        static vector<Ref<Renderer>> textRenderers;

    public:
        static void createSceneRenderer();
        static void createScreenRenderer();
        static void createLineRenderer();
        static void createQuadRenderer();
        static void createCircleRenderer();
        static void createOutlineRenderer();
        static void createSkyboxRenderer();
        static void createTextRenderers();
        static void createPointRenderer();

    private:
        static RenderSystemCallback* callback;
    };

}