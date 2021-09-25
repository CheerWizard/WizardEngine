//
// Created by mecha on 23.09.2021.
//

#include "ViewProjectionMatrix.h"

namespace engine {

    const ViewProjection2d& ViewProjection2d::applyChanges() {
        value = orthographicMatrix.applyChanges().value * glm::inverse(transformMatrix.applyChanges().value);
        isUpdated = true;
        return *this;
    }

    const ViewProjection3d& ViewProjection3d::applyChanges() {
        value = perspectiveMatrix.applyChanges().value * glm::inverse(transformMatrix.applyChanges().value);
        isUpdated = true;
        return *this;
    }

}