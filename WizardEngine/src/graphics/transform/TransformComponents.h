//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../core/math/ModelMatrices.h"

#define DEFAULT_TRANSFORM_NAME "transform"

namespace engine {

    typedef ModelMatrix2d Transform2dComponent;
    typedef ModelMatrix3d Transform3dComponent;

    Transform2dComponent transform2d();
    Transform3dComponent transform3d();
    Transform3dComponent transform3d(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale);
}
