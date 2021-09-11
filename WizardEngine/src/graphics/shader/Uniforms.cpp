//
// Created by mecha on 06.09.2021.
//

#include "Uniforms.h"
#include "glad/glad.h"

#include "glm/glm.hpp"

namespace engine {

    void func() {
        auto uniform = Uniform<float>("light", 0.5f);
        auto location = glGetUniformLocation(1, uniform.name);
        auto uniform2 = Uniform<glm::vec4>("light", glm::vec4());
        glUniform1f(location, uniform.value);
        glUniform4f(location, uniform2.value.x, uniform2.value.y, uniform2.value.z, uniform2.value.w);
    }

}