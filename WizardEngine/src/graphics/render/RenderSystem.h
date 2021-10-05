//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../core/TreeCache.h"
#include "../../core/CameraController.h"

#include "../shader/ShaderCache.h"

#include "../GraphicsFactory.h"

#include "../../ecs/System.h"

#include "../geometry/ShapeComponent.h"
#include "../../math/TransformComponent.h"
#include "material/MaterialComponents.h"

#include "string"

namespace engine {

    class RenderSystem : public System, public WindowCallback {

        typedef TreeCache<std::string, VertexBuffer> VertexBufferCache;

    public:
        RenderSystem(const Ref<GraphicsFactory> &graphicsFactory) : graphicsFactory(graphicsFactory) {
            create();
        }

        virtual ~RenderSystem() = default;

    public:
        void onPrepare() override;
        void onUpdate() override;

        void addShader(const std::string& name, const Ref<Shader>& shader);
        void addShader(const Ref<Shader>& shader);
        ShaderError loadShader(const ShaderProps& shaderProps, VertexFormat* vertexFormat);
        ShaderError loadShader(const ShaderProps& shaderProps);
        Ref<Shader> getShader(const std::string& name);
        bool shaderExists(const std::string& name) const;

        void loadTexture(const std::string &filePath);
        void loadTextureData(const void* data);

        void updateFboSpecification(const FramebufferSpecification &framebufferSpecification);

        void enableDepth();

        void setPolygonMode(const PolygonMode &polygonMode);

        void renderShape(const std::string &shaderName,
                         ShapeComponent &shapeComponent,
                         const uint32_t &vertexStart,
                         const uint32_t &indexStart);

        void renderScene(const std::string &shaderName, const ShapeComponent &shapeComponent);

    public:
        void onWindowClosed() override;
        void onWindowResized(unsigned int width, unsigned int height) override;

    protected:
        virtual void renderMaterial(Ref<Shader> &shader, const entt::entity &entity) = 0;
        virtual void renderTransform(Ref<Shader> &shader, const entt::entity &entity) = 0;

    protected:
        void destroy();

    public:
        inline void setCameraController(CameraController* cameraController) {
            this->cameraController = cameraController;
        }

    private:
        void create();
        void prepareVertexBuffer(const std::string &shaderName, VertexFormat* vertexFormat);

    protected:
        Ref<GraphicsFactory> graphicsFactory;

        Ref<Drawer> drawer;
        Ref<VertexArray> vertexArray;
        VertexBufferCache vertexBufferCache;
        Ref<IndexBuffer> indexBuffer;
        Ref<TextureBuffer> textureBuffer;
        Ref<FrameBuffer> frameBuffer;
        Ref<UniformBuffer> uniformBuffer;

        ShaderCache shaderCache;

        CameraController* cameraController = nullptr; // weak reference.

    private:
        void renderCamera(const Ref<Shader>& shader);
        ShaderError handleShaderError(const Ref<Shader> &shader);

    };

    class RenderSystem2d : public RenderSystem {

    public:
        RenderSystem2d(const Ref<GraphicsFactory> &graphicsFactory) : RenderSystem(graphicsFactory) {}
        ~RenderSystem2d() override = default;

    protected:
        void renderMaterial(Ref<Shader> &shader, const entt::entity &entity) override;
        void renderTransform(Ref<Shader> &shader, const entt::entity &entity) override;

    };

    class RenderSystem3d : public RenderSystem {

    public:
        RenderSystem3d(const Ref<GraphicsFactory> &graphicsFactory) : RenderSystem(graphicsFactory) {}
        ~RenderSystem3d() override = default;

    protected:
        void renderMaterial(Ref<Shader> &shader, const entt::entity &entity) override;
        void renderTransform(Ref<Shader> &shader, const entt::entity &entity) override;

    };

}