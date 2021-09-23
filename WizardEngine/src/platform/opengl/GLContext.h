//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../graphics/GraphicsContext.h"

#include "render/GLRenderer.h"
#include "shader/GLShader.h"

#include "buffers/GLVertexBuffer.h"
#include "buffers/GLIndexBuffer.h"
#include "buffers/GLVertexArray.h"

struct GLFWwindow;

namespace engine {

    class GLContext : public GraphicsContext {

    public:
        explicit GLContext(GLFWwindow* window);
        ~GLContext() override = default;

    public:
        void onCreate() override;
        void swapBuffers() override;
        void clearDisplay() override;

        Ref<engine::Renderer> newRenderer(
                ShaderCache *shaderCache,
                GraphicsObjectCache *graphicsObjectCache,
                const Ref<engine::VertexArray> &vertexArray) override;

        Ref<engine::VertexArray> newVertexArray(
                VertexBufferCache *vertexBufferCache,
                const Ref<engine::IndexBuffer> &indexBuffer) override;

        Ref<VertexBuffer> newVertexBuffer(Vertex *vertex) override;

        Ref<IndexBuffer> newIndexBuffer() override;
        Ref<IndexBuffer> newIndexBuffer(const uint32_t &indexCount) override;

        Ref<engine::Shader> newShader(const ShaderProps &shaderProps, Vertex* vertex) override;

    private:
        std::string getAPIName() override;
        const unsigned char *getVendorName() override;
        const unsigned char *getRendererName() override;
        const unsigned char *getVersion() override;

    private:
        GLFWwindow* _window;

    };

}