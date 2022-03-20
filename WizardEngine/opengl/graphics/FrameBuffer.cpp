//
// Created by mecha on 27.09.2021.
//

#include <platform/graphics/FrameBuffer.h>
#include <sstream>
#include <glad/glad.h>

namespace engine {

    void FrameBuffer::setSpecification(const FramebufferSpecification &specification) {
        this->specification = specification;
        for (const auto& spec : specification.attachmentSpecification.textureSpecs) {
            if (spec.textureFormat != FramebufferTextureFormat::DEPTH24STENCIL8) {
                colorAttachmentSpecs.emplace_back(spec);
            } else {
                depthAttachmentSpec = spec;
            }
        }
    }

    void FrameBuffer::loadAttachments() {
        bind();

        attachColorSpecs();
        attachDepthSpecs();
        createDrawBuffers();

        ENGINE_ASSERT(isCompleted(), "Framebuffer is incomplete!");

        unbind();
    }

    void FrameBuffer::create() {
        glCreateFramebuffers(1, &id);
    }

    void FrameBuffer::destroy() {
        glDeleteFramebuffers(1, &id);
        glDeleteTextures((GLsizei) colorAttachments.size(), colorAttachments.data());
        glDeleteTextures(1, &depthAttachment);
        colorAttachments.clear();
        depthAttachment = 0;
    }

    void FrameBuffer::recreate() {
        destroy();
        create();
    }

    bool FrameBuffer::isCompleted() {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    void FrameBuffer::attachColorSpecs() {
        auto specsSize = colorAttachmentSpecs.size();
        if (specsSize) {
            colorAttachments.resize(specsSize);
            createTextures(colorAttachments.size(), colorAttachments.data());
            attachColorTextures();
        }
    }

    void FrameBuffer::attachDepthSpecs() {
        if (depthAttachmentSpec.textureFormat != FramebufferTextureFormat::None) {
            createTextures(1, &depthAttachment);
            attachDepthTexture();
        }
    }

    void FrameBuffer::createDrawBuffers() {
        if (colorAttachments.size() > 1) {
            ENGINE_ASSERT(colorAttachments.size() <= 4, "createDrawBuffers()");
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers((GLsizei) colorAttachments.size(), buffers);
        } else if (colorAttachments.empty()) {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }
    }

    uint32_t FrameBuffer::convertTextureFormat(const FramebufferTextureFormat &format) {
        switch (format) {
            case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
            case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
            default: return 0;
        }
    }

    void FrameBuffer::attachColorTextures() {
        for (auto i = 0 ; i < colorAttachments.size() ; i++) {
            bindTexture(colorAttachments[i]);

            uint32_t colorFormat;
            uint32_t colorInternalFormat;
            convertTextureFormat(i, colorInternalFormat, colorFormat);

            if (specification.samples > 1) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                                        (GLsizei) specification.samples,
                                        colorInternalFormat,
                                        (GLsizei) specification.width,
                                        (GLsizei) specification.height,
                                        GL_FALSE);
            } else {
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             (GLint) colorInternalFormat,
                             (GLsizei) specification.width,
                             (GLsizei) specification.height,
                             0,
                             colorFormat,
                             GL_UNSIGNED_BYTE,
                             nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, getTextureTarget(), colorAttachments[i], 0);
        }
    }

    void FrameBuffer::attachDepthTexture() {
        bindTexture(depthAttachment);
        auto depthFormat = convertTextureFormat(depthAttachmentSpec.textureFormat);

        if (specification.samples > 1) {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                                    (GLsizei) specification.samples,
                                    depthFormat,
                                    (GLsizei) specification.width,
                                    (GLsizei) specification.height,
                                    GL_FALSE);
        } else {
            glTexStorage2D(GL_TEXTURE_2D, 1, depthFormat,(GLsizei) specification.width,(GLsizei) specification.height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, getTextureTarget(), depthAttachment, 0);
    }

    uint32_t FrameBuffer::getTextureTarget() {
        return specification.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    void FrameBuffer::createTextures(const uint32_t &count, uint32_t *outAttachmentId) {
        glCreateTextures(getTextureTarget(), (GLsizei) count, outAttachmentId);
    }

    void FrameBuffer::bindTexture(const uint32_t &attachmentId) {
        glBindTexture(getTextureTarget(), attachmentId);
    }

    void FrameBuffer::convertTextureFormat(const uint32_t &specIndex, uint32_t &outInternalFormat, uint32_t &outFormat) {
        switch (colorAttachmentSpecs[specIndex].textureFormat) {
            case FramebufferTextureFormat::RGBA8:
                outInternalFormat = GL_RGBA8;
                outFormat = GL_RGBA;
                break;
            case FramebufferTextureFormat::RED_INTEGER:
                outInternalFormat = GL_R32I;
                outFormat = GL_RED_INTEGER;
                break;
        }
    }

    void FrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }

    void FrameBuffer::setViewPort() {
        glViewport(0, 0, (GLsizei) specification.width, (GLsizei) specification.height);
    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0) {
            ENGINE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }
        specification.width = width;
        specification.height = height;

        recreate();
        loadAttachments();
        setViewPort();
    }

    int FrameBuffer::readPixel(uint32_t attachmentIndex, int x, int y) {
        ENGINE_ASSERT(attachmentIndex < colorAttachments.size(), "readPixel()");

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void FrameBuffer::removeAttachment(uint32_t attachmentIndex, int value) {
        ENGINE_ASSERT(attachmentIndex < colorAttachments.size(), "removeAttachment()");

        auto& spec = colorAttachmentSpecs[attachmentIndex];
        auto& texture = colorAttachments[attachmentIndex];
        auto textureFormat = convertTextureFormat(spec.textureFormat);
        glClearTexImage(texture, 0, textureFormat, GL_INT,&value);
    }
}
