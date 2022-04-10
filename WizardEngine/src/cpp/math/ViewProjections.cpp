//
// Created by mecha on 23.09.2021.
//

#include <math/ViewProjections.h>

namespace engine::math {

    void ViewProjections::update(ViewProjection2d &viewProjection2D) {
        viewProjection2D.isUpdated = true;
        Projections::update(viewProjection2D.orthographicMatrix);
        math::update(viewProjection2D.viewMatrix);
        viewProjection2D.value = viewProjection2D.orthographicMatrix.value * viewProjection2D.viewMatrix.value;
    }

    void ViewProjections::update(ViewProjection3d &viewProjection3D) {
        viewProjection3D.isUpdated = true;
        Projections::update(viewProjection3D.perspectiveMatrix);
        math::update(viewProjection3D.viewMatrix);
        viewProjection3D.value = viewProjection3D.perspectiveMatrix.value * viewProjection3D.viewMatrix.value;
    }

}