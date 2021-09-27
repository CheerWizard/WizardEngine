//
// Created by mecha on 12.09.2021.
//

#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "TextureBuffer.h"

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
            createTextureBuffer();
        }

        VertexArray(VertexBufferCache* vertexBufferCache, const Ref<IndexBuffer>& indexBuffer) :
        vertexBufferCache(vertexBufferCache),
        indexBuffer(indexBuffer) {
            id = 0;
            createTextureBuffer();
        }

        VertexArray(VertexBufferCache* vertexBufferCache, const Ref<IndexBuffer>& indexBuffer, const Ref<TextureBuffer> &textureBuffer) :
        vertexBufferCache(vertexBufferCache),
        indexBuffer(indexBuffer),
        textureBuffer(textureBuffer) {
            id = 0;
        }

        ~VertexArray() = default;

    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

        void createVertexBuffer(VertexFormat* vertexFormat, const std::string &shaderName);
        void createIndexBuffer(const uint32_t &indexCount);
        void createIndexBuffer();
        void createTextureBuffer();

        void loadVertexBuffer(const std::string &shaderName, const VertexData &vertexData);
        void loadIndexBuffer(const IndexData &indexData);
        void loadTextureBuffer(const std::string &filePath);
        void loadTextureBufferData(const void* data);

        void bindLastVertexBuffer(const std::string &shaderName);
        void bindIndexBuffer();
        void bindTextureBuffer();

        void activateTextureBuffer(const int &slot);

    protected:
        virtual void destroy();

    public:
        inline const Ref<IndexBuffer>& getIndexBuffer() const {
            return indexBuffer;
        }

        inline const std::vector<Ref<VertexBuffer>>& getVertexBuffers(const std::string &shaderName) const {
            return vertexBufferCache->get(shaderName);
        }

        inline const Ref<TextureBuffer>& getTextureBuffer() const {
            return textureBuffer;
        }

    protected:
        uint32_t id;
        VertexBufferCache* vertexBufferCache;
        Ref<IndexBuffer> indexBuffer;
        Ref<TextureBuffer> textureBuffer;

    };

}