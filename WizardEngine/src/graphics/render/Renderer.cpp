//
// Created by mecha on 05.09.2021.
//

#include "Renderer.h"

namespace engine {

    void Renderer::destroy() {
        vertexArray.reset();
        shaderCache->clear();
        graphicsObjectCache->clear();
    }

    void Renderer::onPrepare() {
        for (auto &iterator : *graphicsObjectCache) {
            auto shaderName = iterator.first;
            auto graphicsObjects = iterator.second;
            auto shader = shaderCache->get(shaderName);
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
        for (auto &iterator : *graphicsObjectCache) {
            auto shaderName = iterator.first;
            auto graphicsObjects = iterator.second;
            auto shader = shaderCache->get(shaderName);
            auto vertexBuffers = vertexArray->getVertexBuffers(shaderName);
            auto indexBuffer = vertexArray->getIndexBuffer();

            shader->start();
            vertexArray->bind();

            uint32_t totalIndexCount = 0;
            for (const auto& graphicsObject : graphicsObjects) {
                totalIndexCount += graphicsObject->indexData.indexCount;
                if (graphicsObject->isUpdated) {
                    updateObject(graphicsObject);
                    graphicsObject->isUpdated = false;
                }

                // todo move this responsibility to render subsystem. For ex. to MaterialSystem.
                if (graphicsObject->perspectiveProjection != nullptr) {
                    shader->setUniform(*graphicsObject->perspectiveProjection);
                }
                if (graphicsObject->orthographicProjection != nullptr) {
                    shader->setUniform(*graphicsObject->orthographicProjection);
                }
                if (graphicsObject->brightness != nullptr) {
                    shader->setUniform(*graphicsObject->brightness);
                }
                if (graphicsObject->transform != nullptr) {
                    shader->setUniform(*graphicsObject->transform);
                }
                if (graphicsObject->textureSampler != nullptr) {
                    shader->setUniform(*graphicsObject->textureSampler);
//                    vertexArray->activateTextureBuffer(graphicsObject->textureSampler->value);
                }
            }

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->enableAttributes();
            }

            vertexArray->bindTextureBuffer();
            vertexArray->activateTextureBuffer(graphicsObjects[0]->textureSampler->value);

            drawIndices(totalIndexCount);

            vertexArray->unbind();
            shader->stop();
        }
    }

    void Renderer::loadObject(const Ref<GraphicsObject> &graphicsObject) {
        addObject(graphicsObject);
        updateObject(graphicsObject);
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

    uint32_t Renderer::addObject(const Ref<GraphicsObject> &graphicsObject) {
        return graphicsObjectCache->add(graphicsObject->shaderName, graphicsObject);
    }

    void Renderer::updateObject(const Ref<GraphicsObject> &graphicsObject) {
        vertexArray->bindLastVertexBuffer(graphicsObject->shaderName);
        vertexArray->loadVertexBuffer(graphicsObject->shaderName, graphicsObject->vertexData);
        vertexArray->bindIndexBuffer();
        vertexArray->loadIndexBuffer(graphicsObject->indexData);
    }

    const Ref<GraphicsObject>& Renderer::getGraphicsObject(const std::string &shaderName, const uint32_t &objectIndex) {
        return graphicsObjectCache->get(shaderName, objectIndex);
    }

    void Renderer::loadTexture(const std::string &filePath) {
        vertexArray->bindTextureBuffer();
        vertexArray->loadTextureBuffer(filePath);
    }

    void Renderer::loadTextureData(const void *data) {
        vertexArray->loadTextureBufferData(data);
    }

}