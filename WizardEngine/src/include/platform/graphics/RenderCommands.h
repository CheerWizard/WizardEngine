//
// Created by mecha on 01.11.2021.
//

#pragma once

#include <core/Logger.h>
#include "cstdint"

namespace engine {

    struct Color { float r,g,b,a; };

    enum TestOperator {
        ALWAYS, NEVER,
        EQUAL, NOT_EQUAL,
        LESS, GREATER,
        LESS_EQUAL, GREATER_EQUAL
    };

    enum TestAction {
        KEEP, ZERO, REPLACE,
        INCREASE, DECREASE, INVERT,
        INCREASE_WRAP, DECREASE_WRAP
    };

    struct StencilTestActions {
        TestAction stencilFails;
        TestAction stencilPassDepthFails;
        TestAction stencilPassDepthPass;
    };

    enum PolygonMode {
        DOT, LINE, FILL
    };

    void drawQuads(const uint32_t &indexCount);
    void drawTriangles(const uint32_t &indexCount);
    void drawTriangles(const uint32_t &indexCount, const uint32_t &instanceCount);
    void drawLines(const uint32_t &vertexCount);

    void enableCulling();
    void disableCulling();

    void setPolygonMode(const PolygonMode &polygonMode);

    const unsigned char* getAPIName();
    const unsigned char* getVendorName();
    const unsigned char* getRendererName();
    const unsigned char* getVersion();

    void setMSAA(bool enabled);

    void clearColorBuffer();

    void setClearColor(const Color& color);
    void setDepthTest(bool enabled);
    void clearDepthBuffer();
    void setDepthBufferState(bool readOnly);
    void setDepthTestOperator(const TestOperator& testOperator);
    int depthBits();

    void setStencilTest(bool enabled);
    void clearStencilBuffer();
    void setStencilBufferState(bool readOnly);
    void setStencilTestOperator(const TestOperator& testOperator, int value, bool readOnly);
    int stencilMask(bool readOnly);
    void setStencilTestActions(const StencilTestActions& stencilTestActions);

    struct CullingComponent {
        bool enabled = false;
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
