//
// Created by mecha on 01.11.2021.
//

#include "GLModule.h"
#include "GLFactory.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace engine {

    void GLModule::createContext(void* nativeWindow) {
        ENGINE_INFO("createContext()");
        newGraphicsFactory();
        glfwMakeContextCurrent((GLFWwindow*) nativeWindow);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ENGINE_ASSERT(status, "Failed to initialize Glad!");
    }

    Ref<GraphicsFactory> GLModule::newGraphicsFactory() {
        graphicsFactory = createRef<GLFactory>();
        return graphicsFactory;
    }

}