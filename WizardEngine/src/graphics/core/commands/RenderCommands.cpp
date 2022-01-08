//
// Created by mecha on 17.11.2021.
//

#include "RenderCommands.h"
#include "../../../core/Logger.h"

namespace engine {

    PolygonMode RenderCommands::activePolygonMode = FILL;
    bool RenderCommands::isCullingEnabled = false;

    void RenderCommands::toggleCulling() {
        isCullingEnabled = !isCullingEnabled;
        if (isCullingEnabled) {
            enableCulling();
        } else {
            disableCulling();
        }
    }

    void RenderCommands::setCulling(const CullingComponent &culling) {
        if (culling.enabled != isCullingEnabled) {
            toggleCulling();
        }
    }

    void RenderCommands::trySetPolygonMode(const PolygonMode &polygonMode) {
        if (polygonMode != activePolygonMode) {
            activePolygonMode = polygonMode;
            setPolygonMode(polygonMode);
        }
    }

    void RenderCommands::logApiInfo() {
        ENGINE_INFO("Graphics API : {0}", getAPIName());
        ENGINE_INFO("Version : {0}", getVersion());
        ENGINE_INFO("Vendor : {0}", getVendorName());
        ENGINE_INFO("Renderer : {0}", getRendererName());
    }

}