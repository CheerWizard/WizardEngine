//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <math/ModelMatrices.h>

namespace engine::graphics {

    component(Transform2dComponent) {
        math::ModelMatrix2d modelMatrix;

        Transform2dComponent() {
            modelMatrix = math::ModelMatrix2d();
            modelMatrix.name = "transform";
        }
    };

    component(Transform3dComponent) {
        math::ModelMatrix3d modelMatrix;

        Transform3dComponent() {
            modelMatrix = math::ModelMatrix3d();
            modelMatrix.name = "transform";
        }

        Transform3dComponent(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale) {
            modelMatrix = math::ModelMatrix3d { pos, rot, scale };
            modelMatrix.name = "transform";
        }

        Transform3dComponent(const glm::vec3 &scale) {
            modelMatrix = math::ModelMatrix3d();
            modelMatrix.scale = scale;
            modelMatrix.name = "transform";
        }
    };
}
