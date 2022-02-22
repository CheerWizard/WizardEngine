//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "ViewMatrices.h"
#include "Projections.h"

namespace engine {

    struct ViewProjection2d : Mat4fUniform {
        ViewMatrix2d viewMatrix;
        OrthographicMatrix orthographicMatrix;
    };

    struct ViewProjection3d : Mat4fUniform {
        ViewMatrix3d viewMatrix;
        PerspectiveMatrix perspectiveMatrix;
    };

    class ViewProjections final {

    private:
        ViewProjections() = default;
        ~ViewProjections() = default;

    public:
        static void update(ViewProjection2d &viewProjection2D);
        static void update(ViewProjection3d &viewProjection3D);

    };

}