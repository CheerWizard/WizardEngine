//
// Created by mecha on 06.10.2021.
//

#include "Renderer.h"

namespace engine {

    void Renderer::create(const Ref<GraphicsFactory> &graphicsFactory) {
        _vertexArray = graphicsFactory->newVertexArray();
        _vertexBuffer = graphicsFactory->newVertexBuffer();
        _indexBuffer = graphicsFactory->newIndexBuffer();
        _textureBuffer = graphicsFactory->newTextureBuffer();
        _drawer = graphicsFactory->newDrawer();
        _uniformBuffer = graphicsFactory->newUniformBuffer();

        _shader->bindAttributes();
        _shader->bindUniformBlock();

        _vertexArray->bind();

        _vertexBuffer->prepare(_shader->getVertexFormat());

        _indexBuffer->bind();
        _indexBuffer->allocate();

        _textureBuffer->bind();

        if (_shader->getState() != ShaderState::NO_UNIFORM_BLOCKS) {
            _uniformBuffer->prepare(_shader->getUniformBlockFormat());
        }

        _vertexArray->unbind();
    }

    void Renderer::destroy() {
    }

    void Renderer::render(const Entity &entity) {
        begin();

        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        renderMesh(entity, totalVertexCount, totalIndexCount);

        renderTransform(entity);
        renderCamera(entity);
        renderPolygonMode(entity);

        drawElements(totalIndexCount);

        end();
    }

    void Renderer::begin() {
        _shader->start();
        _vertexArray->bind();
    }

    void Renderer::end() {
        _vertexArray->unbind();
        _shader->stop();
    }

    void Renderer::renderMaterial(const Entity& entity) {
        if (entity.has<TextureComponent>()) {
            auto& texture = entity.get<TextureComponent>();
            _shader->setUniform(texture);
            _textureBuffer->activate(texture.value);
        }
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

    void Renderer::drawElements(const uint32_t &indexCount) {
        _vertexBuffer->enableAttributes();
        _drawer->drawElements(indexCount);
    }

    void Renderer::drawElements(const uint32_t &indexCount, const uint32_t &instanceCount) {
        _vertexBuffer->enableAttributes();
        _drawer->drawElements(indexCount, instanceCount);
    }

    void Renderer::renderMesh(
            const Entity& entity,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount
    ) {
        renderMesh(entity, entity, previousVertexCount, previousIndexCount);
    }

    void Renderer::renderMesh(
            const uint32_t &instanceID,
            const Entity &entity,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount
    ) {
        ENGINE_INFO("renderMesh() - entityId: {0}, entityName: {1}", (uint32_t) entity.getId(), entity.get<TagComponent>().tag);

        if (!entity.has<MeshComponent>()) {
            ENGINE_INFO("No MeshComponent for Entity {0}", entity.get<TagComponent>().tag);
            return;
        }

        auto& meshComponent = entity.get<MeshComponent>();
        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            meshComponent.updateStart(previousVertexCount, previousIndexCount);
            meshComponent.setInstanceId(instanceID);
            meshComponent.updateCounts();
            loadMesh(meshComponent);
        }

        previousIndexCount += meshComponent.totalIndexCount;
        previousVertexCount += meshComponent.totalVertexCount;
    }

    void Renderer::renderCamera(const Entity &entity) {
        if (entity.has<Camera2dComponent>()) {
            renderCamera(entity.get<Camera2dComponent>());
        }

        if (entity.has<Camera3dComponent>()) {
            renderCamera(entity.get<Camera3dComponent>());
        }
    }

    bool Renderer::renderTransform(const uint32_t &instanceId, const Entity &entity) {
        if (entity.has<Transform2dComponent>()) {
            _shader->setUniformArrayElement(instanceId, entity.get<Transform2dComponent>());
            return true;
        }

        if (entity.has<Transform3dComponent>()) {
            _shader->setUniformArrayElement(instanceId, entity.get<Transform3dComponent>());
            return true;
        }

        return false;
    }

    bool Renderer::renderTransform(const Entity &entity) {
        if (entity.has<Transform2dComponent>()) {
            _shader->setUniform(entity.get<Transform2dComponent>());
            return true;
        }

        if (entity.has<Transform3dComponent>()) {
            _shader->setUniform(entity.get<Transform3dComponent>());
            return true;
        }

        return false;
    }

    void Renderer::renderCamera(Mat4fUniform &camera) {
        if (_shader->getState() == ShaderState::NO_UNIFORM_BLOCKS) {
            _shader->setUniform(camera);
        } else {
            if (camera.isUpdated) {
                camera.isUpdated = false;

                auto uniformData = UniformData {
                    camera.toFloatPtr(),
                    0
                };
                _uniformBuffer->bind();
                _uniformBuffer->load(uniformData);
                _uniformBuffer->unbind();
            }
        }
    }

    void Renderer::renderPolygonMode(const Entity &entity) {
        if (entity.has<PolygonMode>()) {
            _drawer->setPolygonMode(entity.get<PolygonMode>());
        }
    }
}