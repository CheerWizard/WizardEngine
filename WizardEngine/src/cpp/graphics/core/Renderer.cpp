//
// Created by mecha on 19.03.2022.
//

#include <graphics/core/Renderer.h>

namespace engine::graphics {

    void Renderer::create(u8 attributeCategory) {
        shaderProgram.bindVertexFormat();
        shaderProgram.getVertexFormat().setAttrCategory(attributeCategory);
    }

    void Renderer::release() {
        for (auto& vRenderModel : vRenderModels) {
            vRenderModel.release();
        }
        for (auto& viRenderModel : viRenderModels) {
            viRenderModel.release();
        }
        vRenderModels.clear();
        viRenderModels.clear();
        shaderProgram.release();
        entityHandlers.clear();
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

    void Renderer::handleEntity(ecs::Registry &registry, ecs::entity_id entityId, u32 index) {
        for (const auto& entityHandler : entityHandlers) {
            entityHandler.handle(registry, entityId, index, shaderProgram);
        }
    }

    void Renderer::addEntityHandler(const EntityHandler &entityHandler) {
        entityHandlers.emplace_back(entityHandler);
    }

    void Renderer::addEntityHandler(const Handle &handle) {
        entityHandlers.emplace_back(handle);
    }

    void PrimitiveRenderer::renderQuad() {
        if (!shaderProgram.isReady()) return;

        shaderProgram.start();

        drawV(DrawType::TRIANGLE_STRIP, 4);

        shaderProgram.stop();
    }

    void PrimitiveRenderer::renderQuad(u32 textureId) {
        if (!shaderProgram.isReady()) return;

        shaderProgram.start();

        TextureBuffer::activate(0);
        TextureBuffer::bind(textureId, TextureType::TEXTURE_2D);
        drawV(DrawType::TRIANGLE_STRIP, 4);

        shaderProgram.stop();
    }

    void PrimitiveRenderer::release() {
        shaderProgram.release();
    }
}
