//
// Created by mecha on 26.09.2021.
//

#include "ViewMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    ViewMatrix3d &ViewMatrix3d::applyChanges() {
//        auto identity = glm::mat4(1);
//        auto rotMatX = glm::rotate(identity, rotation.x, {1, 0, 0});
//        auto rotMatY = glm::rotate(rotMatX, rotation.y, {0, 1, 0});
//        auto rotMatZ = glm::rotate(rotMatY, rotation.z, {0, 0, 1});
//        auto translateMat = glm::translate(rotMatY, -position);
        auto yAxis = glm::vec3(0, 1, 0);
        auto zAxis = glm::vec3(0, 0, 1);
        value = glm::lookAt(position, position + zAxis, yAxis);
        isUpdated = true;
        return *this;
    }

    ViewMatrix2d &ViewMatrix2d::applyChanges() {
        auto identity = glm::mat4(1);
        auto rotMat = glm::rotate(identity, rotation, {0, 0, 1});
        auto viewMatrix = glm::translate(rotMat, -position);
        value = viewMatrix;
        isUpdated = true;
        return *this;
    }

}