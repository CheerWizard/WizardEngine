//
// Created by mecha on 12.11.2021.
//

#include <platform/graphics/VertexArray.h>
#include <glad/glad.h>

namespace engine {

    void VertexArray::create() {
        glGenVertexArrays(1, &id);
    }

    void VertexArray::destroy() {
        glDeleteVertexArrays(1, &id);
    }

    void VertexArray::recreate() {
        destroy();
        create();
    }

    void VertexArray::bind() const {
        glBindVertexArray(id);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
    }

}