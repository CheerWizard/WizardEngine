//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "TransformMatrix.h"

namespace engine {

    struct TransformComponent2d {
        TransformMatrix2d transformMatrix;

        TransformComponent2d(const char* name) :
        transformMatrix(TransformMatrix2d(name)) {}

        TransformComponent2d(const char* name,
                           const glm::fvec2 &position,
                           const float &rotation,
                           const glm::fvec2 &scale) :
                           transformMatrix(TransformMatrix2d(name, position, rotation, scale)) {}

        TransformComponent2d(const TransformMatrix2d &transformMatrix) :
        transformMatrix(transformMatrix) {}

    public:
        void applyChanges() {
            transformMatrix.applyChanges();
        }

    };

    struct TransformComponent3d {
        TransformMatrix3d transformMatrix;

        TransformComponent3d(const char* name) :
        transformMatrix(TransformMatrix3d(name)) {}

        TransformComponent3d(const char* name,
                             const glm::fvec3 &position,
                             const glm::fvec3 &rotation,
                             const glm::fvec3 &scale) :
                             transformMatrix(TransformMatrix3d(name, position, rotation, scale)) {}

         TransformComponent3d(const TransformMatrix3d &transformMatrix) :
         transformMatrix(transformMatrix) {}

    public:
        void applyChanges() {
            transformMatrix.applyChanges();
        }

    };

}
