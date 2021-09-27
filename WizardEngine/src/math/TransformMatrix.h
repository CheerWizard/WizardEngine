//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "Uniform.h"

#define DEFAULT_TRANSLATION {0.5, 0.5, 0.5}
#define DEFAULT_ROTATION {0, 0, 0}
#define DEFAULT_SCALE {1, 1, 1}

namespace engine {

    struct TransformMatrix : Mat4fUniform {
        glm::fvec3 position;
        glm::fvec3 rotation;
        glm::fvec3 scale;

        TransformMatrix(const char* name,
                        const glm::fvec3 &position = DEFAULT_TRANSLATION,
                        const glm::fvec3 &rotation = DEFAULT_ROTATION,
                        const glm::fvec3 &scale = DEFAULT_SCALE) :
                        Mat4fUniform(name),
                        position(position),
                        rotation(rotation),
                        scale(scale) {
        }

        const TransformMatrix& applyChanges() override;

    };

}