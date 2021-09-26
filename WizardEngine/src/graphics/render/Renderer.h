//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../core/TreeCache.h"

#include "../buffers/VertexArray.h"

#include "../shader/ShaderCache.h"

#include "../../core/CameraController.h"

#include "../../ecs/Scene.h"

#include "../geometry/ShapeComponent.h"
#include "../../math/TransformComponent.h"
#include "../material/MaterialComponents.h"

#include "string"

namespace engine {

    class Renderer {

    public:
        Renderer(ShaderCache* shaderCache,
                 const Ref<VertexArray> &vertexArray) :
        shaderCache(shaderCache),
        vertexArray(vertexArray) {}

        virtual ~Renderer() = default;

    public:
        void onPrepare();
        void onUpdate();

        void addShader(const std::string& name, const Ref<Shader>& shader);
        void addShader(const Ref<Shader>& shader);
        Ref<Shader> loadShader(const ShaderProps& shaderProps, VertexFormat* vertexFormat);
        Ref<Shader> getShader(const std::string& name);
        bool shaderExists(const std::string& name) const;

        void loadTexture(const std::string &filePath);
        void loadTextureData(const void* data);

    protected:
        virtual void drawIndices(const uint32_t &indexCount) = 0;

    protected:
        void destroy();

    public:
        void setCameraController(CameraController* cameraController) {
            this->cameraController = cameraController;
        }

        void setActiveScene(Scene* activeScene) {
            this->activeScene = activeScene;
        }

    protected:
        Ref<VertexArray> vertexArray;
        ShaderCache* shaderCache;
        CameraController* cameraController = nullptr; // weak reference.
        Scene* activeScene = nullptr; // weak reference.

    private:
        void updateShapeComponent(const std::string &shaderName, const ShapeComponent &shapeComponent);
        void updateMaterial();

    };

}