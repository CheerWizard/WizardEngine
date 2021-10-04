//
// Created by mecha on 27.09.2021.
//

#pragma once

#include "Buffer.h"
#include "../../core/Assert.h"

#define DEFAULT_FRAMES_COUNT 8192

namespace engine {

    enum class FramebufferTextureFormat {
        None = 0,
        // Color
        RGBA8,
        RED_INTEGER,
        // Depth/stencil
        DEPTH24STENCIL8,
        // Defaults
        Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureSpecification {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(const FramebufferTextureFormat &format)
        : textureFormat(format) {}

        FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
    };

    struct FramebufferAttachmentSpecification {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification> &textureSpecs)
        : textureSpecs(textureSpecs) {}

        std::vector<FramebufferTextureSpecification> textureSpecs;
    };

    struct FramebufferSpecification {
        uint32_t width = 0, height = 0;
        FramebufferAttachmentSpecification attachmentSpecification;
        uint32_t samples = 1;
        bool swapChainTarget = false;
    };

    class FrameBuffer : public Buffer {

    public:

        FrameBuffer(const uint32_t &framesCount = DEFAULT_FRAMES_COUNT) : Buffer() {
            capacity = framesCount;
        }

        FrameBuffer(const FramebufferSpecification &specification,
                    const uint32_t &framesCount = DEFAULT_FRAMES_COUNT) :
                    Buffer()
                    {
                        capacity = framesCount;
                        setSpecification(specification);
                    }

        ~FrameBuffer() override = default;

    public:
        inline const FramebufferSpecification& getSpecification() {
            return specification;
        }

        const uint32_t& getColorAttachment(uint32_t index = 0) const {
            ENGINE_ASSERT(index < colorAttachments.size(), "getColorAttachment()");
            return colorAttachments[index];
        }

        inline const std::vector<uint32_t>& getColorAttachments() {
            return colorAttachments;
        }

    public:
        void setSpecification(const FramebufferSpecification &framebufferSpecification);
        void loadAttachments();

    public:
        virtual void setViewPort() = 0;
        virtual void resize(uint32_t width, uint32_t height) = 0;
        virtual int readPixel(uint32_t attachmentIndex, int x, int y) = 0;
        virtual void removeAttachment(uint32_t attachmentIndex, int value) = 0;

    protected:
        virtual bool isCompleted() = 0;
        virtual void attachColorSpecs() = 0;
        virtual void attachDepthSpecs() = 0;
        virtual void createDrawBuffers() = 0;

    protected:
        FramebufferSpecification specification;
        FramebufferTextureSpecification depthAttachmentSpec = FramebufferTextureFormat::None;
        std::vector<FramebufferTextureSpecification> colorAttachmentSpecs;
        std::vector<uint32_t> colorAttachments;
        uint32_t depthAttachment = 0;

    };

}
