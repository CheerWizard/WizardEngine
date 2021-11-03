//
// Created by mecha on 23.09.2021.
//

#include "ViewProjectionMatrix.h"

namespace engine {

    ViewProjection2d& ViewProjection2d::applyChanges() {
        value = orthographicMatrix.applyChanges().value * viewMatrix.applyChanges().value;
        isUpdated = true;
        return *this;
    }

    ViewProjection3d& ViewProjection3d::applyChanges() {
        value = perspectiveMatrix.applyChanges().value * viewMatrix.applyChanges().value;
        isUpdated = true;
        return *this;
    }

}