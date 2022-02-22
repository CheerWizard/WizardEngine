//
// Created by mecha on 12.11.2021.
//

#pragma once

#include "VertexFormat.h"
#include "Uniform.h"
#include "UniformBlockFormat.h"

#include <glm/glm.hpp>

namespace engine::shader {

    enum ShaderState : uint8_t {
        READY = 0,
        FAILED_READ_FILE = 1,
        FAILED_COMPILE = 2,
        FAILED_LINKING = 3,
        NO_VERTEX_ATTRS = 4,
        NO_UNIFORM_BLOCKS = 5,
        NOT_READY = 6
    };

}