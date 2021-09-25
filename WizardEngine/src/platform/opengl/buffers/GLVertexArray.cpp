//
// Created by mecha on 12.09.2021.
//

#include "GLVertexArray.h"

#include "glad/glad.h"

namespace engine {

    void GLVertexArray::create() {
        glGenVertexArrays(1, &id);
    }

    void GLVertexArray::destroy() {
        glDeleteVertexArrays(1, &id);
        VertexArray::destroy();
    }

    void GLVertexArray::bind() {
        glBindVertexArray(id);
    }

    void GLVertexArray::unbind() {
        glBindVertexArray(0);
    }

}