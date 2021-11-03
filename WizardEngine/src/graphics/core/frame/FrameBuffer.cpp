//
// Created by mecha on 27.09.2021.
//

#include "FrameBuffer.h"

#include <sstream>

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

    std::string Color::toString() const {
        std::stringstream ss;
        ss << "r: " << r << " g: " << g << " b: " << b << " a: " << a;
        return ss.str();
    }

}