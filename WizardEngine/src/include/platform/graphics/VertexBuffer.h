//
// Created by mecha on 07.09.2021.
//

#pragma once

#include <graphics/core/shader/VertexFormat.h>
#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/buffer_data/Buffer.h>

#define DEFAULT_VERTEX_COUNT 1000

namespace engine::graphics {

    // VBO - Vertex Buffer Object allocated for GPU pipeline
    class VertexBuffer final : public Buffer {

    public:
        VertexBuffer() : Buffer(DEFAULT_VERTEX_COUNT) {
            create();
        }
        VertexBuffer(const uint32_t& vertexCount) : Buffer(vertexCount) {
            create();
        }
        ~VertexBuffer() = default;

    public:
        // lifetime functions
        void create();
        void destroy();
        void recreate();
        // bind/unbind
        void bind() const;
        static void unbind() ;
        // GPU allocations
        void alloc();
        void alloc(const uint32_t &vertexCount);
        static void malloc(const size_t &memorySize);
        void setFormat(const shader::VertexFormat &vertexFormat);
        void setFormat(const shader::VertexFormat &vertexFormat, const uint32_t& vertexCount);
        // GPU data load
        template<typename T>
        void load(const VertexData<T> &vertexData);
        static void load(const void* vertices, const size_t& subDataOffset, const size_t& memorySize);

    public:
        void enableAttributes();
        void disableAttributes();

    private:
        void bindAttributes();

    private:
        u32 id = 0;
        shader::VertexFormat vertexFormat;
    };

    template<typename T>
    void VertexBuffer::load(const VertexData<T> &vertexData) {
        auto vertexSize = vertexFormat.getSize();
        auto subDataOffset = vertexData.vertexStart * vertexSize;
        auto memorySize = vertexData.vertexCount * vertexSize;
        load(vertexData.vertices, subDataOffset, memorySize);
    }

}