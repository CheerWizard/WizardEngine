//
// Created by mecha on 24.10.2021.
//

#include "FrameController.h"

namespace engine {

    const std::vector<uint32_t> &FrameController::updateSpecs(const FramebufferSpecification &framebufferSpecification) {
        _frameBuffer.setSpecification(framebufferSpecification);
        _frameBuffer.loadAttachments();
        const auto& colors = _frameBuffer.getColorAttachments();
        ENGINE_INFO("Update FBO specs. Color id: {0}", colors[0]);
        return colors;
    }

    void FrameController::resize(const uint32_t &width, const uint32_t &height) {
        _frameBuffer.resize(width, height);
    }

    void FrameController::begin() {
        _frameBuffer.bind();
        _frameBuffer.enableDepth();
        _frameBuffer.clearDepth({0.2, 0.2, 0.2, 1});
    }

    void FrameController::end() {
        _frameBuffer.unbind();
        _frameBuffer.disableDepth();
        _frameBuffer.clearColor({1.0, 1.0, 1.0, 1.0});
    }

    void FrameController::setViewPort() {
        _frameBuffer.setViewPort();
    }

    void FrameController::resetFrame() {
        begin();
        end();
    }

    const std::vector<uint32_t> &FrameController::updateSpecs(const uint32_t &width, const uint32_t &height) {
        engine::FramebufferSpecification framebufferSpecification;
        framebufferSpecification.attachmentSpecification = {
                engine::FramebufferTextureFormat::RGBA8,
                engine::FramebufferTextureFormat::RED_INTEGER,
                engine::FramebufferTextureFormat::Depth
        };
        framebufferSpecification.width = width;
        framebufferSpecification.height = height;
        return updateSpecs(framebufferSpecification);
    }

    void FrameController::release() {
        _frameBuffer.destroy();
    }

}