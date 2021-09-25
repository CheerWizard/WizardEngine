//
// Created by mecha on 25.09.2021.
//

#include "ProjectionMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    const OrthographicMatrix& OrthographicMatrix::applyChanges() {
        value = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        isUpdated = true;
        return *this;
    }

    const PerspectiveMatrix& PerspectiveMatrix::applyChanges() {
        value = glm::perspective(fieldOfView, aspectRatio, zNear, zFar);
        isUpdated = true;
        return *this;
    }

}