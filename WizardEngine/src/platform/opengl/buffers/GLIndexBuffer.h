//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "../../../graphics/buffers/IndexBuffer.h"

namespace engine {

    class GLIndexBuffer : public IndexBuffer {

    public:
        GLIndexBuffer(const uint32_t &indexCount = DEFAULT_INDEX_COUNT) : IndexBuffer(indexCount) {
            create();
        }

        ~GLIndexBuffer() override {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;

        void allocate() override;

        void load(const IndexData &indexData) override;

        void recreate() override;

    private:
        void create();
        void destroy();

    };

}