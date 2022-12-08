//
// Created by mecha on 29.08.2022.
//

#pragma once

#include <core/core.h>

namespace engine::graphics {

    class ENGINE_API VideoStats final {
    public:
        static int getMaxTextureUnits();
        static const char* getAPIName();
        static const unsigned char * getVendorName();
        static const unsigned char* getRendererName();
        static const unsigned char* getVersion();
    };

}
