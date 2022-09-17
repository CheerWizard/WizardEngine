//
// Created by mecha on 01.11.2021.
//

#pragma once

#include <core/primitives.h>
#include <io/Logger.h>
#include <graphics/core/buffer_data/VertexData.h>

namespace engine::graphics {

    struct ENGINE_API Color { f32 r,g,b,a; };

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

    struct ENGINE_API StencilTestActions {
        TestAction stencilFails;
        TestAction stencilPassDepthFails;
        TestAction stencilPassDepthPass;
    };

    ENGINE_API void drawV(const DrawType& drawType, const uint32_t& vertexCount);
    ENGINE_API void drawV(const DrawType& drawType, const uint32_t& vertexCount, const uint32_t& instanceCount);
    ENGINE_API void drawVI(const DrawType& drawType, const uint32_t& indexCount);
    ENGINE_API void drawVI(const DrawType& drawType, const uint32_t& indexCount, const uint32_t& instanceCount);

    ENGINE_API void setMSAA(bool enabled);

    ENGINE_API void clearColorBuffer();

    ENGINE_API void setClearColor(const Color& color);
    ENGINE_API void setDepthTest(bool enabled);
    ENGINE_API void clearDepthBuffer();
    ENGINE_API void setDepthBufferState(bool readOnly);
    ENGINE_API void setDepthTestOperator(const TestOperator& testOperator);
    ENGINE_API int depthBits();

    ENGINE_API void setStencilTest(bool enabled);
    ENGINE_API void clearStencilBuffer();
    ENGINE_API void setStencilBufferState(bool readOnly);
    ENGINE_API void setStencilTestOperator(const TestOperator& testOperator, int value, bool readOnly);
    ENGINE_API int stencilMask(bool readOnly);
    ENGINE_API void setStencilTestActions(const StencilTestActions& stencilTestActions);

    enum BlendFactor {
        F_ZERO, F_ONE,
        SRC_COLOR, ONE_MINUS_SRC_COLOR,
        DST_COLOR, ONE_MINUS_DST_COLOR,
        SRC_ALPHA, ONE_MINUS_SRC_ALPHA,
        DST_ALPHA, ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR, ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA
    };

    enum BlendOperator {
        ADD, SUBTRACT, REVERSE_SUBTRACT, MIN, MAX
    };

    ENGINE_API void setBlendMode(bool isEnabled);
    ENGINE_API void setBlendFunction(const BlendFactor& srcFactor, const BlendFactor& destFactor);
    ENGINE_API void setBlendFunctionSeparate(
        const BlendFactor& srcRgbFactor,
        const BlendFactor& destRgbFactor,
        const BlendFactor& srcAlphaFactor,
        const BlendFactor& destAlphaFactor
    );
    ENGINE_API void setBlendEquation(const BlendOperator& srcDestOperator);

    enum PolygonMode {
        DOT, LINES, FILL
    };

    enum FaceType {
        BACK, FRONT, FRONT_AND_BACK
    };

    enum FrontFaceType {
        CLOCK_WISE, COUNTER_CLOCK_WISE
    };

    ENGINE_API void setPolygonMode(const FaceType& faceType, const PolygonMode &polygonMode);
    ENGINE_API void setCullFaceMode(bool isEnabled);
    ENGINE_API void setCullFace(const FaceType& faceType);
    ENGINE_API void setFrontFace(const FrontFaceType& frontFaceType);

    component(CullingComponent) {
        bool enabled = false;
        FaceType faceType = BACK;
        FrontFaceType frontFaceType = CLOCK_WISE;
        CullingComponent() = default;
        CullingComponent(bool enabled) : enabled(enabled) {}
    };

    class ENGINE_API Culling final {

    private:
        Culling() = default;
        ~Culling() = default;

    public:
        static void setCulling(const CullingComponent &culling);

    private:
        static bool isCullingEnabled;
    };

    component(PolygonModeComponent) {
        FaceType face = FRONT_AND_BACK;
        PolygonMode mode = FILL;
        PolygonModeComponent() = default;
        PolygonModeComponent(const FaceType& face, const PolygonMode& mode) : face(face), mode(mode) {}
    };

    class ENGINE_API PolygonModes final {

    private:
        PolygonModes() = default;
        ~PolygonModes() = default;

    public:
        static void setPolygonMode(const PolygonModeComponent& polygonModeComponent);

    private:
        static PolygonMode polygonMode;
        static FaceType faceType;
    };

    ENGINE_API void enableSRGB();
}
