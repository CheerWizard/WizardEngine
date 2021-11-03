//
// Created by mecha on 02.10.2021.
//

#pragma once

#include "Buffer.h"
#include "UniformData.h"
#include "../shader/UniformBlockFormat.h"

namespace engine {

    class UniformBuffer : public Buffer {

    public:
        UniformBuffer() : Buffer() {}

        UniformBuffer(UniformBlockFormat* uniformBlockFormat) :
        Buffer(),
        uniformBlockFormat(uniformBlockFormat) {}

    public:
        void prepare(UniformBlockFormat* uniformBlockFormat);

    public:
        virtual void allocate() = 0;
        virtual void load(const UniformData &uniformData) = 0;
        virtual void setUniformBlockPointer() = 0;

    public:
        inline void setUniformBlockFormat(UniformBlockFormat* uniformBlockFormat) {
            this->uniformBlockFormat = uniformBlockFormat;
        }

        inline UniformBlockFormat *getUniformBlockFormat() const {
            return uniformBlockFormat;
        }

    protected:
        UniformBlockFormat* uniformBlockFormat;

    };

}