//
// Created by mecha on 26.10.2021.
//

#pragma once

#include <graphics/core/math/ViewProjections.h>

namespace engine {

    struct Camera2dComponent : ViewProjection2d {
        Camera2dComponent() : ViewProjection2d() {
            name = "camera";
        }
    };

    struct Camera3dComponent : ViewProjection3d {
        Camera3dComponent() : ViewProjection3d() {
            name = "camera";
        }

        Camera3dComponent(const float& aspectRatio) : ViewProjection3d(aspectRatio) {
            name = "camera";
        }
    };

}