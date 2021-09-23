//
// Created by mecha on 12.09.2021.
//

#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include "../../core/Memory.h"
#include "../../core/TreeCache.h"

#include "string"

namespace engine {

    typedef TreeCache<std::string, VertexBuffer> VertexBufferCache;

    class VertexArray {

    public:

        VertexArray(VertexBufferCache* vertexBufferCache) :
        vertexBufferCache(vertexBufferCache) {
            id = 0;
            createIndexBuffer();
        }

        VertexArray(VertexBufferCache* vertexBufferCache, const Ref<IndexBuffer>& indexBuffer) :
        vertexBufferCache(vertexBufferCache),
        indexBuffer(indexBuffer) {
            id = 0;
        }

        ~VertexArray() = default;

    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

        void createVertexBuffer(Vertex* vertex, const std::string &shaderName);
        void loadVertexBuffer(const std::string &shaderName,
                              const uint32_t &vertexStart,
                              float* vertices);

        void createIndexBuffer(const uint32_t &indexCount);
        void createIndexBuffer();
        void loadIndexBuffer(const uint32_t &indexStart, uint32_t* indices);

    protected:
        virtual void destroy();

    public:
        inline const Ref<IndexBuffer>& getIndexBuffer() const {
            return indexBuffer;
        }

        inline const std::vector<Ref<VertexBuffer>>& getVertexBuffers(const std::string &shaderName) const {
            return vertexBufferCache->get(shaderName);
        }

    protected:
        uint32_t id;
        VertexBufferCache* vertexBufferCache;
        Ref<IndexBuffer> indexBuffer;

    };

}