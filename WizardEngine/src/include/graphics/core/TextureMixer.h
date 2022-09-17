//
// Created by mecha on 31.08.2022.
//

#pragma once

#include <graphics/core/Renderer.h>
#include <platform/graphics/FrameBuffer.h>

namespace engine::graphics {

    class ENGINE_API TextureMixer final {

    public:
        TextureMixer() = default;
        TextureMixer(const FrameBufferFormat& frameBufferFormat);

    public:
        void release();
        u32 render(const vector<u32>& textures);

    private:
        Ref<FrameBuffer> frameBuffer;
        BaseShaderProgram shaderProgram;
    };

}