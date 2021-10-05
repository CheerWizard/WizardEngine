//
// Created by mecha on 05.10.2021.
//

#pragma once

#include "../../graphics/GraphicsFactory.h"

namespace engine {

    class GLFactory : public GraphicsFactory {

    public:
        GLFactory() : GraphicsFactory() {}
        ~GLFactory() = default;

    public:
        Ref <Drawer> newDrawer() override;
        Ref <VertexArray> newVertexArray() override;
        Ref <VertexBuffer> newVertexBuffer(VertexFormat *vertexFormat) override;
        Ref <IndexBuffer> newIndexBuffer() override;
        Ref <IndexBuffer> newIndexBuffer(const uint32_t &indexCount) override;
        Ref <TextureBuffer> newTextureBuffer() override;
        Ref <FrameBuffer> newFrameBuffer(const FramebufferSpecification &framebufferSpecification) override;
        Ref <FrameBuffer> newFrameBuffer() override;
        Ref <UniformBuffer> newUniformBuffer(UniformBlockFormat *uniformBlockFormat) override;
        Ref <UniformBuffer> newUniformBuffer() override;
        Ref <Shader> newShader(const ShaderProps &shaderProps, VertexFormat *vertexFormat) override;
        Ref <Shader> newShader(const ShaderProps &shaderProps) override;
    };

}
