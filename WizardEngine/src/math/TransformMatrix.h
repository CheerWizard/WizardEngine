//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "Uniform.h"

#define DEFAULT_TRANSLATION_3D {0.5, 0.5, 0.5}
#define DEFAULT_ROTATION_3D {0, 0, 0}
#define DEFAULT_SCALE_3D {1, 1, 1}

#define DEFAULT_TRANSLATION_2D {0.5, 0.5}
#define DEFAULT_ROTATION_2D 0
#define DEFAULT_SCALE_2D {1, 1}

namespace engine {

    struct TransformMatrix3d : Mat4fUniform {
        glm::fvec3 position;
        glm::fvec3 rotation;
        glm::fvec3 scale;

        TransformMatrix3d(const char* name,
                        const glm::fvec3 &position = DEFAULT_TRANSLATION_3D,
                        const glm::fvec3 &rotation = DEFAULT_ROTATION_3D,
                        const glm::fvec3 &scale = DEFAULT_SCALE_3D) :
                        Mat4fUniform(name),
                        position(position),
                        rotation(rotation),
                        scale(scale) {
        }

        TransformMatrix3d& applyChanges() override;

    };

    struct TransformMatrix2d : Mat4fUniform {
        glm::fvec2 position;
        float rotation;
        glm::fvec2 scale;

        TransformMatrix2d(const char* name,
                          const glm::fvec2 &position = DEFAULT_TRANSLATION_2D,
                          const float &rotation = DEFAULT_ROTATION_2D,
                          const glm::fvec2 &scale = DEFAULT_SCALE_2D) :
                          Mat4fUniform(name),
                          position(position),
                          rotation(rotation),
                          scale(scale) {
        }

        TransformMatrix2d& applyChanges() override;

    };

}