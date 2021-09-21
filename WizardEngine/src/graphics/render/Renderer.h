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

        void loadVertices(const std::string &shaderName,
                          const uint32_t &vertexStart,
                          float *vertices);
        void loadIndices(const std::string &shaderName,
                         const uint32_t &indexStart,
                         uint32_t *indices);

    protected:
        virtual void drawIndices(uint32_t* indices, const uint32_t &indexCount) = 0;

    protected:
        void destroy();

    protected:
        Ref<VertexArray> vertexArray;
        ShaderCache* shaderCache;
        GraphicsObjectCache* graphicsObjectCache;

    };

}