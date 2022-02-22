//
// Created by mecha on 07.09.2021.
//

#pragma once

#include <graphics/core/shader/VertexFormat.h>
#include <graphics/core/buffer_data/VertexData.h>

#define DEFAULT_VERTEX_COUNT 400000

namespace engine {

    // VBO - Vertex Buffer Object allocated for GPU pipeline
    class VertexBuffer final {

    public:
        VertexBuffer() {
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
        void unbind() const;
        // GPU allocations
        void alloc(const uint32_t &vertexCount = DEFAULT_VERTEX_COUNT);
        void malloc(const size_t &memorySize);
        void setFormat(const shader::VertexFormat &vertexFormat, const uint32_t &vertexCount = DEFAULT_VERTEX_COUNT);
        // GPU data load
        void load(const VertexData &vertexData);

    public:
        void enableAttributes();
        void disableAttributes();

    private:
        void bindAttributes();

    private:
        uint32_t id = 0;
        shader::VertexFormat vertexFormat;
    };

}