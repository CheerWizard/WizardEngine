//
// Created by mecha on 19.03.2022.
//

#include <graphics/core/Renderer.h>

namespace engine {

    void Renderer::create(const AttributeCategory& attributeCategory) {
        shaderProgram->bindVertexFormat();
        shaderProgram->getVertexFormat().setAttrCategory(attributeCategory);
        createRenderModel(DEFAULT_VERTEX_COUNT);
        createRenderModel(DEFAULT_VERTEX_COUNT, DEFAULT_INDEX_COUNT);
    }

    void Renderer::release() {
        for (auto& vRenderModel : vRenderModels) {
            engine::release(vRenderModel);
        }
        for (auto& viRenderModel : viRenderModels) {
            engine::release(viRenderModel);
        }
        vRenderModels.clear();
        viRenderModels.clear();
        shaderProgram->release();
    }

    VRenderModel& Renderer::createRenderModel(const uint32_t& vertexCount) {
        VRenderModel renderModel(vRenderModels.size(), vertexCount);
        renderModel.vao.bind();
        renderModel.vbo.setFormat(shaderProgram->getVertexFormat());
        renderModel.vao.unbind();
        vRenderModels.emplace_back(renderModel);
        return vRenderModels[renderModel.id];
    }

    VIRenderModel& Renderer::createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount) {
        VIRenderModel renderModel(viRenderModels.size(), vertexCount, indexCount);
        renderModel.vao.bind();
        renderModel.vbo.setFormat(shaderProgram->getVertexFormat());
        renderModel.ibo.bind();
        renderModel.ibo.alloc();
        renderModel.vao.unbind();
        viRenderModels.emplace_back(renderModel);
        return viRenderModels[renderModel.id];
    }

    void VRenderer::create() {
        shaderProgram->bindVertexFormat();
        vRenderModel.vao.bind();
        vRenderModel.vbo.setFormat(shaderProgram->getVertexFormat());
        vRenderModel.vao.unbind();
    }

    void VRenderer::destroy() {
        shaderProgram->release();
    }

    void VIRenderer::create() {
        shaderProgram->bindVertexFormat();
        viRenderModel.vao.bind();
        viRenderModel.vbo.setFormat(shaderProgram->getVertexFormat());
        viRenderModel.ibo.bind();
        viRenderModel.ibo.alloc();
        viRenderModel.vao.unbind();
    }

    void VIRenderer::destroy() {
        shaderProgram->release();
    }
}
