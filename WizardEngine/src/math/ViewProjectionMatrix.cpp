//
// Created by mecha on 23.09.2021.
//

#include "ViewProjectionMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    const glm::fmat4& ViewProjectionMatrix::applyChanges() {
        auto translationMatrix = glm::translate(glm::mat4(1.0f), position);
        auto rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1));
        auto scaledMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
        auto modelMatrix = translationMatrix * rotationMatrix * scaledMatrix;
        auto viewMatrix = glm::inverse(modelMatrix);

        value = projectionMatrix.applyChanges() * viewMatrix;
        return value;
    }

    const glm::fmat4& OrthographicMatrix::applyChanges() {
        value = glm::ortho(left, right, bottom, top, -1.0f, -1.0f);
        return value;
    }

    const glm::fmat4& PerspectiveMatrix::applyChanges() {
        value = glm::perspective(fieldOfView, aspectRatio, zNear, zFar);
        return value;
    }
}