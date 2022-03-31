//
// Created by mecha on 27.09.2021.
//

#pragma once

#include <core/Assert.h>
#include <graphics/core/texture/Texture.h>

namespace engine {

    struct ColorAttachment {
        ColorFormat format = ColorFormat::NONE;
        uint32_t id = 0;
    };

    struct DepthStencilAttachment {
        DepthStencilFormat format = DepthStencilFormat::NONE;
        uint32_t id = 0;
    };

    struct RenderBufferAttachment {
        DepthStencilFormat format = DepthStencilFormat::NONE;
        uint32_t id = 0;
    };

    struct FrameBufferFormat {
        uint32_t width = 0, height = 0;
        uint32_t samples = 1;
        bool useIdenticalSampleLocations = true;
        bool swapChainTarget = false;
        std::vector<ColorAttachment> colorAttachments;
        DepthStencilAttachment depthStencilAttachment;
        RenderBufferAttachment renderBufferAttachment;
    };

    class FrameBuffer final {

    public:
        FrameBuffer() {
            create();
        }

        FrameBuffer(const FrameBufferFormat &format) {
            create();
            setFormat(format);
        }

        ~FrameBuffer() = default;

    public:
        inline const FrameBufferFormat& getFormat() {
            return format;
        }

        [[nodiscard]] ColorAttachment getColorAttachment(uint32_t index = 0) const {
            const auto& size = format.colorAttachments.size();
            if (index >= size) {
                ENGINE_ERR("getColorAttachment() failed with index out of bounds : [index: {0}, size: {1}]", index, size);
                return {};
            }
            return format.colorAttachments[index];
        }

        inline const std::vector<ColorAttachment>& getColorAttachments() {
            return format.colorAttachments;
        }

        inline const DepthStencilAttachment& getDepthStencilAttachment() {
            return format.depthStencilAttachment;
        }

    public:
        // lifetime functions
        void create();
        void destroy();
        void recreate();
        // bind/unbind
        void bind() const;
        void unbind();
        void bindRead() const;
        void bindWrite() const;
        // loading specs, attachments and reading GPU data
        void setFormat(const FrameBufferFormat &format);
        void loadAttachments();
        const std::vector<ColorAttachment>& updateFormat(const FrameBufferFormat &format);
        void setViewPort() const;
        void resize(uint32_t width, uint32_t height);
        int readPixel(uint32_t attachmentIndex, int x, int y) const;
        void removeAttachment(uint32_t attachmentIndex, int value);

    private:
        bool isCompleted();
        void attachColors();
        void attachDepthStencil();
        void attachRbo();
        void createDrawBuffers() const;
        void attachColorTextures();
        void attachDepthTexture();
        void bindTexture(const uint32_t &attachmentId);
        uint32_t getTextureTarget() const;

    private:
        uint32_t id = 0;
        FrameBufferFormat format;

    public:
        static void readWriteFrameBuffers(FrameBuffer& src, FrameBuffer& target);
    };

}
