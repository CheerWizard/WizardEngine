//
// Created by mecha on 05.09.2021.
//

#include "GLDrawer.h"

#include "glad/glad.h"

namespace engine {

    void GLDrawer::drawIndices(const uint32_t &indexCount) {
        glDrawElements(GL_TRIANGLES, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

}