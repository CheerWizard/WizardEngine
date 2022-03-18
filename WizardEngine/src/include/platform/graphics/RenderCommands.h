//
// Created by mecha on 01.11.2021.
//

#pragma once

#include <core/Logger.h>
#include "cstdint"

namespace engine {

    enum DepthTestOperator {
        ALWAYS, NEVER, EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL
    };

    void drawQuads(const uint32_t &indexCount);
    void drawTriangles(const uint32_t &indexCount);
    void drawTriangles(const uint32_t &indexCount, const uint32_t &instanceCount);
    void enableCulling();
    void disableCulling();
    void setPolygonMode(const uint8_t &mode);
    const unsigned char* getAPIName();
    const unsigned char* getVendorName();
    const unsigned char* getRendererName();
    const unsigned char* getVersion();
    void enableMSAA();
    void disableMSAA();
    void setDepthBufferState(bool readOnly);
    void setDepthTestOperator(const DepthTestOperator& depthTestOperator);
    int depthBits();

    struct CullingComponent {
        bool enabled = false;
    };

    enum PolygonMode : uint8_t {
        VERTEX = 0,
        LINE = 1,
        FILL = 2
    };

    class RenderCommands final {

    private:
        RenderCommands() = default;
        ~RenderCommands() = default;

    public:
        static void toggleCulling();
        static void setCulling(const CullingComponent &culling);
        static void trySetPolygonMode(const PolygonMode &polygonMode);
        static void logApiInfo();

    private:
        static PolygonMode activePolygonMode;
        static bool isCullingEnabled;

    };

}
