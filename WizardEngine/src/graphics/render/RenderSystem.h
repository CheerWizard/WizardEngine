//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Drawer.h"

#include "../../core/TreeCache.h"

#include "../buffers/VertexArray.h"
#include "../buffers/FrameBuffer.h"
#include "../buffers/UniformBuffer.h"

#include "../shader/ShaderCache.h"

#include "../../core/CameraController.h"

#include "../../ecs/System.h"

#include "../geometry/ShapeComponent.h"
#include "../../math/TransformComponent.h"
#include "material/MaterialComponents.h"

#include "string"

namespace engine {

    class RenderSystem : public System, public WindowCallback {

    public:
        RenderSystem(ShaderCache* shaderCache,
                     const Ref<VertexArray> &vertexArray,
                     const Ref<Drawer> &drawer,
                     const Ref<FrameBuffer> &frameBuffer,
                     const Ref<UniformBuffer> &uniformBuffer
                     ) :
        shaderCache(shaderCache),
        vertexArray(vertexArray),
        drawer(drawer),
        frameBuffer(frameBuffer),
        uniformBuffer(uniformBuffer) {}

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

    protected:
        ShaderCache* shaderCache;
        Ref<VertexArray> vertexArray;
        Ref<Drawer> drawer;
        Ref<FrameBuffer> frameBuffer;
        Ref<UniformBuffer> uniformBuffer;
        CameraController* cameraController = nullptr; // weak reference.

    private:
        void renderShape(const std::string &shaderName,
                         ShapeComponent &shapeComponent,
                         const uint32_t &vertexStart,
                         const uint32_t &indexStart);
        void renderCamera(const Ref<Shader>& shader);
        void onShaderLoaded(const ShaderError& shaderError, const std::string &shaderName);

    };

    class RenderSystem2d : public RenderSystem {

    public:
        RenderSystem2d(ShaderCache* shaderCache,
                     const Ref<VertexArray> &vertexArray,
                     const Ref<Drawer> &drawer,
                     const Ref<FrameBuffer> &frameBuffer,
                     const Ref<UniformBuffer> &uniformBuffer
                     ) :
                     RenderSystem(shaderCache, vertexArray, drawer, frameBuffer, uniformBuffer) {}

        ~RenderSystem2d() override = default;

    protected:
        void renderMaterial(Ref<Shader> &shader, const entt::entity &entity) override;
        void renderTransform(Ref<Shader> &shader, const entt::entity &entity) override;

    };

    class RenderSystem3d : public RenderSystem {

    public:
        RenderSystem3d(ShaderCache* shaderCache,
                       const Ref<VertexArray> &vertexArray,
                       const Ref<Drawer> &drawer,
                       const Ref<FrameBuffer> &frameBuffer,
                       const Ref<UniformBuffer> &uniformBuffer
                       ) :
                       RenderSystem(shaderCache, vertexArray, drawer, frameBuffer, uniformBuffer) {}

        ~RenderSystem3d() override = default;

    protected:
        void renderMaterial(Ref<Shader> &shader, const entt::entity &entity) override;
        void renderTransform(Ref<Shader> &shader, const entt::entity &entity) override;

    };

}