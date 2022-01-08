//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "../../../graphics/core/buffer_data/IndexData.h"

#define DEFAULT_INDEX_COUNT 2000000

namespace engine {

    // Single IBO - Index Buffer Object.
    // Can be used as graphics API interface
    class IndexBuffer final {

    public:
        IndexBuffer() {
            create();
        }
        ~IndexBuffer() = default;

    public:
        // lifetime functions
        void create();
        void destroy();
        void recreate();
        // bind/unbind
        void bind();
        void unbind();
        // GPU allocations
        void alloc(); // allocation with default index count
        void alloc(const uint32_t &indexCount);
        void malloc(const size_t &memorySize);
        // GPU data load
        void load(const IndexData &indexData);

    private:
        uint32_t id = 0;

    };

}