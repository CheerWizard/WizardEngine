//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../../graphics/core/RenderSettings.h"

namespace engine {

    class GLRenderSettings : public RenderSettings {

    public:
        GLRenderSettings() = default;
        ~GLRenderSettings() override = default;

    public:
        void enableCulling() override;
        void disableCulling() override;
        void setPolygonMode() override;

    private:
        const char* getAPIName() override;
        const unsigned char *getVendorName() override;
        const unsigned char *getRendererName() override;
        const unsigned char *getVersion() override;

    };

}
