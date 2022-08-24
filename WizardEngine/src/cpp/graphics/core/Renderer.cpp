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

    void VRenderer::init() {
        shaderProgram.bindVertexFormat();
    }

    void VRenderer::release() {
        shaderProgram.release();
    }

    void VIRenderer::init() {
        shaderProgram.bindVertexFormat();
    }

    void VIRenderer::release() {
        shaderProgram.release();
    }

    void VRenderer::renderQuad(const u32& textureId) {
        if (!shaderProgram.isReady()) return;

        begin();

        TextureBuffer::activate(0);
        TextureBuffer::bind(textureId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D));

        end(DrawType::QUAD, 4);
    }

    void VRenderer::begin() {
        shaderProgram.start();
    }

    void VRenderer::begin(const ecs::Entity &entity) {
        shaderProgram.start();
        shaderProgram.update(entity);
    }

    void VRenderer::end(const DrawType& drawType, const u32& vertexCount) const {
        vRenderModel.vao.bind();
        drawV(drawType, vertexCount);
        ShaderProgram::stop();
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

    void VIRenderer::begin(const ecs::Entity &entity) {
        shaderProgram.start();
        shaderProgram.update(entity);
    }

    void VIRenderer::end(const DrawType& drawType, const u32& indexCount) const {
        viRenderModel.vao.bind();
        drawVI(drawType, indexCount);
        ShaderProgram::stop();
    }
}
