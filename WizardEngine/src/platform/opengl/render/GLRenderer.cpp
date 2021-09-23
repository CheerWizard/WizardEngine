//
// Created by mecha on 05.09.2021.
//

#include "GLRenderer.h"

#include "glad/glad.h"

namespace engine {

    void GLRenderer::drawIndices(const uint32_t &indexCount) {
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

}