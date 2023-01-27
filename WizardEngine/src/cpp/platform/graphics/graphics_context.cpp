#include <platform/graphics/graphics_context.h>
#include <core/Assert.h>

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace engine::graphics {

    void initContext(void *nativeWindow) {
        ENGINE_INFO("graphics::initContext()");
        glfwMakeContextCurrent((GLFWwindow*) nativeWindow);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ENGINE_ASSERT(status, "Failed to initialize Glad!");
    }

}