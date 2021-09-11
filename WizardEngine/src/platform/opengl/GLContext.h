//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../graphics/GraphicsContext.h"

#include "render/GLRenderer.h"
#include "shader/GLShader.h"

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

        Renderer* newRenderer() override;

        Ref<Shader> newShader(const std::string &filepath) override;
        Ref<Shader> newShader(const std::string &name,
                              const std::string &vertexSrc,
                              const std::string &fragmentSrc) override;

        Ref<VertexBuffer> newVertexBuffer(const std::vector<Vertex> &vertices, const uint32_t &size) override;

        Ref<IndexBuffer> newIndexBuffer(const std::vector<int> &indices, const uint32_t &size) override;

    private:
        std::string getAPIName() override;
        const unsigned char *getVendor() override;
        const unsigned char *getRenderer() override;
        const unsigned char *getVersion() override;

    private:
        GLFWwindow* _window;

    };

}