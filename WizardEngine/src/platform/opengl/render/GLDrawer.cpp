//
// Created by mecha on 05.09.2021.
//

#include "GLDrawer.h"

#include "glad/glad.h"

#include "../../../core/Logger.h"

namespace engine {

    void GLDrawer::drawQuadsIndices(const uint32_t &indexCount) {
        ENGINE_INFO("drawQuadsIndices()");
        glDrawElements(GL_QUADS, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void GLDrawer::drawByIndices(const uint32_t &indexCount) {
        ENGINE_INFO("drawByIndices()");
        glDrawElements(GL_TRIANGLES, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void GLDrawer::enableDepth() {
        ENGINE_INFO("enableDepth()");
        glEnable(GL_DEPTH_TEST);
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

    void GLDrawer::drawIndicesInstanced(const uint32_t &indexCount, const uint32_t &instanceCount) {
        glDrawElementsInstanced(GL_TRIANGLES, (GLsizei) indexCount, GL_UNSIGNED_INT, 0, instanceCount);
    }

    void GLDrawer::multiDrawIndices(const uint32_t &totalIndexCount, const uint32_t &drawCount) {
//        glMultiDrawElements(GL_TRIANGLES, totalIndexCount, GL_UNSIGNED_INT, 0, drawCount);
    }

    void GLDrawer::enableCulling() {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }

    void GLDrawer::setPolygonMode(const PolygonMode &polygonMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + polygonMode);
    }

}