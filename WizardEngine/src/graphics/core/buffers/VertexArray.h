//
// Created by mecha on 12.09.2021.
//

#pragma once

#include <cstdint>

namespace engine {

    class VertexArray {

    public:
        VertexArray() = default;
        ~VertexArray() = default;

    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        uint32_t id = 0;

    };

}