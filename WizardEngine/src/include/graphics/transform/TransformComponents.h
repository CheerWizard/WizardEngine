//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <math/ModelMatrices.h>
#include <ecs/ecs.h>

namespace engine::graphics {

    using namespace core;
    using namespace math;

    component(Transform2dComponent) {
        serializable()
        math::ModelMatrix2d modelMatrix;

        Transform2dComponent() {
            modelMatrix = math::ModelMatrix2d();
            modelMatrix.name = "transform";
        }
    };

    component(Transform3dComponent) {
        serializable()
        math::ModelMatrix3d modelMatrix;

        Transform3dComponent() {
            modelMatrix = math::ModelMatrix3d();
            modelMatrix.name = "transform";
        }

        Transform3dComponent(const vec3f &pos, const vec3f &rot, const vec3f &scale) {
            modelMatrix = math::ModelMatrix3d { pos, rot, scale };
            modelMatrix.name = "transform";
        }

        Transform3dComponent(const vec3f &scale) {
            modelMatrix = math::ModelMatrix3d();
            modelMatrix.scale = scale;
            modelMatrix.name = "transform";
        }
    };
}
