//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../core/Memory.h"
#include "../core/Assert.h"

#include "render/Renderer.h"
#include "shader/Shader.h"
#include "buffers/VertexArray.h"

#include "string"
#include "vector"

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

        virtual Ref<Renderer> newRenderer(
                ShaderCache* shaderCache,
                GraphicsObjectCache* graphicsObjectCache,
                const Ref<VertexArray>& vertexArray) = 0;

        virtual Ref<VertexArray> newVertexArray(
                VertexBufferCache* vertexBufferCache,
                const Ref<IndexBuffer>& indexBuffer) = 0;

        virtual Ref<VertexBuffer> newVertexBuffer() = 0;
        virtual Ref<VertexBuffer> newVertexBuffer(Vertex* vertex) = 0;

        virtual Ref<IndexBuffer> newIndexBuffer() = 0;
        virtual Ref<IndexBuffer> newIndexBuffer(const uint32_t &indexCount) = 0;

        virtual Ref<Shader> newShader(const std::string& filepath) = 0;
        virtual Ref<Shader> newShader(
                const std::string& name,
                const std::string& vertexSrc,
                const std::string& fragmentSrc) = 0;

    private:
        virtual std::string getAPIName() = 0;
        virtual const unsigned char* getVendorName() = 0;
        virtual const unsigned char* getRendererName() = 0;
        virtual const unsigned char* getVersion() = 0;

    };

}