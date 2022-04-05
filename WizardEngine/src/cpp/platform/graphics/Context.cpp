//
// Created by mecha on 01.11.2021.
//

#include <platform/graphics/Context.h>
#include <core/Assert.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace engine::graphics {

    void initContext(void *nativeWindow) {
        ENGINE_INFO("createContext()");
        glfwMakeContextCurrent((GLFWwindow*) nativeWindow);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ENGINE_ASSERT(status, "Failed to initialize Glad!");
    }

}