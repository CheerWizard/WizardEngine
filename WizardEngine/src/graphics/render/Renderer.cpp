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
            indexBuffer->bind();

            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->setVertex(shader->getVertex());
                vertexBuffer->bind();
                vertexBuffer->allocate();
                vertexBuffer->prepare();
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

            // bind shader and vertex array
            shader->bind();
            vertexArray->bind();

            // bind buffers, enable vertex attributes
            indexBuffer->bind();
            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->bind();
                vertexBuffer->enableAttributes();
            }

            drawIndices(graphicsObjects[0]->indices, indexBuffer->getCapacity());

            // disable vertex attributes, unbind buffers.
            for (const auto& vertexBuffer : vertexBuffers) {
                vertexBuffer->disableAttributes();
            }
            vertexBuffers[0]->unbind();
            indexBuffer->unbind();

            // unbind shader and vertex array.
            vertexArray->unbind();
            shader->unbind();
        }
    }

    void Renderer::loadIndices(const std::string &shaderName, const uint32_t &indexStart, uint32_t *indices) {
    }

    void Renderer::loadVertices(const std::string &shaderName, const uint32_t &vertexStart, float *vertices) {

    }

}