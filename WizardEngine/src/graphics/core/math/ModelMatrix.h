//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "../shader/Uniform.h"

#define DEFAULT_TRANSLATION_3D {0.5, 0.5, 0.5}
#define DEFAULT_ROTATION_3D {0, 0, 0}
#define DEFAULT_SCALE_3D {1, 1, 1}

#define DEFAULT_TRANSLATION_2D {0.5, 0.5}
#define DEFAULT_ROTATION_2D 0
#define DEFAULT_SCALE_2D {1, 1}

namespace engine {

    struct ModelMatrix3d : Mat4fUniform {
        glm::fvec3 position;
        glm::fvec3 rotation;
        glm::fvec3 scale;

        ModelMatrix3d(const char* name,
                        const glm::fvec3 &position = DEFAULT_TRANSLATION_3D,
                        const glm::fvec3 &rotation = DEFAULT_ROTATION_3D,
                        const glm::fvec3 &scale = DEFAULT_SCALE_3D) :
                        Mat4fUniform(name),
                        position(position),
                        rotation(rotation),
                        scale(scale) {
            applyChanges();
        }

        ModelMatrix3d& applyChanges() override;

    };

    struct ModelMatrix2d : Mat4fUniform {
        glm::fvec2 position;
        float rotation;
        glm::fvec2 scale;

        ModelMatrix2d(const char* name,
                          const glm::fvec2 &position = DEFAULT_TRANSLATION_2D,
                          const float &rotation = DEFAULT_ROTATION_2D,
                          const glm::fvec2 &scale = DEFAULT_SCALE_2D) :
                          Mat4fUniform(name),
                          position(position),
                          rotation(rotation),
                          scale(scale) {
            applyChanges();
        }

        ModelMatrix2d& applyChanges() override;

    };

}