//
// Created by mecha on 23.09.2021.
//

#include <graphics/core/math/ViewProjections.h>

namespace engine {

    void ViewProjections::update(ViewProjection2d &viewProjection2D) {
        viewProjection2D.isUpdated = true;
        Projections::update(viewProjection2D.orthographicMatrix);
        ViewMatrices::update(viewProjection2D.viewMatrix);
        viewProjection2D.value = viewProjection2D.orthographicMatrix.value * viewProjection2D.viewMatrix.value;
    }

    void ViewProjections::update(ViewProjection3d &viewProjection3D) {
        viewProjection3D.isUpdated = true;
        Projections::update(viewProjection3D.perspectiveMatrix);
        ViewMatrices::update(viewProjection3D.viewMatrix);
        viewProjection3D.value = viewProjection3D.perspectiveMatrix.value * viewProjection3D.viewMatrix.value;
    }

}