//
// Created by mecha on 02.10.2021.
//

#pragma once

#include "../../../graphics/buffers/UniformBuffer.h"

namespace engine {

    class GLUniformBuffer : public UniformBuffer {

    public:
        GLUniformBuffer() : UniformBuffer() {
            create();
        }

        GLUniformBuffer(UniformBlockFormat* uniformBlockFormat) : UniformBuffer(uniformBlockFormat) {
            create();
        }

        ~GLUniformBuffer() override {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;

        void setUniformBlockPointer() override;

        void allocate() override;
        void load(const UniformData &uniformData) override;

    private:
        void create();
        void destroy();

    };

}