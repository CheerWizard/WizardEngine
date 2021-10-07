//
// Created by mecha on 06.10.2021.
//

#pragma once

#include "../GraphicsFactory.h"
#include "../shader/ShaderCache.h"

#include "../../core/CameraController.h"

#include "../geometry/MeshComponent.h"
#include "material/MaterialComponents.h"
#include "../../math/TransformComponent.h"

namespace engine {

    class Renderer {

    public:
        Renderer(const Ref<GraphicsFactory> &graphicsFactory) : _graphicsFactory(graphicsFactory) {
            create();
        }

        ~Renderer() {
            destroy();
        }

    public:
        void prepare(const Ref<Shader>& shader);

        void loadMesh(MeshComponent &meshComponent);
        void loadTexture(const std::string &filePath);
        void loadTextureData(const void* data);

        const std::vector<uint32_t>& updateFboSpecs(const FramebufferSpecification &framebufferSpecification);
        void resizeFrame(const uint32_t &width, const uint32_t &height);

        void start();
        void stop();
        void startFrame();
        void endFrame();
        void renderMesh(MeshComponent &meshComponent);
        void renderMaterial(TextureComponent &material);
        void renderTransform(TransformComponent2d &transformComponent2D);
        void renderTransform(TransformComponent3d &transformComponent3D);
        void renderCamera();
        void drawByIndices(const uint32_t &indexCount);

        void enableDepth();
        void setPolygonMode(const PolygonMode &polygonMode);

    public:
        inline void setCameraController(const Ref<CameraController> &cameraController) {
            _cameraController = cameraController;
        }

    private:
        void create();
        void destroy();

    private:
        Ref<GraphicsFactory> _graphicsFactory;

        Ref<Drawer> _drawer;
        Ref<VertexArray> _vertexArray;
        Ref<VertexBuffer> _vertexBuffer;
        Ref<IndexBuffer> _indexBuffer;
        Ref<FrameBuffer> _frameBuffer;
        Ref<TextureBuffer> _textureBuffer;
        Ref<UniformBuffer> _uniformBuffer;
        Ref<Shader> _shader;
        Ref<CameraController> _cameraController;

    };

}