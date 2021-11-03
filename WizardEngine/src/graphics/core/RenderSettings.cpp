//
// Created by mecha on 01.11.2021.
//

#include "RenderSettings.h"

#include "../../core/Logger.h"

namespace engine {

    void RenderSettings::toggleCulling() {
        isCullingEnabled = !isCullingEnabled;
        if (isCullingEnabled) {
            enableCulling();
        } else {
            disableCulling();
        }
    }

    void RenderSettings::setCulling(const CullingComponent &culling) {
        if (culling.enabled != isCullingEnabled) {
            toggleCulling();
        }
    }

    void RenderSettings::setPolygonMode(const PolygonMode &polygonMode) {
        if (polygonMode != activePolygonMode) {
            activePolygonMode = polygonMode;
            setPolygonMode();
        }
    }

    void RenderSettings::printInfo() {
        ENGINE_INFO("Graphics API : {0}", getAPIName());
        ENGINE_INFO("Version : {0}", getVersion());
        ENGINE_INFO("Vendor : {0}", getVendorName());
        ENGINE_INFO("Renderer : {0}", getRendererName());
    }

}