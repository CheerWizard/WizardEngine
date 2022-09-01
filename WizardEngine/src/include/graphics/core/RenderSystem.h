//
// Created by mecha on 05.09.2021.
//

#pragma once

#include <ecs/Scene.h>
#include <graphics/core/Screen.h>
#include <graphics/text/TextRenderer.h>
#include <graphics/materials/Material.h>
#include <graphics/materials/Color.h>
#include <graphics/skybox/Skybox.h>
#include <graphics/core/TextureMixer.h>
#include <graphics/post_effects/PostEffects.h>

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

    private:
        RenderSystem() = default;

    public:
        static void onUpdate();
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
        static ScreenRenderer screenRenderer;
        // skybox
        static SkyboxRenderer skyboxRenderer;
        // scene
        static vector<Ref<Renderer>> sceneRenderers;
        // outlining
        static vector<Ref<Renderer>> outlineRenderers;
        // points
//        static VRenderer<PointVertex> pointRenderer;
        // text
        static vector<Ref<Renderer>> textRenderers;
        // post effects
        static vector<PostEffectRenderer> postEffectRenderers;
        // mixer
        static TextureMixer textureMixer;
        // HDR/LDR
        static HdrEffectRenderer hdrEffectRenderer;

    private:
        static RenderSystemCallback* callback;
    };

}