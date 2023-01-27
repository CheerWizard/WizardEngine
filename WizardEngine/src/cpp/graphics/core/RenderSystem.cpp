//
// Created by mecha on 05.09.2021.
//

#include <graphics/core/RenderSystem.h>
#include <profiler/Profiler.h>

namespace engine::graphics {

    u32 RenderSystem::finalRenderTargetId = 0;
    // frames
    Ref<Scene> RenderSystem::activeScene;
    Ref<FrameBuffer> RenderSystem::sceneFrame;
    Ref<FrameBuffer> RenderSystem::msaaFrame;
    Ref<FrameBuffer> RenderSystem::shadowsFrame;
    // screen
    ScreenRenderer RenderSystem::screenRenderer;
    // skybox
    SkyboxRenderer RenderSystem::skyboxRenderer;
    // HDR env
    HdrEnvRenderer RenderSystem::hdrEnvRenderer;
    // scene
    Ref<Renderer> RenderSystem::batchRenderer;
    Ref<Renderer> RenderSystem::instanceRenderer;
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
    // Gaussian Blur
    GaussianBlurEffectRenderer RenderSystem::gaussianBlurRenderer;

    void RenderSystem::onDestroy() {
        screenRenderer.release();
        skyboxRenderer.release();
        hdrEnvRenderer.release();
        batchRenderer->release();
        instanceRenderer->release();
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
        PROFILE_FUNCTION();

//        shadowsFrame->setViewPort();
//        shadowsFrame->bind();
//        clearBuffer(BufferBit::DEPTH);

        if (msaaFrame->getFormat().samples > 1) {
            msaaFrame->bind();
        } else {
            sceneFrame->bind();
        }

        setClearColor(0.5, 0.5, 0.5, 1);
        clearBuffer(BufferBit::COLOR | BufferBit::DEPTH);
        enableDepthTest();

        // notify that scene frame begin drawing
        if (callback != nullptr) {
            callback->onFrameBegin(sceneFrame);
        }

        // enables transparency
        enableBlendMode();
        setBlendFunction(BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA);
        // write to stencil buffer
        enableStencilTest();
        setStencilTestActions({
            TestAction::KEEP,
            TestAction::KEEP,
            TestAction::REPLACE
        });
        clearBuffer(BufferBit::COLOR | BufferBit::DEPTH | BufferBit::STENCIL);
        setStencilTestOperator(TestOperator::ALWAYS, 1, false);
        setStencilMask(0xFF);

        auto& registry = activeScene->getRegistry();
        // scene
        batchRenderer->render(registry);
        instanceRenderer->render(registry);
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
        setStencilTestOperator(TestOperator::NOT_EQUAL, 1, false);
        stencilMask(true);
        disableDepthTest();
        for (const auto& outlineRenderer : outlineRenderers) {
            outlineRenderer->render(registry);
        }
        // write to stencil buffer
        setStencilMask(0xFF);
        setStencilTestOperator(TestOperator::ALWAYS, 0, false);
        enableDepthTest();
        // skybox and HDR env
        setDepthTestOperator(TestOperator::LESS_EQUAL); // we need to pass depth test for some skybox pixels
        // todo HDR env not working yet
//        hdrEnvRenderer.render(activeScene->getHdrEnv(), activeScene->getCamera());
        skyboxRenderer.render(activeScene->getSkybox(), activeScene->getCamera());
        setDepthTestOperator(TestOperator::LESS);
        // notify that scene frame end drawing
        if (callback != nullptr) {
            callback->onFrameEnd(sceneFrame);
        }
        if (msaaFrame->getFormat().samples > 1) {
            // read/write from MSAA frame into scene frame
            FrameBuffer::readWriteFrameBuffers(*msaaFrame.get(), *sceneFrame.get());
        }
        // post-processing effects
        vector<u32> postProcessedTextures;
        // render target 1 without post effects
        ColorAttachment sceneTexture1;
        sceneFrame->getColorAttachment(0, sceneTexture1);
        postProcessedTextures.emplace_back(sceneTexture1.id);
        // render target 2 with post effects
        ColorAttachment sceneTexture2;
        sceneFrame->getColorAttachment(1, sceneTexture2);
        u32 postProcessedTexture = sceneTexture2.id;
        for (PostEffectRenderer& postEffectRenderer : postEffectRenderers) {
            postProcessedTexture = postEffectRenderer.render(postProcessedTexture);
        }
        // Gaussian Blur effect for render target 2
        postProcessedTexture = gaussianBlurRenderer.render(postProcessedTexture);
        postProcessedTextures.emplace_back(postProcessedTexture);

        // mixing all textures
        u32 mixedTexture = textureMixer.render(postProcessedTextures);

        // apply HDR/LDR to final texture
        finalRenderTargetId = hdrEffectRenderer.render(mixedTexture);

        // bind to window default frame buffer and draw screen
        disableDepthTest();
        FrameBuffer::bindDefault();
#ifdef VISUAL // render into ImGui viewport
        setClearColor(0.5, 0.5, 0.5, 1);
        clearBuffer(BufferBit::COLOR | BufferBit::DEPTH);
#else // render into default viewport
        screenRenderer.renderQuad(finalRenderTargetId);
#endif
    }

    void RenderSystem::setRenderSystemCallback(RenderSystemCallback* renderSystemCallback) {
        callback = renderSystemCallback;
    }

    void RenderSystem::removeRenderSystemCallback() {
        callback = nullptr;
    }
}
