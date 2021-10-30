//
// Created by mecha on 06.10.2021.
//

#pragma once

#include "../GraphicsFactory.h"

#include "../geometry/MeshComponent.h"
#include "material/MaterialComponents.h"
#include "../../transform/TransformComponents.h"
#include "../../camera/CameraComponents.h"
#include "../../ecs/Components.h"

#include "../../ecs/Entity.h"

#include "../sources/TextureSource.h"

namespace engine {

    class Renderer {

    public:
        Renderer(
                const Ref<GraphicsFactory> &graphicsFactory,
                const Ref<Shader> &shader,
                const Ref<TextureSource> &textureSource
        ) : _shader(shader), _textureSource(textureSource) {
            create(graphicsFactory);
        }

        ~Renderer() {
            destroy();
        }

    public:
        void render(const Entity& entity);

        void renderMesh(
                const Entity& entity,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount
        );

        void renderMesh(
                const uint32_t &instanceID,
                const Entity& entity,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount
        );

        void renderMaterial(const Entity& entity);

        bool renderTransform(const uint32_t &instanceId, const Entity &entity);
        bool renderTransform(const Entity &entity);

        void renderCamera(const Entity &entity);

        void renderPolygonMode(const Entity &entity);

        void begin();
        void end();

        void drawElements(const uint32_t &indexCount);
        void drawElements(const uint32_t &indexCount, const uint32_t &instanceCount);

        void uploadMesh(MeshComponent &meshComponent);

    private:
        void create(const Ref<GraphicsFactory> &graphicsFactory);
        void destroy();

        void renderCamera(Mat4fUniform &camera);

    private:
        Ref<Shader> _shader;
        Ref<Drawer> _drawer;
        Ref<VertexArray> _vertexArray;
        Ref<VertexBuffer> _vertexBuffer;
        Ref<IndexBuffer> _indexBuffer;
        Ref<UniformBuffer> _uniformBuffer = nullptr;
        Ref<TextureSource> _textureSource;

    };

}