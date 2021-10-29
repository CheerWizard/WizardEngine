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
        glm::vec3 position = DEFAULT_POSITION;
        glm::vec2 textureCoords = DEFAULT_TEXTURE_COORDS;
        float instanceId = 0;
    };

}