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
#include <graphics/hdr_env/hdr_env.h>

namespace engine::graphics {

    using namespace core;
    using namespace ecs;

    class ENGINE_API RenderSystemCallback {
    public:
        virtual ~RenderSystemCallback() = default;
    public:
        virtual void onFrameBegin(const Ref<FrameBuffer>& frameBuffer) = 0;
        virtual void onFrameEnd(const Ref<FrameBuffer>& frameBuffer) = 0;
    };

    class ENGINE_API RenderSystem final {

    private:
        RenderSystem() = default;

    public:
        static void onUpdate();
        static void onDestroy();

    public:
        static void setRenderSystemCallback(RenderSystemCallback* renderSystemCallback);
        static void removeRenderSystemCallback();

    public:
        // final render target id
        static u32 finalRenderTargetId;
        // frames
        static Ref<Scene> activeScene;
        static Ref<FrameBuffer> sceneFrame;
        // screen
        static ScreenRenderer screenRenderer;
        static bool enableScreenRenderer;
        // skybox
        static SkyboxRenderer skyboxRenderer;
        // HDR env
        static HdrEnvRenderer hdrEnvRenderer;
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
        // gaussian blur effect
        static GaussianBlurEffectRenderer gaussianBlurRenderer;
        // mixer
        static TextureMixer textureMixer;
        // HDR/LDR
        static HdrEffectRenderer hdrEffectRenderer;

    private:
        static RenderSystemCallback* callback;
    };
}