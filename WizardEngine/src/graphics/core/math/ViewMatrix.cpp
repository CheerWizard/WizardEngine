//
// Created by mecha on 26.09.2021.
//

#include "ViewMatrix.h"

namespace engine {

    ViewMatrix3d &ViewMatrix3d::applyChanges() {
        auto yAxis = glm::vec3(0, 1, 0);
        auto zAxis = glm::vec3(0, 0, 1);
        position.applyChanges();
        value = glm::lookAt(position.value, position.value + zAxis, yAxis);
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