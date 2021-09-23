//
// Created by mecha on 12.09.2021.
//

#pragma once

#include "vector"
#include "string"

#include "geometry/Vertex.h"
#include "../core/Memory.h"

namespace engine {

    struct GraphicsObject {
        std::string shaderName;
        float* vertices;
        uint32_t* indices;
        uint32_t verticesSize;
        uint32_t indicesSize;

        GraphicsObject(const std::string &shaderName) : shaderName(shaderName) {
            vertices = new float[0];
            indices = new uint32_t[0];
            verticesSize = 0;
            indicesSize = 0;
        }

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
                indicesSize(indicesSize) {}

    };

}