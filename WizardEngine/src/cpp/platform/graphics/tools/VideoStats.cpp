//
// Created by mecha on 29.08.2022.
//

#include <platform/graphics/tools/VideoStats.h>
#include <glad/glad.h>

namespace engine::graphics {

    int VideoStats::getMaxSlots() {
        GLint slots;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &slots);
        return slots;
    }

}