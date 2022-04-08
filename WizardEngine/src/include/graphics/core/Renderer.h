//
// Created by mecha on 06.10.2021.
//

#pragma once

#include <graphics/core/RenderModel.h>
#include <graphics/core/shader/BaseShader.h>
#include <ecs/Components.h>
#include <graphics/transform/TransformComponents.h>
#include <platform/graphics/RenderCommands.h>
#include <platform/graphics/TextureBuffer.h>

#define INSTANCE_COUNT_LIMIT 128

using namespace engine::shader;

namespace engine::graphics {

    class Renderer {

    public:
        Renderer() = default;
        Renderer(
                const BaseShaderProgram& shaderProgram,
                const DrawType& drawType = DrawType::TRIANGLE,
                const AttributeCategory& attributeCategory = VERTEX
        ) : shaderProgram(shaderProgram), drawType(drawType) {
            create(attributeCategory);
        }
        ~Renderer() = default;

    public:
        // call this function when you are ready to release data from GPU
        void release();

    protected:
        void create(const AttributeCategory& attributeCategory);
        VRenderModel& createRenderModel(const uint32_t& vertexCount);
        VIRenderModel& createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount);
        template<typename T>
        bool validate(VRenderModel &renderModel, VertexDataComponent<T>& vertexDataComponent);
        template<typename T>
        bool validate(VIRenderModel &renderModel, BaseMeshComponent<T>& meshComponent);

    protected:
        std::vector<VRenderModel> vRenderModels;
        std::vector<VIRenderModel> viRenderModels;
        BaseShaderProgram shaderProgram;
        DrawType drawType = DrawType::TRIANGLE; // that's not ideal, because we need to dispatch this every frame!
    };

    class BatchRenderer : public Renderer {

    public:
        BatchRenderer() : Renderer() {}
        BatchRenderer(const BaseShaderProgram& shaderProgram, const DrawType& drawType = DrawType::TRIANGLE)
        : Renderer(shaderProgram, drawType) {
            init();
        }

    public:
        template<typename T, typename V>
        void renderV(entt::registry &registry);
        template<typename T, typename V>
        void renderVI(entt::registry &registry);

    private:
        void init();
    };

    class InstanceRenderer : public Renderer {

    public:
        InstanceRenderer() : Renderer() {}
        InstanceRenderer(const BaseShaderProgram& shaderProgram, const DrawType& drawType = DrawType::TRIANGLE)
        : Renderer(shaderProgram, drawType) {
            init();
        }

    public:
        template<typename T, typename V>
        void renderV(entt::registry &registry);
        template<typename T, typename V>
        void renderVI(entt::registry &registry);

    private:
        void init();
    };

    class MultiRenderer {

    public:
        MultiRenderer() = default;
        MultiRenderer(const BatchRenderer& batchRenderer, const InstanceRenderer& instanceRenderer)
        : batchRenderer(batchRenderer), instanceRenderer(instanceRenderer) {}

        MultiRenderer(
                const BaseShaderProgram& batchShader,
                const BaseShaderProgram& instanceShader,
                const DrawType& drawType = DrawType::TRIANGLE
        ) {
            batchRenderer = BatchRenderer(batchShader, drawType);
            instanceRenderer = InstanceRenderer(instanceShader, drawType);
        }

    public:
        template<typename T, typename V>
        void render(entt::registry& registry);
        template<typename T, typename V>
        void renderV(entt::registry& registry);
        template<typename T, typename V>
        void renderVI(entt::registry& registry);

    public:
        void release();

    private:
        BatchRenderer batchRenderer;
        InstanceRenderer instanceRenderer;

    };

    class VRenderer {

    public:
        VRenderer() = default;
        VRenderer(const BaseShaderProgram& shaderProgram)
        : shaderProgram(shaderProgram) {
            create();
        }

        ~VRenderer() = default;

    public:
        [[nodiscard]] inline const BaseShaderProgram& getShaderProgram() {
            return shaderProgram;
        }

    public:
        template<typename T, typename V>
        void render(const Entity& entity);
        template<typename V>
        void render(VertexDataComponent<V>& vertexDataComponent);
        template<typename V>
        void render(VertexDataComponent<V>& vertexDataComponent, const uint32_t& textureId);
        void renderQuad(const u32& textureId);

    public:
        void release();
        // uploads vertex data to vertex buffer, as data that shouldn't change
        template<typename T>
        void uploadStatic(const VertexDataComponent<T>& vertexDataComponent);

    private:
        void create();

    private:
        VRenderModel vRenderModel;
        BaseShaderProgram shaderProgram;

    };

    class VIRenderer {

    public:
        VIRenderer() = default;
        VIRenderer(const BaseShaderProgram& shaderProgram)
        : shaderProgram(shaderProgram) {
            create();
        }

        ~VIRenderer() = default;

    public:
        template<typename T, typename V>
        void render(const Entity& entity);

    public:
        void release();

    private:
        void create();

    private:
        VIRenderModel viRenderModel;
        BaseShaderProgram shaderProgram;
    };

    template<typename T, typename V>
    void BatchRenderer::renderV(entt::registry &registry) {
        if (!shaderProgram.isReady()) return;

        auto entities = registry.view<T, VertexDataComponent<BatchVertex<V>>>();
        if (entities.size_hint() == 0) return; // nothing to render

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t nextRenderModelId = 0;
        for (auto [entity, transform, vertexDataComponent] : entities.each()) {
            if (drawType != vertexDataComponent.drawType) continue;

            vertexDataComponent.renderModelId += nextRenderModelId;
            auto& renderModel = vRenderModels[vertexDataComponent.renderModelId];
            if (!validate<BatchVertex<V>>(renderModel, vertexDataComponent)) {
                nextRenderModelId++;
            }
        }

        for (auto& renderModel : vRenderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t i = 0;
            for (auto [entity, transform, vertexDataComponent] : entities.each()) {
                if (renderModel.id != vertexDataComponent.renderModelId || drawType != vertexDataComponent.drawType) {
                    i++; // shift instance id
                    continue;
                }

                tryUploadBatch(i, vertexDataComponent, totalVertexCount, renderModel);
                shaderProgram.getVShader().setUniformArrayElement(i++, transform);
                if (i > INSTANCE_COUNT_LIMIT) {
                    renderModel.vao.bind();
                    drawV(drawType, totalVertexCount);
                    i = 0;
                    totalVertexCount = 0;
                }
            }

            if (i > 0 && totalVertexCount > 0) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount);
            }
            resetCounts(renderModel);
        }

        ShaderProgram::stop();
    }

    template<typename T, typename V>
    void BatchRenderer::renderVI(entt::registry &registry) {
        if (!shaderProgram.isReady()) return;

        auto entities = registry.view<T, BaseMeshComponent<BatchVertex<V>>>();
        if (entities.size_hint() == 0) return; // nothing to render

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t nextRenderModelId = 0;
        for (auto [entity, transform, mesh] : entities.each()) {
            if (drawType != mesh.drawType) continue;

            mesh.renderModelId += nextRenderModelId;
            auto& renderModel = viRenderModels[mesh.renderModelId];
            if (!validate<BatchVertex<V>>(renderModel, mesh)) {
                nextRenderModelId++;
            }
        }

        for (auto& renderModel : viRenderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t totalIndexCount = 0;
            uint32_t i = 0;
            for (auto [entity, transform, mesh] : entities.each()) {
                if (renderModel.id != mesh.renderModelId || drawType != mesh.drawType) {
                    i++; // shift instance id
                    continue;
                }

                tryUploadBatchMesh(i, mesh, totalVertexCount, totalIndexCount, renderModel);
                shaderProgram.getVShader().setUniformArrayElement(i++, transform);
                if (i > INSTANCE_COUNT_LIMIT) {
                    renderModel.vao.bind();
                    drawVI(drawType, totalIndexCount);
                    i = 0;
                    totalVertexCount = 0;
                    totalIndexCount = 0;
                }
            }

            if (i > 0 && totalIndexCount > 0) {
                renderModel.vao.bind();
                drawVI(drawType, totalIndexCount);
            }
            resetCounts(renderModel);
        }

        ShaderProgram::stop();
    }

    template<typename T, typename V>
    void InstanceRenderer::renderV(entt::registry& registry) {
        if (!shaderProgram.isReady()) return;

        auto entities = registry.view<T, VertexDataComponent<InstanceVertex<V>>>();
        if (entities.size_hint() == 0) return; // nothing to render

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t totalVertexCount = 0;
        uint32_t i = 0;
        uint32_t renderModelId = 0;
        bool renderModelReady = false;
        for (auto [entity, transform, vertexDataComponent] : entities.each()) {
            if (drawType != vertexDataComponent.drawType) {
                i++; // shift id
                continue;
            }

            if (!renderModelReady) {
                auto& oldRenderModel = vRenderModels[vertexDataComponent.renderModelId];
                resetCounts(oldRenderModel);
                validate<InstanceVertex<V>>(oldRenderModel, vertexDataComponent);
                renderModelId = vertexDataComponent.renderModelId;

                auto& renderModel = vRenderModels[vertexDataComponent.renderModelId];
                tryUpload<InstanceVertex<V>>(vertexDataComponent, totalVertexCount, renderModel);

                renderModelReady = true;
            }

            shaderProgram.getVShader().setUniformArrayElement(i++, transform);
            // if transform count is out of limit, then draw current instances and repeat iteration!
            auto& renderModel = vRenderModels[renderModelId];
            if (i > INSTANCE_COUNT_LIMIT) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount, i);
                i = 0;
            }
        }

        auto& renderModel = vRenderModels[renderModelId];
        if (i > 0 && totalVertexCount > 0) {
            renderModel.vao.bind();
            drawV(drawType, totalVertexCount, i);
        }

        ShaderProgram::stop();
    }

    template<typename T, typename V>
    void InstanceRenderer::renderVI(entt::registry& registry) {
        if (!shaderProgram.isReady()) return;

        auto entities = registry.view<T, BaseMeshComponent<InstanceVertex<V>>>();
        if (entities.size_hint() == 0) return; // nothing to render

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        uint32_t i = 0;
        uint32_t renderModelId = 0;
        bool renderModelReady = false;
        for (auto [entity, transform, mesh] : entities.each()) {
            if (drawType != mesh.drawType) {
                i++; // next id
                continue;
            }

            if (!renderModelReady) {
                auto& oldRenderModel = viRenderModels[mesh.renderModelId];
                resetCounts(oldRenderModel);
                validate<InstanceVertex<V>>(oldRenderModel, mesh);
                renderModelId = mesh.renderModelId;

                auto& renderModel = viRenderModels[mesh.renderModelId];
                tryUpload<InstanceVertex<V>>(mesh, totalVertexCount, totalIndexCount, renderModel);

                renderModelReady = true;
            }

            shaderProgram.getVShader().setUniformArrayElement(i++, transform);
            // if transform count is out of limit, then draw current instances and continue iteration!
            auto& renderModel = viRenderModels[renderModelId];
            if (i > INSTANCE_COUNT_LIMIT) {
                renderModel.vao.bind();
                drawVI(drawType, totalIndexCount, i);
                i = 0;
            }
        }

        auto& renderModel = viRenderModels[renderModelId];
        if (i > 0 && totalIndexCount > 0) {
            renderModel.vao.bind();
            drawVI(drawType, totalIndexCount, i);
        }

        ShaderProgram::stop();
    }

    template<typename T, typename V>
    void MultiRenderer::render(entt::registry& registry) {
        renderV<T,V>(registry);
        renderVI<T,V>(registry);
    }

    template<typename T, typename V>
    void MultiRenderer::renderV(entt::registry& registry) {
        batchRenderer.renderV<T,V>(registry);
        instanceRenderer.renderV<T,V>(registry);
    }

    template<typename T, typename V>
    void MultiRenderer::renderVI(entt::registry& registry) {
        batchRenderer.renderVI<T,V>(registry);
        instanceRenderer.renderVI<T,V>(registry);
    }

    template<typename T, typename V>
    void VRenderer::render(const Entity &entity) {
        if (!entity.isValid()) return;

        auto vertexDataComponent = entity.getPtr<VertexDataComponent<V>>();
        if (!shaderProgram.isReady() || !vertexDataComponent) return;

        shaderProgram.start();
        shaderProgram.update(entity);

        uint32_t totalVertexCount = 0;
        tryUpload(*vertexDataComponent, totalVertexCount, vRenderModel);
        // try to upload transform
        auto transform = entity.getPtr<T>();
        if (transform) {
            shaderProgram.getVShader().setUniform(*transform);
        }

        vRenderModel.vao.bind();
        drawV(vertexDataComponent->drawType, totalVertexCount);

        ShaderProgram::stop();
    }

    template<typename V>
    void VRenderer::render(VertexDataComponent<V>& vertexDataComponent) {
        if (!shaderProgram.isReady()) return;

        shaderProgram.start();

        uint32_t totalVertexCount = 0;
        tryUpload(vertexDataComponent, totalVertexCount, vRenderModel);
        vRenderModel.vao.bind();
        drawV(vertexDataComponent.drawType, totalVertexCount);

        ShaderProgram::stop();
    }

    template<typename V>
    void VRenderer::render(VertexDataComponent<V>& vertexDataComponent, const uint32_t& textureId) {
        if (!shaderProgram.isReady()) return;

        shaderProgram.start();

        TextureBuffer::activate(0);
        TextureBuffer::bind(textureId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D));

        uint32_t totalVertexCount = 0;
        tryUpload(vertexDataComponent, totalVertexCount, vRenderModel);
        vRenderModel.vao.bind();
        drawV(vertexDataComponent.drawType, totalVertexCount);

        ShaderProgram::stop();
    }

    template<typename T>
    void VRenderer::uploadStatic(const VertexDataComponent<T>& vertexDataComponent) {
        graphics::uploadStatic(vertexDataComponent, vRenderModel);
    }

    template<typename T, typename V>
    void VIRenderer::render(const Entity &entity) {
        if (!entity.isValid()) return;

        auto mesh = entity.getPtr<BaseMeshComponent<V>>();
        if (!shaderProgram.isReady() || !mesh) return;

        shaderProgram.start();
        shaderProgram.update(entity);

        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        tryUpload(*mesh, totalVertexCount, totalIndexCount, viRenderModel);
        // try to upload transform
        auto transform = entity.getPtr<T>();
        if (transform) {
            shaderProgram.getVShader().setUniform(*transform);
        }

        viRenderModel.vao.bind();
        drawVI(mesh->drawType, totalIndexCount);

        ShaderProgram::stop();
    }

    template<typename T>
    bool Renderer::validate(VRenderModel &renderModel, VertexDataComponent<T>& vertexDataComponent) {
        if (!hasCapacity(renderModel, vertexDataComponent)) {
            auto vertexCount = vertexDataComponent.vertexData.vertexCount > DEFAULT_VERTEX_COUNT
                               ? vertexDataComponent.vertexData.vertexCount * 3 : DEFAULT_VERTEX_COUNT;
            auto& newRenderModel = createRenderModel(vertexCount);
            vertexDataComponent.renderModelId = newRenderModel.id;
            increaseCounts(newRenderModel, vertexDataComponent);
            return false;
        }
        increaseCounts(renderModel, vertexDataComponent);
        return true;
    }

    template<typename T>
    bool Renderer::validate(VIRenderModel &renderModel, BaseMeshComponent<T>& meshComponent) {
        if ((meshComponent.totalIndexCount == 0 || meshComponent.totalVertexCount == 0) && meshComponent.meshCount > 0) {
            updateCounts(meshComponent);
        }
        if (!hasCapacity(renderModel, meshComponent)) {
            auto vertexCount = meshComponent.totalVertexCount > DEFAULT_VERTEX_COUNT ? meshComponent.totalVertexCount * 3 : DEFAULT_VERTEX_COUNT;
            auto indexCount = meshComponent.totalIndexCount > DEFAULT_INDEX_COUNT ? meshComponent.totalIndexCount * 3 : DEFAULT_INDEX_COUNT;
            auto& newRenderModel = createRenderModel(vertexCount, indexCount);
            meshComponent.renderModelId = newRenderModel.id;
            increaseCounts(newRenderModel, meshComponent);
            return false;
        }
        increaseCounts(renderModel, meshComponent);
        return true;
    }
}
