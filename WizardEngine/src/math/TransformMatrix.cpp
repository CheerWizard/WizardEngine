//
// Created by mecha on 23.09.2021.
//

#include "TransformMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    const TransformMatrix &TransformMatrix::applyChanges() {
        auto translationMatrix = glm::translate(glm::mat4(1), position);
        auto rotationMatrixX = glm::rotate(translationMatrix, rotation.x, glm::vec3(1, 0, 0));
        auto rotationMatrixY = glm::rotate(rotationMatrixX, rotation.y, glm::vec3(0, 1, 0));
        auto rotationMatrixZ = glm::rotate(rotationMatrixY, rotation.z, glm::vec3(0, 0, 1));
        auto modelMatrix = glm::scale(rotationMatrixZ, scale);
        value = modelMatrix;
        isUpdated = true;
        return *this;
    }

}