//
// Created by mecha on 27.10.2021.
//

#include "Drawer.h"

namespace engine {

    void Drawer::toggleCulling() {
        isCullingEnabled = !isCullingEnabled;
        if (isCullingEnabled) {
            enableCulling();
        } else {
            disableCulling();
        }
    }

    void Drawer::setCulling(const CullingComponent &culling) {
        if (culling.enabled != isCullingEnabled) {
            toggleCulling();
        }
    }

    void Drawer::setPolygonMode(const PolygonMode &polygonMode) {
        if (polygonMode != activePolygonMode) {
            activePolygonMode = polygonMode;
            setPolygonMode();
        }
    }
}