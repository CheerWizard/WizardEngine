//
// Created by mecha on 05.09.2021.
//

#include "RenderSystem.h"

namespace engine {

    void RenderSystem::destroy() {
        delete shaderCache;
    }

    void RenderSystem::onPrepare() {
        frameBuffer->setViewPort();

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

        auto renderableEntities = activeScene->getEntities().view<ShapeComponent>();
        if (renderableEntities.empty()) {
            ENGINE_WARN("Renderer : Nothing to draw!");
            return;
        }

        // bind to our own frame buffer
        frameBuffer->bind();
        drawer->enableDepth();
        drawer->clearDepth({0.2, 0.2, 0.2, 1});

        for (const auto& iterator : *shaderCache) {
            auto shaderName = iterator.first;
            auto shader = iterator.second;
            auto vertexBuffers = vertexArray->getVertexBuffers(shaderName);
            auto indexBuffer = vertexArray->getIndexBuffer();

            shader->start();
            vertexArray->bind();

            renderCamera(shader);

            uint32_t totalIndexCount = 0;
            uint32_t totalVertexCount = 0;
            for (const auto& renderableEntity : renderableEntities) {
                auto& shapeComponent = renderableEntities.get<ShapeComponent>(renderableEntity);

                renderShape(shaderName, shapeComponent, totalVertexCount, totalIndexCount);
                renderMaterial(shader, renderableEntity);
                renderTransform(shader, renderableEntity);

                totalIndexCount += shapeComponent.indexData.indexCount;
                totalVertexCount += shapeComponent.vertexData.vertexCount;
            }

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->enableAttributes();
            }

            drawer->drawIndices(totalIndexCount);

            vertexArray->unbind();
            shader->stop();
        }

        // bind to default frame buffer.
        frameBuffer->unbind();
        drawer->disableDepth();
        drawer->clearColor({1.0, 1.0, 1.0, 1.0});

    }

    void RenderSystem::addShader(const std::string &name, const Ref<Shader> &shader) {
        shaderCache->add(name, shader);
        vertexArray->createVertexBuffer(shader->getVertexFormat(), shader->getName());
    }

    void RenderSystem::addShader(const Ref<Shader> &shader) {
        shaderCache->add(shader);
        vertexArray->createVertexBuffer(shader->getVertexFormat(), shader->getName());
    }

    ShaderError RenderSystem::loadShader(const ShaderProps &shaderProps, VertexFormat *vertexFormat) {
        auto shaderError = shaderCache->load(shaderProps, vertexFormat);
        onShaderLoaded(shaderError, shaderProps.name);
        return shaderError;
    }

    ShaderError RenderSystem::loadShader(const ShaderProps &shaderProps) {
        auto shaderError = shaderCache->load(shaderProps);
        onShaderLoaded(shaderError, shaderProps.name);
        return shaderError;
    }

    void RenderSystem::onShaderLoaded(const ShaderError &shaderError, const std::string &shaderName) {
        if (shaderError == ShaderError::NONE) {
            auto shader = shaderCache->get(shaderName);
            vertexArray->createVertexBuffer(shader->getVertexFormat(), shader->getName());
        }
    }

    Ref<Shader> RenderSystem::getShader(const std::string &name) {
        return shaderCache->get(name);
    }

    bool RenderSystem::shaderExists(const std::string &name) const {
        return shaderCache->exists(name);
    }

    void RenderSystem::renderShape(const std::string &shaderName,
                                   ShapeComponent &shapeComponent,
                                   const uint32_t &vertexStart,
                                   const uint32_t &indexStart) {
        if (!shapeComponent.isUpdated) return;
        shapeComponent.isUpdated = false;

        auto& indexData = shapeComponent.indexData;
        auto& currentIndexStart = indexData.indexStart;
        auto& vertexData = shapeComponent.vertexData;
        auto& currentVertexStart = vertexData.vertexStart;

        if (currentVertexStart != vertexStart || currentIndexStart != indexStart) {
            currentVertexStart = vertexStart;
            currentIndexStart = indexStart;
            for (auto i = 0 ; i < indexData.indexCount ; i++) {
                auto& index = indexData.indices[i];
                index += currentVertexStart;
            }
        }

        vertexArray->bindLastVertexBuffer(shaderName);
        vertexArray->loadVertexBuffer(shaderName, vertexData);
        vertexArray->bindIndexBuffer();
        vertexArray->loadIndexBuffer(indexData);
    }

    void RenderSystem::loadTexture(const std::string &filePath) {
        vertexArray->bindTextureBuffer();
        vertexArray->loadTextureBuffer(filePath);
    }

    void RenderSystem::loadTextureData(const void *data) {
        vertexArray->loadTextureBufferData(data);
    }

    void RenderSystem::renderCamera(const Ref<Shader> &shader) {
        if (cameraController != nullptr) {
            shader->setUniform(cameraController->getCamera());
        }
    }

    void RenderSystem::enableDepth() {
        drawer->enableDepth();
    }

    void RenderSystem3d::renderMaterial(Ref<Shader> &shader, const entt::entity &entity) {
        if (!activeScene->hasComponent<TextureComponent>(entity)) return;

        auto texture = activeScene->getComponent<TextureComponent>(entity);
        shader->setUniform(texture.texture);
        vertexArray->activateTextureBuffer(texture.texture.value);
    }

    void RenderSystem3d::renderTransform(Ref<Shader> &shader, const entt::entity &entity) {
        if (!activeScene->hasComponent<TransformComponent3d>(entity)) return;

        auto transform3d = activeScene->getComponent<TransformComponent3d>(entity);
        shader->setUniform(transform3d.transformMatrix);
    }

    void RenderSystem2d::renderMaterial(Ref<Shader> &shader, const entt::entity &entity) {
        if (!activeScene->hasComponent<TextureComponent>(entity)) return;

        auto texture = activeScene->getComponent<TextureComponent>(entity);
        shader->setUniform(texture.texture);
        vertexArray->activateTextureBuffer(texture.texture.value);
    }

    void RenderSystem2d::renderTransform(Ref<Shader> &shader, const entt::entity &entity) {
        if (!activeScene->hasComponent<TransformComponent2d>(entity)) return;

        auto transform2d = activeScene->getComponent<TransformComponent2d>(entity);
        shader->setUniform(transform2d.transformMatrix);
    }

    void RenderSystem::onWindowClosed() {
    }

    void RenderSystem::onWindowResized(unsigned int width, unsigned int height) {

    }

    void RenderSystem::updateFboSpecification(const FramebufferSpecification &framebufferSpecification) {
        frameBuffer->setSpecification(framebufferSpecification);
        frameBuffer->loadAttachments();
        activeScene->setColorTextures(frameBuffer->getColorAttachments());
    }

}