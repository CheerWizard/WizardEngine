//
// Created by mecha on 23.09.2021.
//

#include "ViewProjectionMatrix.h"

namespace engine {

    const ViewProjection2d& ViewProjection2d::applyChanges() {
        value = orthographicMatrix.applyChanges().value * viewMatrix.applyChanges().value;
        isUpdated = true;
        return *this;
    }

    const ViewProjection3d& ViewProjection3d::applyChanges() {
        value = perspectiveMatrix.applyChanges().value * viewMatrix.applyChanges().value;
        isUpdated = true;
        return *this;
    }

}