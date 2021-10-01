//
// Created by mecha on 05.09.2021.
//

#include "GLContext.h"

#include "render/GLDrawer.h"
#include "shader/GLShader.h"

#include "buffers/GLVertexBuffer.h"
#include "buffers/GLIndexBuffer.h"
#include "buffers/GLVertexArray.h"
#include "buffers/GLTextureBuffer.h"

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
        ENGINE_INFO("clearDisplay()");
        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLContext::enableDepth() {
        ENGINE_INFO("enableDepth()");
        glEnable(GL_DEPTH_TEST);
//        glEnable(GL_CULL_FACE);
//        glCullFace(GL_BACK);
    }

    void GLContext::setViewPort(const uint32_t &width, const uint32_t &height) {
        ENGINE_INFO("setViewPort()");
        glViewport(0, 0, width, height);
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

    Ref<Shader> GLContext::newShader(const ShaderProps &shaderProps, VertexFormat* vertexFormat) {
        return createRef<GLShader>(shaderProps, vertexFormat);
    }

    Ref<engine::Shader> GLContext::newShader(const ShaderProps &shaderProps) {
        return createRef<GLShader>(shaderProps);
    }

    Ref<VertexBuffer> GLContext::newVertexBuffer(VertexFormat *vertexFormat) {
        return createRef<GLVertexBuffer>(vertexFormat);
    }

    Ref<IndexBuffer> GLContext::newIndexBuffer() {
        return createRef<GLIndexBuffer>();
    }

    Ref<IndexBuffer> GLContext::newIndexBuffer(const uint32_t &indexCount) {
        return createRef<GLIndexBuffer>(indexCount);
    }

    Ref<TextureBuffer> GLContext::newTextureBuffer() {
        return createRef<GLTextureBuffer>();
    }

    Ref<VertexArray> GLContext::newVertexArray(VertexBufferCache *vertexBufferCache,
                                               const Ref<IndexBuffer> &indexBuffer) {
        return createRef<GLVertexArray>(vertexBufferCache, indexBuffer);
    }

    Ref<Drawer> GLContext::newDrawer() {
        return createRef<GLDrawer>();
    }

    Ref<RenderSystem> GLContext::newRenderSystem2d() {
        auto indexBuffer = newIndexBuffer();
        auto vertexBufferCache = new VertexBufferCache();
        auto shaderCache = new ShaderCache();
        auto vertexArray = newVertexArray(vertexBufferCache, indexBuffer);
        auto drawer = newDrawer();
        return newRenderSystem2d(shaderCache, vertexArray, drawer);
    }

    Ref<RenderSystem> GLContext::newRenderSystem2d(ShaderCache *shaderCache, const Ref<VertexArray> &vertexArray,
                                                   const Ref<Drawer> &drawer) {
        return createRef<RenderSystem2d>(shaderCache, vertexArray, drawer);
    }

    Ref<RenderSystem> GLContext::newRenderSystem3d() {
        auto indexBuffer = newIndexBuffer();
        auto vertexBufferCache = new VertexBufferCache();
        auto shaderCache = new ShaderCache();
        auto vertexArray = newVertexArray(vertexBufferCache, indexBuffer);
        auto drawer = newDrawer();
        return newRenderSystem3d(shaderCache, vertexArray, drawer);
    }

    Ref<RenderSystem> GLContext::newRenderSystem3d(ShaderCache *shaderCache, const Ref<VertexArray> &vertexArray,
                                                   const Ref<Drawer> &drawer) {
        return createRef<RenderSystem3d>(shaderCache, vertexArray, drawer);
    }

}