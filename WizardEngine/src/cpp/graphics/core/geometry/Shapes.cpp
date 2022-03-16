//
// Created by mecha on 24.09.2021.
//

#include <graphics/core/geometry/Shapes.h>

namespace engine {

    MeshComponent Shapes::newTriangle() {
        Mesh* meshes = new Mesh {
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

    MeshComponent Shapes::newSquare() {
        Mesh* meshes = new Mesh {
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

    MeshComponent Shapes::newCube() {
        Mesh* meshes = new Mesh {
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

    Vertex *Shapes::newTriangleVertices() {
        auto v1 = Vertex {
                {-0.5f, -0.5f, 0.0f },
                {0, 0}
        };

        auto v2 = Vertex {
                {0.5f, -0.5f, 0.0f},
                {1, 0}
        };

        auto v3 = Vertex {
                {0.0f,  0.5f, 0.0f },
                {1, 1}
        };

        return new Vertex[] {
                v1, v2, v3
        };
    }

    uint32_t *Shapes::newTriangleIndices() {
        return new uint32_t[] {0, 1, 2};
    }

    Vertex *Shapes::newSquareVertices() {
        auto v1 = Vertex {
                { -1, -1, 0 },
                {0, 0}
        };

        auto v2 = Vertex {
                { -1,  1, 0 },
                {0, 1}
        };

        auto v3 = Vertex {
                {1,  1, 0 },
                {1, 1}
        };

        auto v4 = Vertex {
                {1, -1, 0 },
                {1, 0}
        };

        return new Vertex[] {
                v1, v2, v3, v4
        };
    }

    uint32_t *Shapes::newSquareIndices() {
        return new uint32_t[] {
                0, 2, 1,
                0, 3, 2
        };
    }

    Vertex *Shapes::newCubeVertices() {
        auto v1 = Vertex {
                {-0.5f,0.5f,-0.5f},
                {0,0,},
                {-0.5f,0.5f,-0.5f},
        };

        auto v2 = Vertex {
                {-0.5f,-0.5f,-0.5f},
                {0,1},
                {-0.5f,-0.5f,-0.5f},
        };

        auto v3 = Vertex {
                {0.5f,-0.5f,-0.5f},
                {1,1},
                {0.5f,-0.5f,-0.5f},
        };

        auto v4 = Vertex {
                {0.5f,0.5f,-0.5f},
                {1,0},
                {0.5f,0.5f,-0.5f},
        };

        auto v5 = Vertex {
                {-0.5f,0.5f,0.5f},
                {0,0},
                {-0.5f,0.5f,0.5f},
        };

        auto v6 = Vertex {
                {-0.5f,-0.5f,0.5f},
                {0,1,},
                {-0.5f,-0.5f,0.5f},
        };

        auto v7 = Vertex {
                {0.5f,-0.5f,0.5f},
                {1,1,},
                {0.5f,-0.5f,0.5f},
        };

        auto v8 = Vertex {
                {0.5f,0.5f,0.5f},
                {1,0,},
                {0.5f,0.5f,0.5f},
        };

        auto v9 = Vertex {
                {0.5f,0.5f,-0.5f},
                {0,0,},
                {0.5f,0.5f,-0.5f},
        };

        auto v10 = Vertex {
                {0.5f,-0.5f,-0.5f},
                {0,1,},
                {0.5f,-0.5f,-0.5f},
        };

        auto v11 = Vertex {
                {0.5f,-0.5f,0.5f},
                {1,1,},
                {0.5f,-0.5f,0.5f},
        };

        auto v12 = Vertex {
                {0.5f,0.5f,0.5f},
                {1,0,},
                {0.5f,0.5f,0.5f},
        };

        auto v13 = Vertex {
                {-0.5f,0.5f,-0.5f},
                {0,0,},
                {-0.5f,0.5f,-0.5f},
        };

        auto v14 = Vertex {
                {-0.5f,-0.5f,-0.5f},
                {0,1,},
                {-0.5f,-0.5f,-0.5f},
        };

        auto v15 = Vertex {
                {-0.5f,-0.5f,0.5f},
                {1,1,},
                {-0.5f,-0.5f,0.5f},
        };

        auto v16 = Vertex {
                {-0.5f,0.5f,0.5f},
                {1,0,},
                {-0.5f,0.5f,0.5f},
        };

        auto v17 = Vertex {
                {-0.5f,0.5f,0.5f},
                {0,0,},
                {-0.5f,0.5f,0.5f},
        };

        auto v18 = Vertex {
                {-0.5f,0.5f,-0.5f},
                {0,1,},
                {-0.5f,0.5f,-0.5f},
        };

        auto v19 = Vertex {
                { 0.5f,0.5f,-0.5f},
                {1,1,},
                { 0.5f,0.5f,-0.5f},
        };

        auto v20 = Vertex {
                {0.5f,0.5f,0.5f},
                {1,0,},
                {0.5f,0.5f,0.5f},
        };

        auto v21 = Vertex {
                {-0.5f,-0.5f,0.5f},
                {0,0,},
                {-0.5f,-0.5f,0.5f},
        };

        auto v22 = Vertex {
                {-0.5f,-0.5f,-0.5f},
                {0,1,},
                {-0.5f,-0.5f,-0.5f},
        };

        auto v23 = Vertex {
                {0.5f,-0.5f,-0.5f},
                {1,1,},
                {0.5f,-0.5f,-0.5f},
        };

        auto v24 = Vertex {
                {0.5f,-0.5f,0.5f},
                {1,0,},
                {0.5f,-0.5f,0.5f},
        };

        return new Vertex[] {
                v1, v2, v3, v4,
                v5, v6, v7, v8,
                v9, v10, v11, v12,
                v13, v14, v15, v16,
                v17, v18, v19, v20,
                v21, v22, v23, v24,
        };
    }

    uint32_t *Shapes::newCubeIndices() {
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