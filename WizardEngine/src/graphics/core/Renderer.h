//
// Created by mecha on 06.10.2021.
//

#pragma once

#include "../GraphicsFactory.h"
#include "shader/ShaderController.h"
#include "../../ecs/Components.h"
#include "../../ecs/Family.h"
#include "geometry/MeshComponent.h"

#define INSTANCE_COUNT_LIMIT 128

namespace engine {

    class Renderer {

    public:
        Renderer(const Ref<Shader> &shader, const Ref<GraphicsFactory> &graphicsFactory) : shader(shader) {
            create(graphicsFactory);
        }

        Renderer(
                const Ref<Shader> &shader,
                const Ref<GraphicsFactory> &graphicsFactory,
                const std::vector<Ref<ShaderController>> &shaderControllers
        ) : shaderControllers(shaderControllers), shader(shader) {
            create(graphicsFactory);
        }

        ~Renderer() {
            clear();
        }

    public:
        void render(const Entity& entity);
        void renderInstanced(const Family &family);
        void renderBatched(const std::vector<Entity> &entities);

        void tryUpdatePolygonMode(const Entity &entity);

    public:
        void tryUploadMesh(
                const Entity& entity,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount
        );
        void tryUploadMesh(
                uint32_t &instanceID,
                const Entity& entity,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount
        );
        void uploadMesh(MeshComponent &meshComponent);

        void add(const Ref<ShaderController> &shaderController);
        void clear();

    private:
        void create(const Ref<GraphicsFactory> &graphicsFactory);

        void drawElements(const uint32_t &indexCount);
        void drawElements(const uint32_t &indexCount, const uint32_t &instanceCount);
        void updateShaderControllers(const Entity &entity);
        void begin();
        void end();

    private:
        Ref<Drawer> drawer;
        Ref<RenderSettings> settings;
        // buffers
        Ref<VertexArray> vertexArray;
        Ref<VertexBuffer> vertexBuffer;
        Ref<IndexBuffer> indexBuffer;
        // shader controllers
        Ref<Shader> shader;
        std::vector<Ref<ShaderController>> shaderControllers;
    };

}