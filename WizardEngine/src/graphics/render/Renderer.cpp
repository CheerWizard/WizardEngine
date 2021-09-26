//
// Created by mecha on 05.09.2021.
//

#include "Renderer.h"

namespace engine {

    void Renderer::destroy() {
        vertexArray.reset();
        shaderCache->clear();
        delete shaderCache;
    }

    void Renderer::onPrepare() {
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

    void Renderer::onUpdate() {
        if (activeScene == nullptr) {
            ENGINE_WARN("Renderer : No active scene!");
            return;
        }

        auto shapeEntities = activeScene->getEntities().group<ShapeComponent, TransformComponent>();
        if (shapeEntities.empty()) {
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
            for (const auto& entity : shapeEntities) {
                auto [shape, transform] = shapeEntities.get<ShapeComponent, TransformComponent>(entity);

                totalIndexCount += shape.indexData.indexCount;

                if (shape.isUpdated) {
                    updateShapeComponent(shaderName, shape);
                    shape.isUpdated = true;
                }

//                transform.transformMatrix.rotation.z += 0.001f;
//                transform.transformMatrix.rotation.x += 0.001f;
//                transform.transformMatrix.rotation.y += 0.001f;
//                transform.applyChanges();

                shader->setUniform(transform.transformMatrix);
            }

            auto materials = activeScene->getEntities().view<TextureComponent>();
            for (const auto& material : materials) {
                auto texture = materials.get<TextureComponent>(material);
                // todo add Material sub system
                shader->setUniform(texture.texture);
                vertexArray->activateTextureBuffer(texture.texture.value);
            }

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->enableAttributes();
            }

            drawIndices(totalIndexCount);

            vertexArray->unbind();
            shader->stop();
        }
    }

    void Renderer::addShader(const std::string &name, const Ref<Shader> &shader) {
        shaderCache->add(name, shader);
        vertexArray->createVertexBuffer(shader->getVertexFormat(), shader->getName());
    }

    void Renderer::addShader(const Ref<Shader> &shader) {
        shaderCache->add(shader);
        vertexArray->createVertexBuffer(shader->getVertexFormat(), shader->getName());
    }

    Ref<Shader> Renderer::loadShader(const ShaderProps &shaderProps, VertexFormat *vertexFormat) {
        auto newShader = shaderCache->load(shaderProps, vertexFormat);
        vertexArray->createVertexBuffer(newShader->getVertexFormat(), newShader->getName());
        return newShader;
    }

    Ref<Shader> Renderer::getShader(const std::string &name) {
        return shaderCache->get(name);
    }

    bool Renderer::shaderExists(const std::string &name) const {
        return shaderCache->exists(name);
    }

    void Renderer::updateShapeComponent(const std::string &shaderName, const ShapeComponent &shapeComponent) {
        vertexArray->bindLastVertexBuffer(shaderName);
        vertexArray->loadVertexBuffer(shaderName, shapeComponent.vertexData);
        vertexArray->bindIndexBuffer();
        vertexArray->loadIndexBuffer(shapeComponent.indexData);
    }

    void Renderer::loadTexture(const std::string &filePath) {
        vertexArray->bindTextureBuffer();
        vertexArray->loadTextureBuffer(filePath);
    }

    void Renderer::loadTextureData(const void *data) {
        vertexArray->loadTextureBufferData(data);
    }

}