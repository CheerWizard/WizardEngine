//
// Created by mecha on 24.09.2021.
//

#pragma once

#include "MeshComponent.h"
#include "../../ecs/Entity.h"

namespace engine {

    struct Triangle : MeshComponent {
        Triangle() : MeshComponent(new Mesh {
            VertexData {
                createVertices(),
                0,
                3
            },
            IndexData {
                createIndices(),
                0,
                3
            }
        }) {}

    private:
        static uint32_t *createIndices();
        static Vertex *createVertices();
    };

    struct Square : MeshComponent {
        Square() : MeshComponent(new Mesh {
            VertexData {
                createVertices(),
                0,
                4
            },
            IndexData {
                createIndices(),
                0,
                6
            }
        }) {}

    private:
        static uint32_t *createIndices();
        static Vertex *createVertices();
    };

    struct Cube : MeshComponent {
        Cube() : MeshComponent(new Mesh {
            VertexData {
                createVertices(),
                0,
                24
            },
            IndexData {
                createIndices(),
                0,
                36
            }
        }) {}

    private:
        static uint32_t *createIndices();
        static Vertex *createVertices();

    };

}