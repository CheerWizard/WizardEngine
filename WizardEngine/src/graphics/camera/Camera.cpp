//
// Created by mecha on 29.10.2021.
//

#include "Camera.h"

namespace engine {

    void Camera2D::create() {
        add<Camera2dComponent>(CameraComponents::camera2D());
    }

    void Camera3D::create(const float& aspectRatio) {
        add<Camera3dComponent>(CameraComponents::camera3D(aspectRatio));
    }

}