//
// Created by mecha on 07.09.2021.
//

#pragma once

#include <glm/glm.hpp>

namespace engine {

    struct Vertex {
        glm::vec3 position = { 0.5f, 0.5f, 0.5f };
        glm::vec2 textureCoords = { 0.25f, -0.25f };
        glm::vec3 normal = { 0, 0, 0 };
        float instanceId = 0;
        float textureSlot = 0;
    };

}
