//
// Created by mecha on 12.09.2021.
//

#pragma once

#include "vector"
#include "string"

#include "geometry/Vertex.h"

namespace engine {

    struct GraphicsObject {
        float* vertices;
        uint32_t* indices;
        std::string shaderName;
        uint32_t verticesSize;
        uint32_t indicesSize;

        GraphicsObject(
                const std::string &shaderName,
                float* vertices,
                uint32_t* indices,
                uint32_t verticesSize,
                uint32_t indicesSize
                ) :
                shaderName(shaderName),
                vertices(vertices),
                indices(indices),
                verticesSize(verticesSize),
                indicesSize(indicesSize)
                {}
    };

}