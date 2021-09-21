//
// Created by mecha on 05.09.2021.
//

#include "GLRenderer.h"

#include "glad/glad.h"

namespace engine {

    void GLRenderer::drawIndices(uint32_t *indices, const uint32_t &indexCount) {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indices);
    }

}