//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "../shader/VertexAttribute.h"

#include "glm/glm.hpp"

#include "vector"

#define DEFAULT_POSITION {0.5f, 0.5f, 0.5f}
#define DEFAULT_TEXTURE_COORDS {0.25f, -0.25f}

namespace engine {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 textureCoords;
//        glm::vec4 color;

        Vertex(const glm::vec3 &position = DEFAULT_POSITION,
               const glm::vec2 &textureCoords = DEFAULT_TEXTURE_COORDS,
               const glm::vec4 &color = {0.5, 0.75, 0, 1}) :
               position(position),
//               color(color),
               textureCoords(textureCoords)
               {}
    };

}