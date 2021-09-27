//
// Created by mecha on 05.09.2021.
//

#include "GraphicsContext.h"

namespace engine {

    void GraphicsContext::printInfo() {
        ENGINE_INFO("Graphics API : {0}", getAPIName());
        ENGINE_INFO("Version : {0}", getVersion());
        ENGINE_INFO("Vendor : {0}", getVendorName());
        ENGINE_INFO("Renderer : {0}", getRendererName());
    }

}