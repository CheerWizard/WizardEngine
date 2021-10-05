//
// Created by mecha on 12.09.2021.
//

#pragma once

#include "../../../graphics/buffers/VertexArray.h"

namespace engine {

    class GLVertexArray : public VertexArray {

    public:
        GLVertexArray() : VertexArray() {
            create();
        }

        ~GLVertexArray() {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;

    private:
        void create();
        void destroy();

    };

}