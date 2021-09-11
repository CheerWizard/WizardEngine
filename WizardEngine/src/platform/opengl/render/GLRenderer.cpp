//
// Created by mecha on 05.09.2021.
//

#include "GLRenderer.h"

#include "glad/glad.h"

namespace engine {

    void GLRenderer::onCreate() {
        glGenVertexArrays(1, &_vertexArray);
        glBindVertexArray(_vertexArray);

        float vertices[3 * 3] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
        };

        glGenBuffers(1, &_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

        unsigned int indices[] = { 0 , 1 , 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void GLRenderer::onUpdate() {
        glBindVertexArray(_vertexArray);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

    void GLRenderer::onDestroy() {

    }

}