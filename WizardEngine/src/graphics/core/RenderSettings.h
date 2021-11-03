//
// Created by mecha on 01.11.2021.
//

#pragma once

#include <cstdint>

namespace engine {

    struct CullingComponent {
        bool enabled = false;
    };

    enum PolygonMode : uint8_t {
        POINT = 0,
        LINE = 1,
        FILL = 2
    };

    // interface for direct setting options to GPU renderer.
    class RenderSettings {

    public:
        RenderSettings() = default;
        virtual ~RenderSettings() = default;

    public:
        void toggleCulling();
        void setCulling(const CullingComponent &culling);
        void setPolygonMode(const PolygonMode &polygonMode);
        void printInfo();

    public:
        virtual void enableCulling() = 0;
        virtual void disableCulling() = 0;
        virtual void setPolygonMode() = 0;

    private:
        virtual const char* getAPIName() = 0;
        virtual const unsigned char* getVendorName() = 0;
        virtual const unsigned char* getRendererName() = 0;
        virtual const unsigned char* getVersion() = 0;

    protected:
        PolygonMode activePolygonMode = FILL;
        bool isCullingEnabled = false;

    };

}