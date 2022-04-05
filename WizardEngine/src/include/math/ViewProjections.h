//
// Created by mecha on 23.09.2021.
//

#pragma once

#include <math/ViewMatrices.h>
#include <math/Projections.h>

namespace engine::math {

    struct ViewProjection2d;
    struct ViewProjection3d;

    class ViewProjections final {

    private:
        ViewProjections() = default;
        ~ViewProjections() = default;

    public:
        static void update(ViewProjection2d &viewProjection2D);
        static void update(ViewProjection3d &viewProjection3D);

    };

    struct ViewProjection2d : Mat4fUniform {
        ViewMatrix2d viewMatrix;
        OrthographicMatrix orthographicMatrix;

        ViewProjection2d() {
            ViewProjections::update(*this);
        }
    };

    struct ViewProjection3d : Mat4fUniform {
        ViewMatrix3d viewMatrix;
        PerspectiveMatrix perspectiveMatrix;

        ViewProjection3d() {
            ViewProjections::update(*this);
        }

        ViewProjection3d(const float& aspectRatio) {
            perspectiveMatrix.aspectRatio = aspectRatio;
            ViewProjections::update(*this);
        }
    };

}