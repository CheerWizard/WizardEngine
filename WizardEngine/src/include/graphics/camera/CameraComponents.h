//
// Created by mecha on 26.10.2021.
//

#pragma once

#include <graphics/core/math/ViewProjections.h>

#define DEFAULT_CAMERA_NAME "camera"

namespace engine {

    typedef ViewProjection2d Camera2dComponent;
    typedef ViewProjection3d Camera3dComponent;

    class CameraComponents {

    public:
        // stack allocated cameras!
        static Camera2dComponent camera2D() {
            auto camera = Camera2dComponent {};
            camera.name = DEFAULT_CAMERA_NAME;
            return camera;
        }

        static Camera3dComponent camera3D(const float& aspectRatio) {
            auto camera = Camera3dComponent {};
            camera.name = DEFAULT_CAMERA_NAME;
            camera.perspectiveMatrix.aspectRatio = aspectRatio;
            return camera;
        }

        // heap allocated cameras!
        static Camera2dComponent* newCamera2D() {
            auto* camera = new Camera2dComponent {};
            camera->name = DEFAULT_CAMERA_NAME;
            return camera;
        }

        static Camera3dComponent* newCamera3D(const float& aspectRatio) {
            auto* camera = new Camera3dComponent {};
            camera->name = DEFAULT_CAMERA_NAME;
            camera->perspectiveMatrix.aspectRatio = aspectRatio;
            return camera;
        }
    };
}