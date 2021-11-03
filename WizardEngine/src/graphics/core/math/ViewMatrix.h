//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../shader/Uniform.h"

#define DEFAULT_VIEW_POS_3D {0, 0, 1}
#define DEFAULT_VIEW_ROT_3D {0, 0, 0}
#define DEFAULT_VIEW_POS_2D {0, 0, 1}

namespace engine {

    struct ViewMatrix3d : Mat4fUniform {
        glm::vec3 position;
        glm::vec3 rotation;
        float scale;

        ViewMatrix3d(
                const char* name,
                const glm::vec3 &position = DEFAULT_VIEW_POS_3D,
                const glm::vec3 &rotation = DEFAULT_VIEW_ROT_3D,
                const float &scale = 1
        ) : Mat4fUniform(name), position(position), rotation(rotation), scale(scale) {}

        ViewMatrix3d& applyChanges() override;

    };

    struct ViewMatrix2d : Mat4fUniform {
        glm::vec3 position;
        float rotation;

        ViewMatrix2d(
                const char* name,
                const glm::vec3 &position = DEFAULT_VIEW_POS_2D,
                const float &rotation = 0
        ) : Mat4fUniform(name), position(position), rotation(rotation) {}

       ViewMatrix2d& applyChanges() override;

    };

}