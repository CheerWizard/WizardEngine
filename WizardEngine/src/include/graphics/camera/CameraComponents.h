//
// Created by mecha on 26.10.2021.
//

#pragma once

#include <math/ViewProjections.h>

namespace engine::graphics {

    component(Camera2dComponent) {
        math::ViewProjection2d viewProjection;
        Camera2dComponent() {
            viewProjection = math::ViewProjection2d();
            viewProjection.name = "camera";
        }
    };

    component(Camera3dComponent) {
        math::ViewProjection3d viewProjection;

        Camera3dComponent() {
            viewProjection = math::ViewProjection3d();
            viewProjection.name = "camera";
        }

        Camera3dComponent(float aspectRatio) {
            viewProjection = math::ViewProjection3d(aspectRatio);
            viewProjection.name = "camera";
        }
    };

}