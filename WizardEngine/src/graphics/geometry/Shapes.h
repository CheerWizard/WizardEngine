//
// Created by mecha on 24.09.2021.
//

#pragma once

#include "../GraphicsObject.h"

#define CREATE_TRIANGLE(shaderName) engine::createRef<engine::Triangle>(engine::Triangle { shaderName })
#define CREATE_CUBE(shaderName) engine::createRef<engine::Cube>(engine::Cube { shaderName })

namespace engine {

    struct Triangle : GraphicsObject {
        Triangle(const std::string &shaderName) : GraphicsObject(shaderName) {
            this->vertexData = VertexData {
                createVertices()
            };
            this->indexData = IndexData {
                createIndices()
            };
        }

    private:
        static uint32_t *createIndices();
        static Vertex *createVertices();
    };

    struct Cube : GraphicsObject {
        Cube(const std::string &shaderName) : GraphicsObject(shaderName) {
            this->vertexData = VertexData {
                createVertices(),
                0,
                24
            };
            this->indexData = IndexData {
                createIndices(),
                0,
                36
            };
        }

    private:
        static uint32_t *createIndices();
        static Vertex *createVertices();

    };

}