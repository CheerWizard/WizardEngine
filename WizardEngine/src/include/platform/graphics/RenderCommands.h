//
// Created by mecha on 01.11.2021.
//

#pragma once

#include <core/primitives.h>
#include <ecs/ecs.h>

namespace engine::graphics {

    using namespace engine::core;

    struct ENGINE_API DrawType final {
        static u32 QUAD;
        static u32 TRIANGLE;
        static u32 TRIANGLE_STRIP;
        static u32 LINE;
        static u32 LINE_STRIP;
        static u32 LINE_LOOP;
        static u32 POINTS;
    };

    struct ENGINE_API TestOperator final {
        static u32 ALWAYS;
        static u32 NEVER;
        static u32 EQUAL;
        static u32 NOT_EQUAL;
        static u32 LESS;
        static u32 LESS_EQUAL;
        static u32 GREATER;
        static u32 GREATER_EQUAL;
    };

    struct ENGINE_API TestAction final {
        static u32 KEEP;
        static u32 ZERO;
        static u32 REPLACE;
        static u32 INVERT;
        static u32 INCREASE;
        static u32 INCREASE_WRAP;
        static u32 DECREASE;
        static u32 DECREASE_WRAP;
    };

    struct ENGINE_API StencilTestActions final {
        u32 stencilFails;
        u32 stencilPassDepthFails;
        u32 stencilPassDepthPass;
    };

    struct ENGINE_API BufferBit final {
        static u32 FALSE;
        static u32 TRUE;
        static u32 COLOR;
        static u32 DEPTH;
        static u32 STENCIL;
    };

    ENGINE_API void drawV(u32 drawType, u32 vertexCount);
    ENGINE_API void drawV(u32 drawType, u32 vertexCount, u32 instanceCount);
    ENGINE_API void drawVI(u32 drawType, u32 indexCount);
    ENGINE_API void drawVI(u32 drawType, u32 indexCount, u32 instanceCount);

    ENGINE_API void disableMSAA();
    ENGINE_API void enableMSAA();

    ENGINE_API void clearBuffer(u32 mask);
    ENGINE_API void setClearColor(float r, float g, float b, float a);

    ENGINE_API void enableDepthTest();
    ENGINE_API void disableDepthTest();
    ENGINE_API void setDepthMask(u32 mask);
    ENGINE_API void setDepthTestOperator(u32 testOperator);
    ENGINE_API int depthBits();

    ENGINE_API void enableStencilTest();
    ENGINE_API void disableStencilTest();
    ENGINE_API void setStencilMask(u32 mask);
    ENGINE_API void setStencilBufferState(bool readOnly);
    ENGINE_API void setStencilTestOperator(u32 testOperator, int value, u32 mask);
    ENGINE_API int stencilMask(bool readOnly);
    ENGINE_API void setStencilTestActions(const StencilTestActions& stencilTestActions);

    struct ENGINE_API BlendFactor final {
        static u32 F_ZERO;
        static u32 F_ONE;
        static u32 SRC_COLOR;
        static u32 ONE_MINUS_SRC_COLOR;
        static u32 DST_COLOR;
        static u32 ONE_MINUS_DST_COLOR;
        static u32 SRC_ALPHA;
        static u32 ONE_MINUS_SRC_ALPHA;
        static u32 DST_ALPHA;
        static u32 ONE_MINUS_DST_ALPHA;
        static u32 CONSTANT_COLOR;
        static u32 ONE_MINUS_CONSTANT_COLOR;
        static u32 CONSTANT_ALPHA;
        static u32 ONE_MINUS_CONSTANT_ALPHA;
    };

    struct ENGINE_API BlendOperator final {
        static u32 ADD;
        static u32 SUBTRACT;
        static u32 REVERSE_SUBTRACT;
        static u32 MIN;
        static u32 MAX;
    };

    ENGINE_API void enableBlendMode();
    ENGINE_API void disableBlendMode();
    ENGINE_API void setBlendFunction(u32 srcFactor, u32 destFactor);
    ENGINE_API void setBlendFunctionSeparate(
        u32 srcRgbFactor,
        u32 destRgbFactor,
        u32 srcAlphaFactor,
        u32 destAlphaFactor
    );
    ENGINE_API void setBlendEquation(u32 srcDestOperator);

    struct ENGINE_API PolygonMode final {
        static u32 DOT;
        static u32 LINES;
        static u32 FILL;
    };

    struct ENGINE_API FaceType final {
        static u32 BACK;
        static u32 FRONT;
        static u32 FRONT_AND_BACK;
    };

    struct ENGINE_API FrontFaceType final {
        static u32 CLOCK_WISE;
        static u32 COUNTER_CLOCK_WISE;
    };

    ENGINE_API void setPolygonMode(u32 faceType, u32 polygonMode);
    ENGINE_API void enableCullFace();
    ENGINE_API void disableCullFace();
    ENGINE_API void setCullFace(u32 faceType);
    ENGINE_API void setFrontFace(u32 frontFaceType);

    serialize_component(CullingComponent) {
        bool enabled = false;
        u32 faceType = FaceType::BACK;
        u32 frontFaceType = FrontFaceType::CLOCK_WISE;
        CullingComponent() = default;
        CullingComponent(bool enabled) : enabled(enabled) {}

        ENGINE_API void serialize(YAML::Emitter &out) override;
        ENGINE_API void deserialize(const YAML::Node &parent) override;
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

    serialize_component(PolygonModeComponent) {
        u32 face = FaceType::FRONT_AND_BACK;
        u32 mode = PolygonMode::FILL;
        PolygonModeComponent() = default;
        PolygonModeComponent(u32 face, u32 mode) : face(face), mode(mode) {}

        ENGINE_API void serialize(YAML::Emitter &out) override;
        ENGINE_API void deserialize(const YAML::Node &parent) override;
    };

    class ENGINE_API PolygonModes final {

    private:
        PolygonModes() = default;
        ~PolygonModes() = default;

    public:
        static void setPolygonMode(const PolygonModeComponent& polygonModeComponent);

    private:
        static u32 polygonMode;
        static u32 faceType;
    };

    ENGINE_API void enableSRGB();
}
