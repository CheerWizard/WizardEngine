//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Renderer.h"

#include "../../core/TreeCache.h"
#include "../../core/CameraController.h"
#include "../shader/ShaderCache.h"
#include "../GraphicsFactory.h"

#include "../../ecs/System.h"

#include "../geometry/MeshComponent.h"
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
        void onUpdate() override;

        void addShader(const std::string& name, const Ref<Shader>& shader);
        void addShader(const Ref<Shader>& shader);
        ShaderError loadShader(const ShaderProps& shaderProps, VertexFormat* vertexFormat);
        ShaderError loadShader(const ShaderProps& shaderProps);
        Ref<Shader> getShader(const std::string& name);
        bool shaderExists(const std::string& name) const;

        void updateFboSpecification(const FramebufferSpecification &framebufferSpecification);

        void enableDepth();
        void setPolygonMode(const PolygonMode &polygonMode);

        void loadTexture(const std::string &fileName);

    public:
        void onWindowClosed() override;
        void onWindowResized(unsigned int width, unsigned int height) override;

    protected:
        virtual void renderMaterial(const entt::entity &entity) = 0;
        virtual void renderTransform(const entt::entity &entity) = 0;

    protected:
        void destroy();

    public:
        inline void setSceneCameraController(const Ref<CameraController> &cameraController) {
            sceneRenderer->setCameraController(cameraController);
        }

    private:
        void create();

    protected:
        Ref<GraphicsFactory> graphicsFactory;
        Ref<Renderer> sceneRenderer;

        ShaderCache shaderCache;
        VertexBufferCache vertexBufferCache;

    private:
        ShaderError handleShaderError(const Ref<Shader> &shader);

    };

    class RenderSystem2d : public RenderSystem {

    public:
        RenderSystem2d(const Ref<GraphicsFactory> &graphicsFactory) : RenderSystem(graphicsFactory) {}
        ~RenderSystem2d() override = default;

    protected:
        void renderMaterial(const entt::entity &entity) override;
        void renderTransform(const entt::entity &entity) override;

    };

    class RenderSystem3d : public RenderSystem {

    public:
        RenderSystem3d(const Ref<GraphicsFactory> &graphicsFactory) : RenderSystem(graphicsFactory) {}
        ~RenderSystem3d() override = default;

    protected:
        void renderMaterial(const entt::entity &entity) override;
        void renderTransform(const entt::entity &entity) override;

    };

}