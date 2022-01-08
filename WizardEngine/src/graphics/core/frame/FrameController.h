//
// Created by mecha on 24.10.2021.
//

#pragma once

#include "../../../core/Memory.h"
#include "../../../platform/includes/graphics/fbo.h"

namespace engine {

    class FrameController {

    public:
        FrameController() = default;
        FrameController(const FrameBuffer &frameBuffer) : _frameBuffer(frameBuffer) {}
        ~FrameController() {
            release();
        }

    public:
        inline const std::vector<uint32_t>& getFrameColors() {
            return _frameBuffer.getColorAttachments();
        }

    public:
        void begin();
        void end();
        const std::vector<uint32_t>& updateSpecs(const FramebufferSpecification &framebufferSpecification);
        void resize(const uint32_t &width, const uint32_t &height);
        void setViewPort();
        void resetFrame();
        const std::vector<uint32_t>& updateSpecs(const uint32_t &width, const uint32_t &height);
        void release();

    private:
        FrameBuffer _frameBuffer;

    };

}
