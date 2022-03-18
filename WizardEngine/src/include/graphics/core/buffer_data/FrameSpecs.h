//
// Created by mecha on 12.11.2021.
//

#pragma once

#include "vector"
#include "string"

namespace engine {

    struct Color {
        float r,g,b,a;
    };

    std::string str(const Color &color);

    enum class FramebufferTextureFormat {
        None = 0,
        // Color
        RGBA8,
        RED_INTEGER,
        // Depth, Stencil
        DEPTH16,
        DEPTH24PAD8,
        DEPTH24STENCIL8,
        DEPTH32
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
}
