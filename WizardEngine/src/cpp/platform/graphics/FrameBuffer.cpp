//
// Created by mecha on 27.09.2021.
//

#include <platform/graphics/FrameBuffer.h>
#include <sstream>
#include <glad/glad.h>

namespace engine::graphics {

    uint32_t toGLColorFormat(const ColorFormat &format) {
        switch (format) {
            case ColorFormat::RGBA8:       return GL_RGBA8;
            case ColorFormat::RED_INTEGER: return GL_RED_INTEGER;
            default: return 0;
        }
    }

    uint32_t toGLDepthStencilFormat(const DepthStencilFormat &format) {
        switch (format) {
            case DepthStencilFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
            case DepthStencilFormat::DEPTH16:         return GL_DEPTH_COMPONENT16;
            case DepthStencilFormat::DEPTH32:         return GL_DEPTH32F_STENCIL8;
            default: return 0;
        }
    }

    void toGLInternalAndDataFormat(const ColorFormat& colorFormat, uint32_t &internalFormat, uint32_t &dataFormat) {
        switch (colorFormat) {
            case ColorFormat::RGBA8:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            case ColorFormat::RED_INTEGER:
                internalFormat = GL_R32I;
                dataFormat = GL_RED_INTEGER;
                break;
            default: break;
        }
    }

    void FrameBuffer::setFormat(const FrameBufferFormat &format) {
        this->format = format;
    }

    const std::vector<ColorAttachment>& FrameBuffer::updateFormat(const FrameBufferFormat &format) {
        setFormat(format);
        loadAttachments();
        const auto& colors = getColorAttachments();
        ENGINE_INFO("Update FBO format. Color buffer id: {0}", colors[0].id);
        return colors;
    }

    void FrameBuffer::loadAttachments() {
        bind();

        attachColors();
        attachDepthStencil();
        attachRbo();
        createDrawBuffers();

        ENGINE_ASSERT(isCompleted(), "Framebuffer is incomplete!");

        bindDefault();
    }

    void FrameBuffer::create() {
        glCreateFramebuffers(1, &id);
    }

    void FrameBuffer::destroy() {
        glDeleteFramebuffers(1, &id);

        for (auto& colorAttachment : format.colorAttachments) {
            glDeleteTextures(1, &colorAttachment.id);
        }
        glDeleteTextures(1, &format.depthStencilAttachment.id);
        glDeleteRenderbuffers(1, &format.renderBufferAttachment.id);
    }

    void FrameBuffer::recreate() {
        destroy();
        create();
    }

    bool FrameBuffer::isCompleted() {
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        ENGINE_INFO("FrameBuffer [id: {0}, status: {1}]", id, status);
        return status == GL_FRAMEBUFFER_COMPLETE;
    }

    void FrameBuffer::attachColors() {
        if (format.colorAttachments.empty()) return;

        for (int i = 0 ; i < format.colorAttachments.size() ; i++) {
            auto& colorAttachment = format.colorAttachments[i];

            glCreateTextures(getTextureTarget(), 1, &colorAttachment.id);
            bindTexture(colorAttachment.id);

            uint32_t colorInternalFormat;
            uint32_t colorDataFormat;
            toGLInternalAndDataFormat(colorAttachment.format, colorInternalFormat, colorDataFormat);

            if (format.samples > 1) {
                glTexImage2DMultisample(
                        GL_TEXTURE_2D_MULTISAMPLE,
                        format.samples,
                        colorInternalFormat,
                        format.width,
                        format.height,
                        GL_TRUE
                );

                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            } else {
                glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        (GLint) colorInternalFormat,
                        (GLsizei) format.width,
                        (GLsizei) format.height,
                        0,
                        colorDataFormat,
                        GL_UNSIGNED_BYTE,
                        nullptr
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            bindTexture(0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, getTextureTarget(), colorAttachment.id, 0);
        }
    }

    void FrameBuffer::attachDepthStencil() {
        if (format.depthStencilAttachment.format == DepthStencilFormat::NONE) return;

        auto& depthStencilAttachment = format.depthStencilAttachment;
        glCreateTextures(getTextureTarget(), 1, &depthStencilAttachment.id);
        bindTexture(depthStencilAttachment.id);

        auto depthFormat = toGLDepthStencilFormat(depthStencilAttachment.format);

        if (format.samples > 1) {
            glTexImage2DMultisample(
                    GL_TEXTURE_2D_MULTISAMPLE,
                    (GLsizei) format.samples,
                    depthFormat,
                    (GLsizei) format.width,
                    (GLsizei) format.height,
                    GL_TRUE
            );
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } else {
            glTexStorage2D(GL_TEXTURE_2D, 1, depthFormat,(GLsizei) format.width,(GLsizei) format.height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        bindTexture(0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, getTextureTarget(), depthStencilAttachment.id, 0);
    }

    void FrameBuffer::attachRbo() {
        if (format.renderBufferAttachment.format == DepthStencilFormat::NONE) return;

        auto& rbo = format.renderBufferAttachment;
        glCreateRenderbuffers(1, &rbo.id);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo.id);

        auto depthStencilFormat = toGLDepthStencilFormat(format.renderBufferAttachment.format);

        if (format.samples > 1) {
            glRenderbufferStorageMultisample(
                    GL_RENDERBUFFER,
                    format.samples,
                    depthStencilFormat,
                    format.width,
                    format.height
            );
        } else {
            glRenderbufferStorage(
                    GL_RENDERBUFFER,
                    depthStencilFormat,
                    format.width,
                    format.height
            );
        }

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER,
                format.renderBufferAttachment.id
        );
    }

    void FrameBuffer::createDrawBuffers() const {
        if (format.colorAttachments.empty()) {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
            return;
        }

        auto colorsSize = format.colorAttachments.size();
        if (colorsSize > 1) {
            ENGINE_ASSERT(colorsSize <= 4, "createDrawBuffers()");
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers((GLsizei) colorsSize, buffers);
        } else {
            GLenum buffer[1] = { GL_COLOR_ATTACHMENT0 };
            glDrawBuffers(1, buffer);
        }
    }

    uint32_t FrameBuffer::getTextureTarget() const {
        return format.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    void FrameBuffer::bindTexture(const uint32_t &attachmentId) {
        glBindTexture(getTextureTarget(), attachmentId);
    }

    void FrameBuffer::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }

    void FrameBuffer::bindRead() const {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    }

    void FrameBuffer::bindWrite() const {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
    }

    void FrameBuffer::setViewPort() const {
        glViewport(0, 0, (GLsizei) format.width, (GLsizei) format.height);
    }

    void FrameBuffer::bindDefault() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0) {
            ENGINE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }
        format.width = width;
        format.height = height;

        recreate();
        loadAttachments();
        setViewPort();
    }

    int FrameBuffer::readPixel(uint32_t attachmentIndex, int x, int y) const {
        ENGINE_ASSERT(attachmentIndex < format.colorAttachments.size(), "readPixel()");

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void FrameBuffer::removeAttachment(uint32_t attachmentIndex, int value) {
        ENGINE_ASSERT(attachmentIndex < format.colorAttachments.size(), "removeAttachment()");

        auto& colorAttachment = format.colorAttachments[attachmentIndex];
        auto textureFormat = toGLColorFormat(colorAttachment.format);
        glClearTexImage(colorAttachment.id, 0, textureFormat, GL_INT,&value);
    }

    void FrameBuffer::readWriteFrameBuffers(FrameBuffer& src, FrameBuffer& target) {
        src.bindRead();
        target.bindWrite();
        const auto& srcFormat = src.getFormat();
        const auto& targetFormat = target.getFormat();
        glBlitFramebuffer(
                0, 0, srcFormat.width, srcFormat.height,
                0, 0, targetFormat.width, targetFormat.height,
                GL_COLOR_BUFFER_BIT, GL_NEAREST
        );
    }

}