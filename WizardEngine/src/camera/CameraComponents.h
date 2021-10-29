//
// Created by mecha on 26.10.2021.
//

#pragma once

#include "../math/ViewProjectionMatrix.h"

#define DEFAULT_CAMERA_NAME "camera"

namespace engine {

    typedef ViewProjection2d Camera2dComponent;
    typedef ViewProjection3d Camera3dComponent;

    class CameraComponents {

    public:
        // stack allocated cameras!
        inline static Camera2dComponent camera2D() {
            return {
                DEFAULT_CAMERA_NAME,
                ViewMatrix2d {
                        nullptr
                },
                OrthographicMatrix {
                    ""
                }
            };
        }

        inline static Camera3dComponent camera3D(const float& aspectRatio) {
            return {
                DEFAULT_CAMERA_NAME,
                ViewMatrix3d {
                nullptr
                },
                PerspectiveMatrix {
                nullptr,
                aspectRatio
                }
            };
        }

        // heap allocated cameras!
        inline static Camera2dComponent* newCamera2D() {
            return new Camera2dComponent {
                DEFAULT_CAMERA_NAME,
                ViewMatrix2d {
                    nullptr
                    },
                    OrthographicMatrix {
                    ""
                }
            };
        }

        inline static Camera3dComponent* newCamera3D(const float& aspectRatio) {
            return new Camera3dComponent {
                DEFAULT_CAMERA_NAME,
                ViewMatrix3d {
                    nullptr
                    },
                    PerspectiveMatrix {
                    nullptr,
                    aspectRatio
                }
            };
        }
    };
}