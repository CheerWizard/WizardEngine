//
// Created by mecha on 23.09.2021.
//

#include <graphics/core/math/ModelMatrices.h>

namespace engine {

    void updateModel2d(ModelMatrix2d &modelMatrix2D) {
        modelMatrix2D.isUpdated = true;

        auto identity = glm::mat4(1);
        auto translationMatrix = glm::translate(identity, glm::vec3(modelMatrix2D.position, 1));
        auto rotationMatrix = glm::rotate(translationMatrix, modelMatrix2D.rotation, glm::vec3(0, 0, 1));
        modelMatrix2D.value = glm::scale(rotationMatrix, glm::vec3(modelMatrix2D.scale, 1));
    }

    void updateModel3d(ModelMatrix3d &modelMatrix3D) {
        modelMatrix3D.isUpdated = true;

        auto identity = glm::mat4(1);
        auto posMat = glm::translate(identity, modelMatrix3D.position);

        auto& rot = modelMatrix3D.rotation;
        auto rotMatX = glm::rotate(identity, rot.x, glm::vec3(1, 0, 0));
        auto rotMatY = glm::rotate(identity, rot.y, glm::vec3(0, 1, 0));
        auto rotMatZ = glm::rotate(identity, rot.z, glm::vec3(0, 0, 1));
        auto rotMat = rotMatZ * rotMatY * rotMatX;

        auto scaleMat = glm::scale(identity, modelMatrix3D.scale);

        modelMatrix3D.value = posMat * rotMat * scaleMat;
    }
}