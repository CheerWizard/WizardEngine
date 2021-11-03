//
// Created by mecha on 01.11.2021.
//

#include "GLRenderSettings.h"

#include <glad/glad.h>

namespace engine {

    void GLRenderSettings::enableCulling() {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }

    void GLRenderSettings::disableCulling() {
        glDisable(GL_CULL_FACE);
    }

    void GLRenderSettings::setPolygonMode() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + activePolygonMode);
    }

    const char* GLRenderSettings::getAPIName() {
        return "OpenGL";
    }

    const unsigned char *GLRenderSettings::getVendorName() {
        return glGetString(GL_VENDOR);
    }

    const unsigned char *GLRenderSettings::getRendererName() {
        return glGetString(GL_RENDERER);
    }

    const unsigned char *GLRenderSettings::getVersion() {
        return glGetString(GL_VERSION);
    }
}