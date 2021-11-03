//
// Created by mecha on 05.10.2021.
//

#pragma once

#include "../core/Memory.h"

#include "core/Drawer.h"
#include "core/RenderSettings.h"

#include "core/buffers/VertexArray.h"
#include "core/buffers/IndexBuffer.h"
#include "core/buffers/VertexBuffer.h"
#include "core/buffers/TextureBuffer.h"
#include "core/buffers/UniformBuffer.h"

#include "core/frame/FrameBuffer.h"

#include "core/shader/Shader.h"

namespace engine {

    class GraphicsFactory {

    public:
        GraphicsFactory() = default;
        virtual ~GraphicsFactory() = default;

    public:
        virtual Ref<Drawer> newDrawer() = 0;
        virtual Ref<VertexArray> newVertexArray() = 0;

        virtual Ref<VertexBuffer> newVertexBuffer() = 0;
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

        virtual Ref<RenderSettings> newRenderSettings() = 0;
    };

}