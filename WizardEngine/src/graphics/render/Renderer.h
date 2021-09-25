//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../core/TreeCache.h"

#include "../GraphicsObject.h"

#include "../buffers/VertexArray.h"

#include "../shader/ShaderCache.h"

#include "string"

namespace engine {

    typedef TreeCache<std::string, GraphicsObject> GraphicsObjectCache;

    class Renderer {

    public:
        Renderer(ShaderCache* shaderCache,
                 GraphicsObjectCache* graphicsObjectCache,
                 const Ref<VertexArray> &vertexArray) :
        shaderCache(shaderCache),
        graphicsObjectCache(graphicsObjectCache),
        vertexArray(vertexArray) {}

        virtual ~Renderer() = default;

    public:
        void onPrepare();
        void onUpdate();

        uint32_t addObject(const Ref<GraphicsObject>& graphicsObject);
        void updateObject(const Ref<GraphicsObject>& graphicsObject);
        void loadObject(const Ref<GraphicsObject>& graphicsObject);
        const Ref<GraphicsObject>& getGraphicsObject(const std::string &shaderName, const uint32_t &objectIndex);

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

    protected:
        Ref<VertexArray> vertexArray;
        ShaderCache* shaderCache;
        GraphicsObjectCache* graphicsObjectCache;

    };

}