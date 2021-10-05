//
// Created by mecha on 05.09.2021.
//

#include "RenderSystem.h"

namespace engine {

    void RenderSystem::create() {
        drawer = graphicsFactory->newDrawer();
        vertexArray = graphicsFactory->newVertexArray();
        indexBuffer = graphicsFactory->newIndexBuffer();
        textureBuffer = graphicsFactory->newTextureBuffer();
        frameBuffer = graphicsFactory->newFrameBuffer();
        uniformBuffer = graphicsFactory->newUniformBuffer();
    }

    void RenderSystem::destroy() {
        shaderCache.clear();
        vertexBufferCache.clear();
    }

    void RenderSystem::onPrepare() {
        frameBuffer->setViewPort();

        for (auto &iterator : shaderCache) {
            auto shaderName = iterator.first;
            auto shader = iterator.second;
            auto vertexBuffers = vertexBufferCache.get(shaderName);

            shader->bindAttributes();
            shader->bindUniformBlock();

            vertexArray->bind();

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->setVertex(shader->getVertexFormat());
                vertexBuffer->bind();
                vertexBuffer->allocate();
                vertexBuffer->setAttributesPointer();
            }

            indexBuffer->bind();
            indexBuffer->allocate();

            textureBuffer->bind();

            uniformBuffer->setUniformBlockFormat(shader->getUniformBlockFormat());
            uniformBuffer->bind();
            uniformBuffer->allocate();
            uniformBuffer->unbind();
            uniformBuffer->setUniformBlockPointer();

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

        for (const auto& iterator : shaderCache) {
            auto shaderName = iterator.first;
            auto shader = iterator.second;
            auto vertexBuffers = vertexBufferCache.get(shaderName);

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

            drawer->drawTrianglesIndices(totalIndexCount);

            vertexArray->unbind();
            shader->stop();
        }

        // bind to default frame buffer.
        frameBuffer->unbind();
        drawer->disableDepth();
        drawer->clearColor({1.0, 1.0, 1.0, 1.0});
    }

    void RenderSystem::addShader(const std::string &name, const Ref<Shader> &shader) {
        shaderCache.add(name, shader);
        prepareVertexBuffer(name, shader->getVertexFormat());
    }

    void RenderSystem::addShader(const Ref<Shader> &shader) {
        addShader(shader->getName(), shader);
    }

    ShaderError RenderSystem::loadShader(const ShaderProps &shaderProps, VertexFormat *vertexFormat) {
        auto shader = graphicsFactory->newShader(shaderProps, vertexFormat);
        return handleShaderError(shader);
    }

    ShaderError RenderSystem::loadShader(const ShaderProps &shaderProps) {
        auto shader = graphicsFactory->newShader(shaderProps);
        return handleShaderError(shader);
    }

    ShaderError RenderSystem::handleShaderError(const Ref<Shader> &shader) {
        auto shaderError = shader->getShaderError();
        if (shaderError == ShaderError::NONE) {
            shaderCache.add(shader);
            prepareVertexBuffer(shader->getName(), shader->getVertexFormat());
        }
        return shaderError;
    }

    Ref<Shader> RenderSystem::getShader(const std::string &name) {
        return shaderCache.get(name);
    }

    bool RenderSystem::shaderExists(const std::string &name) const {
        return shaderCache.exists(name);
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

        auto& vbo = vertexBufferCache.last(shaderName);
        vbo->bind();
        vbo->load(vertexData);

        indexBuffer->bind();
        indexBuffer->load(indexData);
    }

    void RenderSystem::loadTexture(const std::string &filePath) {
        textureBuffer->bind();
        textureBuffer->load(filePath);
    }

    void RenderSystem::loadTextureData(const void *data) {
        textureBuffer->bind();
        textureBuffer->loadData(data);
    }

    void RenderSystem::renderCamera(const Ref<Shader> &shader) {
        if (cameraController != nullptr) {
            auto& camera = cameraController->getCamera();
            if (camera.isUpdated) {
                camera.isUpdated = false;

                auto uniformData = UniformData {
                    camera.toFloatPtr(),
                    0,
                    1
                };
                uniformBuffer->bind();
                uniformBuffer->load(uniformData);
                uniformBuffer->unbind();
            }
        }
    }

    void RenderSystem::enableDepth() {
        drawer->enableDepth();
    }

    void RenderSystem::setPolygonMode(const PolygonMode &polygonMode) {
        drawer->setPolygonMode(polygonMode);
    }

    void RenderSystem3d::renderMaterial(Ref<Shader> &shader, const entt::entity &entity) {
        if (!activeScene->hasComponent<TextureComponent>(entity)) return;

        auto texture = activeScene->getComponent<TextureComponent>(entity);
        shader->setUniform(texture.texture);
        textureBuffer->activate(texture.texture.value);
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
        textureBuffer->activate(texture.texture.value);
    }

    void RenderSystem2d::renderTransform(Ref<Shader> &shader, const entt::entity &entity) {
        if (!activeScene->hasComponent<TransformComponent2d>(entity)) return;

        auto transform2d = activeScene->getComponent<TransformComponent2d>(entity);
        shader->setUniform(transform2d.transformMatrix);
    }

    void RenderSystem::onWindowClosed() {
    }

    void RenderSystem::onWindowResized(unsigned int width, unsigned int height) {
        frameBuffer->setViewPort();
    }

    void RenderSystem::updateFboSpecification(const FramebufferSpecification &framebufferSpecification) {
        frameBuffer->setSpecification(framebufferSpecification);
        frameBuffer->loadAttachments();
        activeScene->setTextureId(frameBuffer->getColorAttachment(0));
    }

    void RenderSystem::renderScene(const std::string &shaderName, const ShapeComponent &shapeComponent) {
        auto& vbo = vertexBufferCache.last(shaderName);
        vbo->bind();
        vbo->load(shapeComponent.vertexData);

        indexBuffer->bind();
        indexBuffer->load(shapeComponent.indexData);
    }

    void RenderSystem::prepareVertexBuffer(const std::string &shaderName, VertexFormat* vertexFormat) {
        if (vertexBufferCache.isEmpty(shaderName)) {
            vertexBufferCache.add(shaderName, graphicsFactory->newVertexBuffer(vertexFormat));
        }
        vertexBufferCache.last(shaderName)->setVertex(vertexFormat);
    }

}