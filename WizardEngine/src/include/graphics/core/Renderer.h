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

    typedef std::function<void(ecs::Registry&, ecs::entity_id)> Handle;
    struct EntityHandler {
        Handle handle;

        EntityHandler() = default;
        explicit EntityHandler(const Handle& handle) : handle(handle) {}
        explicit EntityHandler(Handle&& handle) : handle(handle) {}
        EntityHandler(const EntityHandler&) = default;
    };

    class Renderer {

    public:
        Renderer() = default;
        explicit Renderer(
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

        void addEntityHandler(const EntityHandler& entityHandler);
        void addEntityHandler(const Handle& handle);

    protected:
        void create(const AttributeCategory& attributeCategory);
        VRenderModel& createRenderModel(const uint32_t& vertexCount);
        VIRenderModel& createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount);

        template<typename T>
        bool validate(VertexDataComponent<T>& vertexDataComponent);
        template<typename T>
        bool validate(BaseMeshComponent<T>& meshComponent);

        void handleEntity(ecs::Registry& registry, ecs::entity_id entityId);

    protected:
        vector<VRenderModel> vRenderModels;
        vector<VIRenderModel> viRenderModels;
        BaseShaderProgram shaderProgram;
        DrawType drawType = DrawType::TRIANGLE;
        vector<EntityHandler> entityHandlers;
    };

    #define RENDERER_INIT(Child, Parent) \
    public:                      \
    explicit Child() : Parent() {}      \
                             \
    explicit Child(const BaseShaderProgram& shaderProgram, const DrawType& drawType = DrawType::TRIANGLE) \
    : Parent(shaderProgram, drawType) {}

    class BatchRenderer : public Renderer {

    RENDERER_INIT(BatchRenderer, Renderer)

    public:
        template<typename T, typename V>
        void renderV(ecs::Registry &registry);
        template<typename T, typename V>
        void renderVI(ecs::Registry &registry);
    };

    class InstanceRenderer : public Renderer {

    RENDERER_INIT(InstanceRenderer, Renderer)

    public:
        template<typename T, typename V>
        void renderV(ecs::Registry &registry);
        template<typename T, typename V>
        void renderVI(ecs::Registry &registry);
    };

    template<class Renderer1 = Renderer, class Renderer2 = Renderer>
    class MultiRenderer {
    public:
        MultiRenderer() = default;
        MultiRenderer(const Renderer1& renderer1, const Renderer2& renderer2)
        : renderer1(renderer1), renderer2(renderer2) {}

        MultiRenderer(
                const BaseShaderProgram& shader1,
                const BaseShaderProgram& shader2,
                const DrawType& drawType = DrawType::TRIANGLE
        ) {
            renderer1 = Renderer1(shader1, drawType);
            renderer2 = Renderer2(shader2, drawType);
        }

    public:
        template<typename T, typename V>
        void render(ecs::Registry& registry);
        template<typename T, typename V>
        void renderV(ecs::Registry& registry);
        template<typename T, typename V>
        void renderVI(ecs::Registry& registry);

        void addEntityHandler(const EntityHandler& entityHandler);
        void addEntityHandler(const Handle& handle);

    public:
        void release();

    private:
        Renderer1 renderer1;
        Renderer2 renderer2;
    };

    typedef MultiRenderer<BatchRenderer, InstanceRenderer> DefaultRenderer;

    class VRenderer {

    public:
        VRenderer() = default;
        explicit VRenderer(const BaseShaderProgram& shaderProgram) : shaderProgram(shaderProgram) {
            init();
        }
        ~VRenderer() = default;

    public:
        [[nodiscard]] inline const BaseShaderProgram& getShaderProgram() {
            return shaderProgram;
        }

    public:
        template<typename T, typename V>
        void render(const ecs::Entity& entity);
        template<typename T, typename V>
        void renderStatic(const ecs::Entity& entity);
        template<typename V>
        void render(VertexDataComponent<V>& vertexDataComponent);
        template<typename V>
        void render(VertexDataComponent<V>& vertexDataComponent, const uint32_t& textureId);
        void renderQuad(const u32& textureId);

        void release();

        template<typename T>
        void uploadStatic(const VertexDataComponent<T>& vertexDataComponent);
        template<typename T>
        void upload(VertexDataComponent<T>& vertexDataComponent);

    private:
        void init();

        void begin();
        void begin(const ecs::Entity& entity);
        void end(const DrawType& drawType, const u32& vertexCount) const;

        template<typename Vertex>
        void validate(const VertexDataComponent<Vertex>& vertexDataComponent);

        template<typename Transform>
        void uploadTransform(const ecs::Entity& entity);

    private:
        VRenderModel vRenderModel;
        BaseShaderProgram shaderProgram;
    };

    class VIRenderer {

    public:
        VIRenderer() = default;
        explicit VIRenderer(const BaseShaderProgram& shaderProgram) : shaderProgram(shaderProgram) {
            init();
        }
        ~VIRenderer() = default;

    public:
        template<typename T, typename V>
        void render(const ecs::Entity& entity);

        template<typename T>
        void upload(BaseMeshComponent<T>& meshComponent);

        void release();

    private:
        void init();

        void begin(const ecs::Entity& entity);
        void end(const DrawType& drawType, const u32& indexCount) const;

        template<typename Transform>
        void uploadTransform(const ecs::Entity& entity);

    private:
        VIRenderModel viRenderModel;
        BaseShaderProgram shaderProgram;
    };

    template<typename Transform, typename Vertex>
    void BatchRenderer::renderV(ecs::Registry &registry) {
        typedef VertexDataComponent<BatchVertex<Vertex>> Geometry;
        if (!shaderProgram.isReady() || registry.empty_entity() || registry.empty_components<Geometry>()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t nextRenderModelId = 0;
        registry.each<Transform, Geometry>([this, &nextRenderModelId](Transform* transform, Geometry* geometry) {
            if (drawType != geometry->drawType) return;

            geometry->renderModelId += nextRenderModelId;
            if (!validate<BatchVertex<Vertex>>(*geometry)) {
                nextRenderModelId++;
            }
        });

        for (auto& renderModel : vRenderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t i = 0;
            registry.each<Transform, Geometry>([this, &totalVertexCount, &renderModel, &i, &registry](Transform* transform, Geometry* geometry) {
                if (renderModel.id != geometry->renderModelId || drawType != geometry->drawType) {
                    i++; // shift instance id
                    return;
                }

                tryUploadBatch(i, *geometry, totalVertexCount, renderModel);
                shaderProgram.getVShader().setUniformArrayElement(i++, transform->modelMatrix);
                handleEntity(registry, transform->entityId);
                if (i > INSTANCE_COUNT_LIMIT) {
                    renderModel.vao.bind();
                    drawV(drawType, totalVertexCount);
                    i = 0;
                    totalVertexCount = 0;
                }
            });

            if (i > 0 && totalVertexCount > 0) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount);
            }
            resetCounts(renderModel);
        }

        ShaderProgram::stop();
    }

    template<typename Transform, typename Vertex>
    void BatchRenderer::renderVI(ecs::Registry &registry) {
        typedef BaseMeshComponent<BatchVertex<Vertex>> Mesh;
        if (!shaderProgram.isReady() || registry.empty_entity() || registry.empty_components<Mesh>()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t nextRenderModelId = 0;
        registry.each<Transform, Mesh>([this, &nextRenderModelId](Transform* transform, Mesh* mesh) {
            if (drawType != mesh->drawType) return;

            mesh->renderModelId += nextRenderModelId;
            if (!validate<BatchVertex<Vertex>>(*mesh)) {
                nextRenderModelId++;
            }
        });

        for (auto& renderModel : viRenderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t totalIndexCount = 0;
            uint32_t i = 0;
            registry.each<Transform, Mesh>([this, &renderModel, &totalVertexCount, &totalIndexCount, &i, &registry](Transform* transform, Mesh* mesh) {
                if (renderModel.id != mesh->renderModelId || drawType != mesh->drawType) {
                    i++; // shift instance id
                    return;
                }

                tryUploadBatchMesh(i, *mesh, totalVertexCount, totalIndexCount, renderModel);
                shaderProgram.getVShader().setUniformArrayElement(i++, transform->modelMatrix);
                handleEntity(registry, transform->entityId);
                if (i > INSTANCE_COUNT_LIMIT) {
                    renderModel.vao.bind();
                    drawVI(drawType, totalIndexCount);
                    i = 0;
                    totalVertexCount = 0;
                    totalIndexCount = 0;
                }
            });

            if (i > 0 && totalIndexCount > 0) {
                renderModel.vao.bind();
                drawVI(drawType, totalIndexCount);
            }
            resetCounts(renderModel);
        }

        ShaderProgram::stop();
    }

    template<typename Transform, typename Vertex>
    void InstanceRenderer::renderV(ecs::Registry& registry) {
        typedef VertexDataComponent<InstanceVertex<Vertex>> Geometry;
        if (!shaderProgram.isReady() || registry.empty_entity() || registry.empty_components<Geometry>()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t totalVertexCount = 0;
        uint32_t i = 0;
        uint32_t renderModelId = 0;
        bool renderModelReady = false;

        registry.each<Transform, Geometry>([this, &i, &renderModelReady, &renderModelId, &totalVertexCount, &registry](Transform* transform, Geometry* geometry) {
            if (drawType != geometry->drawType) {
                i++; // shift id
                return ;
            }

            if (!renderModelReady) {
                validate<InstanceVertex<Vertex>>(*geometry);
                renderModelId = geometry->renderModelId;

                auto& renderModel = vRenderModels[geometry->renderModelId];
                tryUpload<InstanceVertex<Vertex>>(*geometry, totalVertexCount, renderModel);

                renderModelReady = true;
            }

            shaderProgram.getVShader().setUniformArrayElement(i++, transform->modelMatrix);
            handleEntity(registry, transform->entityId);
            // if transform count is out of limit, then draw current instances and repeat iteration!
            auto& renderModel = vRenderModels[renderModelId];
            if (i > INSTANCE_COUNT_LIMIT) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount, i);
                i = 0;
            }
        });

        auto& renderModel = vRenderModels[renderModelId];
        if (i > 0 && totalVertexCount > 0) {
            renderModel.vao.bind();
            drawV(drawType, totalVertexCount, i);
        }

        ShaderProgram::stop();
    }

    template<typename Transform, typename Vertex>
    void InstanceRenderer::renderVI(ecs::Registry& registry) {
        typedef BaseMeshComponent<InstanceVertex<Vertex>> Mesh;
        if (!shaderProgram.isReady() || registry.empty_entity() || registry.empty_components<Mesh>()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        uint32_t i = 0;
        uint32_t renderModelId = 0;
        bool renderModelReady = false;

        registry.each<Transform, Mesh>([this, &i, &renderModelReady, &renderModelId, &totalVertexCount, &totalIndexCount, &registry]
        (Transform* transform, Mesh* mesh) {
            if (drawType != mesh->drawType) {
                i++; // next id
                return;
            }

            if (!renderModelReady) {
                validate<InstanceVertex<Vertex>>(*mesh);
                renderModelId = mesh->renderModelId;

                auto& renderModel = viRenderModels[mesh->renderModelId];
                tryUpload<InstanceVertex<Vertex>>(*mesh, totalVertexCount, totalIndexCount, renderModel);

                renderModelReady = true;
            }

            shaderProgram.getVShader().setUniformArrayElement(i++, transform->modelMatrix);
            handleEntity(registry, transform->entityId);
            // if transform count is out of limit, then draw current instances and continue iteration!
            auto& renderModel = viRenderModels[renderModelId];
            if (i > INSTANCE_COUNT_LIMIT) {
                renderModel.vao.bind();
                drawVI(drawType, totalIndexCount, i);
                i = 0;
            }
        });

        auto& renderModel = viRenderModels[renderModelId];
        if (i > 0 && totalIndexCount > 0) {
            renderModel.vao.bind();
            drawVI(drawType, totalIndexCount, i);
        }

        ShaderProgram::stop();
    }

    template<class Renderer1, class Renderer2>
    template<typename Transform, typename Vertex>
    void MultiRenderer<Renderer1, Renderer2>::render(ecs::Registry& registry) {
        renderV<Transform, Vertex>(registry);
        renderVI<Transform, Vertex>(registry);
    }

    template<class Renderer1, class Renderer2>
    template<typename Transform, typename Vertex>
    void MultiRenderer<Renderer1, Renderer2>::renderV(ecs::Registry& registry) {
        renderer1.template renderV<Transform, Vertex>(registry);
        renderer2.template renderV<Transform, Vertex>(registry);
    }

    template<class Renderer1, class Renderer2>
    template<typename Transform, typename Vertex>
    void MultiRenderer<Renderer1, Renderer2>::renderVI(ecs::Registry& registry) {
        renderer1.template renderVI<Transform, Vertex>(registry);
        renderer2.template renderVI<Transform, Vertex>(registry);
    }

    template<class Renderer1, class Renderer2>
    void MultiRenderer<Renderer1, Renderer2>::release() {
        renderer1.release();
        renderer2.release();
    }

    template<class Renderer1, class Renderer2>
    void MultiRenderer<Renderer1, Renderer2>::addEntityHandler(const EntityHandler &entityHandler) {
        renderer1.addEntityHandler(entityHandler);
        renderer2.addEntityHandler(entityHandler);
    }

    template<class Renderer1, class Renderer2>
    void MultiRenderer<Renderer1, Renderer2>::addEntityHandler(const Handle &handle) {
        renderer1.addEntityHandler(handle);
        renderer2.addEntityHandler(handle);
    }

    template<typename Transform, typename Vertex>
    void VRenderer::render(const ecs::Entity &entity) {
        VertexDataComponent<Vertex>* vertexDataComponent = entity.get<VertexDataComponent<Vertex>>();
        if (!shaderProgram.isReady() || !vertexDataComponent) return;

        begin(entity);

        upload(*vertexDataComponent);
        uploadTransform<Transform>(entity);

        end(vertexDataComponent->drawType, vertexDataComponent->vertexData.vertexCount);
    }

    template<typename Transform, typename Vertex>
    void VRenderer::renderStatic(const ecs::Entity &entity) {
        VertexDataComponent<Vertex>* vertexDataComponent = entity.get<VertexDataComponent<Vertex>>();
        if (!shaderProgram.isReady() || !vertexDataComponent) return;

        begin(entity);

        uploadTransform<Transform>(entity);

        end(vertexDataComponent->drawType, vertexDataComponent->vertexData.vertexCount);
    }

    template<typename Vertex>
    void VRenderer::render(VertexDataComponent<Vertex>& vertexDataComponent) {
        if (!shaderProgram.isReady()) return;

        begin();

        upload(vertexDataComponent);

        end(vertexDataComponent.drawType, vertexDataComponent.vertexData.vertexCount);
    }

    template<typename Vertex>
    void VRenderer::render(VertexDataComponent<Vertex>& vertexDataComponent, const uint32_t& textureId) {
        if (!shaderProgram.isReady()) return;

        begin();

        upload(vertexDataComponent);
        TextureBuffer::activate(0);
        TextureBuffer::bind(textureId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D));

        end(vertexDataComponent.drawType, vertexDataComponent.vertexData.vertexCount);
    }

    template<typename Vertex>
    void VRenderer::validate(const VertexDataComponent<Vertex>& vertexDataComponent) {
        if (!hasCapacity(vRenderModel, vertexDataComponent)) {
            vRenderModel = { 0, vertexDataComponent.vertexData.vertexCount };
            vRenderModel.vao.bind();
            vRenderModel.vbo.setFormat(shaderProgram.getVertexFormat());
            VertexArray::unbind();
        }
    }

    template<typename Vertex>
    void VRenderer::upload(VertexDataComponent<Vertex> &vertexDataComponent) {
        validate(vertexDataComponent);
        if (vertexDataComponent.isUpdated) {
            vertexDataComponent.isUpdated = false;
            vRenderModel.upload(vertexDataComponent);
        }
    }

    template<typename Vertex>
    void VRenderer::uploadStatic(const VertexDataComponent<Vertex>& vertexDataComponent) {
        validate(vertexDataComponent);
        vRenderModel.uploadStatic(vertexDataComponent);
    }

    template<typename Transform>
    void VRenderer::uploadTransform(const ecs::Entity &entity) {
        auto transform = entity.get<Transform>();
        if (transform) {
            shaderProgram.getVShader().setUniform(transform->modelMatrix);
        }
    }

    template<typename Transform, typename Vertex>
    void VIRenderer::render(const ecs::Entity &entity) {
        BaseMeshComponent<Vertex>* mesh = entity.get<BaseMeshComponent<Vertex>>();
        if (!shaderProgram.isReady() || !mesh) return;

        begin(entity);

        upload(*mesh);
        uploadTransform<Transform>(entity);

        end(mesh->drawType, mesh->totalIndexCount);
    }

    template<typename Vertex>
    void VIRenderer::upload(BaseMeshComponent<Vertex> &meshComponent) {
        if (!hasCapacity(viRenderModel, meshComponent)) {
            viRenderModel = { 0, meshComponent.totalVertexCount, meshComponent.totalIndexCount };
            viRenderModel.vao.bind();
            viRenderModel.vbo.setFormat(shaderProgram.getVertexFormat());
            viRenderModel.ibo.bind();
            viRenderModel.ibo.alloc();
            VertexArray::unbind();
        }

        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            viRenderModel.upload(meshComponent);
        }
    }

    template<typename Transform>
    void VIRenderer::uploadTransform(const ecs::Entity &entity) {
        auto transform = entity.get<Transform>();
        if (transform) {
            shaderProgram.getVShader().setUniform(transform->modelMatrix);
        }
    }

    template<typename T>
    bool Renderer::validate(VertexDataComponent<T>& vertexDataComponent) {
        if (vRenderModels.empty() || !hasCapacity(vRenderModels[vertexDataComponent.renderModelId], vertexDataComponent)) {
            auto vertexCount = vertexDataComponent.vertexData.vertexCount > DEFAULT_VERTEX_COUNT
                               ? vertexDataComponent.vertexData.vertexCount * 3 : DEFAULT_VERTEX_COUNT;
            auto& newRenderModel = createRenderModel(vertexCount);
            vertexDataComponent.renderModelId = newRenderModel.id;
            increaseCounts(newRenderModel, vertexDataComponent);
            return false;
        }

        increaseCounts(vRenderModels[vertexDataComponent.renderModelId], vertexDataComponent);
        return true;
    }

    template<typename T>
    bool Renderer::validate(BaseMeshComponent<T>& meshComponent) {
        if ((meshComponent.totalIndexCount == 0 || meshComponent.totalVertexCount == 0) && meshComponent.meshCount > 0) {
            updateStartAndCounts(meshComponent, 0, 0);
        }

        if (viRenderModels.empty() || !hasCapacity(viRenderModels[meshComponent.renderModelId], meshComponent)) {
            auto vertexCount = meshComponent.totalVertexCount > DEFAULT_VERTEX_COUNT ? meshComponent.totalVertexCount * 3 : DEFAULT_VERTEX_COUNT;
            auto indexCount = meshComponent.totalIndexCount > DEFAULT_INDEX_COUNT ? meshComponent.totalIndexCount * 3 : DEFAULT_INDEX_COUNT;
            auto& newRenderModel = createRenderModel(vertexCount, indexCount);
            meshComponent.renderModelId = newRenderModel.id;
            increaseCounts(newRenderModel, meshComponent);
            return false;
        }

        increaseCounts(viRenderModels[meshComponent.renderModelId], meshComponent);
        return true;
    }
}
