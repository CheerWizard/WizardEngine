//
// Created by mecha on 24.09.2021.
//

#pragma once

#include "ShapeComponent.h"
#include "../../ecs/Entity.h"

namespace engine {

    struct Triangle : ShapeComponent {
        Triangle() : ShapeComponent() {
            vertexData = { createVertices() };
            indexData = { createIndices() };
        }

    private:
        static uint32_t *createIndices();
        static Vertex *createVertices();
    };

    struct Square : ShapeComponent {
        Square() : ShapeComponent() {
            vertexData = { createVertices() };
            indexData = { createIndices() };
        }

    private:
        static uint32_t *createIndices();
        static Vertex *createVertices();
    };

    struct Cube : ShapeComponent {
        Cube() : ShapeComponent() {
            vertexData = { createVertices(),0,24 };
            indexData = { createIndices(), 0, 36 };
        }

    private:
        static uint32_t *createIndices();
        static Vertex *createVertices();

    };

}