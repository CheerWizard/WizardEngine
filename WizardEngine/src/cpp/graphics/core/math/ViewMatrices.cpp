//
// Created by mecha on 26.09.2021.
//

#include <graphics/core/math/ViewMatrices.h>

namespace engine {

    void ViewMatrices::update(ViewMatrix2d &viewMatrix2D) {
        viewMatrix2D.isUpdated = true;
        auto identity = glm::mat4(1);
        auto rotMat = glm::rotate(identity, viewMatrix2D.rotation, {0, 0, 1});
        viewMatrix2D.value = glm::translate(rotMat, -viewMatrix2D.position);
    }

    void ViewMatrices::update(ViewMatrix3d &viewMatrix3D) {
        viewMatrix3D.isUpdated = true;
        viewMatrix3D.position.isUpdated = true;
        auto yAxis = glm::vec3(0, 1, 0);
        auto zAxis = glm::vec3(0, 0, 1);
        const auto& pos = viewMatrix3D.position.value;
        viewMatrix3D.value = glm::lookAt(pos, pos  + zAxis, yAxis);
    }

}