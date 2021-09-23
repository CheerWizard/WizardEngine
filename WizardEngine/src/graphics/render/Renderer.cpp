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

            shader->prepare();

            vertexArray->bind();

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->setVertex(shader->getVertex());
                vertexBuffer->bind();
                vertexBuffer->allocate();
                vertexBuffer->prepare();
            }

            indexBuffer->bind();
            indexBuffer->allocate();

            for (const auto& graphicsObject : graphicsObjects) {
                // TODO consider how to manipulate with vertexStart and indexStart params.
                vertexArray->loadVertexBuffer(graphicsObject->shaderName, 0, graphicsObject->vertices);
                vertexArray->loadIndexBuffer(0, graphicsObject->indices);
            }

            vertexBuffers[0]->unbind();
            indexBuffer->unbind();
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

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->enableAttributes();
            }

            drawIndices(indexBuffer->getCapacity());

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->disableAttributes();
            }

            vertexArray->unbind();
            shader->stop();
        }
    }

    void Renderer::loadIndices(const std::string &shaderName, const uint32_t &indexStart, uint32_t *indices) {

    }

    void Renderer::loadVertices(const std::string &shaderName, const uint32_t &vertexStart, float *vertices) {

    }

    void Renderer::loadObject(const Ref<GraphicsObject> &graphicsObject) {
        graphicsObjectCache->add(graphicsObject->shaderName, graphicsObject);
    }

    void Renderer::addShader(const std::string &name, const Ref<Shader> &shader) {
        shaderCache->add(name, shader);
        vertexArray->createVertexBuffer(shader->getVertex(), shader->getName());
    }

    void Renderer::addShader(const Ref<Shader> &shader) {
        shaderCache->add(shader);
        vertexArray->createVertexBuffer(shader->getVertex(), shader->getName());
    }

    Ref<Shader> Renderer::loadShader(const ShaderProps &shaderProps, Vertex *vertex) {
        auto newShader = shaderCache->load(shaderProps, vertex);
        vertexArray->createVertexBuffer(newShader->getVertex(), newShader->getName());
        return newShader;
    }

    Ref<Shader> Renderer::getShader(const std::string &name) {
        return shaderCache->get(name);
    }

    bool Renderer::shaderExists(const std::string &name) const {
        return shaderCache->exists(name);
    }

}