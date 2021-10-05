//
// Created by mecha on 05.09.2021.
//

#include <glad/glad.h>

#include "GLContext.h"
#include "GLFactory.h"

#include "GLFW/glfw3.h"

namespace engine {

    GLContext::GLContext(void* nativeWindow) : _window((GLFWwindow*) nativeWindow) {
        ENGINE_ASSERT(nativeWindow, "Window is null!")
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

    Ref<GraphicsFactory> GLContext::newGraphicsFactory() {
        return createRef<GLFactory>();
    }

}