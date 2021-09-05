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

}