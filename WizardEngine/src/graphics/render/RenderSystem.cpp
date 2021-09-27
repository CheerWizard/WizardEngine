//
// Created by mecha on 05.09.2021.
//

#include "RenderSystem.h"

namespace engine {

    void RenderSystem::destroy() {
        vertexArray.reset();
        shaderCache->clear();
        delete shaderCache;
    }

    void RenderSystem::onPrepare() {
        for (auto &iterator : *shaderCache) {
            auto shaderName = iterator.first;
            auto shader = iterator.second;
            auto vertexBuffers = vertexArray->getVertexBuffers(shaderName);
            auto indexBuffer = vertexArray->getIndexBuffer();

            shader->bindAttributes();
            vertexArray->bind();

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->setVertex(shader->getVertexFormat());
                vertexBuffer->bind();
                vertexBuffer->allocate();
                vertexBuffer->setAttributesPointer();
            }

            indexBuffer->bind();
            indexBuffer->allocate();

            vertexArray->bindTextureBuffer();

            vertexArray->unbind();
        }
    }

    void RenderSystem::onUpdate() {
        if (activeScene == nullptr) {
            ENGINE_WARN("Renderer : No active scene!");
            return;
        }

        auto renderableEntities = activeScene->getEntities().group<ShapeComponent, TransformComponent>();
        if (renderableEntities.empty()) {
            ENGINE_WARN("Renderer : Nothing to draw!");
            return;
        }

        for (const auto& iterator : *shaderCache) {
            auto shaderName = iterator.first;
            auto shader = iterator.second;
            auto vertexBuffers = vertexArray->getVertexBuffers(shaderName);
            auto indexBuffer = vertexArray->getIndexBuffer();

            shader->start();
            vertexArray->bind();

            if (cameraController != nullptr) {
                shader->setUniform(cameraController->getCamera());
            }

            uint32_t totalIndexCount = 0;
            for (const auto& renderableEntity : renderableEntities) {
                auto [shape, transform] = renderableEntities.get<ShapeComponent, TransformComponent>(renderableEntity);
                totalIndexCount += shape.indexData.indexCount;

                renderShape(shaderName, shape);
                renderMaterial(shader, renderableEntity);
                shader->setUniform(transform.transformMatrix);
            }

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->enableAttributes();
            }

            drawIndices(totalIndexCount);

            vertexArray->unbind();
            shader->stop();
        }
    }

    void RenderSystem::addShader(const std::string &name, const Ref<Shader> &shader) {
        shaderCache->add(name, shader);
        vertexArray->createVertexBuffer(shader->getVertexFormat(), shader->getName());
    }

    void RenderSystem::addShader(const Ref<Shader> &shader) {
        shaderCache->add(shader);
        vertexArray->createVertexBuffer(shader->getVertexFormat(), shader->getName());
    }

    Ref<Shader> RenderSystem::loadShader(const ShaderProps &shaderProps, VertexFormat *vertexFormat) {
        auto newShader = shaderCache->load(shaderProps, vertexFormat);
        vertexArray->createVertexBuffer(newShader->getVertexFormat(), newShader->getName());
        return newShader;
    }

    Ref<Shader> RenderSystem::getShader(const std::string &name) {
        return shaderCache->get(name);
    }

    bool RenderSystem::shaderExists(const std::string &name) const {
        return shaderCache->exists(name);
    }

    void RenderSystem::renderShape(const std::string &shaderName, ShapeComponent &shapeComponent) {
        if (!shapeComponent.isUpdated) return;
        shapeComponent.isUpdated = false;

        vertexArray->bindLastVertexBuffer(shaderName);
        vertexArray->loadVertexBuffer(shaderName, shapeComponent.vertexData);
        vertexArray->bindIndexBuffer();
        vertexArray->loadIndexBuffer(shapeComponent.indexData);
    }

    void RenderSystem::loadTexture(const std::string &filePath) {
        vertexArray->bindTextureBuffer();
        vertexArray->loadTextureBuffer(filePath);
    }

    void RenderSystem::loadTextureData(const void *data) {
        vertexArray->loadTextureBufferData(data);
    }

    void RenderSystem::renderMaterial(Ref<Shader> &shader, const entt::entity &entity) {
        auto texture = activeScene->getComponent<TextureComponent>(entity);
        shader->setUniform(texture.texture);
        vertexArray->activateTextureBuffer(texture.texture.value);
    }

}