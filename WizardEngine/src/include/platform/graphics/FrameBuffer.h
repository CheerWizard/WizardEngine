//
// Created by mecha on 27.09.2021.
//

#pragma once

#include <core/Assert.h>
#include <graphics/core/buffer_data/FrameSpecs.h>

namespace engine {

    class FrameBuffer final {

    public:
        FrameBuffer() {
            create();
        }

        FrameBuffer(const FramebufferSpecification &specification) {
            create();
            setSpecification(specification);
        }

        ~FrameBuffer() = default;

    public:
        inline const FramebufferSpecification& getSpecification() {
            return specification;
        }

        [[nodiscard]] const uint32_t& getColorAttachment(uint32_t index = 0) const {
            ENGINE_ASSERT(index < colorAttachments.size(), "getColorAttachment()");
            return colorAttachments[index];
        }

        inline const std::vector<uint32_t>& getColorAttachments() {
            return colorAttachments;
        }

    public:
        // lifetime functions
        void create();
        void destroy();
        void recreate();
        // bind/unbind
        void bind();
        void unbind();
        // loading specs, attachments and reading GPU data
        void setSpecification(const FramebufferSpecification &framebufferSpecification);
        void loadAttachments();
        void setViewPort();
        void resize(uint32_t width, uint32_t height);
        int readPixel(uint32_t attachmentIndex, int x, int y);
        void removeAttachment(uint32_t attachmentIndex, int value);
        // commands for depth buffer
        void enableDepth();
        void disableDepth();
        void clearDepth(const Color &color);
        void clearColor(const Color &color);

    private:
        bool isCompleted();
        void attachColorSpecs();
        void attachDepthSpecs();
        void createDrawBuffers();
        void attachColorTextures();
        void attachDepthTexture();
        void createTextures(const uint32_t &count, uint32_t* outAttachmentId);
        void bindTexture(const uint32_t &attachmentId);
        uint32_t getTextureTarget();
        void convertTextureFormat(const uint32_t &specIndex,
                                  uint32_t &outInternalFormat,
                                  uint32_t &outFormat);
        static uint32_t convertTextureFormat(const FramebufferTextureFormat &format);

    private:
        uint32_t id = 0;
        FramebufferSpecification specification;
        FramebufferTextureSpecification depthAttachmentSpec = FramebufferTextureFormat::None;
        std::vector<FramebufferTextureSpecification> colorAttachmentSpecs;
        std::vector<uint32_t> colorAttachments;
        uint32_t depthAttachment = 0;

    };

}
