//
// Created by mecha on 05.09.2021.
//

#include "RenderSystem.h"

namespace engine {

    void RenderSystem::create() {
        sceneRenderer = createRef<Renderer>(graphicsFactory);
    }

    void RenderSystem::destroy() {
        shaderCache.clear();
        vertexBufferCache.clear();
        activeScene = nullptr;
    }

    void RenderSystem::onUpdate() {
        if (activeScene == nullptr) {
            ENGINE_WARN("RenderSystem : No active scene!");
            return;
        }

        auto renderableEntities = activeScene->getEntities().view<MeshComponent>();
        if (renderableEntities.empty()) {
            ENGINE_WARN("RenderSystem : Nothing to draw!");
            return;
        }

        sceneRenderer->startFrame();

        sceneRenderer->start();

        sceneRenderer->renderCamera();

        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        for (const auto& renderableEntity : renderableEntities) {
            auto& meshComponent = renderableEntities.get<MeshComponent>(renderableEntity);
            if (meshComponent.isUpdated) {
                meshComponent.isUpdated = false;
                meshComponent.updateStart(totalVertexCount, totalIndexCount);
                sceneRenderer->loadMesh(meshComponent);
            }

            meshComponent.updateCounts();
            totalIndexCount += meshComponent.indexCount;
            totalVertexCount += meshComponent.vertexCount;

            renderMaterial(renderableEntity);
            renderTransform(renderableEntity);
        }

        sceneRenderer->drawByIndices(totalIndexCount);

        sceneRenderer->stop();

        sceneRenderer->endFrame();
    }

    void RenderSystem::addShader(const std::string &name, const Ref<Shader> &shader) {
        shaderCache.add(name, shader);
        sceneRenderer->prepare(shader);
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
            sceneRenderer->prepare(shader);
        }
        return shaderError;
    }

    Ref<Shader> RenderSystem::getShader(const std::string &name) {
        return shaderCache.get(name);
    }

    bool RenderSystem::shaderExists(const std::string &name) const {
        return shaderCache.exists(name);
    }

    void RenderSystem::enableDepth() {
        sceneRenderer->enableDepth();
    }

    void RenderSystem::setPolygonMode(const PolygonMode &polygonMode) {
        sceneRenderer->setPolygonMode(polygonMode);
    }

    void RenderSystem3d::renderMaterial(const entt::entity &entity) {
        if (!activeScene->hasComponent<TextureComponent>(entity)) return;

        auto& texture = activeScene->getComponent<TextureComponent>(entity);
        sceneRenderer->renderMaterial(texture);
    }

    void RenderSystem3d::renderTransform(const entt::entity &entity) {
        if (!activeScene->hasComponent<TransformComponent3d>(entity)) return;

        auto& transform3d = activeScene->getComponent<TransformComponent3d>(entity);
        sceneRenderer->renderTransform(transform3d);
    }

    void RenderSystem2d::renderMaterial(const entt::entity &entity) {
        if (!activeScene->hasComponent<TextureComponent>(entity)) return;

        auto& texture = activeScene->getComponent<TextureComponent>(entity);
        sceneRenderer->renderMaterial(texture);
    }

    void RenderSystem2d::renderTransform(const entt::entity &entity) {
        if (!activeScene->hasComponent<TransformComponent2d>(entity)) return;

        auto& transform2d = activeScene->getComponent<TransformComponent2d>(entity);
        sceneRenderer->renderTransform(transform2d);
    }

    void RenderSystem::onWindowClosed() {
    }

    void RenderSystem::onWindowResized(const uint32_t &width, const uint32_t &height) {
        sceneRenderer->resizeFrame(width, height);
    }

    void RenderSystem::updateFboSpecification(const FramebufferSpecification &framebufferSpecification) {
        const auto& colorTextures = sceneRenderer->updateFboSpecs(framebufferSpecification);
        ENGINE_INFO("Update FBO specs. Scene texture id : {0}", colorTextures[0]);
        activeScene->setTextureId(colorTextures[0]);
    }

    void RenderSystem::loadTexture(const std::string &fileName) {
        sceneRenderer->loadTexture(fileName);
    }

}