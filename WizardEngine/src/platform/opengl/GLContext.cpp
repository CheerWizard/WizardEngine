//
// Created by mecha on 05.09.2021.
//

#include "GLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace engine {

    GLContext::GLContext(GLFWwindow *window) : _window(window) {
        ENGINE_ASSERT(window, "Window is null!")
    }

    void GLContext::onCreate() {
        ENGINE_INFO("onCreate()");

        glfwMakeContextCurrent(_window);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ENGINE_ASSERT(status, "Failed to initialize Glad!")
    }

    void GLContext::swapBuffers() {
        ENGINE_INFO("swapBuffers()");
        glfwSwapBuffers(_window);
    }

    void GLContext::clearDisplay() {
        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    std::string GLContext::getAPIName() {
        return "OpenGL";
    }

    const unsigned char *GLContext::getVendor() {
        return glGetString(GL_VENDOR);
    }

    const unsigned char *GLContext::getRenderer() {
        return glGetString(GL_RENDERER);
    }

    const unsigned char *GLContext::getVersion() {
        return glGetString(GL_VERSION);
    }

    Renderer* GLContext::newRenderer() {
        return new GLRenderer();
    }

    Ref<Shader> GLContext::newShader(const std::string& filepath) {
        return createRef<GLShader>(filepath);
    }

    Ref<Shader> GLContext::newShader(const std::string &name,
                                     const std::string &vertexSrc,
                                     const std::string &fragmentSrc) {
        return createRef<GLShader>(name, vertexSrc, fragmentSrc);
    }

    Ref<VertexBuffer> GLContext::newVertexBuffer(const std::vector<Vertex> &vertices, const uint32_t &size) {
        return createRef<VertexBuffer>(vertices, size);
    }

    Ref<IndexBuffer> GLContext::newIndexBuffer(const std::vector<int> &indices, const uint32_t &size) {
        return createRef<IndexBuffer>(indices, size);
    }

}