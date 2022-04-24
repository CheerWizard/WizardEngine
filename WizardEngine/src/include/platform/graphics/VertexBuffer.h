//
// Created by mecha on 07.09.2021.
//

#pragma once

#include <graphics/core/shader/VertexFormat.h>
#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/buffer_data/graphics_buffer.h>

#define DEFAULT_VERTEX_COUNT 1000

namespace engine::graphics {

    // VBO - Vertex buffer Object allocated for GPU pipeline
    class VertexBuffer final : public Buffer {

    public:
        VertexBuffer() : Buffer() {}
        VertexBuffer(const uint32_t& vertexCount) : Buffer(vertexCount) {}
        ~VertexBuffer() = default;

    public:
        // lifetime functions
        void create();
        void destroy();
        void recreate();
        // bind/unbind
        void bind() const;
        static void unbind();
        // GPU allocations
        void allocDynamic();
        void allocDynamic(const uint32_t &vertexCount);
        static void mallocDynamic(const size_t &memorySize);
        void setFormat(const shader::VertexFormat &vertexFormat);
        void setFormat(const shader::VertexFormat &vertexFormat, const uint32_t& vertexCount);
        // GPU data load
        template<typename T>
        void load(const VertexData<T> &vertexData);
        static void load(const void* vertices, const size_t& subDataOffset, const size_t& memorySize);
        template<typename T>
        void loadStatic(const VertexData<T>& vertexData);
        static void loadStatic(const void* vertices, const size_t& memorySize);

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

    template<typename T>
    void VertexBuffer::loadStatic(const VertexData<T> &vertexData) {
        auto vertexSize = vertexFormat.getSize();
        auto memorySize = vertexData.vertexCount * vertexSize;
        loadStatic(vertexData.vertices, memorySize);
    }
}