//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "TransformMatrix.h"

namespace engine {

    struct TransformComponent {
        TransformMatrix transformMatrix;

        TransformComponent(const char* name) :
        transformMatrix(TransformMatrix(name)) {}

        TransformComponent(const char* name,
                           const glm::fvec3 &position,
                           const glm::fvec3 &rotation,
                           const glm::fvec3 &scale) : transformMatrix(TransformMatrix(name, position, rotation, scale)) {}

        TransformComponent(const TransformMatrix &transformMatrix) :
        transformMatrix(transformMatrix) {}

    public:
        void applyChanges() {
            transformMatrix.applyChanges();
        }

    };

}
