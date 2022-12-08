//
// Created by mecha on 29.08.2022.
//

#include <platform/graphics/tools/VideoStats.h>
#include <glad/glad.h>

namespace engine::graphics {

    int VideoStats::getMaxTextureUnits() {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
        return max;
    }

    const char* VideoStats::getAPIName() {
        return "OpenGL";
    }

    const unsigned char* VideoStats::getVendorName() {
        return glGetString(GL_VENDOR);
    }

    const unsigned char* VideoStats::getRendererName() {
        return glGetString(GL_RENDERER);
    }

    const unsigned char *VideoStats::getVersion() {
        return glGetString(GL_VERSION);
    }

}