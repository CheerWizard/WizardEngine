//
// Created by mecha on 05.09.2021.
//

#include "GLContext.h"

#include "GLFW/glfw3.h"

namespace engine {

    GLContext::GLContext(GLFWwindow *window) : _window(window) {
        ENGINE_ASSERT(window, "Window is null!")
    }

    void GLContext::onCreate() {
        ENGINE_INFO("create()");

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

    const unsigned char *GLContext::getVendorName() {
        return glGetString(GL_VENDOR);
    }

    const unsigned char *GLContext::getRendererName() {
        return glGetString(GL_RENDERER);
    }

    const unsigned char *GLContext::getVersion() {
        return glGetString(GL_VERSION);
    }

    Ref<Shader> GLContext::newShader(const ShaderProps &shaderProps, Vertex* vertex) {
        return createRef<GLShader>(shaderProps, vertex);
    }

    Ref<VertexBuffer> GLContext::newVertexBuffer(Vertex *vertex) {
        return createRef<GLVertexBuffer>(vertex);
    }

    Ref<IndexBuffer> GLContext::newIndexBuffer() {
        return createRef<GLIndexBuffer>();
    }

    Ref<IndexBuffer> GLContext::newIndexBuffer(const uint32_t &indexCount) {
        return createRef<GLIndexBuffer>(indexCount);
    }

    Ref<Renderer> GLContext::newRenderer(ShaderCache *shaderCache, GraphicsObjectCache *graphicsObjectCache,
                                         const Ref<VertexArray> &vertexArray) {
        return createRef<GLRenderer>(shaderCache, graphicsObjectCache, vertexArray);
    }

    Ref<VertexArray>
    GLContext::newVertexArray(VertexBufferCache *vertexBufferCache, const Ref<IndexBuffer> &indexBuffer) {
        return createRef<GLVertexArray>(vertexBufferCache, indexBuffer);
    }

}