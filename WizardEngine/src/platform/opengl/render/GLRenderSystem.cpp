//
// Created by mecha on 05.09.2021.
//

#include "GLRenderSystem.h"

#include "glad/glad.h"

namespace engine {

    void GLRenderSystem::drawIndices(const uint32_t &indexCount) {
        glDrawElements(GL_TRIANGLES, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

}