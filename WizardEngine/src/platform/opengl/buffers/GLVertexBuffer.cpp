//
// Created by mecha on 07.09.2021.
//

#include "GLVertexBuffer.h"

#include "glad/glad.h"

namespace engine {

    void GLVertexBuffer::bind() {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void GLVertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLVertexBuffer::load() {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.size() * Vertex::getSize()), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::getByteSize(), nullptr);
    }

    void GLVertexBuffer::onCreate() {

    }

    void GLVertexBuffer::onDestroy() {

    }

}