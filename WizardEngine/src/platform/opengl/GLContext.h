//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../graphics/GraphicsContext.h"

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
        void enableDepth() override;

        Ref<VertexArray> newVertexArray(
                VertexBufferCache *vertexBufferCache,
                const Ref<IndexBuffer> &indexBuffer) override;

        Ref<IndexBuffer> newIndexBuffer() override;
        Ref<IndexBuffer> newIndexBuffer(const uint32_t &indexCount) override;

        Ref<VertexBuffer> newVertexBuffer(VertexFormat *vertexFormat) override;

        Ref<Shader> newShader(const ShaderProps &shaderProps, VertexFormat *vertexFormat) override;

        Ref<TextureBuffer> newTextureBuffer() override;

        Ref<engine::Drawer> newDrawer() override;

        Ref<RenderSystem> newRenderSystem2d() override;
        Ref<RenderSystem> newRenderSystem2d(ShaderCache *shaderCache, const Ref<VertexArray> &vertexArray,
                                            const Ref<Drawer> &drawer) override;

        Ref<RenderSystem> newRenderSystem3d() override;
        Ref<RenderSystem> newRenderSystem3d(ShaderCache *shaderCache, const Ref<VertexArray> &vertexArray,
                                            const Ref<Drawer> &drawer) override;

        void setViewPort(const uint32_t &width, const uint32_t &height) override;

        Ref<Shader> newShader(const ShaderProps &shaderProps) override;

    private:
        std::string getAPIName() override;
        const unsigned char *getVendorName() override;
        const unsigned char *getRendererName() override;
        const unsigned char *getVersion() override;

    private:
        GLFWwindow* _window;

    };

}