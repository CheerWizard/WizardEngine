//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "glm/glm.hpp"

#define DEFAULT_POSITION {0.5f, 0.5f, 0.5f}
#define DEFAULT_UV {0.25f, -0.25f}
#define DEFAULT_NORMAL { 0, 0, 0 }

namespace engine {

    struct Vertex {
        glm::vec3 position = DEFAULT_POSITION;
        glm::vec2 textureCoords = DEFAULT_UV;
        glm::vec3 normal = DEFAULT_NORMAL;
        float instanceId = 0;
    };

}