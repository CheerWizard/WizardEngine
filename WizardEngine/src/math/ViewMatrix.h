//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "Uniform.h"

#define DEFAULT_VIEW_POS {0, 0, 1}
#define DEFAULT_VIEW_ROT {0, 0, 0}

namespace engine {

    struct ViewMatrix : Mat4fUniform {
        glm::vec3 position;
        glm::vec3 rotation;

        ViewMatrix(const char* name,
                const glm::vec3 &position = DEFAULT_VIEW_POS,
                const glm::vec3 &rotation = DEFAULT_VIEW_ROT) :
                Mat4fUniform(name),
                position(position),
                rotation(rotation)
                {}

        const ViewMatrix &applyChanges() override;

    };

}