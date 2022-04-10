//
// Created by mecha on 19.03.2022.
//

#include <graphics/core/Renderer.h>

namespace engine::graphics {

    void Renderer::create(const AttributeCategory& attributeCategory) {
        shaderProgram.bindVertexFormat();
        shaderProgram.getVertexFormat().setAttrCategory(attributeCategory);
    }

    void Renderer::release() {
        for (auto& vRenderModel : vRenderModels) {
            graphics::release(vRenderModel);
        }
        for (auto& viRenderModel : viRenderModels) {
            graphics::release(viRenderModel);
        }
        vRenderModels.clear();
        viRenderModels.clear();
        shaderProgram.release();
    }

    VRenderModel& Renderer::createRenderModel(const uint32_t& vertexCount) {
        VRenderModel renderModel(vRenderModels.size(), vertexCount);
        renderModel.vao.bind();
        renderModel.vbo.setFormat(shaderProgram.getVertexFormat());
        VertexArray::unbind();
        vRenderModels.emplace_back(renderModel);
        return vRenderModels[renderModel.id];
    }

    VIRenderModel& Renderer::createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount) {
        VIRenderModel renderModel(viRenderModels.size(), vertexCount, indexCount);
        renderModel.vao.bind();
        renderModel.vbo.setFormat(shaderProgram.getVertexFormat());
        renderModel.ibo.bind();
        renderModel.ibo.alloc();
        VertexArray::unbind();
        viRenderModels.emplace_back(renderModel);
        return viRenderModels[renderModel.id];
    }

    void BatchRenderer::init() {
        createRenderModel(DEFAULT_VERTEX_COUNT);
        createRenderModel(DEFAULT_VERTEX_COUNT, DEFAULT_INDEX_COUNT);
    }

    void InstanceRenderer::init() {
        createRenderModel(DEFAULT_VERTEX_COUNT);
        createRenderModel(DEFAULT_VERTEX_COUNT, DEFAULT_INDEX_COUNT);
    }

    void VRenderer::create() {
        vRenderModel = { 0, DEFAULT_VERTEX_COUNT };
        shaderProgram.bindVertexFormat();
        vRenderModel.vao.bind();
        vRenderModel.vbo.setFormat(shaderProgram.getVertexFormat());
        VertexArray::unbind();
    }

    void VRenderer::release() {
        shaderProgram.release();
    }

    void VIRenderer::create() {
        viRenderModel = { 0, DEFAULT_VERTEX_COUNT, DEFAULT_INDEX_COUNT };
        shaderProgram.bindVertexFormat();
        viRenderModel.vao.bind();
        viRenderModel.vbo.setFormat(shaderProgram.getVertexFormat());
        viRenderModel.ibo.bind();
        viRenderModel.ibo.alloc();
        VertexArray::unbind();
    }

    void VIRenderer::release() {
        shaderProgram.release();
    }

    void MultiRenderer::release() {
        batchRenderer.release();
        instanceRenderer.release();
    }

    void VRenderer::renderQuad(const u32& textureId) {
        if (!shaderProgram.isReady()) return;

        shaderProgram.start();

        TextureBuffer::activate(0);
        TextureBuffer::bind(textureId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D));

        vRenderModel.vao.bind();
        drawV(DrawType::QUAD, 4);

        BaseShaderProgram::stop();
    }
}
