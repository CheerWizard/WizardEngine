//
// Created by mecha on 29.10.2021.
//

#include <graphics/camera/Camera.h>

namespace engine {

    void Camera2D::create() {
        add<Camera2dComponent>(Camera2dComponent());
    }

    void Camera3D::create(const float& aspectRatio) {
        add<Camera3dComponent>(Camera3dComponent(aspectRatio));
    }

}