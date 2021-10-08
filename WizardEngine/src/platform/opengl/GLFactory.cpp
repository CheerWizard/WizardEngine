//
// Created by mecha on 05.10.2021.
//

#include "GLFactory.h"

#include "buffers/GLVertexArray.h"
#include "buffers/GLVertexBuffer.h"
#include "buffers/GLIndexBuffer.h"
#include "buffers/GLTextureBuffer.h"
#include "buffers/GLFrameBuffer.h"
#include "buffers/GLUniformBuffer.h"

#include "shader/GLShader.h"
#include "render/GLDrawer.h"

namespace engine {

    Ref<Shader> GLFactory::newShader(const ShaderProps &shaderProps, VertexFormat* vertexFormat) {
        return createRef<GLShader>(shaderProps, vertexFormat);
    }

    Ref<engine::Shader> GLFactory::newShader(const ShaderProps &shaderProps) {
        return createRef<GLShader>(shaderProps);
    }

    Ref<engine::VertexBuffer> GLFactory::newVertexBuffer() {
        return createRef<GLVertexBuffer>();
    }

    Ref<VertexBuffer> GLFactory::newVertexBuffer(VertexFormat *vertexFormat) {
        return createRef<GLVertexBuffer>(vertexFormat);
    }

    Ref<IndexBuffer> GLFactory::newIndexBuffer() {
        return createRef<GLIndexBuffer>();
    }

    Ref<IndexBuffer> GLFactory::newIndexBuffer(const uint32_t &indexCount) {
        return createRef<GLIndexBuffer>(indexCount);
    }

    Ref<TextureBuffer> GLFactory::newTextureBuffer() {
        return createRef<GLTextureBuffer>();
    }

    Ref<VertexArray> GLFactory::newVertexArray() {
        return createRef<GLVertexArray>();
    }

    Ref<Drawer> GLFactory::newDrawer() {
        return createRef<GLDrawer>();
    }

    Ref<FrameBuffer> GLFactory::newFrameBuffer(const FramebufferSpecification &framebufferSpecification) {
        return createRef<GLFrameBuffer>(framebufferSpecification);
    }

    Ref<FrameBuffer> GLFactory::newFrameBuffer() {
        return createRef<GLFrameBuffer>();
    }

    Ref<UniformBuffer> GLFactory::newUniformBuffer(UniformBlockFormat* uniformBlockFormat) {
        return createRef<GLUniformBuffer>(uniformBlockFormat);
    }

    Ref<engine::UniformBuffer> GLFactory::newUniformBuffer() {
        return createRef<GLUniformBuffer>();
    }

}