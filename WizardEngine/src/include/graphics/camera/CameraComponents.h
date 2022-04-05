//
// Created by mecha on 26.10.2021.
//

#pragma once

#include <math/ViewProjections.h>

namespace engine::graphics {

    struct Camera2dComponent : math::ViewProjection2d {
        Camera2dComponent() : ViewProjection2d() {
            name = "camera";
        }
    };

    struct Camera3dComponent : math::ViewProjection3d {
        Camera3dComponent() : ViewProjection3d() {
            name = "camera";
        }

        Camera3dComponent(const float& aspectRatio) : ViewProjection3d(aspectRatio) {
            name = "camera";
        }
    };

}