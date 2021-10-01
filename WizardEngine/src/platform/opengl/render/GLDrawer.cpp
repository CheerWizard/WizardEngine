//
// Created by mecha on 05.09.2021.
//

#include "GLDrawer.h"

#include "glad/glad.h"

#include "../../../core/Logger.h"

namespace engine {

    void GLDrawer::drawIndices(const uint32_t &indexCount) {
        ENGINE_INFO("drawIndices()");
        glDrawElements(GL_TRIANGLES, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void GLDrawer::enableDepth() {
        ENGINE_INFO("enableDepth()");
        glEnable(GL_DEPTH_TEST);
    }

    void GLDrawer::setViewPort(const uint32_t &width, const uint32_t &height) {
        ENGINE_INFO("setViewPort()");
        glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    }

    void GLDrawer::disableDepth() {
        glDisable(GL_DEPTH_TEST);
    }

    void GLDrawer::clearDepth(const Color &color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLDrawer::clearColor(const Color &color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

}