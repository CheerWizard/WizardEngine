//
// Created by mecha on 07.09.2021.
//

#pragma once

#include <graphics/core/buffer_data/IndexData.h>
#include <graphics/core/buffer_data/buffer.h>

#define DEFAULT_INDEX_COUNT 5000

namespace engine::graphics {

    // IBO - Index buffer Object, component of graphics pipeline
    class IndexBuffer final : public buffer {

    public:
        IndexBuffer() : buffer() {}
        IndexBuffer(const uint32_t& indexCount) : buffer(indexCount) {}
        ~IndexBuffer() = default;

    public:
        // lifetime functions
        void create();
        void destroy();
        void recreate();
        // bind/unbind
        void bind() const;
        static void unbind();
        // GPU allocations
        void alloc();
        void alloc(const uint32_t &indexCount);
        static void malloc(const size_t &memorySize);
        // GPU data load
        static void load(const IndexData &indexData);

    private:
        u32 id = 0;

    };

}