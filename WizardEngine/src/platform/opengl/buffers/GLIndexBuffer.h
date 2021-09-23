//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "../../../graphics/buffers/IndexBuffer.h"

namespace engine {

    class GLIndexBuffer : public IndexBuffer {

    public:
        explicit GLIndexBuffer(const uint32_t &indexCount = DEFAULT_INDEX_COUNT) :
        IndexBuffer(indexCount) {
            create();
        }

        ~GLIndexBuffer() override {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;

        void allocate() override;
        void load(const uint32_t &indexStart, uint32_t *subData) override;

    private:
        void create();
        void destroy();

    };

}