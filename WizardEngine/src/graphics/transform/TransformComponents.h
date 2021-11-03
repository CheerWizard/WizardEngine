//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../core/math/ModelMatrix.h"

#define DEFAULT_TRANSFORM_NAME "transform"

namespace engine {

    typedef ModelMatrix2d Transform2dComponent;
    typedef ModelMatrix3d Transform3dComponent;

    class TransformComponents {

    private:
        TransformComponents() = default;
        ~TransformComponents() = default;

    public:
        inline static Transform2dComponent newTransform2d() {
            return { DEFAULT_TRANSFORM_NAME };
        }

        inline static Transform3dComponent newTransform3d() {
            return { DEFAULT_TRANSFORM_NAME };
        }

        inline static Transform3dComponent newTransform3d(const glm::vec3 &pos,
                                                          const glm::vec3 &rot,
                                                          const glm::vec3 &scale) {
            return { DEFAULT_TRANSFORM_NAME, pos, rot, scale };
        }
    };

}
