//
// Created by mecha on 24.10.2021.
//

#pragma once

#include "FrameBuffer.h"
#include "../../core/Memory.h"

namespace engine {

    class FrameController {

    public:
        FrameController(const Ref<FrameBuffer> &frameBuffer) : _frameBuffer(frameBuffer) {}
        ~FrameController() = default;

    public:
        inline const std::vector<uint32_t>& getFrameColors() {
            return _frameBuffer->getColorAttachments();
        }

    public:
        void begin();
        void end();
        const std::vector<uint32_t>& updateSpecs(const FramebufferSpecification &framebufferSpecification);
        void resize(const uint32_t &width, const uint32_t &height);
        void setViewPort();
        void resetFrame();
        const std::vector<uint32_t>& updateSpecs(const uint32_t &width, const uint32_t &height);

    private:
        Ref<FrameBuffer> _frameBuffer;

    };

}
