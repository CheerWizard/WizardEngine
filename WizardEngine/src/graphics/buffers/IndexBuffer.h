//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Buffer.h"
#include "IndexData.h"

#include "vector"

#define DEFAULT_INDEX_COUNT 1200000

namespace engine {

    class IndexBuffer : public Buffer {

    public:
        IndexBuffer(const uint32_t &indexCount = DEFAULT_INDEX_COUNT) {
            capacity = indexCount;
        }

    public:
        virtual void allocate(const uint32_t &indexCount = DEFAULT_INDEX_COUNT) = 0;
        virtual void load(const IndexData& indexData) = 0;

    };

}