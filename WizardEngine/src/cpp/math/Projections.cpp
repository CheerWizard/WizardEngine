//
// Created by mecha on 25.09.2021.
//

#include <math/Projections.h>

namespace engine::math {

    void Projections::update(OrthographicMatrix &orthographicMatrix) {
        orthographicMatrix.isUpdated = true;
        orthographicMatrix.value = glm::ortho(
                orthographicMatrix.left,
                orthographicMatrix.right,
                orthographicMatrix.bottom,
                orthographicMatrix.top,
                -1.0f,
                1.0f
        );
    }

    void Projections::update(PerspectiveMatrix &perspectiveMatrix) {
        perspectiveMatrix.isUpdated = true;
        perspectiveMatrix.value = glm::perspective(
                glm::radians(perspectiveMatrix.fieldOfView),
                perspectiveMatrix.aspectRatio,
                perspectiveMatrix.zNear,
                perspectiveMatrix.zFar
        );
    }

}