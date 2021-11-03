//
// Created by mecha on 05.09.2021.
//

#include "GLDrawer.h"

#include "glad/glad.h"

#include "../../core/Logger.h"

namespace engine {

    void GLDrawer::drawQuadsIndices(const uint32_t &indexCount) {
        ENGINE_INFO("drawQuadsIndices()");
        glDrawElements(GL_QUADS, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void GLDrawer::drawElements(const uint32_t &indexCount) {
        ENGINE_INFO("drawElements(indexCount : {0})", indexCount);
        glDrawElements(GL_TRIANGLES, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void GLDrawer::drawElements(const uint32_t &indexCount, const uint32_t &instanceCount) {
        ENGINE_INFO("drawElements(indexCount : {0}, instanceCount : {1})", indexCount, instanceCount);
        glDrawElementsInstanced(GL_TRIANGLES, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr, (GLsizei)instanceCount);
    }

}