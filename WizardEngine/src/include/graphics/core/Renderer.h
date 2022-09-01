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
#include <graphics/GraphicsObject.h>

using namespace engine::shader;

namespace engine::graphics {

    typedef std::function<void(ecs::Registry&, ecs::entity_id, u32, BaseShaderProgram&)> Handle;
    struct EntityHandler {
        Handle handle;

        EntityHandler() = default;
        explicit EntityHandler(const Handle& handle) : handle(handle) {}
        explicit EntityHandler(Handle&& handle) : handle(handle) {}
        EntityHandler(const EntityHandler&) = default;
    };

    // Renderer which is not using RenderModels for storing geometry
    // It creates primitive geometry under the hood, inside shader program
    class PrimitiveRenderer {

    public:
        PrimitiveRenderer() = default;
        PrimitiveRenderer(const BaseShaderProgram& shaderProgram) : shaderProgram(shaderProgram) {}

    public:
        void release();
        void renderQuad();
        void renderQuad(u32 textureId);

    protected:
        BaseShaderProgram shaderProgram;
    };

    // Basic renderer which is using dynamic array of VRenderModels and VIRenderModels to store geometry.
    // Uses BaseShaderProgram to execute and draw shader, DrawType to specify draw primitives
    // Uses EntityHandlers as a callback to user space during Registry/Entity rendering
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
        inline void setDrawType(const DrawType& drawType) {
            this->drawType = drawType;
        }

        BaseShaderProgram& getShader() {
            return shaderProgram;
        }

    public:
        // implement this for draw algorithm
        virtual void render(ecs::Registry& registry) = 0;
        // call this function when you are ready to release data from GPU
        void release();

        void addEntityHandler(const EntityHandler& entityHandler);
        void addEntityHandler(const Handle& handle);

        template<typename T>
        void createRenderModel(VertexDataComponent<T>& vertexDataComponent);
        template<typename T>
        void createRenderModel(BaseMeshComponent<T>& baseMeshComponent);
        template<typename T>
        VRenderModel& createRenderModel(const vector<VertexDataComponent<T>>& vertexDataComponents);
        template<typename T>
        VIRenderModel& createRenderModel(const vector<BaseMeshComponent<T>>& baseMeshComponents);
        template<typename T>
        void createVRenderModel(vector<Object<T>>& objects);
        template<typename T>
        void createVIRenderModel(vector<Object<T>>& objects);
        template<typename T>
        void createVRenderModelInstanced(Object<T>& object, const vector<Object<T>>& objects);
        template<typename T>
        void createVIRenderModelInstanced(Object<T>& object, const vector<Object<T>>& objects);

        template<typename T>
        void createRenderModels(const vector<VertexDataComponent<T>>& vertexDataComponents);
        template<typename T>
        void createRenderModels(const vector<BaseMeshComponent<T>>& baseMeshComponents);

    protected:
        void create(const AttributeCategory& attributeCategory);
        VRenderModel& createRenderModel(const uint32_t& vertexCount);
        VIRenderModel& createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount);

//        template<typename T>
//        bool validate(VertexDataComponent<T>& vertexDataComponent);
//        template<typename T>
//        bool validate(BaseMeshComponent<T>& meshComponent, u32& meshIndex);

        void handleEntity(ecs::Registry& registry, ecs::entity_id entityId, u32 index);

    protected:
        vector<VRenderModel> vRenderModels;
        vector<VIRenderModel> viRenderModels;
        BaseShaderProgram shaderProgram;
        DrawType drawType = DrawType::TRIANGLE;
        vector<EntityHandler> entityHandlers;
    };

    template<typename Vertex>
    class BatchRenderer : public Renderer {

    public:
        BatchRenderer() : Renderer() {}
        BatchRenderer(
                const BaseShaderProgram& shaderProgram,
                const DrawType& drawType = DrawType::TRIANGLE,
                const AttributeCategory& attributeCategory = VERTEX
        ) : Renderer(shaderProgram, drawType, attributeCategory) {}

    public:
        void render(ecs::Registry& registry) override;
    private:
        void renderV(ecs::Registry &registry);
        void renderVI(ecs::Registry &registry);
    };

    template<typename Vertex>
    class InstanceRenderer : public Renderer {

    public:
        InstanceRenderer() : Renderer() {}
        InstanceRenderer(
                const BaseShaderProgram& shaderProgram,
                const DrawType& drawType = DrawType::TRIANGLE,
                const AttributeCategory& attributeCategory = VERTEX
        ) : Renderer(shaderProgram, drawType, attributeCategory) {}

    public:
        void render(ecs::Registry &registry) override;

    private:
        void renderV(ecs::Registry &registry);
        void renderVI(ecs::Registry &registry);
    };

    template<typename Vertex>
    class VRenderer {

    public:
        VRenderer() = default;
        VRenderer(const BaseShaderProgram& shaderProgram);
        ~VRenderer() = default;

    public:
        void render(const ecs::Entity& entity);
        void renderStatic(const ecs::Entity& entity);
        void render(VertexDataComponent<Vertex>& vertexDataComponent);
        void render(VertexDataComponent<Vertex>& vertexDataComponent, const uint32_t& textureId);

        void release();

        void uploadStatic(const VertexDataComponent<Vertex>& vertexDataComponent);
        void upload(VertexDataComponent<Vertex>& vertexDataComponent);

    protected:
        virtual void uploadUniforms(const ecs::Entity& entity);

    private:
        void validate(const VertexDataComponent<Vertex>& vertexDataComponent);
        template<typename Transform>
        void uploadTransform(const ecs::Entity& entity);

    protected:
        VRenderModel vRenderModel;
        BaseShaderProgram shaderProgram;
    };

    template<typename Vertex>
    class VIRenderer {

    public:
        VIRenderer() = default;
        explicit VIRenderer(const BaseShaderProgram& shaderProgram) : shaderProgram(shaderProgram) {
            init();
        }
        ~VIRenderer() = default;

    public:
        void render(const ecs::Entity& entity);
        void upload(BaseMeshComponent<Vertex>& meshComponent);
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

    template<typename Vertex>
    void BatchRenderer<Vertex>::renderV(ecs::Registry &registry) {
        typedef VertexDataComponent<BatchVertex<Vertex>> Geometry;
        typedef Transform3dComponent Transform;

        if (!shaderProgram.isReady() || registry.empty_entity() || registry.empty_components<Geometry>()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        // batching Geometry and drawing foreach RenderModel
        for (VRenderModel& renderModel : vRenderModels) {
            // for draw call
            u32 totalVertexCount = 0;
            // for indexing geometry as instances
            u32 i = 0;
            for (ecs::entity_id entityId : renderModel.entities) {
                Geometry * geometry = registry.getComponent<Geometry>(entityId);
                Transform* transform = registry.getComponent<Transform>(entityId);
                renderModel.tryUploadBatch(i, *geometry, totalVertexCount);
                shaderProgram.setUniformArrayElement(i, transform->modelMatrix);
                handleEntity(registry, entityId, i);
                i++;
//                ENGINE_INFO("instanceID: {0}, renderModelId: {1}", mesh->getId(), mesh->renderModelId);
            }

            renderModel.vao.bind();
            drawV(drawType, totalVertexCount);
        }

        ShaderProgram::stop();
    }

    template<typename Vertex>
    void BatchRenderer<Vertex>::renderVI(ecs::Registry &registry) {
        typedef BaseMeshComponent<BatchVertex<Vertex>> Mesh;
        typedef Transform3dComponent Transform;

        if (!shaderProgram.isReady() || registry.empty_entity() || registry.empty_components<Mesh>()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        // batching Meshes and drawing foreach RenderModel
        for (VIRenderModel& renderModel : viRenderModels) {
            // for draw call
            u32 totalVertexCount = 0;
            u32 totalIndexCount = 0;
            // for indexing meshes as instances
            u32 i = 0;
            for (ecs::entity_id entityId : renderModel.entities) {
                Mesh* mesh = registry.getComponent<Mesh>(entityId);
                Transform* transform = registry.getComponent<Transform>(entityId);
                renderModel.tryUploadBatchMesh(i, *mesh, totalVertexCount, totalIndexCount);
                shaderProgram.setUniformArrayElement(i, transform->modelMatrix);
                handleEntity(registry, entityId, i);
                i++;
//                ENGINE_INFO("instanceID: {0}, renderModelId: {1}", mesh->getId(), mesh->renderModelId);
            }

            renderModel.vao.bind();
            drawVI(drawType, totalIndexCount);
        }

        ShaderProgram::stop();
    }

    template<typename Vertex>
    void BatchRenderer<Vertex>::render(ecs::Registry &registry) {
        renderV(registry);
        renderVI(registry);
    }

    template<typename Vertex>
    void InstanceRenderer<Vertex>::renderV(ecs::Registry& registry) {
        typedef VertexDataComponent<InstanceVertex<Vertex>> Geometry;
        typedef Transform3dComponent Transform;

        if (!shaderProgram.isReady()
        || registry.empty_entity()
        || registry.empty_components<Geometry>()
        || vRenderModels.empty()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        // instancing Geometry and drawing foreach RenderModel
        for (VRenderModel& renderModel : vRenderModels) {
            // for draw call
            u32 totalVertexCount = 0;
            // for indexing geometry as instances
            u32 i = 0;
            Geometry* geometry = registry.getComponent<Geometry>(renderModel.geometry);
            renderModel.tryUpload(*geometry, totalVertexCount);
            for (ecs::entity_id entityId : renderModel.entities) {
//                ENGINE_INFO("instanceID: {0}, renderModelId: {1}", i, geometry->renderModelId);
                Transform* transform = registry.getComponent<Transform>(entityId);
                shaderProgram.setUniformArrayElement(i, transform->modelMatrix);
                handleEntity(registry, entityId, i);
                if (++i == shaderProgram.getInstancesPerDraw()) {
                    renderModel.vao.bind();
                    drawV(drawType, totalVertexCount, i);
                    i = 0;
                }
            }

            if (i > 0) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount, i);
            }
        }

        ShaderProgram::stop();
    }

    template<typename Vertex>
    void InstanceRenderer<Vertex>::renderVI(ecs::Registry& registry) {
        typedef BaseMeshComponent<InstanceVertex<Vertex>> Mesh;
        typedef Transform3dComponent Transform;

        if (!shaderProgram.isReady()
        || registry.empty_entity()
        || registry.empty_components<Mesh>()
        || viRenderModels.empty()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        // instancing Mesh and drawing foreach RenderModel
        for (VIRenderModel& renderModel : viRenderModels) {
            // for draw call
            u32 totalVertexCount = 0;
            u32 totalIndexCount = 0;
            // for indexing meshes as instances
            u32 i = 0;
            Mesh* mesh = registry.getComponent<Mesh>(renderModel.mesh);
            renderModel.tryUpload(*mesh, totalVertexCount, totalIndexCount);
            for (ecs::entity_id entityId : renderModel.entities) {
//                ENGINE_INFO("instanceID: {0}, renderModelId: {1}", i, mesh->renderModelId);
                Transform* transform = registry.getComponent<Transform>(entityId);
                shaderProgram.setUniformArrayElement(i, transform->modelMatrix);
                handleEntity(registry, entityId, i);
                if (++i == shaderProgram.getInstancesPerDraw()) {
                    renderModel.vao.bind();
                    drawVI(drawType, totalIndexCount, i);
                    i = 0;
                }
            }

            if (i > 0) {
                renderModel.vao.bind();
                drawVI(drawType, totalIndexCount, i);
            }
        }

        ShaderProgram::stop();
    }

    template<typename Vertex>
    void InstanceRenderer<Vertex>::render(ecs::Registry &registry) {
        renderV(registry);
        renderVI(registry);
    }

    template<typename Vertex>
    void VRenderer<Vertex>::render(const ecs::Entity &entity) {
        VertexDataComponent<Vertex>* vertexDataComponent = entity.get<VertexDataComponent<Vertex>>();
        if (!shaderProgram.isReady() || !vertexDataComponent) return;

        shaderProgram.start();

        uploadUniforms(entity);
        upload(*vertexDataComponent);
        uploadTransform<Transform3dComponent>(entity);

        vRenderModel.vao.bind();
        drawV(vertexDataComponent->drawType, vertexDataComponent->vertexData.size);
        shaderProgram.stop();
    }

    template<typename Vertex>
    void VRenderer<Vertex>::renderStatic(const ecs::Entity &entity) {
        VertexDataComponent<Vertex>* vertexDataComponent = entity.get<VertexDataComponent<Vertex>>();
        if (!shaderProgram.isReady() || !vertexDataComponent) return;

        shaderProgram.start();

        ENGINE_INFO("VRenderer::renderStatic()");
        uploadUniforms(entity);
        uploadTransform<Transform3dComponent>(entity);

        vRenderModel.vao.bind();
        drawV(vertexDataComponent->drawType, vertexDataComponent->vertexData.size);
        shaderProgram.stop();
    }

    template<typename Vertex>
    void VRenderer<Vertex>::render(VertexDataComponent<Vertex>& vertexDataComponent) {
        if (!shaderProgram.isReady()) return;

        shaderProgram.start();

        upload(vertexDataComponent);

        vRenderModel.vao.bind();
        drawV(vertexDataComponent.drawType, vertexDataComponent.vertexData.size);
        shaderProgram.stop();
    }

    template<typename Vertex>
    void VRenderer<Vertex>::render(VertexDataComponent<Vertex>& vertexDataComponent, const uint32_t& textureId) {
        if (!shaderProgram.isReady()) return;

        shaderProgram.start();

        upload(vertexDataComponent);
        TextureBuffer::activate(0);
        TextureBuffer::bind(textureId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D));

        vRenderModel.vao.bind();
        drawV(vertexDataComponent.drawType, vertexDataComponent.vertexData.size);
        shaderProgram.stop();
    }

    template<typename Vertex>
    void VRenderer<Vertex>::validate(const VertexDataComponent<Vertex>& vertexDataComponent) {
        if (!vRenderModel.hasCapacity(vertexDataComponent)) {
            vRenderModel = { 0, vertexDataComponent.vertexData.size };
            vRenderModel.vao.bind();
            vRenderModel.vbo.setFormat(shaderProgram.getVertexFormat());
            VertexArray::unbind();
        }
    }

    template<typename Vertex>
    void VRenderer<Vertex>::upload(VertexDataComponent<Vertex> &vertexDataComponent) {
        validate(vertexDataComponent);
        if (vertexDataComponent.isUpdated) {
            vertexDataComponent.isUpdated = false;
            vRenderModel.upload(vertexDataComponent);
        }
    }

    template<typename Vertex>
    void VRenderer<Vertex>::uploadStatic(const VertexDataComponent<Vertex>& vertexDataComponent) {
        validate(vertexDataComponent);
        vRenderModel.uploadStatic(vertexDataComponent);
    }

    template<typename Vertex>
    template<typename Transform>
    void VRenderer<Vertex>::uploadTransform(const ecs::Entity &entity) {
        auto transform = entity.get<Transform>();
        if (transform) {
            shaderProgram.setUniform(transform->modelMatrix);
        }
    }

    template<typename Vertex>
    void VIRenderer<Vertex>::render(const ecs::Entity &entity) {
        BaseMeshComponent<Vertex>* mesh = entity.get<BaseMeshComponent<Vertex>>();
        if (!shaderProgram.isReady() || !mesh) return;

        begin(entity);

        upload(*mesh);
        uploadTransform<Transform3dComponent>(entity);

        end(mesh->drawType, mesh->totalIndexCount);
    }

    template<typename Vertex>
    void VIRenderer<Vertex>::upload(BaseMeshComponent<Vertex> &meshComponent) {
        if (!viRenderModel.hasCapacity(meshComponent)) {
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

    template<typename Vertex>
    template<typename Transform>
    void VIRenderer<Vertex>::uploadTransform(const ecs::Entity &entity) {
        auto transform = entity.get<Transform>();
        if (transform) {
            shaderProgram.setUniform(transform->modelMatrix);
        }
    }

//    template<typename T>
//    bool Renderer::validate(VertexDataComponent<T>& vertexDataComponent) {
//        if (vRenderModels.empty() || !vRenderModels[vertexDataComponent.renderModelId].hasCapacity(vertexDataComponent)) {
//            auto vertexCount = vertexDataComponent.vertexData.size > DEFAULT_VERTEX_COUNT
//                               ? vertexDataComponent.vertexData.size * 3 : DEFAULT_VERTEX_COUNT;
//            auto& newRenderModel = createRenderModel(vertexCount);
//            vertexDataComponent.renderModelId = newRenderModel.id;
//            newRenderModel.increaseCounts(vertexDataComponent);
//            return false;
//        }
//
//        vRenderModels[vertexDataComponent.renderModelId].increaseCounts(vertexDataComponent);
//        return true;
//    }

//    template<typename T>
//    bool Renderer::validate(BaseMeshComponent<T>& meshComponent, u32& meshIndex) {
//        bool shaderHasCapacity = meshIndex < shaderProgram.getInstancesPerDraw();
//        meshIndex = shaderHasCapacity ? meshIndex + 1 : 0; // increment or reset meshIndex
//
//        if (viRenderModels.empty() || !shaderHasCapacity) {
//            auto vertexCount = meshComponent.totalVertexCount > DEFAULT_VERTEX_COUNT
//                    ? meshComponent.totalVertexCount * 3 : DEFAULT_VERTEX_COUNT;
//
//            auto indexCount = meshComponent.totalIndexCount > DEFAULT_INDEX_COUNT
//                    ? meshComponent.totalIndexCount * 3 : DEFAULT_INDEX_COUNT;
//
//            auto& newRenderModel = createRenderModel(vertexCount, indexCount);
//            meshComponent.renderModelId = newRenderModel.id;
//            newRenderModel.increaseCounts(meshComponent);
//            return false;
//        }
//
//        viRenderModels[meshComponent.renderModelId].increaseCounts(meshComponent);
//        return true;
//    }

    template<typename Vertex>
    void VRenderer<Vertex>::release() {
        shaderProgram.release();
    }

    template<typename Vertex>
    void VRenderer<Vertex>::uploadUniforms(const ecs::Entity& entity) {
        // do nothing
        ENGINE_INFO("VRenderer::uploadUniforms()");
    }

    template<typename Vertex>
    VRenderer<Vertex>::VRenderer(const BaseShaderProgram &shaderProgram) {
        this->shaderProgram = shaderProgram;
        this->shaderProgram.bindVertexFormat();
    }

    template<typename Vertex>
    void VIRenderer<Vertex>::init() {
        shaderProgram.bindVertexFormat();
    }

    template<typename Vertex>
    void VIRenderer<Vertex>::release() {
        shaderProgram.release();
    }

    template<typename Vertex>
    void VIRenderer<Vertex>::begin(const ecs::Entity &entity) {
        shaderProgram.start();
        shaderProgram.update(entity);
    }

    template<typename Vertex>
    void VIRenderer<Vertex>::end(const DrawType& drawType, const u32& indexCount) const {
        viRenderModel.vao.bind();
        drawVI(drawType, indexCount);
        ShaderProgram::stop();
    }

    template<typename T>
    void Renderer::createRenderModel(VertexDataComponent<T> &vertexDataComponent) {
        VRenderModel& newRenderModel = createRenderModel(vertexDataComponent.vertexData.size);
        vertexDataComponent.renderModelId = newRenderModel.id;
    }

    template<typename T>
    void Renderer::createRenderModel(BaseMeshComponent<T> &baseMeshComponent) {
        VIRenderModel& newRenderModel = createRenderModel(baseMeshComponent.totalVertexCount, baseMeshComponent.totalIndexCount);
        baseMeshComponent.renderModelId = newRenderModel.id;
    }

    template<typename T>
    VRenderModel& Renderer::createRenderModel(const vector<VertexDataComponent<T>> &vertexDataComponents) {
        u32 vertexCount = 0;
        for (const auto& vertexDataComponent : vertexDataComponents) {
            vertexCount += vertexDataComponent.vertexData.size;
        }
        return createRenderModel(vertexCount);
    }

    template<typename T>
    VIRenderModel& Renderer::createRenderModel(const vector<BaseMeshComponent<T>> &baseMeshComponents) {
        u32 vertexCount = 0;
        u32 indexCount = 0;
        for (const auto& meshComponent : baseMeshComponents) {
            vertexCount += meshComponent.totalVertexCount;
            indexCount += meshComponent.totalIndexCount;
        }
        return createRenderModel(vertexCount, indexCount);
    }

    template<typename T>
    void Renderer::createRenderModels(const vector<VertexDataComponent<T>> &vertexDataComponents) {
        for (auto& vertexDataComponent : vertexDataComponents) {
            createRenderModel(vertexDataComponent);
        }
    }

    template<typename T>
    void Renderer::createRenderModels(const vector<BaseMeshComponent<T>> &baseMeshComponents) {
        for (auto& meshComponent : baseMeshComponents) {
            createRenderModel(meshComponent);
        }
    }

    template<typename T>
    void Renderer::createVRenderModel(vector<Object<T>> &objects) {
        u32 instancesPerDraw = shaderProgram.getInstancesPerDraw();
        u32 k = objects.size() / instancesPerDraw;

        for (u32 i = 0; i < k; i++) {
            vector<VertexDataComponent<T>> geometries;
            u32 vertexCount = 0;
            for (u32 j = 0; j < instancesPerDraw; j++) {
                u32 objectIndex = i * instancesPerDraw + j;
                VertexDataComponent<T>* geometry = objects.at(objectIndex).get<VertexDataComponent<T>>();
                geometries.emplace_back(*geometry);
                vertexCount += geometry->vertexData.size();
            }

            VRenderModel& renderModel = createRenderModel(vertexCount);
            for (u32 j = 0; j < instancesPerDraw; j++) {
                u32 objectIndex = i * instancesPerDraw + j;
                const Object<T>& object = objects.at(objectIndex);
                renderModel.entities.emplace_back(object.getId());
                geometries[j].renderModelId = renderModel.id;
            }
        }
    }

    template<typename T>
    void Renderer::createVIRenderModel(vector<Object<T>> &objects) {
        u32 instancesPerDraw = shaderProgram.getInstancesPerDraw();
        u32 k = objects.size() / instancesPerDraw;

        for (u32 i = 0; i < k; i++) {
            vector<BaseMeshComponent<T>> meshes;
            u32 vertexCount = 0;
            u32 indexCount = 0;
            for (u32 j = 0; j < instancesPerDraw; j++) {
                u32 objectIndex = i * instancesPerDraw + j;
                BaseMeshComponent<T>* mesh = objects.at(objectIndex).get<BaseMeshComponent<T>>();
                meshes.emplace_back(*mesh);
                vertexCount += mesh->totalVertexCount;
                indexCount += mesh->totalIndexCount;
            }

            VIRenderModel& renderModel = createRenderModel(vertexCount, indexCount);
            for (u32 j = 0; j < instancesPerDraw; j++) {
                u32 objectIndex = i * instancesPerDraw + j;
                const Object<T>& object = objects.at(objectIndex);
                renderModel.entities.emplace_back(object.getId());
                meshes[j].renderModelId = renderModel.id;
            }
        }
    }

    template<typename T>
    void Renderer::createVRenderModelInstanced(Object<T> &object, const vector<Object<T>> &objects) {
        VertexDataComponent<T>* geometry = object.get<VertexDataComponent<T>>();
        if (geometry) {
            VRenderModel& renderModel = createRenderModel(geometry->vertexData.size);
            renderModel.geometry = object.getId();
            geometry->renderModelId = renderModel.id;
            for (const Object<T>& obj : objects) {
                renderModel.entities.emplace_back(obj.getId());
            }
        } else {
            ENGINE_ERR("Renderer::createVRenderModel failed: VertexDataComponent does not exists for entity={0}",
                       object.getId());
        }
    }

    template<typename T>
    void Renderer::createVIRenderModelInstanced(Object<T> &object, const vector<Object<T>> &objects) {
        BaseMeshComponent<T>* mesh = object.get<BaseMeshComponent<T>>();
        if (mesh) {
            VIRenderModel& renderModel = createRenderModel(mesh->totalVertexCount, mesh->totalIndexCount);
            renderModel.mesh = object.getId();
            mesh->renderModelId = renderModel.id;
            for (const Object<T>& obj : objects) {
                renderModel.entities.emplace_back(obj.getId());
            }
        } else {
            ENGINE_ERR("Renderer::createVIRenderModel failed: BaseMeshComponent does not exists for entity={0}",
                       object.getId());
        }
    }
}
