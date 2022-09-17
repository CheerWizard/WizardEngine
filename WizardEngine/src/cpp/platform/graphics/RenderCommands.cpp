//
// Created by mecha on 01.11.2021.
//

#include <platform/graphics/RenderCommands.h>
#include <glad/glad.h>

#define GL_DEPTH_BITS 0x0D56

namespace engine::graphics {

    GLenum toGLDrawType(const DrawType& drawType) {
        switch (drawType) {
            case DrawType::QUAD: return GL_QUADS;
            case DrawType::TRIANGLE: return GL_TRIANGLES;
            case DrawType::LINE: return GL_LINES;
            case DrawType::LINE_STRIP: return GL_LINE_STRIP;
            case DrawType::LINE_LOOP: return GL_LINE_LOOP;
            case DrawType::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
            case DrawType::POINTS: return GL_POINTS;
            default: return GL_TRIANGLES;
        }
    }

    void drawV(const DrawType& drawType, const uint32_t& vertexCount) {
        ENGINE_INFO("drawV(drawType: {0}, vertexCount : {1})", drawType, vertexCount);
        glDrawArrays(toGLDrawType(drawType), 0, vertexCount);
    }

    void drawV(const DrawType& drawType, const uint32_t& vertexCount, const uint32_t& instanceCount) {
        ENGINE_INFO("drawV(drawType: {0}, vertexCount: {1}, instanceCount: {2})", drawType, vertexCount, instanceCount);
        glDrawArraysInstanced(toGLDrawType(drawType), 0, vertexCount, instanceCount);
    }

    void drawVI(const DrawType& drawType, const uint32_t& indexCount) {
        ENGINE_INFO("drawVI(drawType: {0}, indexCount: {1})", drawType, indexCount);
        glDrawElements(toGLDrawType(drawType), (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void drawVI(const DrawType& drawType, const uint32_t& indexCount, const uint32_t& instanceCount) {
        ENGINE_INFO("drawVI(drawType: {0}, indexCount: {1}, instanceCount: {2})", drawType, indexCount, instanceCount);
        glDrawElementsInstanced(toGLDrawType(drawType), (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
    }

    void setMSAA(bool enabled) {
        enabled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
    }

    void clearColorBuffer() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void setClearColor(const Color &color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void setDepthTest(bool enabled) {
        enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    void clearDepthBuffer() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void setDepthBufferState(bool readOnly) {
        glDepthMask(readOnly ? GL_FALSE : GL_TRUE);
    }

    GLenum toGLTestOperator(const TestOperator& testOperator) {
        switch (testOperator) {
            case ALWAYS: return GL_ALWAYS;
            case NEVER: return GL_NEVER;
            case EQUAL: return GL_EQUAL;
            case NOT_EQUAL: return GL_NOTEQUAL;
            case LESS: return GL_LESS;
            case GREATER: return GL_GREATER;
            case LESS_EQUAL: return GL_LEQUAL;
            case GREATER_EQUAL: return GL_GEQUAL;
            default: return GL_LESS;
        }
    }

    GLenum toGLTestAction(const TestAction& testAction) {
        switch (testAction) {
            case KEEP: return GL_KEEP;
            case ZERO: return GL_ZERO;
            case REPLACE: return GL_REPLACE;
            case INCREASE: return GL_INCR;
            case DECREASE: return GL_DECR;
            case INVERT: return GL_INVERT;
            case INCREASE_WRAP: return GL_INCR_WRAP;
            case DECREASE_WRAP: return GL_DECR_WRAP;
            default: return KEEP;
        }
    }

    void setDepthTestOperator(const TestOperator& testOperator) {
        glDepthFunc(toGLTestOperator(testOperator));
    }

    int depthBits() {
        GLint bits;
        glGetIntegerv(GL_DEPTH_BITS, &bits);
        return bits;
    }

    void setStencilTest(bool enabled) {
        enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
    }

    void clearStencilBuffer() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    int stencilMask(bool readOnly) {
        return readOnly ? 0x00 : 0xFF;
    }

    void setStencilBufferState(bool readOnly) {
        glStencilMask(stencilMask(readOnly));
    }

    void setStencilTestOperator(const TestOperator& testOperator, int value, bool readOnly) {
        glStencilFunc(toGLTestOperator(testOperator), value, stencilMask(readOnly));
    }

    void setStencilTestActions(const StencilTestActions& stencilTestActions) {
        glStencilOp(
                toGLTestAction(stencilTestActions.stencilFails),
                toGLTestAction(stencilTestActions.stencilPassDepthFails),
                toGLTestAction(stencilTestActions.stencilPassDepthPass)
        );
    }

    GLenum toGLBlendFactor(const BlendFactor& blendFactor) {
        switch (blendFactor) {
            case F_ZERO: return GL_ZERO;
            case F_ONE: return GL_ONE;
            case SRC_COLOR: return GL_SRC_COLOR;
            case ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
            case DST_COLOR: return GL_DST_COLOR;
            case ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
            case SRC_ALPHA: return GL_SRC_ALPHA;
            case ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
            case DST_ALPHA: return GL_DST_ALPHA;
            case ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
            case CONSTANT_COLOR: return GL_CONSTANT_COLOR;
            case ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
            case CONSTANT_ALPHA: return GL_CONSTANT_ALPHA;
            case ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
            default: return GL_SRC_ALPHA;
        }
    }

    void setBlendMode(bool isEnabled) {
        isEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    }

    void setBlendFunction(const BlendFactor& srcFactor, const BlendFactor& destFactor) {
        glBlendFunc(toGLBlendFactor(srcFactor), toGLBlendFactor(destFactor));
    }

    void setBlendFunctionSeparate(
            const BlendFactor& srcRgbFactor,
            const BlendFactor& destRgbFactor,
            const BlendFactor& srcAlphaFactor,
            const BlendFactor& destAlphaFactor
    ) {
        glBlendFuncSeparate(
                toGLBlendFactor(srcRgbFactor),
                toGLBlendFactor(destRgbFactor),
                toGLBlendFactor(srcAlphaFactor),
                toGLBlendFactor(destAlphaFactor)
        );
    }

    GLenum toGLBlendOperator(const BlendOperator& blendOperator) {
        switch (blendOperator) {
            case ADD: return GL_FUNC_ADD;
            case SUBTRACT: return GL_FUNC_SUBTRACT;
            case REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
            case MIN: return GL_MIN;
            case MAX: return GL_MAX;
            default: return GL_FUNC_ADD;
        }
    }

    void setBlendEquation(const BlendOperator& srcDestOperator) {
        glBlendEquation(toGLBlendOperator(srcDestOperator));
    }

    void setCullFaceMode(bool isEnabled) {
        isEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }

    GLenum toGLFaceType(const FaceType& faceType) {
        switch (faceType) {
            case BACK: return GL_BACK;
            case FRONT: return GL_FRONT;
            case FRONT_AND_BACK: return GL_FRONT_AND_BACK;
            default: return GL_BACK;
        }
    }

    void setCullFace(const FaceType& faceType) {
        glCullFace(toGLFaceType(faceType));
    }

    GLenum toGLFrontFaceType(const FrontFaceType& frontFaceType) {
        switch (frontFaceType) {
            case CLOCK_WISE: return GL_CW;
            case COUNTER_CLOCK_WISE: return GL_CCW;
            default: return GL_CW;
        }
    }

    void setFrontFace(const FrontFaceType& frontFaceType) {
        glFrontFace(toGLFrontFaceType(frontFaceType));
    }

    GLenum toGLPolygonMode(const PolygonMode& polygonMode) {
        switch (polygonMode) {
            case DOT: return GL_POINT;
            case LINES: return GL_LINE;
            default: return GL_FILL;
        }
    }

    void setPolygonMode(const FaceType& faceType, const PolygonMode& polygonMode) {
        glPolygonMode(toGLFaceType(faceType), toGLPolygonMode(polygonMode));
    }

    bool Culling::isCullingEnabled = false;

    void Culling::setCulling(const CullingComponent &culling) {
        if (culling.enabled != isCullingEnabled) {
            isCullingEnabled = culling.enabled;
            setCullFaceMode(culling.enabled);
            setCullFace(culling.faceType);
            setFrontFace(culling.frontFaceType);
        }
    }

    PolygonMode PolygonModes::polygonMode = FILL;
    FaceType PolygonModes::faceType = FRONT_AND_BACK;

    void PolygonModes::setPolygonMode(const PolygonModeComponent& polygonModeComponent) {
        if (polygonModeComponent.mode != polygonMode || polygonModeComponent.face != faceType) {
            polygonMode = polygonModeComponent.mode;
            faceType = polygonModeComponent.face;
            graphics::setPolygonMode(polygonModeComponent.face, polygonModeComponent.mode);
        }
    }

    void enableSRGB() {
        glEnable(GL_FRAMEBUFFER_SRGB);
    }
}
