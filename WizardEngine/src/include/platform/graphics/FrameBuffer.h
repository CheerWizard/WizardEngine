//
// Created by mecha on 27.09.2021.
//

#pragma once

#include <core/Assert.h>
#include <graphics/core/texture/Texture.h>

namespace engine::graphics {

    struct ENGINE_API ColorAttachment final {
        int internalFormat = ColorFormat::RED_I32;
        u32 dataFormat = ColorFormat::U_RED;
        u32 pixelsType = PixelsType::U_BYTE;
        u32 id = 0;
    };

    struct ENGINE_API DepthStencilAttachment final {
        u32 internalFormat = DepthStencilFormat::NONE;
        u32 id = 0;
    };

    struct ENGINE_API RenderBufferAttachment final {
        u32 internalFormat = DepthStencilFormat::NONE;
        u32 id = 0;
    };

    struct ENGINE_API DepthAttachment final {
        int internalFormat = 0;
        u32 dataFormat = DepthFormat::NONE;
        u32 pixelsType = PixelsType::FLOAT;
        u32 id = 0;
    };

    struct ENGINE_API FrameBufferFormat final {
        int width = 0, height = 0, samples = 1;
        std::vector<ColorAttachment> colorAttachments;
        DepthStencilAttachment depthStencilAttachment;
        RenderBufferAttachment renderBufferAttachment;
        DepthAttachment depthAttachment;
    };

    class ENGINE_API FrameBuffer final {

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

        [[nodiscard]] bool getColorAttachment(u32 index, ColorAttachment& colorAttachment) const {
            const auto& size = format.colorAttachments.size();
            if (index >= size) {
                ENGINE_ERR("FrameBuffer::getColorAttachment() failed with index out of bounds : [index: {0}, size: {1}]",
                           index, size);
                colorAttachment = {};
                return false;
            }
            colorAttachment = format.colorAttachments.at(index);
            return true;
        }

        [[nodiscard]] inline const std::vector<ColorAttachment>& getColorAttachments() const {
            return format.colorAttachments;
        }

        [[nodiscard]] inline size_t getColorAttachmentsSize() const {
            return format.colorAttachments.size();
        }

        [[nodiscard]] const ColorAttachment& getRenderTarget() {
            return format.colorAttachments.at(renderTargetIndex);
        }

        [[nodiscard]] inline const DepthStencilAttachment& getDepthStencilAttachment() const {
            return format.depthStencilAttachment;
        }

        inline void setRenderTargetIndex(u32 index) {
            size_t renderTargetCount = getColorAttachmentsSize();
            if (index >= renderTargetCount) {
                ENGINE_ERR("FrameBuffer: can't set render target index={0}. RenderTarget count={1}",
                           index, renderTargetCount);
            }
            renderTargetIndex = index;
        }

    public:
        // lifetime functions
        void create();
        void destroy();
        void recreate();
        // bind/unbind
        void bind() const;
        void bindRead() const;
        void bindWrite() const;
        // loading specs, attachments and reading GPU data
        void setFormat(const FrameBufferFormat &format);
        void loadAttachments();
        const std::vector<ColorAttachment>& updateFormat(const FrameBufferFormat &format);
        void setViewPort() const;
        void resize(int width, int height);
        int readPixel(u32 attachmentIndex, s32 x, s32 y) const;
        void removeAttachment(u32 attachmentIndex, s32 value);
        void attachCubeMap(u32 i, u32 textureId);

    private:
        bool isCompleted();
        void attachColors();
        void attachDepthStencil();
        void attachDepth();
        void attachRbo();
        void createDrawBuffers() const;
        void bindTexture(u32 attachmentId);

    private:
        u32 id = 0;
        u32 textureTarget;
        FrameBufferFormat format;
        u32 renderTargetIndex = 0;

    public:
        static void bindDefault();
        static void readWriteFrameBuffers(FrameBuffer& src, FrameBuffer& target);
        static int getMaxColorAttachments();
        static int getMaxDrawBuffers();
    };

}
