//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Drawer.h"

#include "../../core/TreeCache.h"

#include "../buffers/VertexArray.h"

#include "../shader/ShaderCache.h"

#include "../../core/CameraController.h"

#include "../../ecs/System.h"

#include "../geometry/ShapeComponent.h"
#include "../../math/TransformComponent.h"
#include "material/MaterialComponents.h"

#include "string"

namespace engine {

    class RenderSystem : public System {

    public:
        RenderSystem(ShaderCache* shaderCache,
                     const Ref<VertexArray> &vertexArray,
                     const Ref<Drawer> &drawer) :
        shaderCache(shaderCache),
        vertexArray(vertexArray),
        drawer(drawer) {}

        virtual ~RenderSystem() = default;

    public:
        void onPrepare() override;
        void onUpdate() override;

        void addShader(const std::string& name, const Ref<Shader>& shader);
        void addShader(const Ref<Shader>& shader);
        Ref<Shader> loadShader(const ShaderProps& shaderProps, VertexFormat* vertexFormat);
        Ref<Shader> getShader(const std::string& name);
        bool shaderExists(const std::string& name) const;

        void loadTexture(const std::string &filePath);
        void loadTextureData(const void* data);

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
        Ref<VertexArray> vertexArray;
        Ref<Drawer> drawer;
        ShaderCache* shaderCache;
        CameraController* cameraController = nullptr; // weak reference.

    private:
        void renderShape(const std::string &shaderName, ShapeComponent &shapeComponent);
        void renderCamera(const Ref<Shader>& shader);

    };

    class RenderSystem2d : public RenderSystem {

    public:
        RenderSystem2d(ShaderCache* shaderCache,
                     const Ref<VertexArray> &vertexArray,
                     const Ref<Drawer> &drawer) :
                     RenderSystem(shaderCache, vertexArray, drawer) {}

        ~RenderSystem2d() override = default;

    protected:
        void renderMaterial(Ref<Shader> &shader, const entt::entity &entity) override;
        void renderTransform(Ref<Shader> &shader, const entt::entity &entity) override;

    };

    class RenderSystem3d : public RenderSystem {

    public:
        RenderSystem3d(ShaderCache* shaderCache,
                       const Ref<VertexArray> &vertexArray,
                       const Ref<Drawer> &drawer) :
                       RenderSystem(shaderCache, vertexArray, drawer) {}

        ~RenderSystem3d() override = default;

    protected:
        void renderMaterial(Ref<Shader> &shader, const entt::entity &entity) override;
        void renderTransform(Ref<Shader> &shader, const entt::entity &entity) override;

    };

}