//
// Created by mecha on 25.09.2021.
//

#include "ProjectionMatrix.h"

namespace engine {

    OrthographicMatrix& OrthographicMatrix::applyChanges() {
        value = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        isUpdated = true;
        return *this;
    }

    PerspectiveMatrix& PerspectiveMatrix::applyChanges() {
        value = glm::perspective(glm::radians(fieldOfView), aspectRatio, zNear, zFar);
        isUpdated = true;
        return *this;
    }

}