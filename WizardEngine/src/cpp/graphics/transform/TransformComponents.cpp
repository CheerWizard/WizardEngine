//
// Created by mecha on 27.10.2021.
//

#include <graphics/transform/TransformComponents.h>

namespace engine::graphics {

    Transform2dComponent transform2d() {
        math::ModelMatrix2d transform = {};
        transform.name = DEFAULT_TRANSFORM_NAME;
        updateModel2d(transform);
        return transform;
    }

    Transform3dComponent transform3d() {
        math::ModelMatrix3d transform = {};
        transform.name = DEFAULT_TRANSFORM_NAME;
        updateModel3d(transform);
        return transform;
    }

    Transform3dComponent transform3d(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale) {
        math::ModelMatrix3d transform = {pos, rot, scale};
        transform.name = DEFAULT_TRANSFORM_NAME;
        updateModel3d(transform);
        return transform;
    }

    Transform3dComponent transform3dScale(const glm::vec3 &scale) {
        math::ModelMatrix3d transform = {};
        transform.scale = scale;
        transform.name = DEFAULT_TRANSFORM_NAME;
        updateModel3d(transform);
        return transform;
    }
}
