//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "glm/glm.hpp"

namespace engine {

    struct Vertex {
        glm::fvec3 position;
        glm::fvec3 normals;
        glm::ivec2 textureCoords;

        static inline uint32_t getSize() {
            return glm::fvec3::length() * 2 + glm::ivec2 ::length();
        }

        static inline uint32_t getByteSize() {
            return getSize() * 4;
        }
    };

}