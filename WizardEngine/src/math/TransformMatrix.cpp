//
// Created by mecha on 23.09.2021.
//

#include "TransformMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    TransformMatrix3d &TransformMatrix3d::applyChanges() {
        auto identity = glm::mat4(1);

        auto posMat = glm::translate(identity, position);

        auto rotMatX = glm::rotate(identity, rotation.x, glm::vec3(1, 0, 0));
        auto rotMatY = glm::rotate(identity, rotation.y, glm::vec3(0, 1, 0));
        auto rotMatZ = glm::rotate(identity, rotation.z, glm::vec3(0, 0, 1));
        auto rotMat = rotMatZ * rotMatY * rotMatX;

        auto scaleMat = glm::scale(identity, scale);

        auto modelMatrix = posMat * rotMat * scaleMat;
        value = modelMatrix;
        isUpdated = true;
        return *this;
    }

    TransformMatrix2d &TransformMatrix2d::applyChanges() {
        auto identity = glm::mat4(1);
        auto translationMatrix = glm::translate(identity, glm::vec3(position, 1));
        auto rotationMatrix = glm::rotate(translationMatrix, rotation, glm::vec3(0, 0, 1));
        auto modelMatrix = glm::scale(rotationMatrix, glm::vec3(scale, 1));
        value = modelMatrix;
        isUpdated = true;
        return *this;
    }

}