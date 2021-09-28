//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../core/Memory.h"
#include "../core/Assert.h"

#include "render/RenderSystem.h"
#include "render/Drawer.h"

#include "shader/Shader.h"
#include "buffers/VertexArray.h"

#include "string"
#include "vector"

#define CREATE_GRAPHICS_OBJECT(shaderName) engine::createRef<engine::GraphicsObject>(engine::GraphicsObject { shaderName })

namespace engine {

    class GraphicsContext {

    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;

    public:
        void printInfo();

    public:
        virtual void onCreate() = 0;
        virtual void swapBuffers() = 0;
        virtual void clearDisplay() = 0;
        virtual void enableDepth() = 0;
        virtual void setViewPort(const uint32_t &width, const uint32_t &height) = 0;

        virtual Ref<Drawer> newDrawer() = 0;

        virtual Ref<RenderSystem> newRenderSystem2d() = 0;

        virtual Ref<RenderSystem> newRenderSystem2d(ShaderCache* shaderCache,
                                                  const Ref<VertexArray>& vertexArray,
                                                  const Ref<Drawer>& drawer) = 0;


        virtual Ref<RenderSystem> newRenderSystem3d() = 0;

        virtual Ref<RenderSystem> newRenderSystem3d(ShaderCache* shaderCache,
                                                    const Ref<VertexArray>& vertexArray,
                                                    const Ref<Drawer>& drawer) = 0;

        virtual Ref<VertexArray> newVertexArray(
                VertexBufferCache* vertexBufferCache,
                const Ref<IndexBuffer>& indexBuffer) = 0;

        virtual Ref<VertexBuffer> newVertexBuffer(VertexFormat* vertexFormat) = 0;

        virtual Ref<IndexBuffer> newIndexBuffer() = 0;
        virtual Ref<IndexBuffer> newIndexBuffer(const uint32_t &indexCount) = 0;

        virtual Ref<TextureBuffer> newTextureBuffer() = 0;

        virtual Ref<Shader> newShader(const ShaderProps& shaderProps,
                                      VertexFormat* vertexFormat) = 0;

    private:
        virtual std::string getAPIName() = 0;
        virtual const unsigned char* getVendorName() = 0;
        virtual const unsigned char* getRendererName() = 0;
        virtual const unsigned char* getVersion() = 0;

    };

}