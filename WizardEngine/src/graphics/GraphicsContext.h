//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../core/Memory.h"
#include "../core/Assert.h"

#include "render/Renderer.h"
#include "shader/Shader.h"
#include "buffers/VertexBuffer.h"
#include "buffers/IndexBuffer.h"

#include <string>
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

        virtual Renderer* newRenderer() = 0;

        virtual Ref<Shader> newShader(const std::string& filepath) = 0;
        virtual Ref<Shader> newShader(
                const std::string& name,
                const std::string& vertexSrc,
                const std::string& fragmentSrc) = 0;

        virtual Ref<VertexBuffer> newVertexBuffer(
                const std::vector<Vertex>& vertices ,
                const uint32_t& size) = 0;

        virtual Ref<IndexBuffer> newIndexBuffer(
                const std::vector<int>& indices ,
                const uint32_t& size) = 0;

    private:
        virtual std::string getAPIName() = 0;
        virtual const unsigned char* getVendor() = 0;
        virtual const unsigned char* getRenderer() = 0;
        virtual const unsigned char* getVersion() = 0;

    };

}