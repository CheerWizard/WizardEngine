//
// Created by mecha on 24.10.2021.
//

#include <graphics/core/frame/FrameController.h>
#include <platform/graphics/RenderCommands.h>

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

    void FrameController::bind() {
        _frameBuffer.bind();
    }

    void FrameController::unbind() {
        _frameBuffer.unbind();
    }

    void FrameController::setViewPort() {
        _frameBuffer.setViewPort();
    }

    const std::vector<uint32_t> &FrameController::updateSpecs(const uint32_t &width, const uint32_t &height) {
        engine::FramebufferSpecification framebufferSpecification;
        framebufferSpecification.attachmentSpecification = {
                engine::FramebufferTextureFormat::RGBA8,
                engine::FramebufferTextureFormat::RED_INTEGER,
                engine::FramebufferTextureFormat::DEPTH24STENCIL8
        };
        framebufferSpecification.width = width;
        framebufferSpecification.height = height;
        return updateSpecs(framebufferSpecification);
    }

    void FrameController::release() {
        _frameBuffer.destroy();
    }

}