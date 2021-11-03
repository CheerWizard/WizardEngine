//
// Created by mecha on 27.09.2021.
//

#pragma once

#include "../../../graphics/core/frame/FrameBuffer.h"

namespace engine {

    class GLFrameBuffer : public FrameBuffer {

    public:

        GLFrameBuffer(const uint32_t &framesCount = DEFAULT_FRAMES_COUNT) : FrameBuffer(framesCount) {
            create();
        }

        GLFrameBuffer(const FramebufferSpecification &specification) : FrameBuffer(specification) {
            create();
        }

        ~GLFrameBuffer() override {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;
        void resize(uint32_t width, uint32_t height) override;
        int readPixel(uint32_t attachmentIndex, int x, int y) override;
        void removeAttachment(uint32_t attachmentIndex, int value) override;
        void setViewPort() override;
        void recreate() override;
        void enableDepth() override;
        void disableDepth() override;
        void clearDepth(const Color &color) override;
        void clearColor(const Color &color) override;

    protected:
        void attachColorSpecs() override;
        void attachDepthSpecs() override;
        void createDrawBuffers() override;
        bool isCompleted() override;

    private:
        void create();
        void destroy();
        void attachColorTextures();
        void attachDepthTexture();
        void createTextures(const uint32_t &count, uint32_t* outAttachmentId);
        void bindTexture(const uint32_t &attachmentId);
        uint32_t getTextureTarget();
        void convertTextureFormat(const uint32_t &specIndex,
                                  uint32_t &outInternalFormat,
                                  uint32_t &outFormat);

    private:
        static uint32_t convertTextureFormat(const FramebufferTextureFormat &format);

    };

}