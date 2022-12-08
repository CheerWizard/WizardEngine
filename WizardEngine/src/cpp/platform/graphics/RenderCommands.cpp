//
// Created by mecha on 01.11.2021.
//

#include <io/Logger.h>
#include <glad/glad.h>
#define GL_DEPTH_BITS 0x0D56

#include <platform/graphics/RenderCommands.h>

namespace engine::graphics {

    u32 TestOperator::ALWAYS = GL_ALWAYS;
    u32 TestOperator::NEVER = GL_NEVER;
    u32 TestOperator::EQUAL = GL_EQUAL;
    u32 TestOperator::NOT_EQUAL = GL_NOTEQUAL;
    u32 TestOperator::LESS = GL_LESS;
    u32 TestOperator::LESS_EQUAL = GL_LEQUAL;
    u32 TestOperator::GREATER = GL_GREATER;
    u32 TestOperator::GREATER_EQUAL = GL_GEQUAL;

    u32 TestAction::KEEP = GL_KEEP;
    u32 TestAction::ZERO = GL_ZERO;
    u32 TestAction::REPLACE = GL_REPLACE;
    u32 TestAction::INVERT = GL_INVERT;
    u32 TestAction::INCREASE = GL_INCR;
    u32 TestAction::INCREASE_WRAP = GL_INCR_WRAP;
    u32 TestAction::DECREASE = GL_DECR;
    u32 TestAction::DECREASE_WRAP = GL_DECR_WRAP;

    u32 BlendFactor::F_ZERO = GL_ZERO;
    u32 BlendFactor::F_ONE = GL_ONE;
    u32 BlendFactor::SRC_COLOR = GL_SRC_COLOR;
    u32 BlendFactor::ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR;
    u32 BlendFactor::DST_COLOR = GL_DST_COLOR;
    u32 BlendFactor::ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR;
    u32 BlendFactor::SRC_ALPHA = GL_SRC_ALPHA;
    u32 BlendFactor::ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA;
    u32 BlendFactor::DST_ALPHA = GL_DST_ALPHA;
    u32 BlendFactor::ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA;
    u32 BlendFactor::CONSTANT_COLOR = GL_CONSTANT_COLOR;
    u32 BlendFactor::ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR;
    u32 BlendFactor::CONSTANT_ALPHA = GL_CONSTANT_ALPHA;
    u32 BlendFactor::ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA;

    u32 BlendOperator::ADD = GL_FUNC_ADD;
    u32 BlendOperator::SUBTRACT = GL_FUNC_SUBTRACT;
    u32 BlendOperator::REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT;
    u32 BlendOperator::MIN = GL_MIN;
    u32 BlendOperator::MAX = GL_MAX;

    u32 PolygonMode::DOT = GL_POINT;
    u32 PolygonMode::LINES = GL_LINE;
    u32 PolygonMode::FILL = GL_FILL;

    u32 FaceType::BACK = GL_BACK;
    u32 FaceType::FRONT = GL_FRONT;
    u32 FaceType::FRONT_AND_BACK = GL_FRONT_AND_BACK;

    u32 FrontFaceType::CLOCK_WISE = GL_CW;
    u32 FrontFaceType::COUNTER_CLOCK_WISE = GL_CCW;

    u32 DrawType::QUAD = GL_QUADS;
    u32 DrawType::TRIANGLE = GL_TRIANGLES;
    u32 DrawType::TRIANGLE_STRIP = GL_TRIANGLE_STRIP;
    u32 DrawType::LINE = GL_LINES;
    u32 DrawType::LINE_STRIP = GL_LINE_STRIP;
    u32 DrawType::LINE_LOOP = GL_LINE_LOOP;
    u32 DrawType::POINTS = GL_POINTS;

    u32 BufferBit::FALSE = GL_FALSE;
    u32 BufferBit::TRUE = GL_TRUE;
    u32 BufferBit::COLOR = GL_COLOR_BUFFER_BIT;
    u32 BufferBit::DEPTH = GL_DEPTH_BUFFER_BIT;
    u32 BufferBit::STENCIL = GL_STENCIL_BUFFER_BIT;

    void drawV(u32 drawType, u32 vertexCount) {
        ENGINE_INFO("drawV(drawType: {0}, vertexCount : {1})", drawType, vertexCount);
        glDrawArrays(drawType, 0, vertexCount);
    }

    void drawV(u32 drawType, u32 vertexCount, u32 instanceCount) {
        ENGINE_INFO("drawV(drawType: {0}, vertexCount: {1}, instanceCount: {2})", drawType, vertexCount, instanceCount);
        glDrawArraysInstanced(drawType, 0, vertexCount, instanceCount);
    }

    void drawVI(u32 drawType, u32 indexCount) {
        ENGINE_INFO("drawVI(drawType: {0}, indexCount: {1})", drawType, indexCount);
        glDrawElements(drawType, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void drawVI(u32 drawType, u32 indexCount, u32 instanceCount) {
        ENGINE_INFO("drawVI(drawType: {0}, indexCount: {1}, instanceCount: {2})", drawType, indexCount, instanceCount);
        glDrawElementsInstanced(drawType, (GLsizei) indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
    }

    void enableMSAA() {
        glDisable(GL_MULTISAMPLE);
    }

    void disableMSAA() {
        glEnable(GL_MULTISAMPLE);
    }

    void clearBuffer(u32 mask) {
        glClear(mask);
    }

    void setClearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }

    void enableDepthTest() {
        glEnable(GL_DEPTH_TEST);
    }

    void disableDepthTest() {
        glDisable(GL_DEPTH_TEST);
    }

    void setDepthMask(u32 mask) {
        glDepthMask(mask);
    }

    void setDepthTestOperator(u32 testOperator) {
        glDepthFunc(testOperator);
    }

    int depthBits() {
        GLint bits;
        glGetIntegerv(GL_DEPTH_BITS, &bits);
        return bits;
    }

    void enableStencilTest() {
        glEnable(GL_STENCIL_TEST);
    }

    void disableStencilTest() {
        glDisable(GL_STENCIL_TEST);
    }

    int stencilMask(bool readOnly) {
        return readOnly ? 0x00 : 0xFF;
    }

    void setStencilMask(u32 mask) {
        glStencilMask(mask);
    }

    void setStencilTestOperator(u32 testOperator, int value, u32 mask) {
        glStencilFunc(testOperator, value, mask);
    }

    void setStencilTestActions(const StencilTestActions& stencilTestActions) {
        glStencilOp(
                stencilTestActions.stencilFails,
                stencilTestActions.stencilPassDepthFails,
                stencilTestActions.stencilPassDepthPass
        );
    }

    void disableBlendMode() {
        glDisable(GL_BLEND);
    }

    void enableBlendMode() {
        glEnable(GL_BLEND);
    }

    void setBlendFunction(u32 srcFactor, u32 destFactor) {
        glBlendFunc(srcFactor, destFactor);
    }

    void setBlendFunctionSeparate(
            u32 srcRgbFactor,
            u32 destRgbFactor,
            u32 srcAlphaFactor,
            u32 destAlphaFactor
    ) {
        glBlendFuncSeparate(
                srcRgbFactor,
                destRgbFactor,
                srcAlphaFactor,
                destAlphaFactor
        );
    }

    void setBlendEquation(u32 srcDestOperator) {
        glBlendEquation(srcDestOperator);
    }

    void enableCullFace() {
        glEnable(GL_CULL_FACE);
    }

    void disableCullFace() {
        glDisable(GL_CULL_FACE);
    }

    void setCullFace(u32 faceType) {
        glCullFace(faceType);
    }

    void setFrontFace(u32 frontFaceType) {
        glFrontFace(frontFaceType);
    }

    void setPolygonMode(u32 faceType, u32 polygonMode) {
        glPolygonMode(faceType, polygonMode);
    }

    bool Culling::isCullingEnabled = false;

    void Culling::setCulling(const CullingComponent &culling) {
        if (culling.enabled == isCullingEnabled) return;

        isCullingEnabled = culling.enabled;
        isCullingEnabled ? enableCullFace() : disableCullFace();
        setCullFace(culling.faceType);
        setFrontFace(culling.frontFaceType);
    }

    u32 PolygonModes::polygonMode = PolygonMode::FILL;
    u32 PolygonModes::faceType = FaceType::FRONT_AND_BACK;

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

    void CullingComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "CullingComponent";
        yaml::serialize(out, "enabled", enabled);
        yaml::serialize(out, "faceType", faceType);
        yaml::serialize(out, "frontFaceType", frontFaceType);
        out << YAML::EndMap;
    }

    void CullingComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["CullingComponent"];
        if (root) {
            yaml::deserialize(root, "enabled", enabled);
            yaml::deserialize(root, "faceType", faceType);
            yaml::deserialize(root, "frontFaceType", frontFaceType);
        }
    }

    void PolygonModeComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "PolygonModeComponent";
        yaml::serialize(out, "face", face);
        yaml::serialize(out, "mode", mode);
        out << YAML::EndMap;
    }

    void PolygonModeComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["PolygonModeComponent"];
        if (root) {
            yaml::deserialize(root, "face", face);
            yaml::deserialize(root, "mode", mode);
        }
    }
}
