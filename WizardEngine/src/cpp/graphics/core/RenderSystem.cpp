//
// Created by mecha on 05.09.2021.
//

#include <graphics/core/RenderSystem.h>

namespace engine::graphics {
    // frames
    Ref<Scene> RenderSystem::activeScene;
    Ref<FrameBuffer> RenderSystem::sceneFrame;
    Ref<FrameBuffer> RenderSystem::screenFrame;
    // screen
    ScreenRenderer RenderSystem::screenRenderer;
    // skybox
    SkyboxRenderer RenderSystem::skyboxRenderer;
    // scene
    vector<Ref<Renderer>> RenderSystem::sceneRenderers;
    // outlining
    vector<Ref<Renderer>> RenderSystem::outlineRenderers;
    // points
//    VRenderer<PointVertex> RenderSystem::pointRenderer;
    // text
    vector<Ref<Renderer>> RenderSystem::textRenderers;
    // post effects
    vector<PostEffectRenderer> RenderSystem::postEffectRenderers;
    // callback
    RenderSystemCallback* RenderSystem::callback;
    // mixer
    TextureMixer RenderSystem::textureMixer;
    // HDR/LDR
    HdrEffectRenderer RenderSystem::hdrEffectRenderer;

    void RenderSystem::onDestroy() {
        screenRenderer.release();
        skyboxRenderer.release();
//        pointRenderer.release();
        for (const auto& sceneRenderer : sceneRenderers) {
            sceneRenderer->release();
        }
        for (const auto& outlineRenderer : outlineRenderers) {
            outlineRenderer->release();
        }
        for (const auto& textRenderer : textRenderers) {
            textRenderer->release();
        }
        for (auto& postEffectRenderer : postEffectRenderers) {
            postEffectRenderer.release();
        }
        textureMixer.release();
    }

    void RenderSystem::onUpdate() {
        sceneFrame->bind();

        clearDepthBuffer();
        setDepthTest(true);

        // notify that scene frame begin drawing
        if (callback != nullptr) {
            callback->onFrameBegin(sceneFrame);
        }

        // enables transparency
        setBlendMode(true);
        setBlendFunction(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
        // write to stencil buffer
        setStencilTest(true);
        setStencilTestActions({ KEEP, KEEP, REPLACE });
        clearStencilBuffer();
        setStencilTestOperator(ALWAYS, 1, false);
        stencilMask(false);

        auto& registry = activeScene->getRegistry();
        // scene
        for (const auto& renderer : sceneRenderers) {
            renderer->render(registry);
        }
        // text
        for (const auto& textRenderer : textRenderers) {
            textRenderer->render(registry);
        }
        // points
//        registry.each<Points>([](Points* points) {
//            pointRenderer.render(*points);
//        });
        // outlining
        // stop write to stencil buffer
        setStencilTestOperator(NOT_EQUAL, 1, false);
        stencilMask(true);
        setDepthTest(false);
        for (const auto& outlineRenderer : outlineRenderers) {
            outlineRenderer->render(registry);
        }
        // write to stencil buffer
        stencilMask(false);
        setStencilTestOperator(ALWAYS, 0, false);
        setDepthTest(true);
        // skybox
        setDepthTestOperator(LESS_EQUAL); // we need to pass depth test for some skybox pixels
        skyboxRenderer.renderStatic(activeScene->getSkybox());
        setDepthTestOperator(LESS);
        // notify that scene frame end drawing
        if (callback != nullptr) {
            callback->onFrameEnd(sceneFrame);
        }
        // read/write from scene frame into screen frame
//        FrameBuffer::readWriteFrameBuffers(*sceneFrame.get(), *screenFrame.get());

        // post-processing effects
        vector<u32> postProcessedTextures;
        // render target 1 without post effects
        ColorAttachment sceneTexture1;
        sceneFrame->getColorAttachment(0, sceneTexture1);
        postProcessedTextures.emplace_back(sceneTexture1.id);
        // render target 2 has pos effects
        ColorAttachment sceneTexture2;
        sceneFrame->getColorAttachment(1, sceneTexture2);
        u32 postProcessedTexture = sceneTexture2.id;
        for (PostEffectRenderer& postEffectRenderer : postEffectRenderers) {
            postProcessedTexture = postEffectRenderer.render(postProcessedTexture);
        }
        postProcessedTextures.emplace_back(postProcessedTexture);

        // mixing all textures
        u32 mixedTexture = textureMixer.render(postProcessedTextures);

        // apply HDR/LDR to final texture
        u32 finalTexture = hdrEffectRenderer.render(mixedTexture);

        // bind to window default frame buffer and draw screen
        setDepthTest(false);
        FrameBuffer::bindDefault();
        screenRenderer.renderQuad(finalTexture);
    }

    void RenderSystem::setRenderSystemCallback(RenderSystemCallback* renderSystemCallback) {
        callback = renderSystemCallback;
    }

    void RenderSystem::removeRenderSystemCallback() {
        callback = nullptr;
    }
}
