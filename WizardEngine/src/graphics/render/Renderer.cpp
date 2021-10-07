//
// Created by mecha on 06.10.2021.
//

#include "Renderer.h"

namespace engine {

    void Renderer::create() {
        _vertexArray = _graphicsFactory->newVertexArray();
        _vertexBuffer = _graphicsFactory->newVertexBuffer();
        _indexBuffer = _graphicsFactory->newIndexBuffer();
        _frameBuffer = _graphicsFactory->newFrameBuffer();
        _textureBuffer = _graphicsFactory->newTextureBuffer();
        _uniformBuffer = _graphicsFactory->newUniformBuffer();
        _drawer = _graphicsFactory->newDrawer();
    }

    void Renderer::destroy() {
    }

    void Renderer::prepare(const Ref<Shader> &shader) {
        _shader = shader;
        _frameBuffer->setViewPort();

        _shader->bindAttributes();
        _shader->bindUniformBlock();

        _vertexArray->bind();

        _vertexBuffer->prepare(shader->getVertexFormat());

        _indexBuffer->bind();
        _indexBuffer->allocate();

        _textureBuffer->bind();

        _uniformBuffer->prepare(shader->getUniformBlockFormat());

        _vertexArray->unbind();
    }

    void Renderer::renderMesh(MeshComponent &meshComponent) {
        startFrame();
        start();

        renderCamera();

        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            loadMesh(meshComponent);
        }

        drawByIndices(meshComponent.indexCount);

        stop();
        endFrame();
    }

    void Renderer::renderCamera() {
        if (_cameraController != nullptr) {
            auto& camera = _cameraController->getCamera();
            if (camera.isUpdated) {
                camera.isUpdated = false;

                auto uniformData = UniformData {
                    camera.toFloatPtr(),
                    0,
                    1
                };
                _uniformBuffer->bind();
                _uniformBuffer->load(uniformData);
                _uniformBuffer->unbind();
            }
        }
    }

    void Renderer::start() {
        _shader->start();
        _vertexArray->bind();
    }

    void Renderer::stop() {
        _vertexArray->unbind();
        _shader->stop();
    }

    void Renderer::startFrame() {
        _frameBuffer->bind();
        _drawer->enableDepth();
        _drawer->clearDepth({0.2, 0.2, 0.2, 1});
    }

    void Renderer::endFrame() {
        _frameBuffer->unbind();
        _drawer->disableDepth();
        _drawer->clearColor({1.0, 1.0, 1.0, 1.0});
    }

    void Renderer::renderMaterial(TextureComponent &material) {
        _shader->setUniform(material.texture);
        _textureBuffer->activate(material.texture.value);
    }

    void Renderer::renderTransform(TransformComponent2d &transformComponent2D) {
        _shader->setUniform(transformComponent2D.transformMatrix);
    }

    void Renderer::renderTransform(TransformComponent3d &transformComponent3D) {
        _shader->setUniform(transformComponent3D.transformMatrix);
    }

    void Renderer::enableDepth() {
        _drawer->enableDepth();
    }

    void Renderer::setPolygonMode(const PolygonMode &polygonMode) {
        _drawer->setPolygonMode(polygonMode);
    }

    void Renderer::loadMesh(MeshComponent &meshComponent) {
        _vertexBuffer->bind();
        _indexBuffer->bind();

        const auto& meshes = meshComponent.meshes;
        for (auto i = 0; i < meshComponent.meshCount ; i++) {
            const auto& vertexData = meshes[i].vertexData;
            _vertexBuffer->load(vertexData);
            const auto& indexData = meshes[i].indexData;
            _indexBuffer->load(indexData);
        }
    }

    void Renderer::loadTexture(const std::string &filePath) {
        _textureBuffer->bind();
        _textureBuffer->load(filePath);
    }

    void Renderer::loadTextureData(const void *data) {
        _textureBuffer->bind();
        _textureBuffer->loadData(data);
    }

    void Renderer::drawByIndices(const uint32_t &indexCount) {
        _vertexBuffer->enableAttributes();
        _drawer->drawByIndices(indexCount);
    }

    const std::vector<uint32_t> &Renderer::updateFboSpecs(const FramebufferSpecification &framebufferSpecification) {
        _frameBuffer->setSpecification(framebufferSpecification);
        _frameBuffer->loadAttachments();
        return _frameBuffer->getColorAttachments();
    }

    void Renderer::resizeFrame(const uint32_t &width, const uint32_t &height) {
        _frameBuffer->resize(width, height);
    }

}