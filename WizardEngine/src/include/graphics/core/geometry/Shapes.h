//
// Created by mecha on 24.09.2021.
//

#pragma once

#include "MeshComponent.h"
#include "string_view"

namespace engine {

    template<typename T>
    class Shapes final {

    public:
        static BaseMeshComponent<T> newTriangle();
        static BaseMeshComponent<T> newSquare();
        static BaseMeshComponent<T> newCube();

    private:
        static T *newTriangleVertices();
        static uint32_t *newTriangleIndices();

        static T *newSquareVertices();
        static uint32_t *newSquareIndices();

        static T *newCubeVertices();
        static uint32_t *newCubeIndices();
    };

    template<typename T>
    BaseMeshComponent<T> Shapes<T>::newTriangle() {
        BaseMesh<T>* meshes = new Mesh {
                VertexData {
                        newTriangleVertices(),
                        0,
                        3
                },
                IndexData {
                        newTriangleIndices(),
                        0,
                        3
                }
        };
        return { meshes };
    }

    template<typename T>
    BaseMeshComponent<T> Shapes<T>::newSquare() {
        BaseMesh<T>* meshes = new Mesh {
                VertexData {
                        newSquareVertices(),
                        0,
                        4
                },
                IndexData {
                        newSquareIndices(),
                        0,
                        6
                }
        };
        return { meshes };
    }

    template<typename T>
    BaseMeshComponent<T> Shapes<T>::newCube() {
        BaseMesh<T>* meshes = new Mesh {
                VertexData {
                        newCubeVertices(),
                        0,
                        24
                },
                IndexData {
                        newCubeIndices(),
                        0,
                        36
                }
        };
        return { meshes };
    }

    template<typename T>
    T *Shapes<T>::newTriangleVertices() {
        auto v1 = T {
                {-0.5f, -0.5f, 0.0f },
                {0, 0}
        };

        auto v2 = T {
                {0.5f, -0.5f, 0.0f},
                {1, 0}
        };

        auto v3 = T {
                {0.0f,  0.5f, 0.0f },
                {1, 1}
        };

        return new T[] {
                v1, v2, v3
        };
    }

    template<typename T>
    uint32_t *Shapes<T>::newTriangleIndices() {
        return new uint32_t[] {0, 1, 2};
    }

    template<typename T>
    T *Shapes<T>::newSquareVertices() {
        auto v1 = T {
                { -1, -1, 0 },
                {0, 0}
        };

        auto v2 = T {
                { -1,  1, 0 },
                {0, 1}
        };

        auto v3 = T {
                {1,  1, 0 },
                {1, 1}
        };

        auto v4 = T {
                {1, -1, 0 },
                {1, 0}
        };

        return new T[] {
                v1, v2, v3, v4
        };
    }

    template<typename T>
    uint32_t *Shapes<T>::newSquareIndices() {
        return new uint32_t[] {
                0, 2, 1,
                0, 3, 2
        };
    }

    template<typename T>
    T *Shapes<T>::newCubeVertices() {
        auto v1 = T {
                {-0.5f,0.5f,-0.5f},
                {0,0,},
                {-0.5f,0.5f,-0.5f},
        };

        auto v2 = T {
                {-0.5f,-0.5f,-0.5f},
                {0,1},
                {-0.5f,-0.5f,-0.5f},
        };

        auto v3 = T {
                {0.5f,-0.5f,-0.5f},
                {1,1},
                {0.5f,-0.5f,-0.5f},
        };

        auto v4 = T {
                {0.5f,0.5f,-0.5f},
                {1,0},
                {0.5f,0.5f,-0.5f},
        };

        auto v5 = T {
                {-0.5f,0.5f,0.5f},
                {0,0},
                {-0.5f,0.5f,0.5f},
        };

        auto v6 = T {
                {-0.5f,-0.5f,0.5f},
                {0,1,},
                {-0.5f,-0.5f,0.5f},
        };

        auto v7 = T {
                {0.5f,-0.5f,0.5f},
                {1,1,},
                {0.5f,-0.5f,0.5f},
        };

        auto v8 = T {
                {0.5f,0.5f,0.5f},
                {1,0,},
                {0.5f,0.5f,0.5f},
        };

        auto v9 = T {
                {0.5f,0.5f,-0.5f},
                {0,0,},
                {0.5f,0.5f,-0.5f},
        };

        auto v10 = T {
                {0.5f,-0.5f,-0.5f},
                {0,1,},
                {0.5f,-0.5f,-0.5f},
        };

        auto v11 = T {
                {0.5f,-0.5f,0.5f},
                {1,1,},
                {0.5f,-0.5f,0.5f},
        };

        auto v12 = T {
                {0.5f,0.5f,0.5f},
                {1,0,},
                {0.5f,0.5f,0.5f},
        };

        auto v13 = T {
                {-0.5f,0.5f,-0.5f},
                {0,0,},
                {-0.5f,0.5f,-0.5f},
        };

        auto v14 = T {
                {-0.5f,-0.5f,-0.5f},
                {0,1,},
                {-0.5f,-0.5f,-0.5f},
        };

        auto v15 = T {
                {-0.5f,-0.5f,0.5f},
                {1,1,},
                {-0.5f,-0.5f,0.5f},
        };

        auto v16 = T {
                {-0.5f,0.5f,0.5f},
                {1,0,},
                {-0.5f,0.5f,0.5f},
        };

        auto v17 = T {
                {-0.5f,0.5f,0.5f},
                {0,0,},
                {-0.5f,0.5f,0.5f},
        };

        auto v18 = T {
                {-0.5f,0.5f,-0.5f},
                {0,1,},
                {-0.5f,0.5f,-0.5f},
        };

        auto v19 = T {
                { 0.5f,0.5f,-0.5f},
                {1,1,},
                { 0.5f,0.5f,-0.5f},
        };

        auto v20 = T {
                {0.5f,0.5f,0.5f},
                {1,0,},
                {0.5f,0.5f,0.5f},
        };

        auto v21 = T {
                {-0.5f,-0.5f,0.5f},
                {0,0,},
                {-0.5f,-0.5f,0.5f},
        };

        auto v22 = T {
                {-0.5f,-0.5f,-0.5f},
                {0,1,},
                {-0.5f,-0.5f,-0.5f},
        };

        auto v23 = T {
                {0.5f,-0.5f,-0.5f},
                {1,1,},
                {0.5f,-0.5f,-0.5f},
        };

        auto v24 = T {
                {0.5f,-0.5f,0.5f},
                {1,0,},
                {0.5f,-0.5f,0.5f},
        };

        return new T[] {
                v1, v2, v3, v4,
                v5, v6, v7, v8,
                v9, v10, v11, v12,
                v13, v14, v15, v16,
                v17, v18, v19, v20,
                v21, v22, v23, v24,
        };
    }

    template<typename T>
    uint32_t *Shapes<T>::newCubeIndices() {
        return new uint32_t [] {
                0,1,3,
                3,1,2,
                4,5,7,
                7,5,6,
                8,9,11,
                11,9,10,
                12,13,15,
                15,13,14,
                16,17,19,
                19,17,18,
                20,21,23,
                23,21,22
        };
    }
}
