//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "../../../graphics/buffers/VertexBuffer.h"

namespace engine {

    class GLVertexBuffer : public VertexBuffer {

    public:
        explicit GLVertexBuffer(Vertex* vertex) : VertexBuffer(vertex) {
            create();
        }

        ~GLVertexBuffer() override {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;

        void allocate() override;
        void prepare() override;
        void enableAttributes() override;
        void disableAttributes() override;

        void load(const uint32_t &vertexStart, float *subData) override;

    private:
        void create();
        void destroy();

    };

}