//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Buffer.h"
#include "IndexData.h"

#include "vector"

#define DEFAULT_INDEX_COUNT 500000

namespace engine {

    class IndexBuffer : public Buffer {

    public:
        explicit IndexBuffer(const uint32_t &indexCount = DEFAULT_INDEX_COUNT) {
            capacity = indexCount;
        }

    public:
        virtual void allocate() = 0;
        virtual void load(const IndexData& indexData) = 0;

    };

}