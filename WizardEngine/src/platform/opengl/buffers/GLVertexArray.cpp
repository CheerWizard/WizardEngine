//
// Created by mecha on 12.09.2021.
//

#include "GLVertexArray.h"

#include "glad/glad.h"

namespace engine {

    void GLVertexArray::create() {
        glCreateVertexArrays(1, &id);
    }

    void GLVertexArray::destroy() {
        glDeleteVertexArrays(1, &id);
        indexBuffer.reset();
//        vertexBufferCache->clear();
    }

    void GLVertexArray::bind() {
        glBindVertexArray(id);
    }

    void GLVertexArray::unbind() {
        glBindVertexArray(0);
    }

}