//
// Created by mecha on 02.10.2021.
//

#include "UniformBuffer.h"

namespace engine {

    void UniformBuffer::prepare(UniformBlockFormat* uniformBlockFormat) {
        setUniformBlockFormat(uniformBlockFormat);
        bind();
        allocate();
        unbind();
        setUniformBlockPointer();
    }

}