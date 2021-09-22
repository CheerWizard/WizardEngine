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
        VertexArray(VertexBufferCache* vertexBufferCache, const Ref<IndexBuffer>& indexBuffer) :
        vertexBufferCache(vertexBufferCache),
        indexBuffer(indexBuffer) {}

        ~VertexArray() = default;

    public:
        void loadVertices(const std::string &shaderName,
                          const uint32_t &vertexStart,
                          float *vertices);
        void loadIndices(const std::string &shaderName,
                         const uint32_t &indexStart,
                         uint32_t *indices);

    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        virtual void destroy();

    public:
        inline void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
            this->indexBuffer = indexBuffer;
        }

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