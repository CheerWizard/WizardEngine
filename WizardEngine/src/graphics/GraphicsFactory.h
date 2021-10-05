//
// Created by mecha on 05.10.2021.
//

#pragma once

#include "../core/Memory.h"

#include "buffers/VertexArray.h"
#include "buffers/IndexBuffer.h"
#include "buffers/VertexBuffer.h"
#include "buffers/TextureBuffer.h"
#include "buffers/FrameBuffer.h"
#include "buffers/UniformBuffer.h"

#include "render/Drawer.h"

#include "shader/Shader.h"

namespace engine {

    class GraphicsFactory {

    public:
        GraphicsFactory() = default;
        ~GraphicsFactory() = default;

    public:
        virtual Ref<Drawer> newDrawer() = 0;
        virtual Ref<VertexArray> newVertexArray() = 0;

        virtual Ref<VertexBuffer> newVertexBuffer(VertexFormat* vertexFormat) = 0;

        virtual Ref<IndexBuffer> newIndexBuffer() = 0;
        virtual Ref<IndexBuffer> newIndexBuffer(const uint32_t &indexCount) = 0;

        virtual Ref<TextureBuffer> newTextureBuffer() = 0;

        virtual Ref<FrameBuffer> newFrameBuffer(const FramebufferSpecification &framebufferSpecification) = 0;
        virtual Ref<FrameBuffer> newFrameBuffer() = 0;

        virtual Ref<UniformBuffer> newUniformBuffer(UniformBlockFormat* uniformBlockFormat) = 0;
        virtual Ref<UniformBuffer> newUniformBuffer() = 0;

        virtual Ref<Shader> newShader(const ShaderProps& shaderProps, VertexFormat* vertexFormat) = 0;
        virtual Ref<Shader> newShader(const ShaderProps& shaderProps) = 0;

    };

}