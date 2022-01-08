//
// Created by mecha on 24.09.2021.
//

#pragma once

#include "MeshComponent.h"

namespace engine {

    class Shapes final {

    public:
        static MeshComponent newTriangle();
        static MeshComponent newSquare();
        static MeshComponent newCube();

    private:
        static Vertex *newTriangleVertices();
        static uint32_t *newTriangleIndices();

        static Vertex *newSquareVertices();
        static uint32_t *newSquareIndices();

        static Vertex *newCubeVertices();
        static uint32_t *newCubeIndices();
    };

}