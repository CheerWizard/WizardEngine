//
// Created by mecha on 01.11.2021.
//

#include <platform/graphics/RenderCommands.h>
#include <glad/glad.h>

#define GL_DEPTH_BITS 0x0D56

namespace engine {

    GLenum toGLDrawType(const DrawType& drawType) {
        switch (drawType) {
            case QUAD: return GL_QUADS;
            case TRIANGLE: return GL_TRIANGLES;
            case LINE: return GL_LINES;
            case LINE_STRIP: return GL_LINE_STRIP;
            case LINE_LOOP: return GL_LINE_LOOP;
            case TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
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

    void enableCulling() {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }

    void disableCulling() {
        glDisable(GL_CULL_FACE);
    }

    GLenum toGLPolygonMode(const PolygonMode& polygonMode) {
        switch (polygonMode) {
            case DOT: return GL_POINT;
            case LINES: return GL_LINE;
            default: return GL_FILL;
        }
    }

    void setPolygonMode(const PolygonMode& polygonMode) {
        glPolygonMode(GL_FRONT_AND_BACK, toGLPolygonMode(polygonMode));
    }

    const unsigned char* getAPIName() {
        return 0;
    }

    const unsigned char *getVendorName() {
        return glGetString(GL_VENDOR);
    }

    const unsigned char *getRendererName() {
        return glGetString(GL_RENDERER);
    }

    const unsigned char *getVersion() {
        return glGetString(GL_VERSION);
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
