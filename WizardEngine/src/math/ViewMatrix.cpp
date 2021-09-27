//
// Created by mecha on 26.09.2021.
//

#include "ViewMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    const ViewMatrix &ViewMatrix::applyChanges() {
        auto identity = glm::mat4(1);
        auto rotMatX = glm::rotate(identity, rotation.x, {1, 0, 0});
        auto rotMatY = glm::rotate(rotMatX, rotation.y, {0, 1, 0});
        auto rotMatZ = glm::rotate(rotMatY, rotation.z, {0, 0, 1});
        auto posMat = glm::translate(rotMatZ, -position);
        value = posMat;
        isUpdated = true;
        return *this;
    }

}