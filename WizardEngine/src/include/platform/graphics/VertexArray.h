//
// Created by mecha on 12.09.2021.
//

#pragma once

#include <cstdint>

namespace engine {

    // VAO - Vertex Array Object in GPU pipeline
    class VertexArray final {

    public:
        VertexArray() {
            create();
        }
        ~VertexArray() = default;

    public:
        void create();
        void destroy();
        void recreate();
        // bind/unbind
        void bind() const;
        void unbind() const;

    private:
        uint32_t id = 0;

    };

}