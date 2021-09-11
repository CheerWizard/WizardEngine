//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Buffer.h"

#include "vector"

namespace engine {

    class IndexBuffer : public Buffer {

    public:
        IndexBuffer(const std::vector<int>& indices, const uint32_t size) : indices(indices), size(size) {}
        virtual ~IndexBuffer() = default;

        void bind() override;

        void unbind() override;

    protected:
        std::vector<int> indices;
        uint32_t size;

    };

}