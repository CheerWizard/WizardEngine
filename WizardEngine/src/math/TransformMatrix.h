//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "Uniform.h"

namespace engine {

    struct TransformMatrix : Mat4fUniform {
        TransformMatrix(const char* name) : Mat4fUniform(name) {
            create();
        }

    private:
        void create();

    };

}