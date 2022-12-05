//
// Created by mecha on 27.09.2021.
//

#include <platform/graphics/FrameBuffer.h>
#include <sstream>
#include <glad/glad.h>

namespace engine::graphics {

    void FrameBuffer::setFormat(const FrameBufferFormat &format) {
        this->format = format;
    }

    const std::vector<ColorAttachment>& FrameBuffer::updateFormat(const FrameBufferFormat &format) {
        setFormat(format);
        textureTarget = format.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        loadAttachments();
        const auto& colors = getColorAttachments();
        if (colors.empty()) {
            ENGINE_INFO("Update FBO format. No color attachments");
        } else {
            ENGINE_INFO("Update FBO format. Color buffer id: {0}", colors[0].id);
        }
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
        std::string statusStr;
        switch (status) {
            case GL_FRAMEBUFFER_COMPLETE:
                statusStr = "GL_FRAMEBUFFER_COMPLETE";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                statusStr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                statusStr = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                statusStr = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                statusStr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                statusStr = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                statusStr = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
                break;
            default:
                statusStr = "UNKNOWN";
                break;
        }
        ENGINE_INFO("FrameBuffer [id: {0}, status: {1}]", id, statusStr.c_str());
        return status == GL_FRAMEBUFFER_COMPLETE;
    }

    void FrameBuffer::attachColors() {
        if (format.colorAttachments.empty()) return;

        for (int i = 0 ; i < format.colorAttachments.size() ; i++) {
            auto& colorAttachment = format.colorAttachments[i];

            glCreateTextures(textureTarget, 1, &colorAttachment.id);
            bindTexture(colorAttachment.id);

            if (format.samples > 1) {
                glTexImage2DMultisample(
                        GL_TEXTURE_2D_MULTISAMPLE,
                        format.samples,
                        colorAttachment.internalFormat,
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
                        colorAttachment.internalFormat,
                        format.width,
                        format.height,
                        0,
                        colorAttachment.dataFormat,
                        colorAttachment.pixelsType,
                        nullptr
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            bindTexture(0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureTarget, colorAttachment.id, 0);
        }
    }

    void FrameBuffer::attachCubeMap(u32 i, u32 textureId) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureId, 0);
    }

    void FrameBuffer::attachDepthStencil() {
        if (format.depthStencilAttachment.internalFormat == DepthStencilFormat::NONE) return;

        auto& depthStencilAttachment = format.depthStencilAttachment;
        glCreateTextures(textureTarget, 1, &depthStencilAttachment.id);
        bindTexture(depthStencilAttachment.id);

        if (format.samples > 1) {
            glTexImage2DMultisample(
                    GL_TEXTURE_2D_MULTISAMPLE,
                    format.samples,
                    depthStencilAttachment.internalFormat,
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
            glTexStorage2D(GL_TEXTURE_2D, 1,
                           depthStencilAttachment.internalFormat,
                           format.width, format.height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        bindTexture(0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, textureTarget, depthStencilAttachment.id, 0);
    }

    void FrameBuffer::attachRbo() {
        if (format.renderBufferAttachment.internalFormat == DepthStencilFormat::NONE) return;

        auto& rbo = format.renderBufferAttachment;
        glCreateRenderbuffers(1, &rbo.id);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo.id);

        if (format.samples > 1) {
            glRenderbufferStorageMultisample(
                    GL_RENDERBUFFER,
                    format.samples,
                    rbo.internalFormat,
                    format.width,
                    format.height
            );
        } else {
            glRenderbufferStorage(
                    GL_RENDERBUFFER,
                    rbo.internalFormat,
                    format.width,
                    format.height
            );
        }

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
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

    void FrameBuffer::bindTexture(u32 attachmentId) {
        glBindTexture(textureTarget, attachmentId);
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
        glViewport(0, 0, format.width, format.height);
    }

    void FrameBuffer::bindDefault() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::resize(int width, int height) {
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
        ENGINE_ASSERT(attachmentIndex < format.colorAttachments.size(),
                      "FrameBuffer::readPixel(): attachmentIndex >= attachments size");

        ENGINE_INFO("FrameBuffer::readPixel: x: {0} y: {1} index: {2}", x, y, attachmentIndex);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y,
                     1, 1,
                     format.colorAttachments[attachmentIndex].dataFormat,
                     GL_INT, &pixelData);
        return pixelData;
    }

    void FrameBuffer::removeAttachment(uint32_t attachmentIndex, int value) {
        ENGINE_ASSERT(attachmentIndex < format.colorAttachments.size(), "removeAttachment()");

        auto& colorAttachment = format.colorAttachments[attachmentIndex];
        glClearTexImage(colorAttachment.id, 0, colorAttachment.dataFormat, GL_INT, &value);
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